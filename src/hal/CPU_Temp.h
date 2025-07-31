//
// Created by cheolsoon on 25. 7. 21.
//

#ifndef CPU_TEMP_H
#define CPU_TEMP_H

#include <stdio.h>
#include <stdint.h>

extern int32_t CPU_Temp_Int;
extern FILE* CPU_Temp_File;
extern char* CPU_Temp_File_Path;

float Get_CPU_Temp();

#endif // CPU_TEMP_H
