#include <stdint.h>
#include <stdbool.h>
#include "gpio_imx.h"
#include "gpio_pins.h"
#include "board.h"
#include "Motor.h"
#include "ccm_analog_imx6sx.h"


void Forward(void)
{
    GPIO_WritePinOutput(Dir1->base,Dir1->pin,gpioPinSet);
    GPIO_WritePinOutput(Dir2->base,Dir2->pin,gpioPinClear);
    GPIO_WritePinOutput(Dir3->base,Dir3->pin,gpioPinSet);
    GPIO_WritePinOutput(Dir4->base,Dir4->pin,gpioPinClear);
    PWM1_PWMSAR = 9000;
    PWM2_PWMSAR = 9000;
}
void Backward(void)
{
    GPIO_WritePinOutput(Dir1->base,Dir1->pin,gpioPinClear);
    GPIO_WritePinOutput(Dir2->base,Dir2->pin,gpioPinSet);
    GPIO_WritePinOutput(Dir3->base,Dir3->pin,gpioPinClear);
    GPIO_WritePinOutput(Dir4->base,Dir4->pin,gpioPinSet);
    PWM1_PWMSAR = 9000;
    PWM2_PWMSAR = 9000;
}
void TurnLeft(void)
{
    GPIO_WritePinOutput(Dir1->base,Dir1->pin,gpioPinSet);
    GPIO_WritePinOutput(Dir2->base,Dir2->pin,gpioPinClear);
    GPIO_WritePinOutput(Dir3->base,Dir3->pin,gpioPinSet);
    GPIO_WritePinOutput(Dir4->base,Dir4->pin,gpioPinClear);
    PWM1_PWMSAR = 8000;
    PWM2_PWMSAR = 3000;
}
void TurnRight(void)
{
    GPIO_WritePinOutput(Dir1->base,Dir1->pin,gpioPinSet);
    GPIO_WritePinOutput(Dir2->base,Dir2->pin,gpioPinClear);
    GPIO_WritePinOutput(Dir3->base,Dir3->pin,gpioPinSet);
    GPIO_WritePinOutput(Dir4->base,Dir4->pin,gpioPinClear);
    PWM1_PWMSAR = 3000;
    PWM2_PWMSAR = 8000;
}
void Stop(void)
{
    GPIO_WritePinOutput(Dir1->base,Dir1->pin,gpioPinClear);
    GPIO_WritePinOutput(Dir2->base,Dir2->pin,gpioPinClear);
    GPIO_WritePinOutput(Dir3->base,Dir3->pin,gpioPinClear);
    GPIO_WritePinOutput(Dir4->base,Dir4->pin,gpioPinClear);
}

void GPIO_Ctrl_InitDirPin()
{
    /* Dir1 */
    gpio_init_config_t Dir1InitConfig = {
        .pin = Dir1 ->pin,
        .direction = gpioDigitalOutput,
        .interruptMode = gpioNoIntmode
    };
    GPIO_Init(Dir1->base, &Dir1InitConfig);

    /* Dir2 */
    gpio_init_config_t Dir2InitConfig = {
        .pin = Dir2 ->pin,
        .direction = gpioDigitalOutput,
        .interruptMode = gpioNoIntmode
    };
    GPIO_Init(Dir2->base, &Dir2InitConfig);

    /* Dir3 */
    gpio_init_config_t Dir3InitConfig = {
        .pin = Dir3 ->pin,
        .direction = gpioDigitalOutput,
        .interruptMode = gpioNoIntmode
    };
    GPIO_Init(Dir3->base, &Dir3InitConfig);

    /* Dir4 */
    gpio_init_config_t Dir4InitConfig = {
        .pin = Dir4 ->pin,
        .direction = gpioDigitalOutput,
        .interruptMode = gpioNoIntmode
    };
    GPIO_Init(Dir4->base, &Dir4InitConfig);
}

void Init_PWM_CLOCK(void)
{
    /* Power up PLL and enable PLL clock output 528M. */
    CCM_ANALOG_EnablePllClock(CCM_ANALOG, ccmAnalogPllSysClock);
    CCM_ANALOG_PowerUpPll(CCM_ANALOG, ccmAnalogPllSysControl);

    /* PLL is not bypass */
    CCM_ANALOG_SetPllBypass(CCM_ANALOG, ccmAnalogPllSysControl, false);

    /* IPG root clock configuration */
    CCM_SetRootMux(CCM, ccmRootPrePeriphClkSel, ccmRootmuxPrePeriphClkPll2);
    CCM_SetRootMux(CCM,  ccmRootPeriphClkSel, ccmRootmuxPeriphClkPrePeriphClkSel);

    CCM_SetRootDivider(CCM, ccmRootAhbPodf, 3);
    CCM_SetRootDivider(CCM, ccmRootIpgPodf, 1);
}

/* Configure PWM */
void Init_PWM(void)
{
    /* PWM1 & PWM2 */

    /* Configure GPIO IOMUX*/
    IOMUXC_SW_MUX_CTL_PAD_RGMII2_RD3 = IOMUXC_SW_MUX_CTL_PAD_RGMII2_RD3_MUX_MODE(2);
    IOMUXC_SW_PAD_CTL_PAD_RGMII2_RD3 = IOMUXC_SW_PAD_CTL_PAD_RGMII2_RD3_DSE(6)|
                                       IOMUXC_SW_PAD_CTL_PAD_RGMII2_RD3_SPEED(2)|
                                       IOMUXC_SW_PAD_CTL_PAD_RGMII2_RD3_PKE_MASK;

    IOMUXC_SW_MUX_CTL_PAD_RGMII2_RD2 = IOMUXC_SW_MUX_CTL_PAD_RGMII2_RD2_MUX_MODE(2);
    IOMUXC_SW_PAD_CTL_PAD_RGMII2_RD2 = IOMUXC_SW_PAD_CTL_PAD_RGMII2_RD2_DSE(6)|
                                       IOMUXC_SW_PAD_CTL_PAD_RGMII2_RD2_SPEED(2)|
                                       IOMUXC_SW_PAD_CTL_PAD_RGMII2_RD2_PKE_MASK;
    /* Disable PWM */
    PWM1_PWMCR &= ~(1UL << 0);
	PWM2_PWMCR &= ~(1UL << 0);

    PWM1_PWMCR = PWM_PWMCR_POUTC(0)| PWM_PWMCR_CLKSRC(1)| PWM_PWMCR_PRESCALER(0);
	PWM2_PWMCR = PWM_PWMCR_POUTC(0)| PWM_PWMCR_CLKSRC(1)| PWM_PWMCR_PRESCALER(0);

    PWM1_PWMPR = 10000;
    PWM2_PWMPR = 10000;

    /* Enable PWM void GoUp(void)*/
    PWM1_PWMCR |= (1UL << 0);
    PWM2_PWMCR |= (1UL << 0);
}

