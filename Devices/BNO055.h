//*******************************************************************//
// File: BNO055.h                                                    //
// Author: Thomas Mindenhall, Austin Fagen                           //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description: IMU Functions.                                       //
//                                                                   //
//*******************************************************************//

//Guard against multiple recurr.
#ifndef _BNO055_H
#define _BNO055_H

///////////////////////////////////////////////////////////////////////////////
//*****************************Includes**************************************//
///////////////////////////////////////////////////////////////////////////////
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "CONFIG.h"
#include "I2C.h"
#include "Timers.h"
//#include "ILI9341.h"

////////////////////////////////////////////////////////////////////////////////
//*******************************Defines**************************************//
////////////////////////////////////////////////////////////////////////////////

#define BNO_DEVICE          0x28
#define HIGH_SPEED          400000L
#define LOW_SPEED           100000L

////////////////////////////////////////////////////////////////////////////////
//*******************************Page 0***************************************//
////////////////////////////////////////////////////////////////////////////////

#define CHIP_ID             0x00
#define ACC_ID              0x01
#define MAG_ID              0x02
#define GYR_ID              0x03
#define SW_REV_ID_LSB       0x04
#define SW_REV_ID_MSB       0x05
#define BL_REV_ID           0x06
#define PAGE_ID             0x07

#define ACC_X_LSB           0x08
#define ACC_X_MSB           0x09
#define ACC_Y_LSB           0x0A
#define ACC_Y_MSB           0x0B
#define ACC_Z_LSB           0x0C
#define ACC_Z_MSB           0x0D

#define MAG_X_LSB           0x0E
#define MAG_X_MSB           0x0F
#define MAG_Y_LSB           0x10
#define MAG_Y_MSB           0x11
#define MAG_Z_LSB           0x12
#define MAG_Z_MSB           0x13

#define GYR_X_LSB           0x14
#define GYR_X_MSB           0x15
#define GYR_Y_LSB           0x16
#define GYR_Y_MSB           0x17
#define GYR_Z_LSB           0x18
#define GYR_Z_MSB           0x19

#define EUL_HEAD_LSB        0x1A
#define EUL_HEAD_MSB        0x1B
#define EUL_ROLL_LSB        0x1C
#define EUL_ROLL_MSB        0x1D
#define EUL_PITCH_LSB       0x1E
#define EUL_PITCH_MSB       0x1F

#define QUA_W_LSB           0x20
#define QUA_W_MSB           0x21
#define QUA_X_LSB           0x22
#define QUA_X_MSB           0x23
#define QUA_Y_LSB           0x24
#define QUA_Y_MSB           0x25
#define QUA_Z_LSB           0x26
#define QUA_Z_MSB           0x27

#define LIA_X_LSB           0x28
#define LIA_X_MSB           0x29
#define LIA_Y_LSB           0x2A
#define LIA_Y_MSB           0x2B
#define LIA_Z_LSB           0x2C
#define LIA_Z_MSB           0x2D

#define GRV_X_LSB           0x2E
#define GRV_X_MSB           0x2F
#define GRV_Y_LSB           0x30
#define GRV_Y_MSB           0x31
#define GRV_Z_LSB           0x32
#define GRV_Z_MSB           0x33

#define TEMPURATURE         0x34

#define CALIB_STAT          0x35
#define ST_RESULT           0x36
#define INT_STA             0x37
#define SYS_CLK_STAT        0x38
#define SYS_STAT            0x39
#define SYS_ERROR           0x3A
#define UNIT_SEL            0x3B
#define OPR_MODE            0x3D      
#define PWR_MODE            0x3E
#define SYS_TRIG            0x3F
#define TEMP_SOURCE         0x40
#define AXIS_MAP_CONFIG     0x41
#define AXIS_MAP_SIGN       0x42

#define ACC_X_OFF_LSB       0x55
#define ACC_X_OFF_MSB       0x56
#define ACC_Y_OFF_LSB       0x57
#define ACC_Y_OFF_MSB       0x58
#define ACC_Z_OFF_LSB       0x59
#define ACC_Z_OFF_MSB       0x5A

#define MAG_X_OFF_LSB       0x5B
#define MAG_X_OFF_MSB       0x5C
#define MAG_Y_OFF_LSB       0x5D
#define MAG_Y_OFF_MSB       0x5E
#define MAG_Z_OFF_LSB       0x5F
#define MAG_Z_OFF_MSB       0x60

#define GYR_X_OFF_LSB       0x61
#define GYR_X_OFF_MSB       0x62
#define GYR_Y_OFF_LSB       0x63
#define GYR_Y_OFF_MSB       0x64
#define GYR_Z_OFF_LSB       0x65
#define GYR_Z_OFF_MSB       0x66

