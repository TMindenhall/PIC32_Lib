

#include "NMEA.h"


void initNMEA (void){
    nmea_active = 0;
    nmea_ptr = NMEA_Buffer;
    nmea_head = NMEA_Buffer; //Permanent Pointer 
}

int parseNMEA (char * NMEAbuffer,size_t NMEA_size){
    //NMEA_buff_t NMEA_structure;
    uint8_t bytes; //how many bytes we have processed
    bytes = 0;


    /* Check if string is valid */
     bool valid;
     valid = isStringValid(NMEAbuffer);
     if(valid != true){
    #ifdef NMEA_DEBUG
        printf("string is not valid\r\n");
    #endif
        return 0;
     }
    
    /* Move past '$' */
     NMEAbuffer ++;
     bytes ++;
    
    /* Lets Start Parsing the String */
    /* NMEA Type */
    /*************************************************************************/    
    // Pointer to the NMEA String  
    char * ptr;
     
    // What part of the string are we on
    int NMEA_state = 0;
     
    // Match the pointer with String to be handled
    ptr = NMEAbuffer;
    #ifdef NMEA_DEBUG
        printNMEAstate(NMEA_state);
    #endif
    /* Move past '$' */
    NMEA_state++;
    uint8_t prev_bytes = bytes;
    
    /* Grab the type of string, this will populate the proper struct */
    /* Bytes is incremented by the bytes processed, including the delimiter */
    /* This is to keep track incase we have a NULL field inside the string */
    bytes += get_NMEA_type (ptr, bytes);
    
    /* Move the pointer based on the bytes processed*/
    ptr += (bytes - prev_bytes) + 1; bytes ++;
    #ifdef NMEA_DEBUG
     printf("Bytes Proccessed: %d\r\n", bytes);
     printf(ptr); printf("\r\n");
    #endif

     
    /* NMEA Time */
    /************************************************************************/
    /* Now Lets grab the time */
     prev_bytes = bytes;
     if(NMEA_state == 1){
        
        /* Do we have a delimiter next or some data */
        if(lookAhead(ptr)){
        #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
        #endif
            NMEA_state++;
            bytes += get_NMEA_time(ptr, bytes); 
            ptr += (bytes - prev_bytes) + 1; bytes ++;
        #ifdef NMEA_DEBUG
            printf("Bytes Proccessed: %d\r\n", bytes);
            printf(ptr);printf("\r\n");
        #endif
        } else {
            #ifdef NMEA_DEBUG
            printf("\r\n");
            printNMEAstate(NMEA_state);
            printf("Invalid, Moving Ahead\r\n");
            #endif
            uint8_t prev_bytes = bytes;
            while(*ptr == ','){
                ptr ++; bytes ++;
            }   
            #ifdef NMEA_DEBUG
            printf("Moved: %d bytes\r\n", bytes - prev_bytes);
            #endif
            NMEA_state = moveNMEAState(NMEA_state, bytes - prev_bytes);
            #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            printf(ptr);printf("\r\n");
            #endif
        }
     }
        #ifdef NMEA_DEBUG
        printf("\r\n");
        #endif
    /* NMEA LAT */
    /*************************************************************************/
     /* Now Lets grab the Latitude */
     prev_bytes = bytes;
     if(NMEA_state == 2){
        if(lookAhead(ptr)){
        #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
        #endif
            NMEA_state++;
            bytes += get_NMEA_Latitude(ptr, bytes); 
            ptr += (bytes - prev_bytes) + 2; bytes += 2;
        #ifdef NMEA_DEBUG
            printf("Bytes Proccessed: %d\r\n", bytes);
            printf(ptr);printf("\r\n");
        #endif
        } else {
            #ifdef NMEA_DEBUG
            printf("\r\n");
            printNMEAstate(NMEA_state);
            printf("Invalid, Moving Ahead\r\n");
            #endif
            uint8_t prev_bytes = bytes;
            while(*ptr == ','){
                ptr ++; bytes ++;
            }   
            #ifdef NMEA_DEBUG
            printf("Moved: %d bytes\r\n", bytes - prev_bytes);
            #endif
            NMEA_state = moveNMEAState(NMEA_state, bytes - prev_bytes);
            #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            printf(ptr);printf("\r\n");
            #endif
        }
     }
    #ifdef NMEA_DEBUG
      printf("\r\n");
    #endif
    /* NMEA Long */
    /*************************************************************************/
     /* Now Lets grab the Longitude */
     prev_bytes = bytes;
     if(NMEA_state == 3){
        if(lookAhead(ptr)){
            #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            #endif
            NMEA_state++;
            bytes += get_NMEA_Longitude(ptr, bytes); 
            ptr += (bytes - prev_bytes) + 2; bytes += 2;
            #ifdef NMEA_DEBUG
            printf("Bytes Proccessed: %d\r\n", bytes);
            printf(ptr);printf("\r\n");
            #endif
        } else {
            #ifdef NMEA_DEBUG
            printf("\r\n");
            printNMEAstate(NMEA_state);
            printf("Invalid, Moving Ahead\r\n");
            #endif
            uint8_t prev_bytes = bytes;
            while(*ptr == ','){
                ptr ++; bytes ++;
            }   
            #ifdef NMEA_DEBUG
            printf("Moved: %d bytes\r\n", bytes - prev_bytes);
            #endif
            NMEA_state = moveNMEAState(NMEA_state, bytes - prev_bytes);
            #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            printf(ptr);printf("\r\n");
            #endif
        }
     }
      #ifdef NMEA_DEBUG
      printf("\r\n");
      #endif
    /* NMEA Fix */
    /*************************************************************************/
    /* Now Lets grab the Fix Quality */
     prev_bytes = bytes;
     if(NMEA_state == 4){
        if(lookAhead(ptr)){
            #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            #endif
            NMEA_state++;
            bytes += get_NMEA_fix(ptr, bytes); 
            ptr += (bytes - prev_bytes) + 1; bytes ++;
            #ifdef NMEA_DEBUG
            printf("Bytes Proccessed: %d\r\n", bytes);
            printf(ptr);printf("\r\n");
            #endif
        } else {
            #ifdef NMEA_DEBUG
            printf("\r\n");
            printNMEAstate(NMEA_state);
            printf("Invalid, Moving Ahead\r\n");
            #endif
            uint8_t prev_bytes = bytes;
            while(*ptr == ','){
                ptr ++; bytes ++;
            }   
            #ifdef NMEA_DEBUG
            printf("Moved: %d bytes\r\n", bytes - prev_bytes);
            #endif
            NMEA_state = moveNMEAState(NMEA_state, bytes - prev_bytes);
            #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            printf(ptr);printf("\r\n");
            #endif
        }
     }
      #ifdef NMEA_DEBUG
      printf("\r\n");
      #endif
    /* NMEA Sats */
    /*************************************************************************/
    /* Now Lets grab the Number of Sat's */
     prev_bytes = bytes;
     if(NMEA_state == 5){
        if(lookAhead(ptr)){
            #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            #endif
            NMEA_state++;
            bytes += get_NMEA_Sats(ptr, bytes); 
            ptr += (bytes - prev_bytes) + 1; bytes ++;
            #ifdef NMEA_DEBUG
            printf("Bytes Proccessed: %d\r\n", bytes);
            printf(ptr);printf("\r\n");
            #endif
        } else {
            #ifdef NMEA_DEBUG
            printf("\r\n");
            printNMEAstate(NMEA_state);
            printf("Invalid, Moving Ahead\r\n");
            #endif
            uint8_t prev_bytes = bytes;
            while(*ptr == ','){
                ptr ++; bytes ++;
            }   
            #ifdef NMEA_DEBUG
            printf("Moved: %d bytes\r\n", bytes - prev_bytes);
            #endif
            NMEA_state = moveNMEAState(NMEA_state, bytes - prev_bytes);
            #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            printf(ptr);printf("\r\n");
            #endif
        }
     }
      #ifdef NMEA_DEBUG
      printf("\r\n");
      #endif
    /* NMEA Hoz Dil */
    /*************************************************************************/
    /* Now Lets grab the Horizontal Dilation */
 
     prev_bytes = bytes;
     if(NMEA_state == 6){
        if(lookAhead(ptr)){
        #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
        #endif
            NMEA_state++;
            bytes += get_NMEA_hozDilation(ptr, bytes); 
            ptr += (bytes - prev_bytes) + 1; bytes ++;
        #ifdef NMEA_DEBUG
            printf("Bytes Proccessed: %d\r\n", bytes);
            printf(ptr);printf("\r\n");
        #endif
        } else {
        #ifdef NMEA_DEBUG
            printf("\r\n");
            printNMEAstate(NMEA_state);
            printf("Invalid, Moving Ahead\r\n");
        #endif
            uint8_t prev_bytes = bytes;
            while(*ptr == ','){
                ptr ++; bytes ++;
            }   
        #ifdef NMEA_DEBUG
            printf("Moved: %d bytes\r\n", bytes - prev_bytes);
        #endif
            NMEA_state = moveNMEAState(NMEA_state, bytes - prev_bytes);
        #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            printf(ptr);printf("\r\n");
        #endif
        }
     }
    #ifdef NMEA_DEBUG
      printf("\r\n");
    #endif
    /* NMEA Vert */
    /*************************************************************************/
     /* Now Lets grab the Vertical Elevation */

    
     prev_bytes = bytes;
     if(NMEA_state == 7){
        if(lookAhead(ptr)){
        #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
        #endif
            NMEA_state++;
            bytes += get_NMEA_verticalElevation(ptr, bytes); 
            ptr += (bytes - prev_bytes) + 2; bytes += 2;
        #ifdef NMEA_DEBUG
            printf("Bytes Proccessed: %d\r\n", bytes);
            printf(ptr);printf("\r\n");
        #endif
        } else {
        #ifdef NMEA_DEBUG
            printf("\r\n");
            printNMEAstate(NMEA_state);
            printf("Invalid, Moving Ahead\r\n");
        #endif
            uint8_t prev_bytes = bytes;
            while(*ptr == ','){
                ptr ++; bytes ++;
            }   
        #ifdef NMEA_DEBUG        
            printf("Moved: %d bytes\r\n", bytes - prev_bytes);
        #endif
            NMEA_state = moveNMEAState(NMEA_state, bytes - prev_bytes);
        #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            printf(ptr);printf("\r\n");
        #endif
        }
     }
    #ifdef NMEA_DEBUG
      printf("\r\n");
    #endif
    /* NMEA Geoid */
    /*************************************************************************/
     /* Now Lets grab the Height of Geoid */
      prev_bytes = bytes;
     if(NMEA_state == 8){
        if(lookAhead(ptr)){
        #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
        #endif
            NMEA_state++;
            bytes += get_NMEA_heightGeoid(ptr, bytes); 
            ptr += (bytes - prev_bytes) + 1; bytes ++;
        #ifdef NMEA_DEBUG
            printf("Bytes Proccessed: %d\r\n", bytes);
            printf(ptr);printf("\r\n");
        #endif
        } else {
        #ifdef NMEA_DEBUG
            printf("\r\n");
            printNMEAstate(NMEA_state);
            printf("Invalid, Moving Ahead\r\n");
        #endif
            uint8_t prev_bytes = bytes;
            while(*ptr == ','){
                ptr ++; bytes ++;
            }   
        #ifdef NMEA_DEBUG
            printf("Moved: %d bytes\r\n", bytes - prev_bytes);
        #endif
            NMEA_state = moveNMEAState(NMEA_state, bytes - prev_bytes);
        #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            printf(ptr);printf("\r\n");
        #endif
        }
     }
    #ifdef NMEA_DEBUG
      printf("\r\n");
    #endif
    /* NMEA DGPS */
      
     /*************************************************************************/
     /* Now Lets grab the Time since last DGPS update and DGPS ID if there */
     prev_bytes = bytes;
     if(NMEA_state == 9){
        if(lookAhead(ptr)){
        #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
        #endif
            NMEA_state++;
            bytes += get_NMEA_DGPS_age (ptr, bytes); 
            ptr += (bytes - prev_bytes) + 1; bytes ++;
        #ifdef NMEA_DEBUG
            printf("Bytes Proccessed: %d\r\n", bytes);
            printf(ptr);printf("\r\n"); 
        #endif
        } else {
        #ifdef NMEA_DEBUG
            printf("\r\n");
            printNMEAstate(NMEA_state);
            printf("Invalid, Moving Ahead\r\n");
        #endif
            uint8_t prev_bytes = bytes;
            while(*ptr == ','){
                ptr ++; bytes ++;
            }   
        #ifdef NMEA_DEBUG
            printf("Moved: %d bytes\r\n", bytes - prev_bytes);
        #endif
            NMEA_state = moveNMEAState(NMEA_state, bytes - prev_bytes);
        #ifdef NMEA_DEBUG
            printNMEAstate(NMEA_state);
            printf(ptr);printf("\r\n");
        #endif
            
        }
     }
     prev_bytes = bytes;
     if(NMEA_state == 10){
         if(*ptr == '*'){
             printf("found delim\r\n");
             NMEA_state == 11;
             goto checksum;
         }
    #ifdef NMEA_DEBUG
        printNMEAstate(NMEA_state);
    #endif
        NMEA_state++;
        bytes += get_NMEA_DGPS_id (ptr, bytes); 
        ptr += (bytes - prev_bytes) + 1; bytes ++;
    #ifdef NMEA_DEBUG
        printf("Bytes Proccessed: %d\r\n", bytes);
        printf(ptr);printf("\r\n");
    #endif
        } 
     else {
        #ifdef NMEA_DEBUG
            printf("\r\n");
            printNMEAstate(NMEA_state);
            printf("Invalid, Moving Ahead\r\n");
            printf(ptr);printf("\r\n");
        #endif
        }
    #ifdef NMEA_DEBUG
     printNMEAstate(NMEA_state);
     printf("\r\n");
    #endif
    /* NMEA Checksum */
    /*************************************************************************/
     checksum:
     prev_bytes = bytes;
     if(NMEA_state == 11){
        bytes += get_NMEA_checksum (ptr, bytes); 
     }
     
    #ifdef NMEA_DEBUG
     printf("\r\n");
    #endif
     // //since we didnt Iterrate through ptr, we count the last two bytes
      bytes += 2;
      return bytes;
}

