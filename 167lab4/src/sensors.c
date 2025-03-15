#include "sensors.h"
#include <stdio.h>
void SENSORS_Init(void) {
    CAPTOUCH_Init();
    PING_Init();
    QEI_Init();
    I2C_Init();  // Initialize I2C for communication with sensors
    BNO055_Init();
    Uart1_Init(9600);
}




int SENSORS_cupPresent(void) {
    return SENSORS_getCupHeight() ? 1 : 0;
}

int SENSORS_getPosition(void) {
    return QEI_GetPosition();
}

uint8_t data[10] = {0};  // Buffer for UART data

int SENSORS_getCupHeight(void){
    Uart1_rx(data, 10);  // Read data from UART
    int height = 0;
   // printf("printing data\n");
    for(int i = 0; i < 10; i++){
 //       printf("data[%d]: %d\n", i, data[i]);  // Print received data for debugging
        if(!data[i]){
            height++;
        } else {
            break;
        }
    }
    return height;  // Example: just return the data as height
}

#define PING_DEFAULT_DISTANCE 10  // Default distance for water height calculation
int SENSORS_getWaterHeight(void) {
    int distance = PING_GetDistance();
    //need to calibrate maybe
    int height = PING_DEFAULT_DISTANCE - distance;  // Example calculation
    return height;  // Example: just return the data as height
}



int SENSORS_getWaterLevel(void) {
        unsigned char low_water_level[8];
        int waterLevel = 0;
        for (int i = 0; i < 8; i++) {
            low_water_level[i] = I2C_ReadRegister(0x77, 0x00);  // Read each section
            if(low_water_level[i] == 1) {
                waterLevel++;  // Increment water level if section is empty
            } else break;
        }
    return waterLevel;  // Return the calculated height
}

int SENSORS_checkWaterLevel(void){
    return SENSORS_getWaterLevel() < 2 ? 0 : 1;  // Check if water level is below threshold
}

#define Z_THRESH 100
int SENSORS_checkLevel(){
    int zAccel = BNO055_ReadAccelZ();
    return (zAccel < 1000 + Z_THRESH && zAccel > 1000 - Z_THRESH) ? 1 : 0;  // Check if Z acceleration is below threshold
}