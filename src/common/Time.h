//
// Created by Cheolsoon on 25. 7. 23.
//
#ifndef TIME_H
#define TIME_H

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

static struct timeval start_time;

const char* Get_Current_Date();
const char* Get_Current_Time();
void Timer_Start();
long Timer_Get_Elapsed_ms();

#endif // TIME_H