/* @brief Check if the NMEA String is valid.
 * This function checks to see if the leading character is '$' 
 * which indicates a valid start of NMEA string. 
 * 
 * @param buffer - character pointer to start of string. 
 * @return - true if '$' is found at the start of string.
 *         - false by default. 
 * 
 */
bool isStringValid (char * buffer){
    if(*buffer == '$'){
        return true;
    } else
        return false;
}


/* @brief   Looks ahead in the string to see if the next 
 *          character is a delimiter. This is done to see if 
 *          we have data in the next field or if we need to 
 *          move ahead.
 *        
 * @param buffer - character pointer to the current pos. in the string. 
 * @return - false if the next pointer is a delimiter.
 *         - true by default or next character is not a delimiter. 
 * 
 */
bool lookAhead (char * ptr){
    if(*ptr == ','){
        return false;
    }
    return true;
}

/* @brief Prints the current NMEA State.
 *          This prints the current NMEA State to see flow
 *          control following the parsing of data and 
 *          is used only in DEBUG mode.  
 * 
 * @param NMEA_State - int value for NMEA State. 
 * @return void
 * 
 */
void printNMEAstate (int NMEA_State){
    printf("NMEA State is: ");
    switch(NMEA_State){
        case 0:
        printf("(%d)NMEA Type\r\n", NMEA_State); break;
        case 1:
        printf("(%d)NMEA Time\r\n", NMEA_State); break;
        case 2:
        printf("(%d)NMEA Lat\r\n", NMEA_State); break;
        case 3:
        printf("(%d)NMEA Long\r\n", NMEA_State); break;
        case 4:
        printf("(%d)NMEA Fix\r\n", NMEA_State); break;
        case 5:
        printf("(%d)NMEA Sats\r\n", NMEA_State); break;
        case 6:
        printf("(%d)NMEA HDOP\r\n", NMEA_State); break;
        case 7:
        printf("(%d)NMEA MSL Alt\r\n", NMEA_State); break;
        case 8:
        printf("(%d)NMEA Geoid\r\n", NMEA_State); break;
        case 9:
        printf("(%d)NMEA Age of diff.\r\n", NMEA_State); break;
        case 10:
        printf("(%d)NMEA Diff ref station ID\r\n", NMEA_State); break;
        case 11:
        printf("(%d)NMEA Check Sum\r\n", NMEA_State); break;
        default:
        printf("Invalid NMEA State\r\n"); break;
        
    }
    
}


