//
// Created by cheolsoon on 25. 7. 27.
//

#include "BMP180.h"

static int32_t Read16(int32_t fd, uint8_t reg, int16_t *value) {
    uint8_t msb = 0, lsb = 0;
    if (I2C_Read(fd, reg, (char *)&msb) < 0) return -1;
    if (I2C_Read(fd, reg + 1, (char *)&lsb) < 0) return -1;
    *value = (msb << 8) | lsb;
    return 0;
}

int32_t BMP180_Init(BMP180_t *dev, const char *i2c_path) {
    dev->fd = I2C_Open(i2c_path);
    if (dev->fd < 0 || I2C_Configure(dev->fd, BMP180_ADDR) < 0) return -1;

    char id;
    if (I2C_Read(dev->fd, BMP180_REG_CHIP_ID, &id) < 0 || id != 0x55) {
        fprintf(stderr, "BMP180 not detected or invalid ID: 0x%02X\n", id);
        return -1;
    }

    if (Read16(dev->fd, 0xAA, &dev->calib.AC1)  < 0  ||
        Read16(dev->fd, 0xAC, &dev->calib.AC2)  < 0  ||
        Read16(dev->fd, 0xAE, &dev->calib.AC3)  < 0  ||
        Read16(dev->fd, 0xB0, &dev->calib.AC4)  < 0  ||
        Read16(dev->fd, 0xB2, &dev->calib.AC5)  < 0  ||
        Read16(dev->fd, 0xB4, &dev->calib.AC6)  < 0  ||
        Read16(dev->fd, 0xB6, &dev->calib.B1)   < 0  ||
        Read16(dev->fd, 0xB8, &dev->calib.B2)   < 0  ||
        Read16(dev->fd, 0xBA, &dev->calib.MB)   < 0  ||
        Read16(dev->fd, 0xBC, &dev->calib.MC)   < 0  ||
        Read16(dev->fd, 0xBE, &dev->calib.MD)   < 0    ) {
        fprintf(stderr, "Failed to read calibration data.\n");
        return -1;
    }

    return 1;
}

static int32_t BMP180_ReadRawTemp(BMP180_t *dev, int32_t *UT) {
    char cmd = BMP180_CMD_TEMP;
    if (I2C_Write(dev->fd, BMP180_REG_CTRL_MEAS, &cmd) < 0) return -1;
    usleep(4500);  // Wait 4.5ms (min 4.5ms for temperature)

    uint8_t msb, lsb;
    if (I2C_Read(dev->fd, BMP180_REG_OUT_MSB, (char *)&msb) < 0) return -1;
    if (I2C_Read(dev->fd, BMP180_REG_OUT_LSB, (char *)&lsb) < 0) return -1;

    *UT = ((uint16_t)msb << 8) | (uint16_t)lsb;
    return 0;
}

static int32_t BMP180_ReadRawPressure(BMP180_t *dev, int32_t *UP) {
    char cmd = BMP180_CMD_PRESS + (OSS << 6);  // Command with OSS
    if (I2C_Write(dev->fd, BMP180_REG_CTRL_MEAS, &cmd) < 0) return -1;

    // Wait time depends on OSS (datasheet Table 3)
    switch (OSS) {
        case 0: usleep(4500);   break;
        case 1: usleep(7500);   break;
        case 2: usleep(13500);  break;
        case 3: usleep(25500);  break;
        default: usleep(9000);  break;
    }

    uint8_t msb, lsb, xlsb;
    if (I2C_Read(dev->fd, BMP180_REG_OUT_MSB,   (char *)&msb) < 0)  return -1;
    if (I2C_Read(dev->fd, BMP180_REG_OUT_LSB,   (char *)&lsb) < 0)  return -1;
    if (I2C_Read(dev->fd, BMP180_REG_OUT_XLSB,  (char *)&xlsb) < 0) return -1;

    *UP = (((uint32_t)msb << 16) | ((uint32_t)lsb << 8) | ((uint32_t)xlsb)) >> (8 - OSS);
    return 0;
}

static int32_t CompensateTemp(BMP180_t *dev, int32_t UT, float *T) {
    int32_t X1 = ((UT - dev->calib.AC6) * dev->calib.AC5) >> 15;
    int32_t X2 = (dev->calib.MC << 11) / (X1 + dev->calib.MD);
    dev->B5 = X1 + X2;
    *T = ((dev->B5 + 8) >> 4) / 10.0f;
    return 1;
}

static int32_t CompensatePressure(BMP180_t *dev, int32_t UP, int32_t *P) {
    int32_t B6 = dev->B5 - 4000;
    int32_t X1 = (dev->calib.B2 * ((B6 * B6) >> 12)) >> 11;
    int32_t X2 = (dev->calib.AC2 * B6) >> 11;
    int32_t X3 = X1 + X2;
    int32_t B3 = (((dev->calib.AC1 * 4 + X3) << OSS) + 2) >> 2;

    X1 = (dev->calib.AC3 * B6) >> 13;
    X2 = (dev->calib.B1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;

    uint32_t B4 = (dev->calib.AC4 * (uint32_t)(X3 + 32768)) >> 15;
    uint32_t B7 = ((uint32_t)UP - B3) * (50000 >> OSS);

    if (B4 == 0) return -1;
    int32_t p = ( (B7 < 0x80000000) ? ((B7 << 1) / B4) : ((B7 / B4) << 1) );

    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;
    *P = p + ((X1 + X2 + 3791) >> 4);
    return 1;
}

static int Convert_Pressure_to_Alt(BMP_Data_t *data, float L_K_per_km) {
    
    const float P0_Pa = 101325.0f;     // 해수면 기준 압력 (Pa)
    const float R = 8.3144598f;        // 기체 상수 (J/mol·K)
    const float g = 9.80665f;          // 중력가속도 (m/s^2)
    const float M = 0.0289644f;        // 공기 분자량 (kg/mol)

    // 온도 변환: 섭씨 → 켈빈
    float T0_K = data->temp_c + 273.15f;

    // 기온감률: K/km → K/m
    float L_K_per_m = L_K_per_km / 1000.0f;

    // 고도 계산
    float exponent = (R * L_K_per_m) / (g * M);
    float ratio = (float)(data->pressure_pa) / P0_Pa;

    data->alt_m = (T0_K / L_K_per_m) * (1.0f - powf(ratio, exponent)); // [m]
    
    return 1;
}


int32_t BMP180_Read(BMP180_t *dev, BMP_Data_t *data) {
    int32_t UT, UP;
    float dummy_temp;
    if (BMP180_ReadRawTemp(dev, &UT)                        < 0) return -1;
    if (BMP180_ReadRawPressure(dev, &UP)                    < 0) return -1;
    if (CompensateTemp(dev, UT, &(data->temp_c))            < 0) return -1;
    if (CompensatePressure(dev, UP, &(data->pressure_pa))   < 0) return -1;
    if (Convert_Pressure_to_Alt(data, 6.5)                  < 0) return -1; // 표준대기 (ISA) 기온 감률: 6.5 K/km
    return 1;
}

