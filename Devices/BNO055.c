///////////////////////////////////////////////////////////////////////////////
//*****************************Includes**************************************//
///////////////////////////////////////////////////////////////////////////////
#include "BNO055.h"

////////////////////////////////////////////////////////////////////////////////
//**************************Structures****************************************//
////////////////////////////////////////////////////////////////////////////////

BNO_device BNO;
////////////////////////////////////////////////////////////////////////////////
//*******************************FUNCTIONS************************************//
////////////////////////////////////////////////////////////////////////////////

/******************************************************************************
 * Description: Zeros all dynamic values stored from IMU.
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Null_IMU_Values(void) {
    //Null Variables
    BNO.acc_x = 0;
    BNO.acc_y = 0;
    BNO.acc_z = 0;
    
    BNO.gyr_x = 0;
    BNO.gyr_y = 0;
    BNO.gyr_z = 0;
    
    BNO.mag_x = 0;
    BNO.mag_y = 0;
    BNO.mag_z = 0;
    
    BNO.eul_heading = 0;
    BNO.eul_pitch = 0;
    BNO.eul_roll = 0;
    
    BNO.gravity_x = 0;
    BNO.gravity_y = 0;
    BNO.gravity_z = 0;
    
    BNO.lin_acc_x = 0;
    BNO.lin_acc_y = 0;
    BNO.lin_acc_z = 0;
    
    BNO.qua_w = 0;
    BNO.qua_x = 0;
    BNO.qua_y = 0;
    BNO.qua_z = 0;
    
    
}

/******************************************************************************
 * Description: Initializes BNO IMU. *Will contain config notes in final product
 * for now it is used to verify settings*
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void BNO_Init(uint32_t baud_rate) {
    
    uint8_t Status;
    if(!i2c_init_flag)
        I2C_1_Init(baud_rate);
    
    //Put into CONFIG_MODE
    Send_Buffer[0] = 0x00;
    Status = I2C_1_Write(BNO_DEVICE, OPR_MODE, Send_Buffer, 1);
    if(Status != SUCCESS){
        //Print Error Statment
    }    
    Delay_ms(19);
    
    /*
     * PUT CONFIG STATEMENTS HERE
     */
    
    //Put into FUSION MODE
    Send_Buffer[0] = 0x0C;
    Status = I2C_1_Write(BNO_DEVICE, OPR_MODE, Send_Buffer, 1);
    if(Status != SUCCESS){
        //Print Error Statment
    }
    Delay_ms(7);
    //END OF CONFIG
}

/******************************************************************************
 * Description: Calibrates the IMU for use. Stores offset values for later use. 
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID)
 ******************************************************************************/
void BNO_Cal_Routine(void) {
    uint8_t Status;

    //| SYS_CAL | GYR_CAL | ACC_CAL | MAG_CAL | 
    Status = I2C_1_Read(BNO_DEVICE, CALIB_STAT, Recieve_Buffer, 1);
    if(Status != SUCCESS){
        //Print Error Statment
    }
    
    BNO.calib_stat = Recieve_Buffer[0];

    BNO.sys_cal = BNO.calib_stat >> 6;
    BNO.gyr_cal = (BNO.calib_stat >> 4) & 0x03;
    BNO.acc_cal = (BNO.calib_stat >> 2) & 0x03;
    BNO.mag_cal = BNO.calib_stat & 0x03;
    
}

/******************************************************************************
 * Description: Updates the structure of the BNO Device. Does a full I2C Read
 * and stores them in the RX Buffer to be later loaded to the struct.  
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID)
 ******************************************************************************/
