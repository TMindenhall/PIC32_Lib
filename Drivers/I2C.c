//*******************************************************************//
// File: I2C.c                                                       //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description: I2C Config and Use                                   //
//                                                                   //
//*******************************************************************// 

///////////////////////////////////////////////////////////////////////////////
//*****************************Includes**************************************//
///////////////////////////////////////////////////////////////////////////////
//#include <proc/p32mx170f256b.h>
#include "I2C.h"
 
//Refer to BNO055.h and MAX17043.h for Address Defines or...
//Refer to schematic for Address of Devices


uint32_t i2c_timeout_count;
uint16_t i2c_error_flag_reg;
uint8_t Status;
////////////////////////////////////////////////////////////////////////////////
//*******************************FUNCTIONS************************************//
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Description: Initializes I2C Port 1.
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void I2C_1_Init(uint32_t baud_rate){
    
    if(!timerInitFlag){
        Timer_1_Init();
        Timer_1_Start();
    }
    i2c_error_flag_reg = 0;
    
    //Set Baud Rates
    //I2C1BRG = 0x15;                //400kHz
    //I2C1BRG = 0x60;                //100kHz
    I2C1BRG = Calc_I2C_Baud (baud_rate);
    SCL_SetDigitalInput();          //RB8
    SDA_SetDigitalInput();          //RB9
    
    I2C1CONbits.ON = 0;
    I2C1CONbits.DISSLW = 0;
    I2C1CONbits.A10M = 0;
    I2C1CONbits.SMEN = 1;
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ON = 1;
    
    i2c_init_flag = 1;
    IFS1bits.I2C1MIF = 0;
    IFS1bits.I2C1BIF = 0;
    
}

uint8_t I2C_Timeout (void){
    uint32_t start_time;
    start_time = Get_Millis_Since_Boot();
    while((I2C1CON & 0x1F) || I2C1STATbits.TRSTAT){
        if (Get_Millis_Since_Boot() - start_time >= I2C_TIMEOUT)
        {
            i2c_timeout_count++;      // Increment the timeout count
            i2c_error_flag_reg = 1;      // Something happened, raise error flag
            uint8_t dummy = I2C_Stop();  // send the stop bit to prevent further comms
            return FAILED;  // error happened so return immediately
        }
    }
    return SUCCESS;
}

void I2C_Mode_ON (void){
    I2C1CONbits.ON = 1;
}

void I2C_Mode_OFF (void){
    I2C1CONbits.ON = 0;
}

uint8_t I2C_Start (void){
    I2C1CONbits.SEN = 1;
    while(I2C1CONbits.SEN){
        uint32_t start = Get_Millis_Since_Boot();
        if(Get_Millis_Since_Boot() - start >= I2C_TIMEOUT){
            uint8_t dummy = I2C_Stop();
            I2C_Mode_OFF();
            return FAILED;
        }
    }
    return SUCCESS;
}

uint8_t I2C_Stop (void) {
    I2C1CONbits.PEN = 1;
    while(I2C1CONbits.PEN){
        uint32_t start = Get_Millis_Since_Boot();
        if(Get_Millis_Since_Boot() - start >= I2C_TIMEOUT){
            I2C_Mode_OFF();
            return FAILED;
        }
    }
    return SUCCESS;
}

uint8_t I2C_Repeated_Start (void){
    I2C1CONbits.RSEN = 1;
    while(I2C1CONbits.RSEN){
        uint32_t start = Get_Millis_Since_Boot();
        if(Get_Millis_Since_Boot() - start >= I2C_TIMEOUT){
            I2C_Stop();
            I2C_Mode_OFF();
            return FAILED;
        }
    }
    return SUCCESS;
}

void I2C_Load_Buff (uint8_t * data){
    I2C1TRN =  *data;
}

uint8_t I2C_Read_Buff (void){
    return I2C1RCV;
}

uint8_t I2C_Read_ACK (void) {

    if(I2C1STATbits.ACKSTAT) 
        return 1;
    
    else
        return 0;
    
}
/******************************************************************************
 * Description: Reads a single register as a byte from I2C Port 1.
 * 
 * Inputs: Device Address and Register Address.
 * 
 * Returns: Value as a Byte.
 ******************************************************************************/
