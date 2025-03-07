#include "sensors.h"
#include <stdio.h>
void SENSORS_Init(void) {
    CAPTOUCH_Init();
    PING_Init();
    QEI_Init();
}



#define CUP_THRESH 400
int SENSORS_cupPresent(void) {
    printf("time of flight: %d\n", PING_GetTimeofFlight());
    return PING_GetTimeofFlight() < CUP_THRESH;
}

int SENSORS_getPosition(void) {
    return QEI_GetPosition();
}