void BNO_Update_Struct (void) {

    uint8_t Status;
    Status = I2C_1_Read(BNO_DEVICE, CHIP_ID, Recieve_Buffer, 53);
    if(Status != SUCCESS){
        //Print Error Statment
    }

    BNO.chip_id = Recieve_Buffer[0];
    BNO.acc_id = Recieve_Buffer[1];
    BNO.mag_id = Recieve_Buffer[2];
    BNO.gyr_id = Recieve_Buffer[3];

    BNO.sw_rev_id = ((uint16_t) Recieve_Buffer[5] << 8) | Recieve_Buffer[4];
    BNO.bootloader_rev_id = Recieve_Buffer[6];
    BNO.page_id = Recieve_Buffer[7];

    BNO.acc_x = (((int16_t) Recieve_Buffer[9] << 8) | Recieve_Buffer[8]) / 100;
    BNO.acc_y = (((int16_t) Recieve_Buffer[11] << 8) | Recieve_Buffer[10]) / 100;
    BNO.acc_z = (((int16_t) Recieve_Buffer[13] << 8) | Recieve_Buffer[12]) / 100;

    BNO.mag_x = (((int16_t) Recieve_Buffer[15] << 8) | Recieve_Buffer[14]) / 16;
    BNO.mag_y = (((int16_t) Recieve_Buffer[17] << 8) | Recieve_Buffer[16]) / 16;
    BNO.mag_z = (((int16_t) Recieve_Buffer[19] << 8) | Recieve_Buffer[18]) / 16;

    BNO.gyr_x = (((int16_t) Recieve_Buffer[21] << 8) | Recieve_Buffer[20]) / 16;
    BNO.gyr_y = (((int16_t) Recieve_Buffer[23] << 8) | Recieve_Buffer[22]) / 16;
    BNO.gyr_z = (((int16_t) Recieve_Buffer[25] << 8) | Recieve_Buffer[24]) / 16;

    BNO.eul_heading = (((int16_t) Recieve_Buffer[27] << 8) | Recieve_Buffer[26]) / 16;
    BNO.eul_roll = (((int16_t) Recieve_Buffer[29] << 8) | Recieve_Buffer[28]) / 16;
    BNO.eul_pitch = (((int16_t) Recieve_Buffer[31] << 8) | Recieve_Buffer[30]) / 16;

    BNO.qua_w = (((int16_t) Recieve_Buffer[33] << 8) | Recieve_Buffer[32]);
    BNO.qua_x = (((int16_t) Recieve_Buffer[35] << 8) | Recieve_Buffer[34]);
    BNO.qua_y = (((int16_t) Recieve_Buffer[37] << 8) | Recieve_Buffer[36]);
    BNO.qua_z = (((int16_t) Recieve_Buffer[39] << 8) | Recieve_Buffer[38]);

    BNO.lin_acc_x = (((int16_t) Recieve_Buffer[41] << 8) | Recieve_Buffer[40]) / 100;
    BNO.lin_acc_y = (((int16_t) Recieve_Buffer[43] << 8) | Recieve_Buffer[42]) / 100;
    BNO.lin_acc_z = (((int16_t) Recieve_Buffer[45] << 8) | Recieve_Buffer[44]) / 100;

    BNO.gravity_x = (((int16_t) Recieve_Buffer[47] << 8) | Recieve_Buffer[46]) / 100;
    BNO.gravity_y = (((int16_t) Recieve_Buffer[49] << 8) | Recieve_Buffer[48]) / 100;
    BNO.gravity_z = (((int16_t) Recieve_Buffer[51] << 8) | Recieve_Buffer[50]) / 100;    

    BNO.temp = Recieve_Buffer[52];
}

/******************************************************************************
 * Description: Makes virtual heading components, then takes those and makes
 *              unit vectors for those components. The function then takes both
 *              the average acceleration vectors and projects those onto the 
 *              heading vector. 
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID)
 ******************************************************************************/
void Correct_Vectors(void) {
//    //update values
//    Read_LIN();
//    Get_Orientation();
//    //break up heading into components
//    double heading_x, heading_y, magnitude;
//    heading_x = cos(heading * DEG_2_RAD);
//    heading_y = sin(heading * DEG_2_RAD);
//
//    //take the average acceleration
//    sum_lin_x /= acc_count;
//    sum_lin_y /= acc_count;
//    sum_lin_z /= acc_count;
//    acc_count = 0;
//
//    //make the magnitude for the heading
//    magnitude = sqrt((heading_x * heading_x) + (heading_y * heading_y));
//
//    //project the vectors
//    correction_vector_x = sum_lin_x * (heading_x / magnitude);
//    correction_vector_y = sum_lin_y * (heading_y / magnitude);
//    correction_vector_z = sum_lin_z * (1 / magnitude);
//
//    //assign the projection
//    projection = correction_vector_x + correction_vector_y + correction_vector_z;
}

/******************************************************************************
 * Description: Starts a delta_t timer. Uses Delay.c.
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID)
 ******************************************************************************/
void Start_Delta_T(void) {
    Null_Timer_1();
    Timer_1_Start();
}

/******************************************************************************
 * Description: Stops a delta_t timer, then reads the value from the register. 
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: Timer 1 Register Value.
 ******************************************************************************/
uint16_t Read_Delta_Time(void) {
    return Timer_1_Read();
}


/******************************************************************************
 * Description: Computes the integral for position using delta_t and linear
 * acceleration. 
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: a distance in meters as 32bit signed. 
 ******************************************************************************/
double Compute_Position(void) {
//    Correct_Vectors();
//    distance = (.5 * projection) * ((delta_time * delta_time));
//    return distance;
}


/******************************************************************************
 * Description: Reads orientation registers, fixes them to degrees, then assigns
 *              a heading from Euler heading. Uses Repeated Read.  
 * 
 * Inputs: NULL (VOID).
 * 
 * Returns: NULL (VOID).
 ******************************************************************************/
void Get_Orientation(void) {
    BNO.eul_heading = BNO.eul_roll = BNO.eul_pitch = 0;

    I2C_1_Read(BNO_DEVICE, EUL_HEAD_LSB, Recieve_Buffer, 6);
    BNO.eul_heading = ((Recieve_Buffer[1] << 8) | (Recieve_Buffer[0]) / 16);
    BNO.eul_roll = ((Recieve_Buffer[3] << 8) | (Recieve_Buffer[2]) / 16);
    BNO.eul_pitch = ((Recieve_Buffer[5] << 8) | (Recieve_Buffer[4]) / 16);

    heading = BNO.eul_heading;

}
/*****************************************************************************/
/*****************************END OF FILE*************************************/
/*****************************************************************************/