/* @brief Move to the next valid NMEA State
 * 
 * This function moves the NMEA State to the next valid 
 * state, which has data or to the checksum state. 
 * 
 * @param NMEA_State -  Current NMEA_State 
 *        bytes_moved - bytes that the string pointer 
 *                      moved to the next delimiter. 
 * @return - Next NMEA State as an integer. 
 * 
 */
int moveNMEAState (int NMEA_State, int bytes_moved){
    switch(NMEA_State){
        
        //Were in NMEA Type
        case 0:
            switch(bytes_moved){
                //We moved to NMEA Lat
                case 1:
                    return 2; break;
                case 3:
                    return 3; break;
                case 5:
                    return 4; break;
                case 6:
                    return 5; break;
                case 7:
                    return 6; break;
                case 8:
                    return 7; break;
                case 10:
                    return 8; break;
                case 12:
                    return 9; break;
                case 13:
                    return 10; break;
                default:
                    //move to checksum
                    #ifdef NMEA_DEBUG
                    printf("Hit default A\r\n"); 
                    #endif
                    break;
            }
        case 1: 
            switch(bytes_moved){
                case 2:
                    return 2; break;
                case 4:
                    return 3; break;
                case 5:
                    return 5; break;
                case 6:
                    return 6; break;
                case 7:
                    return 7; break;
                case 9:
                    return 8; break;
                case 11:
                    return 9; break;
                case 12:
                    return 10; break;
                default:
                    //move to checksum
                    #ifdef NMEA_DEBUG
                    printf("Hit default B\r\n"); 
                    #endif
                    break;
                
            }
        case 2: 
            switch(bytes_moved){
                case 2:
                    return 3; break;
                case 4:
                    return 4; break;
                case 5:
                    return 5; break;
                case 6:
                    return 6; break;
                case 7:
                    return 7; break;
                case 11:
                    return 9; break;
                default:
                    //move to checksum
                    #ifdef NMEA_DEBUG
                    printf("Hit default C\r\n"); 
                    #endif
                    break;
                
            }
        case 3: 
            switch(bytes_moved){
                case 3:
                    return 3; break;
                case 4:
                    return 4; break;
                case 5:
                    return 5; break;
                case 7:
                    return 6; break;
                default:
                    //move to checksum
                    #ifdef NMEA_DEBUG
                    printf("Hit default D\r\n"); 
                    #endif
                    break;
                
            }
        case 4: 
            switch(bytes_moved){
                case 1:
                    return 6; break;
                case 2:
                    return 7; break;
                case 4:
                    return 8; break;
                case 6:
                    return 9; break;
                case 7:
                    return 10; break;
                default:
                    //move to checksum
                    #ifdef NMEA_DEBUG
                    printf("Hit default E\r\n"); 
                    #endif
                    break;
                
            }
        case 5: 
            switch(bytes_moved){
                case 1:
                    return 7; break;
                case 2:
                    return 8; break;
                case 4:
                    return 9; break;
                case 6:
                    return 10; break;
                default:
                    //move to checksum
                    #ifdef NMEA_DEBUG
                    printf("Hit default F\r\n"); 
                    #endif
                    break;
                
            }
        case 6: 
            switch(bytes_moved){
                case 2:
                    return 8; break;
                case 4:
                    return 9; break;
                case 5:
                    return 10; break;

                default:
                    //move to checksum
                    #ifdef NMEA_DEBUG
                    printf("Hit default G\r\n"); 
                    #endif
                    break;
                
            }
        case 7: 
            switch(bytes_moved){
                case 2:
                    return 9; break;
                case 3:
                    return 10; break;
                default:
                    //move to checksum
                    #ifdef NMEA_DEBUG
                    printf("Hit default H\r\n"); 
                    #endif
                    break;
                
            } 
        case 8: 
            switch(bytes_moved){
                case 1:
                    return 9; break;
                case 2:
                    return 10; break;
                case 3:
                    return 11; break;
                default:
                    //move to checksum
                    #ifdef NMEA_DEBUG
                    printf("Hit default I\r\n"); 
                    #endif
                    break;
                    
                
            }
            case 9: 
                
                default:
                    //move to checksum
                    #ifdef NMEA_DEBUG
                    printf("Hit default J\r\n"); 
                    #endif
                    break;
            
    }
}

