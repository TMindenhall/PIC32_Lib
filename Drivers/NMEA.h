/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NMEA.h
 * Author: tmindenh
 *
 * Created on December 24, 2019, 10:40 AM
 */

#ifndef NMEA_H
#define NMEA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define TIME_DEBUG

#ifdef TIME_DEBUG
#define NMEA_STRING "$GPGGA,172814.0,3723.46587704,N,12202.26957864,W,2,6,1.2,18.893,M,-25.669,M,2.0,0031*4F"
#define NMEA_SIZE sizeof(NMEA_STRING)
uint32_t NMEA_Copy (uint8_t * src, uint8_t * dest, uint16_t size);
#endif

#ifdef __cplusplus
extern "C" {
#endif

//#define NMEA_DEBUG   
    
    typedef struct  {
         struct {
            char NMEA_type [6];
            uint8_t UTC_hour;
            uint8_t UTC_min;
            uint8_t UTC_sec;
            float Lat;
            float Long;
            char Lat_Dir;
            char Long_Dir;
            uint8_t Fix;
            uint8_t Sats;
            float Hoz_dil;
            float Vert_elev;
            float Height_geoid;
            uint8_t Checksum;
        }GPGGA;
        
        struct {
           char NMEA_type[6];
           uint8_t fixType;
           uint8_t fixvalue;
           uint8_t PRNs [12];
           float PDOP;
           float HDOP;
           float VDOP; 
           uint8_t Checksum;
        }GPGSA;
        //Add more as needed
    }NMEA;
    
/* Need to move this to the struct with a union*/
uint8_t nmea_active;
uint8_t NMEA_Buffer [NMEA_SIZE];
uint8_t * nmea_ptr;
uint8_t * nmea_head; //Permanent Pointer    


void initNMEA (void);
int parseNMEA (char * NMEAbuffer,size_t NMEA_size);   
bool isStringValid (char * buffer);
bool lookAhead (char * ptr);
void printNMEAstate (int NMEA_State);
int moveNMEAState (int NMEA_State, int bytes_moved);
int get_NMEA_type (char * NMEA_type, int bytes_current);
int get_NMEA_time (char * ptr, int bytes_current);
int get_NMEA_Latitude (char * ptr, int bytes_current);
int get_NMEA_Longitude (char * ptr, int bytes_current);
int get_NMEA_fix (char * ptr, int bytes_current);
int get_NMEA_Sats (char * ptr, int bytes_current);
int get_NMEA_hozDilation (char * ptr, int bytes_current);
int get_NMEA_verticalElevation (char * ptr, int bytes_current);
int get_NMEA_heightGeoid (char * ptr, int bytes_current);
int get_NMEA_DGPS_age (char * ptr , int bytes_current);
int get_NMEA_DGPS_id (char * ptr, int bytes_current);
int get_NMEA_checksum (char * ptr , int bytes_current);
bool stringCompare (char * src, char * compare, size_t sizeString);
int stringCopy (char * src, char * dest, size_t stringSize);    

#ifdef __cplusplus
}
#endif

#endif /* NMEA_H */

