//
// Created by Cheolsoon on 25. 7. 8.
//

#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "../common/BaseDef.h"

#define UART_BUFFER_SIZE 256
extern char UART_Buffer[UART_BUFFER_SIZE];

#ifndef CRTSCTS
#define CRTSCTS 020000000000
#endif /* END OF CRTSCTS */

#define SERIAL_MIN_READ_BYTE   0
#define SERIAL_TIMEOUT         1/LOOP_HZ

int32_t Serial_Open(const char* port);
int32_t Serial_Configure(int32_t fd, int32_t BaudRate);
int32_t Serial_Write(int32_t fd, const char* data);
int32_t Serial_Read(int32_t fd, char* buffer, int32_t MaxLen);
void Serial_Close(int32_t fd);
int32_t Serial_Flush(int32_t fd);

#endif //SERIAL_IO_H
