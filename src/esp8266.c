/*
 Hardware connections
 Vcc is a separate regulated 3.3V supply with at least 215mA
 /------------------------------\
 |              chip      1   8 |
 | Ant                    2   7 |
 | enna       processor   3   6 |
 |                        4   5 |
 \------------------------------/
ESP8266		UDOO NEO
  1 URxD	PB1		UART out of , 115200 baud
  2 GPIO0			+3.3V for normal operation (ground to flash)
  3 GPIO2			+3.3V
  4 GND		Gnd		GND (70mA)
  5 UTxD	PB0		UART out of ESP8266, 115200 baud
  6 Ch_PD			chip select, 10k resistor to 3.3V
  7 Reset	PB5		can issue output low to cause hardware reset
  8 Vcc				regulated 3.3V supply with at least 70mA
 */

/*
===========================================================
==========          CONSTANTS                    ==========
===========================================================
*/


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "system_MCIMX6X_M4.h"
#include "debug_console_imx.h"
#include "board.h"
#include "uart_imx.h"
#include "esp8266.h"

/*
=============================================================
==========            GLOBAL VARIABLES             ==========
=============================================================
*/
#define MAXTRY 3
#define SERVER_RESPONSE_SIZE 1024
char ServerResponseBuffer[SERVER_RESPONSE_SIZE]; // characters after +IPD,
uint32_t ServerResponseIndex = 0;
#define TX_BUFFER_SIZE 64
#define SEARCHSTRING_LENGHT 16
static char SearchString[SEARCHSTRING_LENGHT];
volatile static uint16_t SearchIndex = 0;
volatile static bool SearchLooking = false;
volatile bool SearchFound = false;
/*
=============================================================
==========            Helper Functions             ==========
=============================================================
*/
//--------ESP8266PrintString--------
// - prints a character to the esp8226 via uart
// Inputs: character to transmit
// Outputs: none

void UART_SendChar(char c)
{
    UART_Putchar(UART5, c);
    while (!UART_GetStatusFlag(UART5, uartStatusTxComplete));
}

void UART_SendString(char *str)
{
    while(*str != 0)
    {
        UART_SendChar(*str);
        str++;
    }
}

static void ESP8266SendCommand(char* inputString)
{
	UART_SendString(inputString);
}

static void _delay_ms(uint32_t miliS)
{
	uint32_t counter = 0;

	counter = (miliS * (SystemCoreClock/1000))/6;
	while (counter--){};
};

char *strcpy(char *d, const char *s)
{
   char *saved = d;
   while (*s)
   {
       *d++ = *s++;
   }
   *d = 0;
   return saved;
}


/*
=======================================================================
==========              search FUNCTIONS                     ==========
=======================================================================
*/


//-------------------SearchStart -------------------
// - start looking for string in received data stream
// Inputs: none
// Outputs: none
static void SearchStart(char *pt)
{
	SearchFound = false;
	SearchLooking = true;
	SearchIndex = 0;

	while (*pt)
	{
		SearchString[SearchIndex++] = *pt++;
	}
	while (SearchIndex < SEARCHSTRING_LENGHT) SearchString[SearchIndex++] = 0;	//delete old data
	SearchIndex = 0;
}

//-------------------SearchCheck -------------------
// - checking each bit received data stream
// Inputs: none
// Outputs: none
void SearchCheck(char letter)
{
	if (SearchLooking)
	{
		if (SearchString[SearchIndex] == letter) // match letter?
		{
			SearchIndex++;

			if (SearchString[SearchIndex] == 0) // end of string/
			{
				SearchFound = true;
				SearchLooking = false;
				debug_printf("\r\nFound\r\n");
			}
		}
		else
		{
			SearchIndex = 0; // start over
		}
	}
}

//-------------------WaitForSearch -------------------
// - start looking for string in received data stream
// Inputs: none
// Outputs: none
static bool WaitForSearch(uint32_t miliS)
{
	uint32_t counter = 0;

	counter = (miliS * (SystemCoreClock/1000))/6;
	while (counter--)
	{
		if (SearchFound)
		{
			return true;
		}
	}

	return false;
}
/*
=======================================================================
==========          ESP8266 PUBLIC FUNCTIONS                 ==========
=======================================================================
*/

