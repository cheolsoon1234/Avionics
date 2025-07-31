//
// Created by admin on 2025-07-03.
//

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

#include "../common/Time.h"

extern FILE *LogFile;
extern int32_t LogFile_Open_Flag;

void LOG_FILE_OPEN();
void LOG_FILE_CLOSE();
void LOG_WRITE(char*, ...);
void LOG_WRITE_WITH_TIME(const char*, ...);

#endif // LOG_H
