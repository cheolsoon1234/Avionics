//
// Created by admin on 2025-07-03.
//

#include "Log.h"

FILE *LogFile = NULL;
int32_t LogFile_Open_Flag = 0;

void LOG_FILE_OPEN() {
    LogFile = fopen("../log/LogFile_"__TIME__"_"__DATE__".csv", "w+");
    if (LogFile == NULL) {
        fprintf(stderr, "[ERROR] Failed to open log file\n");
        exit(1);
    }
    LogFile_Open_Flag = 1;
    return;
}
void LOG_FILE_CLOSE() {
    if (LogFile == NULL) {
        fprintf(stderr, "[WARN] Log file is not open\n");
        return;
    }
    fclose(LogFile);
    LogFile_Open_Flag = 0;
    LogFile = NULL;
    return;
}
void LOG_WRITE(char* data, ...) {
    if (LogFile == NULL) {
        fprintf(stderr, "[WARN] Log file is not open\n");
        return;
    }
    fprintf(LogFile, "%s\r\n", data);
    return;
}
void LOG_WRITE_WITH_TIME(const char* format, ...) {
    if (LogFile == NULL) {
        fprintf(stderr, "[WARN] Log file is not open\n");
        return;
    }
    char message[512];
    va_list args;

    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);  // 가변 인자 -> 문자열로
    va_end(args);

    fprintf(LogFile, "%s, %s\r\n", Get_Current_Time(), message);
    return;
}



