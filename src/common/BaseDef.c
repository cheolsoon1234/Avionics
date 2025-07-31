//
// Created by cheolsoon on 25. 7. 23.
//

#include "BaseDef.h"

// PORT Settings
char *UART0_Port = "/dev/ttyAMA0";  // BT module
char *UART1_Port = "/dev/ttyAMA2";  // IMU module
char *UART2_Port = "/dev/ttyAMA3";  // GPS module
char *I2C_Port   = "/dev/i2c-1";
char *SPI_Port  = "/dev/spidev";    // ADC module

int32_t UART0 = -1;
int32_t UART1 = -1;
int32_t UART2 = -1;
int32_t I2C0  = -1;

// FLAGS
int32_t EJC_ARM_FLAG    = 0;  // EJC 사출 준비 여부
int32_t EJC_FLAG        = 0;  // EJC 사출 여부
int32_t LAST_CMD_ID     = 0;  // 마지막 명령 ID
int32_t EGNITION_FLAG   = 0;  // 점화 여부

int64_t AVIONICS_LOOP_CNT = 0;