#define ACC_RAD_LSB         0x67
#define ACC_RAD_MSB         0x68

#define MAG_RAD_LSB         0x69
#define MAG_RAD_MSB         0x6A

////////////////////////////////////////////////////////////////////////////////
//*******************************Page 1***************************************//
////////////////////////////////////////////////////////////////////////////////

#define ACC_CONFIG          0x08
#define MAG_CONFIG          0x09
#define GYR_CONFIG_A        0x0A
#define GYR_CONFIG_B        0x0B
#define ACC_SLEEP_CONFIG    0x0C
#define GYR_SLEEP_CONFIG    0x0D
#define INT_MSK             0x0F
#define INT_EN              0x10

#define ACC_AM_THRES        0x11
#define ACC_INT_SETT        0x12
#define ACC_HG_DURATION     0x13
#define ACC_HG_THRES        0x14
#define ACC_NM_THRES        0x15
#define ACC_NM_SETT         0x16

#define GYR_INT_SETT        0x17
#define GYR_HR_X_SET        0x18
#define GYR_DUR_X           0x19
#define GYR_HR_Y_SET        0x1A
#define GYR_DUR_Y           0x1B
#define GYR_HR_Z_SET        0x1C
#define GYR_DUR_Z           0x1D

#define GYR_AM_THRES        0x1E
#define GYR_AM_SET          0x1F

#define UNIQUE_ID           0x50 // - 0x5F

//#define PI 3.14159265
#define DEG_2_RAD M_PI / 180
#define RAD_2_DEG 180 / M_PI

////////////////////////////////////////////////////////////////////////////////
//**************************Structures****************************************//
////////////////////////////////////////////////////////////////////////////////

typedef struct BNO_deviceTag {
    /*************************DYNAMIC SENSOR VALUES****************************/
    double acc_x, acc_y, acc_z;
    double gyr_x, gyr_y, gyr_z;
    double mag_x, mag_y, mag_z;
    double gravity_x, gravity_y, gravity_z;
    double lin_acc_x, lin_acc_y, lin_acc_z;
    double eul_heading, eul_roll, eul_pitch;
    int16_t qua_w, qua_x, qua_y, qua_z;
    int8_t temp;

    /*************************STATIC CALIBRATION VALUES****************************/
    uint16_t acc_offset_x, acc_offset_y, acc_offset_z;
    uint16_t gyr_offset_x, gyr_offset_y, gyr_offset_z;
    uint16_t mag_offset_x, mag_offset_y, mag_offset_z;
    uint16_t acc_radius, mag_radius;

    /*************************CONFIGURATION REGS****************************/
    uint8_t calib_stat, sys_cal, gyr_cal, acc_cal, mag_cal;
    uint8_t self_test_rslt;
    uint8_t bno_int_reg;
    uint8_t sys_clk_stat;
    uint8_t sys_err_reg;
    uint8_t unit_reg;
    uint8_t opr_mode;
    uint8_t pwr_mode;
    uint8_t temp_source;
    uint8_t axis_map_config, axis_map_sign;
    uint8_t acc_id; uint8_t gyr_id; uint8_t mag_id; uint8_t chip_id;
    uint16_t sw_rev_id;
    uint8_t bootloader_rev_id;
    uint8_t page_id;


} BNO_device;
////////////////////////////////////////////////////////////////////////////////
//*********************************GLOBALS************************************//
////////////////////////////////////////////////////////////////////////////////
/**********************************IMU VECTOR**********************************/

double mag_unit_x, mag_unit_y, mag_unit_z;
double correction_vector_x, correction_vector_y, correction_vector_z;
double sum_lin_x, sum_lin_y, sum_lin_z;

/*****************************TIME DEPENDANT VAR*******************************/
double distance;
int32_t acc_count;
double last_distance_x,last_distance_y,last_distance_z; //may need these
double projection;
double magnitude;
double heading;
double total_distance_r2;
uint16_t i;
//double delta_time = .00003; // fixed 30uS (time to read a register)
/******************************************************************************/


////////////////////////////////////////////////////////////////////////////////
//*****************************Prototypes*************************************//
////////////////////////////////////////////////////////////////////////////////

void BNO_Init(uint32_t baud_rate);
void BNO_Cal_Routine(void);
void BNO_Man_Update_ACC(void);
void BNO_Man_Update_GYR(void);
void BNO_Man_Update_MAG(void);
void BNO_Man_Update_LIN(void);
void BNO_Full_Man_Update(void);
void Read_LIN(void);
void Correct_Vectors (void);
void Start_Delta_Time(void);
uint16_t Read_Delta_Time(void);
double Compute_Delta_Time(void);
double Compute_Position(void);
double Get_Delta_Time(void);
void Get_Orientation(void);

#endif
/* END OF FILE*/