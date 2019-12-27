//*******************************************************************//
// File: Timers.c                                                    //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description: Delay and Timer Functions                            //
//                                                                   //
//*******************************************************************//

///////////////////////////////////////////////////////////////////////////////
//*****************************Includes**************************************//
///////////////////////////////////////////////////////////////////////////////
#include "Timers.h"
#include "Config.h"

////////////////////////////////////////////////////////////////////////////////
//*******************************FUNCTIONS************************************//
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Description: Initializes Timer 1
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Timer_1_Init(void){
    
    timer1Flag = 0;
    millisSinceBoot = 0;
    
    Timer_1_Stop();         //disable Timer
    Timer_1_Load_Reset_Value();
    T1CONbits.TCKPS = 1;    //1:8 Clock Prescale
    T1CONbits.TCS = 0;      //Timer Clock Source set to internal peripheral clock
    
    
}

/******************************************************************************
 * Description: Starts Timer 1
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Timer_1_Start(void){   //turn the timer on
    T1CONbits.ON = 1;
    timerInitFlag = 1;
}

/******************************************************************************
 * Description: Stops Timer 1
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Timer_1_Stop(void){    //turn the timer off
     T1CONbits.ON = 0;
     timerInitFlag = 0;
}

/******************************************************************************
 * Description: Resets Timer 1 and Resets to Load Value.
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Timer_1_Reset(void){   //resets the timer with the value of 63036
    Timer_1_Stop();
    Timer_1_Load_Reset_Value();
    Timer_1_Start();
}

/******************************************************************************
 * Description: Resets Timer 1 to a value
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Timer_1_Load_Reset_Value(void){   //resets the timer with the value of 63036
    TMR1 = 0xF63C;
}
/******************************************************************************
 * Description: Reads the Timer 1 register value
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: Value of register as an integer.
 ******************************************************************************/
uint16_t Timer_1_Read(void){
    return TMR1;
}
/******************************************************************************
 * Description: Zeros Timer 1
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Null_Timer_1 (void){
    TMR1 = 0;
}

/******************************************************************************
 * Description: Uses Timer 1 to create a processor delay in milliseconds
 * 
 * Inputs: Value as an integer (How many millis you want to delay).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Delay_ms(uint16_t ms){
    uint16_t i;
    for(i = 0; i <= ms;i++){
        IFS0bits.T1IF = 0;
        Timer_1_Reset();
        T1CONbits.ON = 1;
        while(!IFS0bits.T1IF);
    }
        
}


uint32_t Get_Millis_Since_Boot (void){
    return millisSinceBoot;
}
void Timer_1_ISR (void){
    /* TODO: ADD LED HEARTBEAT*/
}
/* END OF FILE */
