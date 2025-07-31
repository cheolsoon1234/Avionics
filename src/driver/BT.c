//
// Created by cheolsoon on 25. 7. 23.
//
#include "BT.h"

int32_t BT_Serial_Connection_Flag = 0;
int32_t BT_Connection_Flag = 0;
int32_t Send_Flag = 0;

int32_t BT_Init() { // TODO Add init configurations 
    UART0 = Serial_Open(UART0_Port);
    if (UART0 < 0) {
        fprintf(stderr, "[ERROR] Failed to open port: %s\n", UART0_Port);
        return -1;
    }

    // BT_Serial_Connection_Flag = Serial_Configure(UART0, 9600);
    // BT_CMD_Send(AT);
    // BT_CMD_Send(AT_BTMODE,2);
    // BT_CMD_Send(AT_BTUART,230400,0,1); // UART Config (BaudRate: 230400, Parity: N, Stopbit: 1)
    // BT_CMD_Send(ATZ);
    BT_Serial_Connection_Flag = Serial_Configure(UART0, 230400);
    BT_CMD_Send(AT); // AT Command Mode
    BT_CMD_Send(AT_BTSCAN);

    return (BT_Serial_Connection_Flag == 1) ? 1 : -1;
}

static unsigned char BT_Checksum_Generator(const char* data) {
    unsigned char Tx_Checksum = 0;

    if (*data == '@') data++;
    while (*data && *data != '\0') {
        Tx_Checksum ^= (unsigned char)(*data++);
    }
    return Tx_Checksum;
}

int32_t BT_Rx_Handler(char* out, size_t max_len) {
    char ch;
    size_t idx = 0;
    int32_t loop_cnt = 0;

    while (idx < max_len - 1 && loop_cnt < MAX_LOOP_CNT) {
        if (Serial_Read(UART0, &ch, 1) <= 0) {
            loop_cnt++;
            continue;
        }

        if (ch == '\n' || ch == '\r') break;  // 패킷 종료 기준

        out[idx++] = ch;
        loop_cnt++;
    }
    out[idx] = '\0';
    return (idx > 0) ? 1 : 0;
}

int32_t BT_Tx_Handler(const char* data) {
    char buffer[512];
    strcpy(buffer, data);

    unsigned char checksum = BT_Checksum_Generator(data);
    size_t len = strlen(buffer);
    snprintf(buffer + len, sizeof(buffer) - len, "*%02X\r\n", checksum); // 송신 데이터 뒷부분에 체크섬과 \r\n 추가

    Send_Flag = Serial_Write(UART0, buffer);
    return (Send_Flag > 0) ? 1 : -1;
}

int32_t BT_CMD_Send(AT_CommandID id, ...) {
    char responce[RESPONCE_MAX_LEN]= {0};

    if (id < 0 || id >= AT_CMD_COUNT) return -1;

    char buffer[128];
    va_list args;
    va_start(args, id);
    vsnprintf(buffer, sizeof(buffer), AT_COMMANDS[id].template, args);
    va_end(args);
    
    Serial_Write(UART0, buffer);
    usleep(1000);
    while (responce[0] == '\0') { // 응답이 올 때까지 대기
        if (BT_Rx_Handler(responce, RESPONCE_MAX_LEN) < 0) {
            fprintf(stderr, "[ERROR] Failed to read BT response\n");
            return -1;
        }
    }
    // BT_Rx_Handler(responce, RESPONCE_MAX_LEN);
    LOG_WRITE_WITH_TIME("BT CMD : %s Responce: %s", AT_COMMANDS[id].template, responce);


    // 여러개 응답을 주는 명령에 대한 처리
    if (id == ATH)                   { BT_Rx_Handler(responce, RESPONCE_MAX_LEN); LOG_WRITE_WITH_TIME("BT CMD : %s Responce: %s", AT_COMMANDS[id].template, responce); }
    if (id == AT_BTSCAN)             { while (responce[0] != 'C') { BT_Rx_Handler(responce, RESPONCE_MAX_LEN); if (responce[0] != '\0') LOG_WRITE_WITH_TIME("BT CMD : %s Responce: %s", AT_COMMANDS[id].template, responce); BT_Connection_Flag = 1; } }
    if (id == AT_BTSCAN_N_T0)        { while (responce[0] != 'C') { BT_Rx_Handler(responce, RESPONCE_MAX_LEN); if (responce[0] != '\0') LOG_WRITE_WITH_TIME("BT CMD : %s Responce: %s", AT_COMMANDS[id].template, responce); BT_Connection_Flag = 1; } }
    if (id == AT_BTSCAN_NUMBER_T0)   { while (responce[0] != 'C') { BT_Rx_Handler(responce, RESPONCE_MAX_LEN); if (responce[0] != '\0') LOG_WRITE_WITH_TIME("BT CMD : %s Responce: %s", AT_COMMANDS[id].template, responce); BT_Connection_Flag = 1; } }
    if (id == ATD)                   {    if (responce[0] == 'O') { BT_Rx_Handler(responce, RESPONCE_MAX_LEN); LOG_WRITE_WITH_TIME("BT CMD : %s Responce: %s", AT_COMMANDS[id].template, responce); } }
    if (id == ATD_NUMBER)            {    if (responce[0] == 'O') { BT_Rx_Handler(responce, RESPONCE_MAX_LEN); LOG_WRITE_WITH_TIME("BT CMD : %s Responce: %s", AT_COMMANDS[id].template, responce); } }
    
    printf("responce : %s\r\n", responce); //For testing
    return 1;
}

int32_t BT_Close() {
    BT_CMD_Send(AT_ESCAPE);
    BT_CMD_Send(ATH);
    return 1;
}