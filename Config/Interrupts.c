//*******************************************************************//
// File: Interrupts.c                                                //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
// Compiler: XC32                                                    //
// MCU: PIC32MX170F256B                                              // 
//                                                                   //
// Description: Contains all the ISR's for the system                //
//                                                                   //
//*******************************************************************// 

///////////////////////////////////////////////////////////////////////////////
//*****************************Includes**************************************//
///////////////////////////////////////////////////////////////////////////////
#include <xc.h>
#include <sys/attribs.h>
//#include "MT3339.h"
//#include "ILI9341.h"
#include "NMEA.h"

////////////////////////////////////////////////////////////////////////////////
//*******************************ISR's****************************************//
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Description: ISR for UART 1 --> RX. ISR has two functions.
 * 1) Echo mode: Echos char's received.
 * 2) GPS mode: Fills GPS strings to NMEA_XFER_BUFF as the sight of $ char.
 * 
 * Inputs: UART1 Vector, Priority Level.
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void __ISR(_UART_1_VECTOR, ipl7AUTO) UART_1_RX_ISR(void) {
    uint8_t rx;
    //Echo Code for Debugging
    if (IFS1bits.U1RXIF) {
        rx = U1RXREG;
        if (rx == '$' && !nmea_active) { //wait for a new string to occur 
            *nmea_ptr = rx;
            nmea_ptr++;
            nmea_active = 1;
        }

        if (rx == '\r' && nmea_active) {
            nmea_active = 0;
            /* Clear Buffer and Start Parsing */
            nmea_ptr = nmea_head;
        }
        if (nmea_active) {
            *nmea_ptr = rx;
            nmea_ptr++;
        } else {
            /* Do Nothing but wait for next start of string */
        }
        IFS1bits.U1RXIF = 0;
    }
}

/******************************************************************************
 * Description: ISR for UART 2 --> RX. Echo mode: Echos char's received.
 * 
 * Inputs: UART2 Vector, Priority Level.
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void __ISR(_UART_2_VECTOR, ipl7SOFT) UART_2_RX_ISR(void) {
    uint8_t rx;
    //Add RX handler
    if (IFS1bits.U2RXIF) {
        if (rx == '$' && !nmea_active) { //wait for a new string to occur 
            *nmea_ptr = rx;
            nmea_ptr++;
            nmea_active = 1;
        }

        if (rx == '\r' && nmea_active) {
            /* Start Parsing and Clear Buffer*/
            nmea_active = 0;
            /* Clear Buffer and Start Parsing */
            nmea_ptr = nmea_head;
        }
        if (nmea_active) {
            *nmea_ptr = rx;
            nmea_ptr++;
        } else {
            /* Do Nothing but wait for next start of string */
        }
        IFS1bits.U2RXIF = 0;
    }
}

/* END OF FILE */
