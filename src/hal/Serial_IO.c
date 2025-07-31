//
// Created by Cheolsoon on 25. 7. 8.
//

#include "Serial_IO.h"

char UART_Buffer[UART_BUFFER_SIZE];

/* 시리얼 포트 open 함수 */
int32_t Serial_Open(const char* Port) {
    int32_t fd = open(Port, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        fprintf(stderr, "[Error] while opening serial port: %s\r\n", Port);
    }
    return fd;
}

/* 시리얼 설정 함수 (Baud Rate 8N1, no flow control) */ 
int32_t Serial_Configure(int32_t fd, int32_t BaudRate) {
    if (fd < 0) return -1;

    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(fd, &tty) != 0) {
        fprintf(stderr, "[Error] while configuring serial port: tcgetattr error\r\n");
        return -1;
    }

    speed_t BaudRate_B;

    switch ( BaudRate ) {
    case 921600 :   BaudRate_B = B921600;     break;
    case 230400 :   BaudRate_B = B230400;     break;
    case 115200 :   BaudRate_B = B115200;     break;
    case 57600 :    BaudRate_B = B57600;      break;
    case 38400 :    BaudRate_B = B38400;      break;
    case 19200 :    BaudRate_B = B19200;      break;
    case 9600 :     BaudRate_B = B9600;       break;
    case 4800 :     BaudRate_B = B4800;       break;
    case 2400 :     BaudRate_B = B2400;       break;
    default :       BaudRate_B = B9600;       break;
    }

    cfsetispeed(&tty, BaudRate_B);
    cfsetospeed(&tty, BaudRate_B);

    tty.c_cflag &= ~PARENB;                         // No parity
    tty.c_cflag &= ~CSTOPB;                         // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;                             // 8 data bits
    tty.c_cflag &= ~CRTSCTS;                        // No flow control
    tty.c_cflag |= CREAD | CLOCAL;                  // Enable receiver, ignore modem control

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // No software flow control
    tty.c_iflag &= ~(ICRNL | INLCR | IGNCR);        // CR/LF 변환 해제
    tty.c_oflag &= ~OPOST;                          // Raw output

    tty.c_cc[VMIN]  = SERIAL_MIN_READ_BYTE;
    tty.c_cc[VTIME] = SERIAL_TIMEOUT;               // 10/LOOP_HZ sec timeout

    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        fprintf(stderr, "[Error] while configuring serial port: tcsetattr error\r\n");
        return -1;
    }

    return 1;
}

/* 시리얼 데이터 쓰기 함수 */
int32_t Serial_Write(int32_t fd, const char* data) {
    return write(fd, data, strlen(data));
}

/* 시리얼 데이터 읽기 함수 */
int32_t Serial_Read(int32_t fd, char* buffer, int32_t MaxLen) {
    return read(fd, buffer, MaxLen);
}

/* 시리얼 포트 닫기 함수 */
void Serial_Close(int32_t fd) {
    close(fd);
}

int32_t Serial_Flush(int32_t fd) {
    if (fd < 0) return -1;
    return tcflush(fd, TCIOFLUSH);
}
