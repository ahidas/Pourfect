#include <stdio.h>
#include <stdlib.h>
#include <Board.h>
#include <BNO055.h>
#include "timers.h"
#include "CAPTOUCH.h"
#include "Oled.h"
#include "buttons.h"
#include "QEI.h"

typedef enum {
    Level_check,
    Liquid_One,
    Liquid_Two,
    Check_state,
    Pour,
    Done_Pour
}State;

//start on level check 
State Current_state =  Level_check;

// to remember what liquid needs to be pour
// 1 - liquid 1
// 2 - liquid 2 
int Liquid = 1;

//drink level
int PourLevel =  0;
int main(void) {
    BOARD_Init();
    TIMER_Init();
    CAPTOUCH_Init();
    OledInit();
    OledClear(0);
    OledUpdate();
    BUTTONS_Init();
    QEI_Init();
    char *oled_sign;
    uint8_t buttons = 0;
    while(1){
        switch(Current_state){
            case Level_check:
                OledDrawString("Check Level");
                OledUpdate();

                HAL_Delay(1000);

                OledClear(0);
                OledUpdate();
                OledDrawString("its level");
                OledUpdate();
                HAL_Delay(100);
                QEI_ResetPosition();
                Current_state = Liquid_One;
            break;

            case Liquid_One:
                oled_sign = (char *)malloc(20 * sizeof(char));
                PourLevel = QEI_GetPosition();

                sprintf(oled_sign, "%d", PourLevel);
                OledClear(0);
                OledDrawString(oled_sign);
                OledUpdate();


                

                buttons = buttons_state();
                if((buttons & 0x1) == 0){
                    printf("press detected\r\n");
                    for(int i = 0; i <= 100; i++){
                        buttons = buttons_state();
                        if((( buttons & 0x1) == 1) && (i >= 7)){
                            printf("Long press \r\n");
                            Current_state = Check_state;
                            break;
                        }
                        if(( buttons & 0x1) == 1){
                            printf("short press\r\n");
                            Current_state = Liquid_Two;
                            break;
                        }
                        HAL_Delay(100);
                    }
                }
                HAL_Delay(100);
            break;

            case Liquid_Two:

                OledClear(0);
                OledDrawString("Water");
                OledUpdate();
                buttons = buttons_state();
                if((buttons & 0x1) == 0){
                    printf("press detected\r\n");
                    for(int i = 0; i <= 100; i++){
                        buttons = buttons_state();
                        if((( buttons & 0x1) == 1) && (i >= 7)){
                            printf("Long press \r\n");
                            Current_state = Check_state;
                            break;
                        }
                        if(( buttons & 0x1) == 1){
                            printf("short press\r\n");
                            Current_state = Liquid_One;
                            break;
                        }
                        HAL_Delay(100);
                    }
                }
                HAL_Delay(100);
        
            break;

            case Check_state:
                OledClear(0);
                OledDrawString("Checking for Cup");
                OledUpdate();
                HAL_Delay(1500);
                Current_state = Pour;

            break;

            case Pour:
                OledClear(0);
                OledDrawString("Pouring");
                OledUpdate();
            break;


            case Done_Pour:
            
            break;
        }



    }

    return 0;
}