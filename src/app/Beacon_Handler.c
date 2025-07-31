//
// Created by cheolsoon on 25. 7. 27.
//
#include "Beacon_Handler.h"

static void Beacon_Make_Message(char* out, size_t out_len) {
    // 날짜 및 시간
    const char* date = Get_Current_Date();
    const char* time_str = Get_Current_Time();

    unsigned char checksum = 0;
    int n = snprintf(out, out_len,
        "@0,%s,%s,%llu,%s,%.6f,%.6f,%.2f,%d,%.2f,%.3f,%.3f,%.3f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%.2f,%d,%d,%d",
        date,
        time_str,
        (unsigned long long int)AVIONICS_LOOP_CNT,
        GPS_Data.utc_time,
        GPS_Data.latitude,
        GPS_Data.longitude,
        GPS_Data.altitude,
        GPS_Data.satellitesUsed,        // GPS 위성 수
        GPS_Data.HDOP,                  // GPS HDOP
        IMU_Data.accel.ax,
        IMU_Data.accel.ay,
        IMU_Data.accel.az,
        IMU_Data.gyro.gx,
        IMU_Data.gyro.gy,
        IMU_Data.gyro.gz,
        IMU_Data.angle.roll,
        IMU_Data.angle.pitch,
        IMU_Data.angle.yaw,
        (IMU_Data.accel.temp+IMU_Data.gyro.temp+IMU_Data.angle.temp)/3.0f,
        BMP_Data.temp_c,
        BMP_Data.pressure_pa,
        BMP_Data.alt_m,
        EJC_ARM_FLAG,
        EJC_FLAG,
        LAST_CMD_ID
    );
    if (n < 0 || (size_t)n >= out_len) {
        if (out_len > 0) out[out_len - 1] = '\0';
        return;
    }
}

void Beacon_Send_Message() {
    char beacon_msg[512] = {0};
    Beacon_Make_Message(beacon_msg, sizeof(beacon_msg));
    if (BT_Tx_Handler(beacon_msg) < 0) {
        printf("[WARN] Beacon message send failed: %s\n", beacon_msg);
        LOG_WRITE_WITH_TIME("[WARN] Beacon message send failed: %s", beacon_msg);
    }
}