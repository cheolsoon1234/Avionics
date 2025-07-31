//
// Created by Cheolsoon on 25. 7. 8.
//

#include "I2C_IO.h"

int32_t I2C_Open(const char *Port) {
    int32_t FileDesc = open(Port, O_RDWR);
    if (FileDesc < 0) {
        fprintf(stderr, "I2C_Open: Failed to open I2C port %s (%s)\n", Port, strerror(errno));
        return -1;
    }
    return FileDesc;
}

int32_t I2C_Configure(int32_t FileDesc, int32_t Address) {
    if (FileDesc < 0) return -1;
    if (ioctl(FileDesc, I2C_SLAVE, Address) < 0) {
        fprintf(stderr, "I2C_Configure: Failed to configure slave address 0x%02X (%s)\n", Address, strerror(errno));
        return -1;
    }
    return 0;
}

int32_t I2C_Write(int32_t FileDesc, uint8_t Register, char *Data) {
    char Buffer[2];
    Buffer[0] = Register;
    Buffer[1] = *Data;

    ssize_t Written = write(FileDesc, Buffer, 2);
    if (Written != 2) {
        fprintf(stderr, "I2C_Write: Failed to write to register 0x%02X (%s)\n", Register, strerror(errno));
        return -1;
    }
    return 0;
}

int32_t I2C_Read(int32_t FileDesc, uint8_t Register, char *Buffer) {
    ssize_t Written = write(FileDesc, &Register, 1);
    if (Written != 1) {
        fprintf(stderr, "I2C_Read: Failed to write register address 0x%02X (wrote %zd bytes, %s)\n", Register, Written, strerror(errno));
        return -1;
    }

    ssize_t ReadBytes = read(FileDesc, Buffer, 1);
    if (ReadBytes != 1) {
        fprintf(stderr, "I2C_Read: Failed to read from register 0x%02X (%s)\n", Register, strerror(errno));
        return -1;
    }

    return 0;
}

void I2C_Close(int32_t FileDesc) {
    close(FileDesc);
}