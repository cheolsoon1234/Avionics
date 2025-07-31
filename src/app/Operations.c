//
// Created by cheolsoon on 25. 7. 27.
//

#include "Operations.h"
IMU_Data_t IMU_Data = {0};
GPS_Data_t GPS_Data = {0};
BMP_Data_t BMP_Data = {0};
float CPU_Temp;

// Flight Data
Flight_Data_t Flight_Data = {1E-37, 1E-37, 1E-37, 1E-37, 0, 0, 0};

// Filter
Moving_Average_Filter_t GPS_Alt_Filter;
Moving_Average_Filter_t BMP_Alt_Filter;

BMP180_t Barometer;

// CPU Temp Operation
void CPU_Temp_Op() {
    CPU_Temp = Get_CPU_Temp();
    LOG_WRITE_WITH_TIME("CPU Tempature, %.3f ºC", CPU_Temp);
    return;
}

// GPS Operations

/*
static void GPS_Print() {
    printf("ID: %s\n", GPS_Data.sentenceID);
    printf("UTC: %s\n", GPS_Data.utc_time);
    printf("Lat: %.6f %c, Lon: %.6f %c\n", GPS_Data.latitude, GPS_Data.lat_dir, GPS_Data.longitude, GPS_Data.lon_dir);
    printf("Fix: %d, Sats: %d\n", GPS_Data.positionFix, GPS_Data.satellitesUsed);
    printf("HDOP: %.2f, Alt: %.2f %c\n", GPS_Data.HDOP, GPS_Data.altitude, GPS_Data.altitudeUnits);
    printf("GeoidSep: %.2f %c\n", GPS_Data.GeoidSeperation, GPS_Data.SeperationUnits);
    printf("DGPS Age: %.2f, Station ID: %d\n", GPS_Data.DGPSAge, GPS_Data.DGPSStationID);
    printf("Checksum: 0x%02X\n", GPS_Data.checksum);
    printf("\r\n");
}
*/
void GPS_Data_Init(GPS_Data_t* data) {
    if (!data) return;

    memset(data->sentenceID, 0, sizeof(data->sentenceID));
    memset(data->utc_time, 0, sizeof(data->utc_time));

    data->latitude = 0.0f;
    data->lat_dir = 'N';

    data->longitude = 0.0f;
    data->lon_dir = 'E';

    data->positionFix = 0;
    data->satellitesUsed = 0;
    data->HDOP = 0.0f;

    data->altitude = 0.0f;
    data->altitudeUnits = 'M';

    data->GeoidSeperation = 0.0f;
    data->SeperationUnits = 'M';

    data->DGPSAge = 0.0f;
    data->DGPSStationID = 0;

    data->checksum = 0;
}

void GPS_Op() {
    if (GPS_Rx_Handler(&GPS_Data) == 1) {
        LOG_WRITE_WITH_TIME("GPS Data,%s,%s,%.6f,%c,%.6f,%c,%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%02X", 
        GPS_Data.sentenceID, GPS_Data.utc_time, 
        GPS_Data.latitude,
        GPS_Data.lat_dir,
        GPS_Data.longitude,
        GPS_Data.lon_dir,
        GPS_Data.positionFix,
        GPS_Data.satellitesUsed,
        GPS_Data.HDOP,
        GPS_Data.altitude,
        GPS_Data.altitudeUnits,
        GPS_Data.GeoidSeperation,
        GPS_Data.SeperationUnits,
        GPS_Data.DGPSAge,
        GPS_Data.DGPSStationID,
        GPS_Data.checksum);
    } else {
        LOG_WRITE_WITH_TIME("[WARN],GPS packet not received in time");
    }
    return;
}

// IMU Operations
static void IMU_Log_Data(const IMU_Data_t* data) {
    if (!(data->got_51 && data->got_52 && data->got_53)) {
        LOG_WRITE_WITH_TIME("[WARN],Incomplete IMU data");
        return;
    }
    LOG_WRITE_WITH_TIME("IMU Data, %.3f,%.3f,%.3f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
        data->accel.ax, data->accel.ay, data->accel.az,
        data->gyro.gx, data->gyro.gy, data->gyro.gz,
        data->angle.roll, data->angle.pitch, data->angle.yaw, 
        (data->accel.temp + data->gyro.temp + data->angle.temp)/3
    );
    return;
}

void IMU_Op() {
    if (IMU_Rx_Handler(&IMU_Data) == 1) IMU_Log_Data(&IMU_Data);
    else LOG_WRITE_WITH_TIME("[WARN],IMU packet not received in time");
    return;
}

