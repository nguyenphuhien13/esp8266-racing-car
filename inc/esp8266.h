#ifndef _ESP8266_H_
#define _ESP8266_H_

#include <stdint.h>
#include <stdbool.h>

// Access point parameters
#define SSID_NAME	"RAI-Student"
#define PASSKEY		"fptsoftware@2017"

#define ESP8266_ENCRYPT_MODE_OPEN			0
#define ESP8266_ENCRYPT_MODE_WEP			1
#define ESP8266_ENCRYPT_MODE_WPA_PSK		2
#define ESP8266_ENCRYPT_MODE_WPA2_PSK		3
#define ESP8266_ENCRYPT_MODE_WPA_WPA2_PSK	4

#define ESP8266_WIFI_MODE_CLIENT			1
#define ESP8266_WIFI_MODE_AP				2
#define ESP8266_WIFI_MODE_AP_AND_CLIENT		3

//-------------------ESP8266_Init --------------
// initializes the module as a client
// Inputs: none
// Outputs: none
void ESP8266_Init(void);

//---------ESP8266_SetWifiMode----------
// configures the esp8266 to operate as a wifi client, access point, or both
// Input: mode accepts ESP8266_WIFI_MODE constants
// output: 1 if success, 0 if fail
bool ESP8266_SetWifiMode(uint8_t mode);

//---------ESP8266_MultipleConnection----------
// enables the esp8266 connection mux, required for starting tcp server
// Input: 0 (single) or 1 (multiple)
// output: 1 if success, 0 if fail
bool ESP8266_MultipleConnection(uint8_t enabled);


// ----------ESP8266_EnableServer-------------
// - open a "TCP" server on port para.
// Inputs: none
// Outputs: 1 if success, 0 if fail
bool ESP8266_EnableServer(uint16_t port);


#endif /* _ESP8266_H_ */