/* Were gonna need some type of pointer to struct */
int get_NMEA_type (char * ptr, int bytes_current){
    char NMEA_type [10]; char * NMEA_ptr = NMEA_type;
    int count = 0;
    int bytes = bytes_current;
    while(*ptr != ','){
         *NMEA_ptr = *ptr;
         ptr++; NMEA_ptr++; bytes++; count++;
    }
     NMEA_type[count] = '\0';
    #ifdef NMEA_DEBUG
        printf("NMEA Type is: %s\r\n", NMEA_type);
    #endif
     
     if(stringCompare(NMEA_type, "GPGGA", sizeof("GPGGA"))){
         
         //(void) stringCopy(NMEA_type, ptr, sizeof(NMEA_type));
         return (bytes - bytes_current);
     }
     if(stringCompare(NMEA_type, "GPGSV", sizeof("GPGSV"))){
         printf("Types Match\r\n");
         return (bytes - bytes_current);
     }
     return (bytes - bytes_current);

}

int get_NMEA_time (char * ptr, int bytes_current){

    int bytes = bytes_current;
    char NMEA_time [12]; char * NMEA_ptr = NMEA_time;
    while(*ptr != ','){
        *NMEA_ptr = *ptr;
        ptr++; NMEA_ptr++; bytes++;
    }
     
    NMEA_time[bytes - bytes_current] = '\0';
    #ifdef NMEA_DEBUG
        printf("NMEA Time is: %s\r\n", NMEA_time);
    #endif
     char NMEA_hour[3] , NMEA_min[3], NMEA_sec[7];
     NMEA_hour[0]   = NMEA_time[0];
     NMEA_hour[1]   = NMEA_time[1];
     NMEA_hour[2]   = '\0'; 
     NMEA_min[0]    = NMEA_time[2];
     NMEA_min[1]    = NMEA_time[3];
     NMEA_min[2]   = '\0'; 
     
     int i = 0;
     while(NMEA_time[i + 4] != '\0'){
         NMEA_sec[i] = NMEA_time[i + 4];
         i++;
         
     }
    #ifdef NMEA_DEBUG     
        printf("Time Stamp is: %s:%s:%s\r\n", NMEA_hour, NMEA_min, NMEA_sec);
    #endif
     return (bytes - bytes_current);
}

