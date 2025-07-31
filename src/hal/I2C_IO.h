//
// Created by Cheolsoon on 25. 7. 8.
//

#ifndef I2C_IO_H
#define I2C_IO_H

#include <stdio.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include <errno.h>
#include <string.h>

#include "../common/BaseDef.h"

int32_t I2C_Open(const char *Port);
int32_t I2C_Configure(int32_t FileDesc, int32_t Address);
int32_t I2C_Write(int32_t FileDesc, uint8_t Register, char* Data);
int32_t I2C_Read(int32_t FileDesc, uint8_t Register, char* Buffer);
void I2C_Close(int32_t FileDesc);

#endif //I2C_IO_H
