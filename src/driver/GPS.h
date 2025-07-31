//
// Created by cheolsoon on 2025-07-22.
//

#ifndef GPS_H
#define GPS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common/BaseDef.h"
#include "../hal/Serial_IO.h"

#define MAX_FIELDS      20
#define GPS_BAUDRATE    9600

extern int32_t GPS_Serial_Connection_Flag;
extern char *fields[MAX_FIELDS];

typedef struct { // For $GPGGA Format
    char sentenceID[7];         // Sentence ID
    char utc_time[16];          // UTC Time ( hhmmss.sss )
    float latitude;             // Latitude ( ddmm.mmmm )
    char lat_dir;               // 'N' = North or 'S' = South
    float longitude;            // Longitude ( dddmm.mmmm )
    char lon_dir;               // 'E' = East or 'W' = West
    int32_t positionFix;            // 0 = Invalid, 1 = Valid SPS, 2 = Valid DGPS, 3 = Valid PPS
    int32_t satellitesUsed;         // Satellites being used (0-12) 2 digit integer (04)
    float HDOP;                 // Horizontal dilution of precision
    float altitude;             // Altitude in meters according to WGS-84 ellipsoid
    char altitudeUnits;         // 'M' = Meters
    float GeoidSeperation;      // Geoid Seperation
    char SeperationUnits;       // 'M' = Meters
    float DGPSAge;              // Age of DGPS data in seconds
    int32_t DGPSStationID;          // (0000)
    unsigned char checksum;     // Checksum (XOR)
} GPS_Data_t;



int32_t GPS_Init();
int32_t GPS_Rx_Handler(GPS_Data_t *data);
// void GPS_Print();
// int32_t GPS_Op();


#endif // GPS_H