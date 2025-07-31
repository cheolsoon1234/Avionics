//
// Created by admin on 2025-07-03.
//

#ifndef BASEDEF_H
#define BASEDEF_H

#include <stdint.h>

#ifndef NULL
#define NULL (void*)0
#endif /* END OF NULL */

// Parameters
#define LOOP_HZ         10
#define MAX_LOOP_CNT    3000
#define BUFFER_SIZE     256

extern int64_t AVIONICS_LOOP_CNT;

// Port Settings 
extern char *UART0_Port;  // BT
extern char *UART1_Port;  // IMU
extern char *UART2_Port;  // GPS
extern char *I2C_Port;    // Barometer
extern char *SPI_Port;    // ADC

extern int32_t UART0;
extern int32_t UART1;
extern int32_t UART2;

// EJC
#define EJC_PIN         529  // GPIO17 Kernal Number (11번 핀)
#define EJC_LED_PIN     539  // GPIO27 Kernal Number (13번 핀)
#define EXP_PEAK_TIME   10.0 // 예상 최고 고도 도달 시간

extern int32_t EJC_ARM_FLAG;
extern int32_t EJC_FLAG;
extern int32_t LAST_CMD_ID;
extern int32_t EGNITION_FLAG;

#endif // BASEDEF_H

