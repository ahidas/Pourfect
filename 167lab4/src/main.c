#include <stdio.h>
#include <stdlib.h>
#include <Board.h>
#include <BNO055.h>
#include "timers.h"
#include "sensors.h"
#include "I2C.h"
#include "DFRobot_RGBLCD1602.h"
#include "uart.h"

// void I2C_Scan()
// {
//     I2C_HandleTypeDef hi2c2;
//     static uint8_t initStatus = FALSE;
//     if (initStatus == FALSE)
//     {

//         hi2c2.Instance = I2C2;
//         hi2c2.Init.ClockSpeed = 100000;
//         hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
//         hi2c2.Init.OwnAddress1 = 0;
//         hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//         hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//         hi2c2.Init.OwnAddress2 = 0;
//         hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//         hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//         if (HAL_I2C_Init(&hi2c2) != HAL_OK)
//         {
//             return ERROR;
//         }
//         initStatus = TRUE;
//     }
//     printf("Scanning I2C bus...\n");
//     for (uint8_t addr = 1; addr < 0x80; addr++) {
//         if (HAL_I2C_IsDeviceReady(&hi2c2, (addr << 1), 1, HAL_MAX_DELAY) == HAL_OK) {
//             printf("Device found at 0x%02X\n", addr);
//         }
//     }
// }

int main(void) {
    BOARD_Init();
    TIMER_Init();
    I2C_Init();
    DFRobot_Init();
   // clear();
    SENSORS_Init();


    unsigned char low_water_level[8];
    const char* string = "Hello, World!";
    int str_len = strlen(string);
    printstr(string);
    int scroll_index = 0;

    while(1){
        printf("position: %d\n", SENSORS_getPosition());
        printf("Is IMU level: %d\n", SENSORS_checkLevel());
        printf("Water level: %d\n", SENSORS_getWaterLevel());
        printf("IS water level enough?: %d\n", SENSORS_checkWaterLevel());
        printf("Cup height: %d\n", SENSORS_getCupHeight());
        printf("Is cup present?: %d\n", SENSORS_cupPresent());
        HAL_Delay(5000);
    }


//     while(1) {
//  // Print received data to console
//         if(scroll_index < str_len) {
//             scrollDisplayLeft();
//             scroll_index++;
//         } else {
//             scroll_index = 0;
//             home();
//             printstr(string);  // Reset the display
//         }
//         scrollDisplayLeft();
//         SENSORS_cupPresent();
//         printf("position: %d\n", SENSORS_getPosition());
//         printf("z_accel: %d\n", BNO055_ReadAccelZ());
//         for (int i = 0; i < 2; i++) {
//             low_water_level[i] = I2C_ReadRegister(0x77, 0x00);  // Read each section
//             if (low_water_level[0] == 0 && low_water_level[1] == 0){
//                 printf("low liquid level detected\n");
//                 // PWM_SetDutyCycle(PWM_4, 100);
//             }else{
//                 printf("liquid level suffcient\n");
//                 // PWM_SetDutyCycle(PWM_4, 0);

//             }
//         }
//         HAL_Delay(1000);

//     }
    return 0;
}