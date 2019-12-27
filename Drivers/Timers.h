//*******************************************************************//
// File: Timers.h                                                     //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description: Delay and Timer Functions                            //
//                                                                   //
//*******************************************************************//
#ifndef TIMERS_H /* Guard against multiple inclusion */
#define TIMERS_H

///////////////////////////////////////////////////////////////////////////////
//*****************************Includes**************************************//
///////////////////////////////////////////////////////////////////////////////
#include <xc.h>
#include <stdint.h>
#include "CONFIG.h"


volatile uint8_t timer1Flag;
uint32_t millisSinceBoot;
uint8_t timerInitFlag;
////////////////////////////////////////////////////////////////////////////////
//*****************************Prototypes*************************************//
////////////////////////////////////////////////////////////////////////////////
void Timer_1_Init(void);
void Timer_1_Start(void);
void Timer_1_Stop(void);
void Timer_1_Reset(void);
void Timer_1_Load_Reset_Value(void);
void Null_Timer_1 (void);
uint16_t Timer_1_Read(void);
void Delay_ms(uint16_t ms);
uint32_t Get_Millis_Since_Boot (void);
void Timer_1_ISR (void);

#endif