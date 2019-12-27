//*******************************************************************//
// File: EUSART.h                                                    //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description: UART Config and Use                                  //
//                                                                   //
//*******************************************************************//

#ifndef _EUSART_H /* Guard against multiple inclusion */
#define _EUSART_H

///////////////////////////////////////////////////////////////////////////////
//**************************** Includes *************************************//
///////////////////////////////////////////////////////////////////////////////
#include <xc.h>
#include "Config.h"
#include "Timers.h"
#include "Return_Types.h"

///////////////////////////////////////////////////////////////////////////////
//***************************** MACROS **************************************//
///////////////////////////////////////////////////////////////////////////////

#define RTS_TRIS                 TRISBbits.TRISB0
#define RTS_LAT                  LATBbits.LATB0
#define RTS_PORT                 PORTBbits.RB0
#define RTS_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define RTS_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define RTS_Toggle()             do {LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RTS_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define RTS_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define RTS_GetValue()           PORTBbits.RB0

#define CTS_TRIS                 TRISBbits.TRISB1
#define CTS_LAT                  LATBbits.LATB1
#define CTS_PORT                 PORTBbits.RB1
#define CTS_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define CTS_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define CTS_Toggle()             do {LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define CTS_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define CTS_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define CTS_GetValue()           PORTBbits.RB1

#define TX_TRIS                 TRISBbits.TRISB4
#define TX_LAT                  LATBbits.LATB4
#define TX_PORT                 PORTBbits.RB4
#define TX_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define TX_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define TX_Toggle()             do {LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define TX_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define TX_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define TX_GetValue()           PORTBbits.RB4

#define RX_TRIS                 TRISAbits.TRISA4
#define RX_LAT                  LATAbits.LATA4
#define RX_PORT                 PORTAbits.RA4
#define RX_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define RX_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define RX_Toggle()             do {LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define RX_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define RX_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define RX_GetValue()           PORTAbits.RA4

///////////////////////////////////////////////////////////////////////////////
//***************************** DEFINES *************************************//
///////////////////////////////////////////////////////////////////////////////

#define UART_SLOW_TIMEOUT   10//ms  
#define UART_FAST_TIMEOUT   5//ms
#define UART_1              1U
#define UART_2              2U
///////////////////////////////////////////////////////////////////////////////
//***************************** GLOBALS *************************************//
///////////////////////////////////////////////////////////////////////////////

uint32_t uart_timeout_cnt;
uint32_t uart_error_reg;

////////////////////////////////////////////////////////////////////////////////
//*****************************Prototypes*************************************//
////////////////////////////////////////////////////////////////////////////////

void UART_1_Init (uint32_t baudrate);
void UART_2_Init (uint32_t baudrate);
void Send_String_U1(uint8_t *ptr);
void Send_String_U2(uint8_t *ptr);
uint16_t Calc_Baud (uint8_t uart_device, uint32_t BaudRate);
uint8_t UART_Slow_Timeout (void);
uint8_t UART_Fast_Timeout (void);
uint8_t uart_transmit_buf(char *pBuf, uint32_t numBytes);

#endif