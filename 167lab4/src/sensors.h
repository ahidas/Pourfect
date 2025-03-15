
#ifndef SENSORS_H
#define SENSORS_H

#include "CAPTOUCH.h"
#include "QEI.h"
#include "PING.h"
#include "uart.h"
#include "I2C.h"
#include "BNO055.h"
#include "timers.h"

void SENSORS_Init(void);


int SENSORS_buttontouched(void);

int SENSORS_cupPresent(void);

int SENSORS_getWaterHeight();

int SENSORS_getPosition(void);

int SENSORS_getCupHeight(void);

int SENSORS_getWaterLevel(void);

int SENSORS_checkWaterLevel(void);

int SENSORS_checkLevel(void);



#endif

