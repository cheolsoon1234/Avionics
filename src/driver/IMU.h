//
// Created by cheolsoon on 25. 7. 27.
//
#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include "../common/BaseDef.h"
#include "../hal/Serial_IO.h"

#define PACKET_LENGTH   11
#define IMU_BaudRate    115200

extern int32_t UART0;

typedef struct {
    float ax, ay, az;
    float temp;
} IMU_Accel_t;

typedef struct {
    float gx, gy, gz;
    float temp;
} IMU_Gyro_t;

typedef struct {
    float roll, pitch, yaw;
    float temp;
} IMU_Angle_t;

typedef struct {
    IMU_Accel_t accel;
    IMU_Gyro_t  gyro;
    IMU_Angle_t angle;
    uint8_t got_51;
    uint8_t got_52;
    uint8_t got_53;
} IMU_Data_t;

int32_t IMU_Init(void);
int32_t IMU_Rx_Handler(IMU_Data_t* out_data);
// void IMU_Op(void);

#endif // IMU_H