int get_NMEA_Latitude (char * ptr, int bytes_current){
    
    int bytes = bytes_current;
    
     char NMEA_latitude_decimal [12]; char * NMEA_ptr = NMEA_latitude_decimal;
     while(*ptr != ','){
        *NMEA_ptr = *ptr;
        ptr++; NMEA_ptr++; bytes ++;
     
     }
     NMEA_ptr++;
     *NMEA_ptr = '\0';
     char NMEA_latitude_deg[3], NMEA_latitude_min[6];
     NMEA_latitude_deg[0] = NMEA_latitude_decimal[0];
     NMEA_latitude_deg[1] = NMEA_latitude_decimal[1];
     NMEA_latitude_deg[2] = '\0';
     float lat_deg = strtof(NMEA_latitude_deg, NULL);
     int count = 0;
     int i;
     for(i = 0; i < sizeof(NMEA_latitude_min); i++){
         NMEA_latitude_min[i] = NMEA_latitude_decimal[i + 2];
         count++;
     }
     NMEA_latitude_min[count + 1] = '\0';
     float lat_min = strtof(NMEA_latitude_min, NULL);
    #ifdef NMEA_DEBUG     
        printf("Latitude is: %2.2f deg %2.3f min\r\n", lat_deg, lat_min);
    #endif
     ptr++; bytes ++;
     char lat_dir; 
     lat_dir = *ptr;
    #ifdef NMEA_DEBUG      
     printf("Lat Direction is: %c\r\n", lat_dir);
    #endif
    return bytes - bytes_current;
}

