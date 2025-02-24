#include <stdio.h>
#include <stdlib.h>
#include <Board.h>
#include <BNO055.h>
#include "timers.h"


int main(void) {
    BOARD_Init();
    BNO055_Init();
    TIMER_Init();

    return 0;
}