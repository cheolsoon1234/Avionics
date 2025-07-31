////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                               Avionics Program                                                     //
// Copyright © 2025 Cheolsoon Han(cheolsoon@gm.gist.ac.kr) All rights reserved                                        //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <signal.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "common/BaseDef.h"
#include "common/Time.h"

#include "driver/GPS.h"
#include "driver/IMU.h"
#include "driver/BMP180.h"
#include "driver/BT.h"

#include "app/Log.h"
#include "app/Operations.h"
#include "app/User_CMD_Handler.h"
#include "app/EJC_Handler.h"
#include "app/Beacon_Handler.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 초기화 처리 함수
static void Init( void ) {
    LOG_FILE_OPEN();
    LOG_WRITE("AVIONICS PROGRAM");
    LOG_WRITE("Copyright © 2025 Cheolsoon Han(cheolsoon@gm.gist.ac.kr) All rights reserved");
    LOG_WRITE("Last Compiled At "__TIMESTAMP__);
    LOG_WRITE_WITH_TIME("Avionics INIT START");

    printf("AVIONICS PROGRAM\n");
    printf("Copyright © 2025 Cheolsoon Han(cheolsoon@gm.gist.ac.kr) All rights reserved\n");
    printf("Last Compiled At "__TIMESTAMP__"\n");
    printf("Avionics INIT START\n");

    // INIT OPERATIONS
    if (GPS_Init())                         { LOG_WRITE("GPS INIT OK");         printf("GPS INIT OK\n");                }
    else                                    { LOG_WRITE("GPS INIT NOT OK");     printf("GPS INIT NOT OK\n");    exit(1);}
    if (IMU_Init())                         { LOG_WRITE("IMU INIT OK");         printf("IMU INIT OK\n");                }
    else                                    { LOG_WRITE("IMU INIT NOT OK");     printf("IMU INIT NOT OK\n");    exit(1);}
    if (BMP180_Init(&Barometer, I2C_Port))  { LOG_WRITE("BMP INIT OK");         printf("BMP INIT OK\n");                }
    else                                    { LOG_WRITE("BMP INIT NOT OK");     printf("BMP INIT NOT OK\n");    exit(1);}
    if (EJC_Init())                         { LOG_WRITE("EJC INIT OK");         printf("EJC INIT OK\n");                }
    else                                    { LOG_WRITE("EJC INIT NOT OK");     printf("EJC INIT NOT OK\n");    exit(1);}
    if (Filters_Init())                     { LOG_WRITE("FILTER INIT OK");      printf("FILTER INIT OK\n");             }
    else                                    { LOG_WRITE("FILTER INIT NOT OK");  printf("FILTER INIT NOT OK\n"); exit(1);}
    if (BT_Init())                          { LOG_WRITE("BT INIT OK");          printf("BT INIT OK\n");                 } 
    else                                    { LOG_WRITE("BT INIT NOT OK");      printf("BT INIT NOT OK\n");     exit(1);}
    GPS_Data_Init(&GPS_Data);

    LOG_WRITE_WITH_TIME("Avionics INIT OK");
    LOG_WRITE_WITH_TIME("Avionics LOOP START");

    printf("Avionics INIT OK\n");
    printf("Avionics LOOP START\n");
    return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 메인 함수 종료시 처리 함수

static void At_Exit( void ) {

    if (BT_Connection_Flag) BT_Close();

    Serial_Close(UART0);
    Serial_Close(UART1);
    Serial_Close(UART2);
    I2C_Close(Barometer.fd);

    EJC_Close();
    
    LOG_WRITE_WITH_TIME("Main Loop Terminated");
    
    LOG_FILE_CLOSE();
    
    printf("Avionics Terminated Successfully\r\n");
    return;
}

static void At_Abort(int sig) {
    printf("System Aborted (Abort Signal: %d)\r\n", sig);
    LOG_WRITE_WITH_TIME("Avionics System Aborted (Abort Signal: %d)", sig);
    exit(1);
    return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 메인 루프
int main( int __argc, char** __argv ) {
    atexit(At_Exit);
    signal(SIGABRT, At_Abort);  // abort() 호출 시
    signal(SIGINT,  At_Abort);  // Ctrl+C 눌렀을 때
    signal(SIGTERM, At_Abort);  // kill 명령어 등

    
    Init();

    while (1) { // MAIN LOOP
        // printf("AVIONICS LOOP: %d\n", AVIONICS_LOOP_CNT);
        CPU_Temp_Op();          
        // printf("CPU Temp Op\n");
        IMU_Op();               
        // printf("IMU Op\n");
        GPS_Op();               
        // printf("GPS Op\n");
        BMP180_Op();            
        // printf("BMP180 Op\n");
        EJC_Check_Conditions(); 
        // printf("Check EJC Condition\n");
        User_CMD_Handler();    
        // printf("USER CMD HANDLER\n");
        Beacon_Send_Message();         
        AVIONICS_LOOP_CNT++;
    }

    // UART0 = Serial_Open(UART0_Port);
    // if (UART0 < 0) {
    //     fprintf(stderr, "[ERROR] Failed to open UART0\n");
    //     exit(1);
    // }
    // if (Serial_Configure(UART0, 230400) < 0) {
    //     fprintf(stderr, "[ERROR] Failed to configure UART0\n");
    //     exit(1);
    // }
    // BT_CMD_Send(AT); // AT Command Mode
    // Serial_Write(UART0, "+++");
    // Serial_Write(UART0, "ATH\r");
    // Serial_Write(UART0, "AT\r");
    // Serial_Write(UART0, "ATZ\r");
    // Serial_Write(UART0, "AT+BTUART,230400,N,1\r");

    // while (1) {
    //     char buffer = 0;
    //     Serial_Read(UART0, &buffer, 1);
    //     if (buffer != '\0') printf("%c", buffer); // Print the received character
    // }
    // LOG_FILE_OPEN();

    // IMU_Init();
    // while (1) {
    //     IMU_Op();
    //     printf("Accel: \t[%.2f,\t%.2f,\t%.2f]\nGyro: \t[%.2f,\t%.2f,\t%.2f]\nAngle: \t[%.2f,\t%.2f,\t%.2f]\n",
    //            IMU_Data.accel.ax, IMU_Data.accel.ay, IMU_Data.accel.az,
    //            IMU_Data.gyro.gx, IMU_Data.gyro.gy, IMU_Data.gyro.gz,
    //            IMU_Data.angle.roll, IMU_Data.angle.pitch, IMU_Data.angle.yaw);
    //            // usleep(10000);
    // }


    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
