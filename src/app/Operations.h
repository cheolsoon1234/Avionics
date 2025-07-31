//
// Created by cheolsoon on 25. 7. 27.
//

#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdint.h>
#include "../common/BaseDef.h"
#include "../common/Time.h"
#include "../driver/IMU.h"
#include "../driver/GPS.h"
#include "../driver/BMP180.h"
#include "../driver/BT.h"
#include "../hal/CPU_Temp.h"
#include "../app/EJC_Handler.h"
#include "../app/Log.h"

#define FILTER_SIZE     20

#define EJC_ALT_DIFF    20.0f // EJC 사출 고도 차dl
#define EJC_ANGLE       75.0f // EJC 사출 각도 차이

extern IMU_Data_t       IMU_Data;
extern GPS_Data_t       GPS_Data;
extern BMP_Data_t       BMP_Data;
extern BMP180_t         Barometer;

extern float CPU_TEMP;

typedef struct {
    float Max_Alt_GPS_Filtered;
    float Cur_Alt_GPS_Filtered;
    float Max_Alt_BMP_Filtered;
    float Cur_Alt_BMP_Filtered;
    float Roll; 
    float Pitch; 
    float Yaw;

} Flight_Data_t;

typedef struct {
    float buffer[FILTER_SIZE];
    int index;
    int count;
    float sum;
} Moving_Average_Filter_t;

void CPU_Temp_Op();
void GPS_Op();
void GPS_Data_Init(GPS_Data_t* data);
void IMU_Op();
void BMP180_Op();
void EJC_Check_Conditions();
int32_t Filters_Init();

#endif // OPERATIONS_H