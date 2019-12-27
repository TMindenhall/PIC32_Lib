//*******************************************************************//
// File: EUSART.c                                                    //
// Author: Thomas Mindenhall                                         //
// Project: Backpack Buddy                                           //
//                                                                   //
//                                                                   //
// Description: UART Config and Use                                  //
//                                                                   //
//*******************************************************************//

///////////////////////////////////////////////////////////////////////////////
//*****************************Includes**************************************//
///////////////////////////////////////////////////////////////////////////////
#include "EUSART.h"

////////////////////////////////////////////////////////////////////////////////
//*******************************FUNCTIONS************************************//
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Description: Initializes UART 1
 * 
 * Inputs: Desired baudrate as an integer.
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void UART_1_Init(uint32_t baudrate) {
    
    
    RX_SetDigitalInput();
    TX_SetDigitalOutput();
    RTS_SetDigitalOutput();
    CTS_SetDigitalOutput();
    
    uint16_t brg = 0;
    U1MODEbits.BRGH = 1; //High Speed Baud
    U1BRG = Calc_Baud(UART_1,baudrate);
    
    
    //U1BRG = 519;                //If BRGH 
    U1MODEbits.UEN = 0; //Rx/Tx enabled and used
    U1MODEbits.WAKE = 1;
    U1MODEbits.PDSEL = 0; //8bit mode, no parity
    U1MODEbits.STSEL = 0; //1 Stop bit
    U1STAbits.UTXISEL = 2; //Tx Interrupt generates on all chars xfer'd
    U1STAbits.UTXINV = 0; //Tx Idles High
    U1STAbits.URXISEL = 0; //Rx Interrupt generates on all chars recv'd
    U1MODEbits.RXINV = 0; //Rx Idles High
    U1MODEbits.RTSMD = 0;
    U1MODEbits.ON = 1; //UART On
    U1STAbits.URXEN = 1; //Enable Rx
    U1STAbits.UTXEN = 1; //Enable Tx

}

/******************************************************************************
 * Description: Initializes UART 2 - Used for GPS Reciever
 * 
 * Inputs: Desired baudrate as an integer.
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void UART_2_Init(uint32_t baudrate) {
    uint16_t brg;

    TRISAbits.TRISA1 = 1;
    //LATBbits.LATB6 = 0;
    LATBbits.LATB3 = 0;

    U2MODEbits.BRGH = 0; //High Speed Baud
    //U2BRG = 519;                //If BRGH
    U2MODEbits.UEN = 0; //Rx/Tx enabled and used
    U2MODEbits.WAKE = 1;
    U2MODEbits.PDSEL = 0; //8bit mode, no parity
    U2MODEbits.STSEL = 0; //1 Stop bit
    U2STAbits.UTXISEL = 2; //Tx Interrupt generates on all chars xfer'd
    U2STAbits.UTXINV = 0; //Tx Idles High
    U2STAbits.URXISEL = 0; //Rx Interrupt generates on all chars recv'd
    U2MODEbits.RXINV = 0; //Rx Idles High
    U2MODEbits.RTSMD = 0;
    U2MODEbits.ON = 1; //UART On
    U2STAbits.URXEN = 1; //Enable Rx
    U2STAbits.UTXEN = 1; //Enable Tx

}

/******************************************************************************
 * Description: Sends a string via UART 1 char by char till null.
 * 
 * Inputs: Pointer to desired string.
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Send_String_U1(uint8_t *ptr) {

    while (*ptr != '\0') {
        U1TXREG = *ptr;
        ptr++;
        while (U1STAbits.UTXBF);
    }
}

/******************************************************************************
 * Description: Sends a string via UART 2 char by char till null.
 * 
 * Inputs: Pointer to desired string.
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Send_String_U2(uint8_t *ptr) {

    while (*ptr != '\0') {
        U2TXREG = *ptr;
        ptr++;
        while (U2STAbits.UTXBF);
    }
}

uint16_t Calc_Baud (uint8_t uart_device, uint32_t BaudRate){
    
    uint16_t brg;
    if(uart_device == UART_1){
        if (U1MODEbits.BRGH) {
            brg = (uint16_t) ((PB_FREQ / 4) / BaudRate) - 1;
        } else {
            brg = (uint16_t) ((PB_FREQ / 16) / BaudRate) - 1;
        }
    }
    if(uart_device == UART_2){
        if (U2MODEbits.BRGH) {
            brg = (uint16_t) ((PB_FREQ / 4) / BaudRate) - 1;
        } else {
            brg = (uint16_t) ((PB_FREQ / 16) / BaudRate) - 1;
        }
    }    
    if(brg == 9){
        brg ++;
    }
    
    return brg;
}

uint8_t UART_Slow_Timeout (void){
    uint32_t start_time;
    start_time = Get_Millis_Since_Boot();
    while(U1STAbits.UTXBF){
        if (Get_Millis_Since_Boot() - start_time >= UART_SLOW_TIMEOUT)
        {
            uart_timeout_cnt++;      // Increment the timeout count
            uart_error_reg = 1;      // Something happened, raise error flag
            return FAILED;  // error happened so return immediately
        }
    }
    return SUCCESS;
}

uint8_t UART_Fast_Timeout (void){
    uint32_t start_time;
    start_time = Get_Millis_Since_Boot();
    while(U1STAbits.UTXBF){
        if (Get_Millis_Since_Boot() - start_time >= UART_FAST_TIMEOUT)
        {
            uart_timeout_cnt++;      // Increment the timeout count
            uart_error_reg = 1;      // Something happened, raise error flag
            return FAILED;  // error happened so return immediately
        }
    }
    return SUCCESS;
}

/*******************************************************************************
 * Description: Transmits all chars in a buffer till number of bytes is 
 * reached. Does error detection. 
 * 
 * Inputs: Pointer to desired string and number of bytest to be transmitted.
 * 
 * Returns: FAILED is error or constraints oob, else SUCCESS.
*******************************************************************************/
uint8_t uart_transmit_buf(char *pBuf, uint32_t numBytes)
{
    uint8_t Status;
    if (numBytes > 4096)
    {
        return FAILED;
    }

    while (numBytes > 0)
    {
        U1TXREG = *pBuf;
        Status = UART_Slow_Timeout ();
        if(Status != SUCCESS){
            //Error Message
            return FAILED;
        }
        pBuf++;
        numBytes--;
    } // end while (numBytes > 0)

    return SUCCESS;
}