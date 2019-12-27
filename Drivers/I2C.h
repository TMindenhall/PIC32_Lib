//*******************************************************************//
// File: I2C.h                                                       //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description: I2C Config and Use                                   //
//                                                                   //
//*******************************************************************//

#ifndef _I2C_H    /* Guard against multiple inclusion */
#define _I2C_H

///////////////////////////////////////////////////////////////////////////////
//*****************************Includes**************************************//
///////////////////////////////////////////////////////////////////////////////
#include <xc.h>
#include <stdint.h>
#include <stddef.h>

#include "Timers.h"
#include "Return_Types.h"
//#include "BNO055.h" //Used for Device Addresses


///////////////////////////////////////////////////////////////////////////////
//******************************DEFINES**************************************//
///////////////////////////////////////////////////////////////////////////////
#define I2C_TIMEOUT 10 //ms
#define I2C_DUMMY_DATA 0x00
#define I2C_READ   1
#define I2C_WRITE  0


// get/set SCL aliases
#define SCL_TRIS                 TRISBbits.TRISB8
#define SCL_LAT                  LATBbits.LATB8
#define SCL_PORT                 PORTBbits.RB8
#define SCL_SetHigh()            do { LATBbits.LATB8 = 1; } while(0)
#define SCL_SetLow()             do { LATBbits.LATB8 = 0; } while(0)
#define SCL_Toggle()             do {LATBbits.LATB8 = ~LATBbits.LATB8; } while(0)
#define SCL_SetDigitalInput()    do { TRISBbits.TRISB8 = 1; } while(0)
#define SCL_SetDigitalOutput()   do { TRISBbits.TRISB8 = 0; } while(0)
#define SCL_GetValue()           PORTBbits.RB8

// get/set SDA aliases
#define SDA_TRIS                 TRISBbits.TRISB9
#define SDA_LAT                  LATBbits.LATB9
#define SDA_PORT                 PORTBbits.RB9
#define SDA_SetHigh()            do { LATBbits.LATB9 = 1; } while(0)
#define SDA_SetLow()             do { LATBbits.LATB9 = 0; } while(0)
#define SDA_SetDigitalInput()    do { TRISBbits.TRISB9 = 1; } while(0)
#define SDA_SetDigitalOutput()   do { TRISBbits.TRISB9 = 0; } while(0)
#define SDA_Toggle()             do { LATBbits.LATB9 = ~LATBbits.LATB9; } while(0)
#define SDA_GetValue()           PORTBbits.RB9

#define START_FAILED            0x00000001
#define STOP_FAILED             0x00000002
#define RSTART_FAILED           0x00000004
#define SEND_FAILED             0x00000008
#define RECV_FAILED             0x00000010
#define ACK_NAK_FAILED          0x00000020

#define I2C_1                   1U
#define I2C_2                   2U

////////////////////////////////////////////////////////////////////////////////
//*********************************GLOBALS************************************//
////////////////////////////////////////////////////////////////////////////////
int16_t i2c_init_flag;
int8_t Send_Buffer[64];
int8_t Recieve_Buffer[64]; //For Repeated Read

////////////////////////////////////////////////////////////////////////////////
//*****************************Prototypes*************************************//
////////////////////////////////////////////////////////////////////////////////
void I2C_1_Init(uint32_t baud_rate); 
uint8_t I2C_Timeout (void);
void I2C_Mode_ON (void);
void I2C_Mode_OFF (void);
uint8_t I2C_Start (void);
uint8_t I2C_Stop (void);
uint8_t I2C_Repeated_Start (void);
void I2C_Load_Buff (uint8_t * data);
uint8_t I2C_Read_Buff (void);
uint8_t I2C_Read_ACK (void);
uint8_t I2C_1_Read(uint8_t device_adr, uint16_t reg_adr, uint8_t * data_ptr, uint8_t numBytes);
uint8_t I2C_1_Write(uint8_t device_adr, uint16_t reg_adr, uint8_t * data_ptr, uint8_t numBytes);
int8_t Xfer_Int (uint8_t adr);
uint16_t Read_Flag(void);
uint16_t Calc_I2C_Baud (uint32_t baud_rate);

#endif

/* *****************************************************************************
 End of File
 */