// BMP Operations
void BMP180_Op() {
    if (BMP180_Read(&Barometer, &BMP_Data) == 1) LOG_WRITE_WITH_TIME("BMP180 Data,%.1f,C,%d,Pa,%f,m",BMP_Data.temp_c, BMP_Data.pressure_pa, BMP_Data.alt_m);
    else LOG_WRITE_WITH_TIME("[WARN],BMP180 packet not received in time");
    return;
}

// Moving Average Filter
static void MovingAverage_Init(Moving_Average_Filter_t* filter) {
    for (int i = 0; i < FILTER_SIZE; ++i)
        filter->buffer[i] = 0.0f;

    filter->index = 0;
    filter->count = 0;
    filter->sum = -1E-37;
}

int32_t Filters_Init() {
    MovingAverage_Init(&GPS_Alt_Filter);
    MovingAverage_Init(&BMP_Alt_Filter);
    return 1;
}

static float MovingAverage_Update(Moving_Average_Filter_t* filter, float new_sample) {
    // 이전 값 제거
    filter->sum -= filter->buffer[filter->index];

    // 새 값 저장
    filter->buffer[filter->index] = new_sample;
    filter->sum += new_sample;

    // 인덱스 갱신
    filter->index = (filter->index + 1) % FILTER_SIZE;

    // 유효한 샘플 수 증가
    if (filter->count < FILTER_SIZE)
        filter->count++;

    // 평균 반환
    return filter->sum / filter->count;
}

static void Check_Is_Fired() {
    if (EGNITION_FLAG == 1) return;
    if (IMU_Data.accel.az > 2.2f) {
        EGNITION_FLAG = 1;
        Timer_Start();
        LOG_WRITE_WITH_TIME("EGNITION_FLAG SET");
        printf("EGNITION_FLAG SET\n");
    }
    return;
}

// Flight data Update
static void Update_Filght_Data() { // 이동 평균으로 계산하기

    Flight_Data.Cur_Alt_GPS_Filtered = MovingAverage_Update(&GPS_Alt_Filter, GPS_Data.altitude);
    Flight_Data.Cur_Alt_BMP_Filtered = MovingAverage_Update(&BMP_Alt_Filter, BMP_Data.alt_m);
    if (Flight_Data.Max_Alt_GPS_Filtered < Flight_Data.Cur_Alt_GPS_Filtered) Flight_Data.Max_Alt_GPS_Filtered = Flight_Data.Cur_Alt_GPS_Filtered;
    if (Flight_Data.Max_Alt_BMP_Filtered < Flight_Data.Cur_Alt_BMP_Filtered) Flight_Data.Max_Alt_BMP_Filtered = Flight_Data.Cur_Alt_BMP_Filtered;
    
    Flight_Data.Roll = IMU_Data.angle.roll;
    Flight_Data.Pitch = IMU_Data.angle.pitch;
    Flight_Data.Yaw = IMU_Data.angle.yaw;

    printf("GPS MAX ALT: %.2f\tCUR ALT %.2f  \nBMP MAX ALT: %.2f\tCUR ALT: %.2f  \nR: %.2f\tP: %.2f\tY: %.2f\n", 
        Flight_Data.Max_Alt_GPS_Filtered, Flight_Data.Cur_Alt_GPS_Filtered, 
        Flight_Data.Max_Alt_BMP_Filtered, Flight_Data.Cur_Alt_BMP_Filtered,
        Flight_Data.Roll,
        Flight_Data.Pitch,
        Flight_Data.Yaw
    );

    return;
}

// Check Ejection Condition 
void EJC_Check_Conditions() { // TODO Add EJC Conditions
    Update_Filght_Data();
    Check_Is_Fired();

    if (EJC_FLAG == 1)                                                              return; // 이미 사출된 경우 사출 조건 체크 안함
    if ((Flight_Data.Max_Alt_GPS_Filtered - GPS_Data.altitude) >= 10.0) { EJC_Op(); return; }
    if ((Flight_Data.Max_Alt_BMP_Filtered - BMP_Data.alt_m) >= 10.0)    { EJC_Op(); return; }
    if (Flight_Data.Roll > 90.0 || Flight_Data.Roll < -90.0)            { EJC_Op(); return; }
    if (Flight_Data.Pitch > 90.0 || Flight_Data.Pitch < -90.0)          { EJC_Op(); return; }
    if (Timer_Get_Elapsed_ms() > (EXP_PEAK_TIME + 1.5)*1000)            { EJC_Op(); return; }

    return;
}