uint8_t I2C_1_Read(uint8_t device_adr, uint16_t reg_adr, uint8_t *data_ptr, uint8_t numBytes){
    
    uint8_t dummy; uint8_t Stat;
    dummy = I2C1TRN;
    dummy = I2C1RCV;
    
    i2c_error_flag_reg = 0;
    
    if(!I2C1CONbits.ON)
        I2C_Mode_ON();
    
    
    Stat = I2C_Start();              //Start condition
    if(Stat != SUCCESS){
        i2c_error_flag_reg |= START_FAILED;
        return FAILED;
    }
    
    I2C1TRN = (device_adr << 1);               //address with R/W set for read
    Stat = I2C_Timeout();
    if(Stat != SUCCESS){
        i2c_error_flag_reg |= SEND_FAILED;
        return FAILED;
    }
    

    I2C1TRN = reg_adr;
    Stat = I2C_Timeout();
    if(Stat != SUCCESS){
        i2c_error_flag_reg |= SEND_FAILED;
        return FAILED;
    }
    
   
    Stat = I2C_Repeated_Start();
    if(Stat != SUCCESS){
        i2c_error_flag_reg |= RSTART_FAILED;
        return FAILED;
    }

    I2C1TRN = (device_adr << 1) + I2C_READ;           //address with R/W set for read
    Stat = I2C_Timeout();
    if(Stat != SUCCESS){
        i2c_error_flag_reg |= SEND_FAILED;
        return FAILED;
    }
    
    dummy = 0;
    while(dummy != numBytes){
       I2C1CONbits.RCEN = 1;               // enable master for 1 byte reception 
       Stat = I2C_Timeout();
       if(Stat != SUCCESS){
            i2c_error_flag_reg |= RECV_FAILED;
            return FAILED;
       }
    
        *data_ptr = I2C1RCV;
        if(dummy ==(numBytes -1)){
            I2C1CONbits.ACKDT = 1;
        } else {
            I2C1CONbits.ACKDT = 0;
        }
        I2C1CONbits.ACKEN = 1;
        Stat = I2C_Timeout();
        if(Stat != SUCCESS){
            i2c_error_flag_reg |= ACK_NAK_FAILED;
            return FAILED;
        }
        dummy++; data_ptr++;
    }
    Stat = I2C_Stop();
    if(Stat != SUCCESS){
        i2c_error_flag_reg |= STOP_FAILED;
        return FAILED;
    }
    return SUCCESS;
}

/******************************************************************************
 * Description: Writes to a single register as a byte from I2C Port 1.
 * 
 * Inputs: Device Address and Register Address, Value to be written as a Byte.
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
uint8_t I2C_1_Write(uint8_t device_adr, uint16_t reg_adr, uint8_t * data_ptr, uint8_t numBytes){
    uint8_t dummy;
    uint8_t Stat;
    
    dummy = I2C1TRN;
    Stat = I2C_Start();              //Start condition
    if(Stat != SUCCESS){
        i2c_error_flag_reg |= START_FAILED;
        return FAILED;
    }
   
    I2C1TRN = (device_adr << 1);               //address with R/W set for read
    Stat = I2C_Timeout();
    if(Stat != SUCCESS){
        i2c_error_flag_reg |= SEND_FAILED;
        return FAILED;
    }
    
    I2C1TRN = reg_adr;
    Stat = I2C_Timeout();
    if(Stat != SUCCESS){
        i2c_error_flag_reg |= SEND_FAILED;
        return FAILED;
    }
    
    dummy = 0;
    while(dummy != numBytes){
        I2C1TRN = *data_ptr;
        Stat = I2C_Timeout();
        if(Stat != SUCCESS){
            i2c_error_flag_reg |= SEND_FAILED;
            return FAILED;
        }
        data_ptr++;
        dummy++;
    }
    Stat = I2C_Stop();
    if(Stat != SUCCESS){
        i2c_error_flag_reg |= STOP_FAILED;
        return FAILED;
    }
    
    return SUCCESS;
}

/******************************************************************************
 * Description: Transfers a single byte out of a buffer (Recieve_Buffer) at a
 * specified address.
 * 
 * Inputs: Register address.
 * 
 * Returns: Value inside of register as a byte (signed).
 ******************************************************************************/
int8_t Xfer_Int (uint8_t adr){
    
    return Recieve_Buffer[adr];
}

/******************************************************************************
 * Description: Reads a flag.
 * 
 * Inputs: NULL (VOID)
 * 
 * Returns: Value of the flag as a integer.
 ******************************************************************************/
uint16_t Read_Flag (void){

    return i2c_error_flag_reg;
}

uint16_t Calc_I2C_Baud (uint32_t baud_rate){
    uint16_t brg = 0;
    
    // We have to break up the step to avoid underflow
    double den = 2 * baud_rate;
    den = 1/den;
    den -= 0.000000104;
    brg = (uint16_t)(den * PB_FREQ);
    brg -= 2;           
    return brg;
}
/* *****************************************************************************
 End of File
 */