int get_NMEA_Longitude (char * ptr, int bytes_current){
    int bytes = bytes_current;
    
    char NMEA_longitude_decimal [8]; char * NMEA_ptr = NMEA_longitude_decimal;
    int count = 0;
    while(*ptr != ','){
        *NMEA_ptr = *ptr;
        ptr++; NMEA_ptr++; bytes ++; count ++;
    }
     NMEA_longitude_decimal[count + 1] = '\0';

     char NMEA_longitude_deg[4], NMEA_longitude_min[5];
     NMEA_longitude_deg[0] = NMEA_longitude_decimal[0];
     NMEA_longitude_deg[1] = NMEA_longitude_decimal[1];
     NMEA_longitude_deg[2] = NMEA_longitude_decimal[2];
     NMEA_longitude_deg[3] = '\0';
     float lng_deg = strtof(NMEA_longitude_deg, NULL);
     count = 0;
     int i;
     for(i = 0; i < sizeof(NMEA_longitude_min); i++){
         NMEA_longitude_min[i] = NMEA_longitude_decimal[i + 3];
         count++;
     }
     NMEA_longitude_min[count + 1] = '\0';
     float lng_min = strtof(NMEA_longitude_min, NULL);
    #ifdef NMEA_DEBUG      
        printf("Longitude is: %2.2f deg %2.3f min\r\n", lng_deg, lng_min);
    #endif 
     ptr++; bytes ++;
     char lng_dir; 
     lng_dir = *ptr;
    #ifdef NMEA_DEBUG
        printf("Long Direction is: %c\r\n", lng_dir);
    #endif
    return (bytes - bytes_current);
}

