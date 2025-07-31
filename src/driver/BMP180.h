//
// Created by cheolsoon on 25. 7. 27.
//
#ifndef BMP180_H
#define BMP180_H

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "../common/BaseDef.h"

#include "../hal/I2C_IO.h"

// I2C 주소
#define BMP180_ADDR             0x77

// OverSampling Setting (0-3)
#define OSS 0

// 레지스터
#define BMP180_REG_CHIP_ID      0xD0
#define BMP180_REG_CTRL_MEAS    0xF4
#define BMP180_REG_OUT_MSB      0xF6
#define BMP180_REG_OUT_LSB      0xF7
#define BMP180_REG_OUT_XLSB     0xF8

// 명령어
#define BMP180_CMD_TEMP         0x2E   // Tempature Measure CMD
#define BMP180_CMD_PRESS        0x34   // Pressure Measure CMD

typedef struct {
    int16_t AC1, AC2, AC3;
    uint16_t AC4, AC5, AC6;
    int16_t B1, B2;
    int16_t MB, MC, MD;
} BMP180_CalibData;

typedef struct {
    int32_t           fd;     // I2C 파일 디스크립터
    BMP180_CalibData  calib;  // 보정 데이터
    int32_t           B5;     // 온도 보정 중간값
} BMP180_t;

typedef struct {
    float temp_c;
    int32_t pressure_pa;
    float alt_m;
} BMP_Data_t;


// 초기화 및 종료
int32_t BMP180_Init(BMP180_t *dev, const char *i2c_path);

// 온도/압력 읽기
int32_t BMP180_Read(BMP180_t *dev, BMP_Data_t *data);


#endif // BMP180_H