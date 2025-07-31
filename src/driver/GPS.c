//
// Created by cheolsoon on 2025-07-22.
//

#include "GPS.h"

int32_t GPS_Serial_Connection_Flag = 0;
char *fields[MAX_FIELDS] = {0};



// ---------------------------
// 1. Checksum Calculation
// ---------------------------
static unsigned char GPS_Msg_CalcChecksum(const char* nmea) {
    unsigned char checksum = 0;
    if (*nmea == '$') nmea++;
    while (*nmea && *nmea != '*') {
        checksum ^= (unsigned char)(*nmea++);
    }
    return checksum;
}

// ---------------------------
// 2. Checksum Validation
// ---------------------------
static int32_t GPS_Msg_CheckChecksum(const char* nmea, unsigned char* out_checksum) {
    const char* star = strchr(nmea, '*');
    if (!star || strlen(star) < 3) return 0;

    uint32_t received = 0;
    if (sscanf(star + 1, "%2x", &received) != 1) return 0;

    unsigned char calc = GPS_Msg_CalcChecksum(nmea);
    if (out_checksum) *out_checksum = calc;

    return (calc == (unsigned char)received);
}
// ---------------------------
// 3. Tokenize NMEA Sentence
// ---------------------------
static int32_t GPS_Msg_Tokenize(char* nmea_body) {
    int32_t count = 0;
    fields[count++] = nmea_body;

    for (char* p = nmea_body; *p && count < MAX_FIELDS; ++p) {
        if (*p == ',') {
            *p = '\0';
            fields[count++] = p + 1;
        }
    }

    return count;
}

// ---------------------------
// 4. Parse Fields into Struct
// ---------------------------
static int32_t GPS_Msg_ParseFields(unsigned char checksum, GPS_Data_t *data) {
    if (!fields[0]) return 0;

    strncpy(data->sentenceID, fields[0], sizeof(data->sentenceID));
    strncpy(data->utc_time, fields[1], sizeof(data->utc_time));
    data->latitude = atof(fields[2]);
    data->lat_dir = fields[3][0];
    data->longitude = atof(fields[4]);
    data->lon_dir = fields[5][0];
    data->positionFix = atoi(fields[6]);
    data->satellitesUsed = atoi(fields[7]);
    data->HDOP = atof(fields[8]);
    data->altitude = atof(fields[9]);
    data->altitudeUnits = fields[10][0];
    data->GeoidSeperation = atof(fields[11]);
    data->SeperationUnits = fields[12][0];
    data->DGPSAge = (fields[13] && strlen(fields[13]) > 0) ? atof(fields[13]) : 0.0;
    data->DGPSStationID = (fields[14] && strlen(fields[14]) > 0) ? atoi(fields[14]) : 0;
    data->checksum = checksum;

    return 1;
}

// ---------------------------
// 5. Main GPS Message Handler
// ---------------------------
static int32_t GPS_Msg_Handler(const char* nmea, GPS_Data_t *data) {
    if (!nmea) return 0;
    if (strncmp(nmea, "$GPGGA", 6) != 0) {
        printf("[Info] Skipping unsupported NMEA sentence: %.6s\n", nmea);
        return 0;
    }

    unsigned char checksum = 0;
    if (!GPS_Msg_CheckChecksum(nmea, &checksum)) {
        printf("[Warning] Checksum mismatch: %s\n", nmea);
        return 0;
    }

    // 복사 후 * 잘라냄
    char buffer[256];
    strncpy(buffer, nmea, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    char* star = strchr(buffer, '*');
    if (star) *star = '\0';

    int32_t count = GPS_Msg_Tokenize(buffer);

    if (count < 15) {
        printf("[Warning] Too few fields (%d): %s\n", count, nmea);
        return 0;
    }

    if (!GPS_Msg_ParseFields(checksum, data)) {
        printf("[Warning] Failed to parse fields: %s\n", nmea);
        return 0;
    }

    return 1;
}

// 시리얼 수신 및 한 줄 단위 처리
int32_t GPS_Rx_Handler(GPS_Data_t *data) {
    char nmea[256] = {0};
    int32_t index = 0;
    char ch;

    for (size_t i = 0; i < MAX_LOOP_CNT; i++) {
        if (Serial_Read(UART2, &ch, 1) <= 0) return -1;
        nmea[index++] = ch;
        if (ch == '\n') break;
    }
    nmea[index] = '\0';

    // GPRMC 메시지만 처리
    if (strstr(nmea, "$GPGGA")) {
        if (!GPS_Msg_Handler(nmea, data)) {
            printf("[Warning] Checksum mismatch or invalid message: %s", nmea);
        }
    }

    return 1;
}

int32_t GPS_Init() {
    UART2 = Serial_Open(UART2_Port);
    if (UART2 < 0) {
        fprintf(stderr, "[ERROR] Failed to open port: %s\n", UART2_Port);
        return -1;
    }
    GPS_Serial_Connection_Flag = Serial_Configure(UART2, GPS_BAUDRATE);
    
    return (GPS_Serial_Connection_Flag == 1) ? 1 : -1;
}
