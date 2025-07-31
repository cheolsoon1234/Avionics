//
// Created by Cheolsoon on 25. 7. 23.
//
#include "Time.h"

const char* Get_Current_Date() {
    static char buffer[20];  // "YYYY-MM-DD HH:MM:SS" (총 19자 + NULL)
    time_t now = time(NULL);  // 현재 시간 (초 단위)
    struct tm* t = localtime(&now);  // 현지 시간 구조체로 변환

    // 문자열 포맷: YYYY-MM-DD HH:MM:SS
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", t);

    return buffer;
}

const char* Get_Current_Time() {
    static char buffer[32];  // "YYYY-MM-DD HH:MM:SS.mmm"
    struct timeval tv;
    struct tm* tm_info;

    gettimeofday(&tv, NULL);           // 초 + 마이크로초 단위 현재 시간
    tm_info = localtime(&tv.tv_sec);   // 초 단위 시간 → 로컬 시간 구조체

    // 날짜 및 시각 포맷 (초까지)
    strftime(buffer, sizeof(buffer), "%H:%M:%S", tm_info);

    // 밀리초 포맷 추가
    int32_t millisec = tv.tv_usec / 1000;
    sprintf(buffer + 8, ".%03d", millisec);  // 문자열 이어쓰기

    return buffer;
}

void Timer_Start() {
    gettimeofday(&start_time, NULL);
}

long Timer_Get_Elapsed_ms() {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);

    long seconds = current_time.tv_sec - start_time.tv_sec;
    long useconds = current_time.tv_usec - start_time.tv_usec;

    return (seconds * 1000) + (useconds / 1000);
}