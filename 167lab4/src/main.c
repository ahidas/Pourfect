#include <stdio.h>
#include <stdlib.h>
#include <Board.h>
#include <BNO055.h>
#include "timers.h"
#include "sensors.h"
#include "I2C.h"
#include "DFRobot_RGBLCD1602.h"


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
    printf("inniting\n");
    printf("inniting\n");
    TIMER_Init();
    I2C_Init();
    BNO055_Init();
    DFRobot_Init();
   // clear();
    SENSORS_Init();
    printstr("Hello, World!");

    while(1) {
        SENSORS_cupPresent();
        printf("position: %d\n", SENSORS_getPosition());
        printf("z_accel: %d\n", BNO055_ReadAccelZ());
        HAL_Delay(1000);
    }
    return 0;
}