int get_NMEA_fix (char * ptr, int bytes_current){
    int bytes = bytes_current;
    
    
    char fix_quality_char = *ptr;
    #ifdef NMEA_DEBUG
        printf("Fix Quality: ");
        switch(fix_quality_char){
            case '0':
                printf("Invaild\r\n"); break;
            case '1':
                printf("GPS Fix (SPS)\r\n"); break;
            case '2':
                printf("DGPS Fix \r\n"); break;
            case '3':
                printf("PPS Fix\r\n"); break;
            case '4':
                printf("Real Time Kinetic Fix\r\n"); break;
            case '5':
                printf("Float RTK Fix\r\n"); break;
            case '6':
                printf("estimaed (Dead Reckoning) Fix\r\n"); break;
            case '7':
                printf("Manual Fix\r\n"); break;
            case '8':
                printf("Simulation Fix\r\n"); break;
            default:
                printf("Invaild Default\r\n"); break;
        } 
    #endif
     ptr++; bytes++;
     return bytes - bytes_current;
}

int get_NMEA_Sats (char * ptr, int bytes_current){
    int bytes = bytes_current;
    
    char num_of_sats_string[2]; char * NMEA_ptr = num_of_sats_string;
    int count = 0;
    while(*ptr != ','){
        *NMEA_ptr = *ptr;
        ptr++; NMEA_ptr++; bytes ++; count++;
     
    }
    num_of_sats_string[count + 1] = '\0';
     
    int numSats = atoi(num_of_sats_string);
    #ifdef NMEA_DEBUG
        printf("Number of Sats: %d \r\n", numSats);
    #endif
    return bytes - bytes_current;
}

int get_NMEA_hozDilation (char * ptr, int bytes_current){
    int bytes =  bytes_current;
    
     char horz_dilation_string [3]; char * NMEA_ptr = horz_dilation_string;
     int count = 0;
     while(*ptr != ','){
        *NMEA_ptr = *ptr;
        ptr++; NMEA_ptr++; bytes ++;
        count++;
     
     }
     horz_dilation_string[count + 1] = '\0';
     float horz_dilation = strtof(horz_dilation_string, NULL);
    #ifdef NMEA_DEBUG
        printf("Horizontal Dilation is: %2.2f meters\r\n", horz_dilation);
    #endif
    return bytes - bytes_current;
}

