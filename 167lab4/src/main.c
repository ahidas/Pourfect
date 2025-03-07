#include <stdio.h>
#include <stdlib.h>
#include <Board.h>
#include <BNO055.h>
#include "timers.h"
#include "sensors.h"

int main(void) {
    BOARD_Init();
    BNO055_Init();
    TIMER_Init();

    SENSORS_Init();
    while(1) {
        SENSORS_cupPresent();
        printf("position: %d\n", SENSORS_getPosition());
        HAL_Delay(1000);
    }
    return 0;
}