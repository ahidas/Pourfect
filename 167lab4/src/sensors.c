#include "sensors.h"
#include <stdio.h>
void SENSORS_Init(void) {
    CAPTOUCH_Init();
    PING_Init();
    QEI_Init();
    I2C_Init();  // Initialize I2C for communication with sensors
   // BNO055_Init();
    Uart1_Init(9600);
}




int SENSORS_cupPresent(void) {
    return SENSORS_getCupHeight() ? 1 : 0;
}

int SENSORS_getPosition(void) {
    return QEI_GetPosition();
}

uint8_t data[10] = {0};  // Buffer for UART data
#define NO_CUP_THRESH 3
int SENSORS_getCupHeight(void){

    int No_cup_count = 0;
    while(No_cup_count < NO_CUP_THRESH){
    if(Uart1_rx(data, 10) == -1){
        printf("ERROR\n");
    }  
    int height = 0;
   // printf("printing data\n");
    for(int i = 0; i < 10; i++){
        if(!data[i]){
            height++;
        } else {
            No_cup_count++;
            break;
        }
    }
    if(height){
        return height;
    }
}


    //printf("height: %d\n", height);
    return 0;  // Example: just return the data as height
}

#define PING_DEFAULT_DISTANCE 64  // Default distance for water height calculation
int SENSORS_getWaterHeight(void) {
    int distance = PING_GetDistance();
    printf("PING distance: %d\n", distance);  // Print distance for debugging
    //need to calibrate maybe
    //printf("distance: %d\n", distance);  // Print distance for debugging
    int height = PING_DEFAULT_DISTANCE - distance;  // Example calculation
    HAL_Delay(100);  // Delay for stability
    return height;  // Example: just return the data as height

    //red brown yellow abc
}




int water_low = 0;
unsigned char water_level_arr[20] = {0};
int SENSORS_getWaterLevel(void) {
        return 11;
        int waterLevel = 0;
        if(water_low){
            return 0;
        }
        for (int i = 0; i < 20; i++) { //read first 8 bytes
            if(i <= 7){
            water_level_arr[i] = I2C_ReadRegister(0x77, 0x77); 
            }
            else {
                water_level_arr[i] = I2C_ReadRegister(0x77, 0x78);
            }
            waterLevel += water_level_arr[i] != 0;
           // printf("water level: %d\n", waterLevel);
        }
        // for (int i = 0; i < 8; i++) {
        //     low_water_level[i] = I2C_ReadRegister(0x77, 0x00);  // Read each section
        //     if(low_water_level[i] == 1) {
        //         waterLevel++;  // Increment water level if section is empty
        //     } else break;
        // }
        // printf("water level: %d", )
    return waterLevel;  // Return the calculated height
}

#define WATER_LOW_THRESH 1
#define WATER_HIGH_THRESH 3
int level_low = 1; //0 means water is low, 1 is water high
int SENSORS_checkWaterLevel(void){
   // return 1;  // Placeholder for water level check logic
    int water_height = SENSORS_getWaterLevel();
    if(water_height < WATER_LOW_THRESH){
        level_low = 0;
    } else if( water_height > WATER_HIGH_THRESH){
        level_low = 1;
    }
    return level_low;  // Check if water level is below threshold
}

#define Z_THRESH 100
int SENSORS_checkLevel(){
    int zAccel = BNO055_ReadAccelZ();
   // printf("Z acceleration: %d\n", zAccel);  // Print Z acceleration for debugging
   return 1;  // Placeholder for level check logic
    return (zAccel < 1000 + Z_THRESH && zAccel > 1000 - Z_THRESH) ? 1 : 0;  // Check if Z acceleration is below threshold
}