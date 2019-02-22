/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include "gpio_pins.h"
/*  Pin USART0_RX */
gpio_config_t gpioDir1 = {
    "Dir1",                                    /* name */
  &IOMUXC_SW_MUX_CTL_PAD_NAND_DATA06, /* muxReg */
  5, /* muxConfig */
  &IOMUXC_SW_PAD_CTL_PAD_NAND_DATA06, /* padReg */
  IOMUXC_SW_PAD_CTL_PAD_NAND_DATA06_SPEED(2) | /* padConfig */
  IOMUXC_SW_PAD_CTL_PAD_NAND_DATA06_PKE_MASK |
  IOMUXC_SW_PAD_CTL_PAD_NAND_DATA06_DSE(6),
  GPIO4, /* base */
  10 /* pin */
};

/*  Pin USART0_TX */
gpio_config_t gpioDir2 = {
    "Dir2",                                    /* name */
  &IOMUXC_SW_MUX_CTL_PAD_NAND_DATA07, /* muxReg */
  5, /* muxConfig */
  &IOMUXC_SW_PAD_CTL_PAD_NAND_DATA07, /* padReg */
  IOMUXC_SW_PAD_CTL_PAD_NAND_DATA07_SPEED(2) | /* padConfig */
  IOMUXC_SW_PAD_CTL_PAD_NAND_DATA07_PKE_MASK |
  IOMUXC_SW_PAD_CTL_PAD_NAND_DATA07_DSE(6),
  GPIO4, /* base */
  11 /* pin */
};

/*  Pin SD4_DAT6 */
gpio_config_t gpioDir3 = {
    "Dir3",                                    /* name */
  &IOMUXC_SW_MUX_CTL_PAD_SD4_DATA6, /* muxReg */
  5, /* muxConfig */
  &IOMUXC_SW_PAD_CTL_PAD_SD4_DATA6, /* padReg */
  IOMUXC_SW_PAD_CTL_PAD_SD4_DATA6_SPEED(2) | /* padConfig */
  IOMUXC_SW_PAD_CTL_PAD_SD4_DATA6_PKE_MASK |
  IOMUXC_SW_PAD_CTL_PAD_SD4_DATA6_DSE(6),
  GPIO6, /* base */
  20 /* pin */
};

/*  Pin SD4_DAT7 */
gpio_config_t gpioDir4 = {
    "Dir4",                                    /* name */
  &IOMUXC_SW_MUX_CTL_PAD_SD4_DATA7, /* muxReg */
  5, /* muxConfig */
  &IOMUXC_SW_PAD_CTL_PAD_SD4_DATA7, /* padReg */
  IOMUXC_SW_PAD_CTL_PAD_SD4_DATA7_SPEED(2) | /* padConfig */
  IOMUXC_SW_PAD_CTL_PAD_SD4_DATA7_PKE_MASK |
  IOMUXC_SW_PAD_CTL_PAD_SD4_DATA7_DSE(6),
  GPIO6, /* base */
  21 /* pin */
};

gpio_config_t gpioLed = {
    "DEBUG_LED",                                    /* name */
  &IOMUXC_SW_MUX_CTL_PAD_NAND_DATA02, /* muxReg */
  5, /* muxConfig */
  &IOMUXC_SW_PAD_CTL_PAD_NAND_DATA02, /* padReg */
  IOMUXC_SW_PAD_CTL_PAD_NAND_DATA02_SPEED(2) | /* padConfig */
  IOMUXC_SW_PAD_CTL_PAD_NAND_DATA02_PKE_MASK |
  IOMUXC_SW_PAD_CTL_PAD_NAND_DATA02_DSE(6),
  GPIO4, /* base */
  6 /* pin */
};


void configure_gpio_pin(gpio_config_t *config)
{
    assert(config);

    *(config->muxReg) = config->muxConfig;
    *(config->padReg) = config->padConfig;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
