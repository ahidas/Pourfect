
#ifndef SENSORS_H
#define SENSORS_H

#include "CAPTOUCH.h"
#include "QEI.h"
#include "PING.h"


void SENSORS_Init(void);


int SENSORS_isTouched(void);

int SENSORS_cupPresent(void);

int SENSORS_getPosition(void);




#endif