int get_NMEA_verticalElevation (char * ptr, int bytes_current){
    int bytes = bytes_current;
    
     char vetical_elev_string [10]; char * NMEA_ptr = vetical_elev_string;
     int count = 0;
     while(*ptr != ','){
        *NMEA_ptr = *ptr;
        ptr++; NMEA_ptr++; bytes ++; count++;
     }
     
     vetical_elev_string[count + 1] = '\0';
     
     float vetical_elev = strtof(vetical_elev_string, NULL);
    #ifdef NMEA_DEBUG
       printf("Vertical Elevation is: %5.2f m \r\n", vetical_elev); 
    #endif
     ptr += 2;  //were ignoring unit part.
     bytes += 2;
    return bytes - bytes_current;
}

int get_NMEA_heightGeoid (char * ptr, int bytes_current){
    int bytes = bytes_current;
    
    char height_of_geoid_String [10]; char * NMEA_ptr = height_of_geoid_String;
    int count = 0;
    while(*ptr != ','){
        *NMEA_ptr = *ptr;
        ptr++; NMEA_ptr++; bytes ++; count++;
    }
    
    height_of_geoid_String[count] = '\0';
    #ifdef NMEA_DEBUG
        printf(height_of_geoid_String); printf("\r\n");
    #endif
    float height_of_geoid = strtof(height_of_geoid_String, NULL);
    
    #ifdef NMEA_DEBUG
        printf("Height of Geoid is: %5.2f m\r\n", height_of_geoid);
    #endif
    ptr += 2;
    bytes += 2;
    return bytes - bytes_current;
}

int get_NMEA_DGPS_age (char * ptr , int bytes_current){
    int bytes = bytes_current;

    char time_since_last_DGPS_string [10]; char * NMEA_ptr = time_since_last_DGPS_string;
    uint8_t count = 0;
    while(*ptr != ','){
        *NMEA_ptr = *ptr;
        ptr++; NMEA_ptr++; bytes ++; count ++;
     
    } 
    time_since_last_DGPS_string[count + 1] = '\0';
    #ifdef NMEA_DEBUG     
    printf("Age of Diff. Corr. is: %s\r\n", time_since_last_DGPS_string);
    #endif
    
    return bytes - bytes_current;
 
}

int get_NMEA_DGPS_id (char * ptr, int bytes_current){
    int bytes = bytes_current;
    char diff_ref_station_id [10]; char * NMEA_ptr = diff_ref_station_id;
    int count = 0;
    while(*ptr != '*'){
        *NMEA_ptr = *ptr;
        ptr++; NMEA_ptr++; bytes ++; count ++;
    }
    diff_ref_station_id [count + 1] = '\0';
    #ifdef NMEA_DEBUG 
        printf("Diff. Ref. Station ID is: %s\r\n", diff_ref_station_id);
    #endif
    
    return bytes - bytes_current;
}

int get_NMEA_checksum (char * ptr, int bytes_current){
        
     /* First Lets calculate checksum */
     int bytes = bytes_current;
     ptr -= bytes; ptr++;
    #ifdef NMEA_DEBUG 
        printf(ptr); printf("\r");
    #endif
     char checkSum_Sting [100];
     int j;
     for(j = 0; j < bytes - 2; j++){
         checkSum_Sting[j] = *ptr;
         ptr++;
     }
     checkSum_Sting[bytes] = '\0';
    #ifdef NMEA_DEBUG      
        printf("\r\n%s \r\n", checkSum_Sting);
    #endif
     int checksum = 0;
     int i;
     for (i = 0; i < strlen(checkSum_Sting); i++){
        checksum ^= checkSum_Sting[i];
         
     }
    
    ptr++; bytes ++;
    int checksum_from_string =  strtol(ptr, NULL, 16);
    #ifdef NMEA_DEBUG     
        printf("Checksum calculated is: %x\r\n", checksum);
        printf("Checksum grabbed is: %x\r\n", checksum_from_string);
        if(checksum == checksum_from_string){
            printf("Checksum Match\r\n");
        }
        else {
            printf("Checksum Mismatch\r\n");
        }
    #endif
}

bool stringCompare (char * src, char * compare, size_t stringSize){
    while(stringSize != 0){
        if(*src != *compare){
            return false;
        }
        src++;  compare++; stringSize--;
    }
    return true;
}

int stringCopy (char * src, char * dest, size_t stringSize){
    size_t bytes = stringSize;
    while(stringSize != 0){
        *dest = *src;
        src++;  dest++; stringSize--;
    }
    
    return bytes;
}
