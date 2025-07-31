//
// Created by cheolsoon on 25. 7. 24.
//

#include "User_CMD_Handler.h"

static int32_t CMD_Checksum_Handler(const char* cmd) {
    unsigned char checksum = 0;
    const char* ptr = cmd;
    if (cmd == NULL || *cmd == '\0') {
        LOG_WRITE_WITH_TIME("USER CMD Checksum Not Valid (Empty Command)");
        printf("USER CMD Checksum Not Valid (Empty Command)\r\n");
        return -1;
    }
    while (*ptr && *ptr != '*') {
        checksum ^= (unsigned char)(*ptr++);
    }

    // '*' 문자가 없으면 잘못된 형식
    if (*ptr != '*') {
        LOG_WRITE_WITH_TIME("USER CMD Checksum Not Valid (No '*')");
        printf("USER CMD Checksum Not Valid (No '*')\r\n");
        return -1;
    }

    ptr++;  // '*' 다음 문자 (실제 checksum 문자)
    if (*ptr == '\0') {
        LOG_WRITE_WITH_TIME("USER CMD Checksum Not Valid (Missing checksum char)");
        printf("USER CMD Checksum Not Valid (Missing checksum char)\r\n");
        return -1;
    }

    unsigned char received_checksum = (unsigned char)(*ptr);

    if (checksum != received_checksum) {
        LOG_WRITE_WITH_TIME("USER CMD Checksum Failed (Calc: 0x%02X, Received: 0x%02X)", checksum, received_checksum);
        printf("USER CMD Checksum Failed (Calc: 0x%02X, Received: 0x%02X)\r\n", checksum, received_checksum);
        return -1;
    }

    return 1;

}

int32_t User_CMD_Handler() {
    char USER_CMD[10] = {0};
    
    if ( BT_Rx_Handler(USER_CMD, RESPONCE_MAX_LEN) != 1 ) return 0;
    if (USER_CMD[0] == '\0') return 0; // 빈 명령어 무시
    printf("USER CMD: %s\r\n", USER_CMD);

    if ( CMD_Checksum_Handler(USER_CMD) == -1 ) return -1;

    switch ( USER_CMD[3] ) {
        case '1':
            LOG_WRITE_WITH_TIME("USER CMD Received (CMD No. G001)");
            printf("USER CMD Received (CMD No. G001)\r\n");
            LAST_CMD_ID = 1;
            EJC_ARM();
            break;

        case '2':
            LOG_WRITE_WITH_TIME("USER CMD Received (CMD No. G002)");
            printf("USER CMD Received (CMD No. G002)\r\n");
            LAST_CMD_ID = 2;
            EJC_DISARM();
            break;

        case '3':
            LOG_WRITE_WITH_TIME("USER CMD Received (CMD No. G003)");
            printf("USER CMD Received (CMD No. G003)\r\n");
            LAST_CMD_ID = 3;
            EJC_Op_Force();
            break;

        case '4': 
            LOG_WRITE_WITH_TIME("USER CMD Received (CMD No. G004)");
            printf("USER CMD Received (CMD No. G004)\r\n");
            LAST_CMD_ID = 4;
            Timer_Start();
            break;

        default:
            break;
    }
    
    return USER_CMD[3];
}