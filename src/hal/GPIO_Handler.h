//
// Created by cheolsoon on 25. 7. 28.
//
#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

void GPIO_Export(int32_t);
void GPIO_Unexport(int32_t);
void GPIO_Set_Direction(int32_t, const char*);
void GPIO_Write(int32_t, int32_t);

#endif // GPIO_HANDLER_H


/*
gpiochip0: GPIOs 512-569, parent: platform/fe200000.gpio, pinctrl-bcm2711:
 gpio-512 (ID_SDA              )
 gpio-513 (ID_SCL              )
 gpio-514 (GPIO2               )
 gpio-515 (GPIO3               )
 gpio-516 (GPIO4               )
 gpio-517 (GPIO5               )
 gpio-518 (GPIO6               )
 gpio-519 (GPIO7               )
 gpio-520 (GPIO8               )
 gpio-521 (GPIO9               )
 gpio-522 (GPIO10              )
 gpio-523 (GPIO11              )
 gpio-524 (GPIO12              )
 gpio-525 (GPIO13              )
 gpio-526 (GPIO14              )
 gpio-527 (GPIO15              )
 gpio-528 (GPIO16              )
 gpio-529 (GPIO17              )
 gpio-530 (GPIO18              )
 gpio-531 (GPIO19              )
 gpio-532 (GPIO20              )
 gpio-533 (GPIO21              )
 gpio-534 (GPIO22              )
 gpio-535 (GPIO23              )
 gpio-536 (GPIO24              )
 gpio-537 (GPIO25              )
 gpio-538 (GPIO26              )
 gpio-539 (GPIO27              )
 gpio-540 (RGMII_MDIO          )
 gpio-541 (RGMIO_MDC           )
 gpio-542 (CTS0                )
 gpio-543 (RTS0                )
 gpio-544 (TXD0                )
 gpio-545 (RXD0                )
 gpio-546 (SD1_CLK             )
 gpio-547 (SD1_CMD             )
 gpio-548 (SD1_DATA0           )
 gpio-549 (SD1_DATA1           )
 gpio-550 (SD1_DATA2           )
 gpio-551 (SD1_DATA3           )
 gpio-552 (PWM0_MISO           )
 gpio-553 (PWM1_MOSI           )
 gpio-554 (STATUS_LED_G_CLK    |ACT                 ) out lo 
 gpio-555 (SPIFLASH_CE_N       )
 gpio-556 (SDA0                )
 gpio-557 (SCL0                )
 gpio-558 (RGMII_RXCLK         )
 gpio-559 (RGMII_RXCTL         )
 gpio-560 (RGMII_RXD0          )
 gpio-561 (RGMII_RXD1          )
 gpio-562 (RGMII_RXD2          )
 gpio-563 (RGMII_RXD3          )
 gpio-564 (RGMII_TXCLK         )
 gpio-565 (RGMII_TXCTL         )
 gpio-566 (RGMII_TXD0          )
 gpio-567 (RGMII_TXD1          )
 gpio-568 (RGMII_TXD2          )
 gpio-569 (RGMII_TXD3          )

 rpi@raspberrypi:~/AVIONICS/build $ sudo raspi-gpio get
BANK0 (GPIO 0 to 27):
GPIO 0: level=1 alt=4 func=TXD2 pull=NONE
GPIO 1: level=1 alt=4 func=RXD2 pull=UP
GPIO 2: level=1 alt=0 func=SDA1 pull=UP
GPIO 3: level=1 alt=0 func=SCL1 pull=UP
GPIO 4: level=1 alt=4 func=TXD3 pull=NONE
GPIO 5: level=1 alt=4 func=RXD3 pull=UP
GPIO 6: level=1 func=INPUT pull=UP
GPIO 7: level=1 func=INPUT pull=UP
GPIO 8: level=1 alt=4 func=TXD4 pull=NONE
GPIO 9: level=1 alt=4 func=RXD4 pull=UP
GPIO 10: level=0 func=INPUT pull=DOWN
GPIO 11: level=0 func=INPUT pull=DOWN
GPIO 12: level=0 func=INPUT pull=DOWN
GPIO 13: level=0 func=INPUT pull=DOWN
GPIO 14: level=1 alt=0 func=TXD0 pull=NONE
GPIO 15: level=1 alt=0 func=RXD0 pull=UP
GPIO 16: level=0 func=INPUT pull=DOWN
GPIO 17: level=0 func=OUTPUT pull=DOWN
GPIO 18: level=0 func=INPUT pull=DOWN
GPIO 19: level=0 func=INPUT pull=DOWN
GPIO 20: level=0 func=INPUT pull=DOWN
GPIO 21: level=0 func=INPUT pull=DOWN
GPIO 22: level=0 func=INPUT pull=DOWN
GPIO 23: level=0 func=INPUT pull=DOWN
GPIO 24: level=0 func=INPUT pull=DOWN
GPIO 25: level=0 func=INPUT pull=DOWN
GPIO 26: level=0 func=INPUT pull=DOWN
GPIO 27: level=0 func=OUTPUT pull=DOWN
BANK1 (GPIO 28 to 45):
GPIO 28: level=1 alt=5 func=RGMII_MDIO pull=UP
GPIO 29: level=0 alt=5 func=RGMII_MDC pull=DOWN
GPIO 30: level=1 func=INPUT pull=UP
GPIO 31: level=1 func=INPUT pull=NONE
GPIO 32: level=1 func=INPUT pull=DOWN
GPIO 33: level=1 func=INPUT pull=DOWN
GPIO 34: level=0 alt=3 func=SD1_CLK pull=NONE
GPIO 35: level=1 alt=3 func=SD1_CMD pull=UP
GPIO 36: level=1 alt=3 func=SD1_DAT0 pull=UP
GPIO 37: level=1 alt=3 func=SD1_DAT1 pull=UP
GPIO 38: level=1 alt=3 func=SD1_DAT2 pull=UP
GPIO 39: level=1 alt=3 func=SD1_DAT3 pull=UP
GPIO 40: level=0 alt=0 func=PWM1_0 pull=NONE
GPIO 41: level=0 alt=0 func=PWM1_1 pull=NONE
GPIO 42: level=0 func=OUTPUT pull=UP
GPIO 43: level=1 func=INPUT pull=UP
GPIO 44: level=1 func=INPUT pull=UP
GPIO 45: level=1 func=INPUT pull=UP
BANK2 (GPIO 46 to 53):
GPIO 46: level=0 func=INPUT pull=UP
GPIO 47: level=0 func=INPUT pull=UP
GPIO 48: level=0 func=INPUT pull=DOWN
GPIO 49: level=0 func=INPUT pull=DOWN
GPIO 50: level=0 func=INPUT pull=DOWN
GPIO 51: level=0 func=INPUT pull=DOWN
GPIO 52: level=0 func=INPUT pull=DOWN
GPIO 53: level=0 func=INPUT pull=DOWN
[ raspi-gpio is deprecated - try `pinctrl` instead ]
 */
