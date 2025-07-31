//
// Created by Cheolsoon on 25. 7. 8.
//

#include "SPI_IO.h"

int32_t SPI_Open(const char* Port) {
    int32_t spi_fd = open(Port, O_RDWR);

    if (spi_fd < 0) {
        fprintf(stderr, "Failed to open the spi bus");
    }
    return spi_fd;
}

int32_t SPI_Configure() {



    return 0;
}

int32_t SPI_Read() {

    
    return 0;
}

int32_t SPI_Write() {

    return 0;
}

int32_t SPI_Close(int32_t spi_fd) {
    return close(spi_fd);
}