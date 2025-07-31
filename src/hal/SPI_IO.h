//
// Created by Cheolsoon on 25. 7. 8.
//

#ifndef SPI_IO_H
#define SPI_IO_H

#include <stdio.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include "../common/BaseDef.h"

int32_t SPI_Open(const char *);
int32_t SPI_Configure();
int32_t SPI_Read();
int32_t SPI_Write();
int32_t SPI_Close(int);


#endif //SPI_IO_H