//-------------------ESP8266_Init --------------
// initializes the module as a client
// Inputs: baud rate: tested with 9600 and 115200
// Outputs: none
void ESP8266_Init(void)
{
	//ESP8266_Reset();
	//_delay_ms(1000);
	ESP8266_SetWifiMode(ESP8266_WIFI_MODE_AP_AND_CLIENT);
	_delay_ms(1000);
	ESP8266_MultipleConnection(1);
	_delay_ms(1000);
	ESP8266_EnableServer(80);
	_delay_ms(1000);
}

static char ClientRequestSearchString[5];
volatile static uint32_t ClientRequestSearchIndex = 0;
volatile static bool ClientRequestSearchLooking = false;
volatile bool ClientRequestSearchFound = false;

//-------------------ServerResponseSearchStart -------------------
// - start looking for server response string in received data stream
// Inputs: none
// Outputs: none
void ServerResponseSearchStart(void){
    strcpy(ClientRequestSearchString, "+IPD,");
	for (ServerResponseIndex = 0; ServerResponseIndex < SERVER_RESPONSE_SIZE; ServerResponseIndex++)
	{
		ServerResponseBuffer[ServerResponseIndex] = 0;
	}
	ServerResponseIndex = 0;

	ClientRequestSearchLooking = true;
	ClientRequestSearchFound = false;
	ClientRequestSearchIndex = 0;
}

//-------------------ServerResponseSearchCheck -------------------
// - start looking for string in received data stream
// Inputs: none
// Outputs: none

void ServerResponseSearchCheck(char letter){
if (ClientRequestSearchLooking)
	{
		if (ClientRequestSearchString[ClientRequestSearchIndex] == letter) /**< match the letter? */
		{
			ClientRequestSearchIndex++;
			if (ClientRequestSearchString[ClientRequestSearchIndex] == 0) /**< is the whole string identical? */
			{
				ClientRequestSearchLooking = false;
				ClientRequestSearchFound = true;
				//UART_SetIntCmd(UART2, uartIntIdle, true); /**< @note Other ways to put this? */
			}
		}
		else
		{
			ClientRequestSearchIndex = 0; /**< start over */
		}
		ServerResponseIndex = 0;
	}
}


//---------ESP8266_SetWifiMode----------
// configures the esp8266 to operate as a wifi client, access point, or both
// since it searches for "no change" it will execute twice when changing modes
// Input: mode accepts ESP8266_WIFI_MODE constants
// output: 1 if success, 0 if fail
bool ESP8266_SetWifiMode(uint8_t mode)
{
	int try = MAXTRY;

	if (mode > ESP8266_WIFI_MODE_AP_AND_CLIENT) return 0; // fail
	SearchStart("OK");
	while (try--)
	{
		ESP8266SendCommand("AT+CWMODE=3\r\n");
		WaitForSearch(1000);
		if (SearchFound) return 1; // success
	}

	return 0; // fail
}

//---------ESP8266_MultipleConnection----------
// enables the esp8266 connection mux, required for starting tcp server
// Input: 0 (single) or 1 (multiple)
// output: 1 if success, 0 if fail
bool ESP8266_MultipleConnection(uint8_t enabled)
{
	int try = MAXTRY;
	SearchStart("OK");

	while (try--)
	{
		ESP8266SendCommand("AT+CIPMUX=1\r\n");
		WaitForSearch(1000);
		if (SearchFound) return 1; // success
	}

	return 0; // fail
}


/*--------ESP8266_EnableServer------------------
  - enables tcp server on specified port
 ***Prickett code for server, not used or tested in this client project
 Inputs: port number
 Outputs: none*/
bool ESP8266_EnableServer(uint16_t port)
{
	int try = MAXTRY;
	SearchStart("OK");
	while (try--)
  	{
		ESP8266SendCommand("AT+CIPSERVER=1,80\r\n");
		WaitForSearch(1000);
		if (SearchFound)
		{
			ServerResponseSearchStart(); // allow searching for client request if a server port is successfully created
			return 1; // success
		}
	}

	return 0; // fail
}
