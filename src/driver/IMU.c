//
// Created by cheolsoon on 25. 7. 27.
//
#include "IMU.h"

static float Parse_Signed16(uint8_t low, uint8_t high, float scale) {
    int16_t raw = (high << 8) | low;
    return raw / 32768.0f * scale;
}

static void IMU_Parse_Accel(uint8_t* packet, IMU_Data_t* data) {
    data->accel.ax = Parse_Signed16(packet[2], packet[3], 16.0f);
    data->accel.ay = Parse_Signed16(packet[4], packet[5], 16.0f);
    data->accel.az = Parse_Signed16(packet[6], packet[7], 16.0f);
    data->accel.temp = (int16_t)((packet[9] << 8) | packet[8]) / 340.0f + 36.53f;
    data->got_51 = 1;
}
static void IMU_Parse_Gyro(uint8_t* packet, IMU_Data_t* data) {
    data->gyro.gx = Parse_Signed16(packet[2], packet[3], 2000.0f);
    data->gyro.gy = Parse_Signed16(packet[4], packet[5], 2000.0f);
    data->gyro.gz = Parse_Signed16(packet[6], packet[7], 2000.0f);
    data->gyro.temp = (int16_t)((packet[9] << 8) | packet[8]) / 340.0f + 36.53f;
    data->got_52 = 1;
}

static void IMU_Parse_Angle(uint8_t* packet, IMU_Data_t* data) {
    data->angle.roll  = Parse_Signed16(packet[2], packet[3], 180.0f);
    data->angle.pitch = Parse_Signed16(packet[4], packet[5], 180.0f);
    data->angle.yaw   = Parse_Signed16(packet[6], packet[7], 180.0f);
    data->angle.temp  = (int16_t)((packet[9] << 8) | packet[8]) / 340.0f + 36.53f;
    data->got_53 = 1;
}

static int32_t IMU_Msg_Handler(uint8_t* packet, IMU_Data_t* data) {
    if (packet[0] != 0x55) return -1;

    uint8_t sum = 0;
    for (int32_t i = 0; i < 10; ++i) sum += packet[i];
    if (sum != packet[10]) return -2;

    switch (packet[1]) {
        case 0x51: IMU_Parse_Accel(packet, data); return 0;
        case 0x52: IMU_Parse_Gyro(packet, data);  return 0;
        case 0x53: IMU_Parse_Angle(packet, data); return 0;
        default: return -3;
    }
}

static int32_t IMU_Find_Valid_Packet(uint8_t* buffer, int32_t length) {
    for (int32_t i = 0; i <= length - PACKET_LENGTH; i++) {
        if (buffer[i] == 0x55 &&
            (buffer[i+1] == 0x51 || buffer[i+1] == 0x52 || buffer[i+1] == 0x53)) {
            return i;
        }
    }
    return -1;
}

static void IMU_Shift_Buffer(uint8_t* buffer, int* idx, int32_t shift) {
    memmove(buffer, buffer + shift, (*idx) - shift);
    *idx -= shift;
}

int32_t IMU_Rx_Handler(IMU_Data_t* out_data) {
    uint8_t buffer[256];
    int32_t idx = 0;

    for (int32_t loop_cnt = 0; loop_cnt < MAX_LOOP_CNT; loop_cnt++) {
        // printf("IMU Loop Cnt %d\n", loop_cnt);
        char byte;
        if (Serial_Read(UART1, &byte, 1) <= 0) continue;

        buffer[idx++] = (uint8_t)byte;
        if (idx < PACKET_LENGTH) continue;

        int32_t start = IMU_Find_Valid_Packet(buffer, idx);
        if (start < 0) {
            if (idx >= BUFFER_SIZE - 1) idx = 0;
            continue;
        }

        if (start + PACKET_LENGTH <= idx) {
            IMU_Msg_Handler(&buffer[start], out_data);
            IMU_Shift_Buffer(buffer, &idx, start + PACKET_LENGTH);

            if (out_data->got_51 && out_data->got_52 && out_data->got_53){\
                Serial_Flush(UART1); // Flush the serial port to clear any remaining data
                return 1; // 모든 패킷 수신 완료
            }
                
        } else if (idx >= BUFFER_SIZE - 1) {
            idx = 0;
        }
    }

    return -1;
}

int32_t IMU_Init() {
    UART1 = Serial_Open(UART1_Port);
    if (UART1 < 0) {
        fprintf(stderr, "[ERROR] Failed to open port: %s\n", UART1_Port);
        return -1;
    }

    if (Serial_Configure(UART1, IMU_BaudRate) < 0) {
        fprintf(stderr, "[ERROR] Failed to configure port\n");
        Serial_Close(UART1);
        return -1;
    }

    return 1;
}