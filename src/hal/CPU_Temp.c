//
// Created by cheolsoon on 25. 7. 21.
//

#include "CPU_Temp.h"

int32_t CPU_Temp_Int = -1;
FILE* CPU_Temp_File;
char* CPU_Temp_File_Path = "/sys/class/thermal/thermal_zone0/temp";


float Get_CPU_Temp() {
    CPU_Temp_File = fopen(CPU_Temp_File_Path, "r"); 

    if (!CPU_Temp_File) {
        printf("[ERROR] Failed to open CPU temperature file: %s\n", CPU_Temp_File_Path);
        return -1.0f; // 파일 열기 실패 시 -1.0 반환
    }

    fscanf(CPU_Temp_File, "%6d", &CPU_Temp_Int); // 온도 형식: 섭씨온도(소숫점 3자리) * 1000, 리눅스 의존성
    fclose(CPU_Temp_File);

    return (float)CPU_Temp_Int/1000; 
}

