#include <stdio.h>
#include <stdlib.h>
#include <Board.h>
#include <BNO055.h>
#include "timers.h"
// #include "CAPTOUCH.h"
#include "Oled.h"
#include "buttons.h"
#include "QEI.h"

typedef enum {
    Level_check,
    Liquid_One,
    Liquid_Two,
    Check_state,
    Pour,
    Done_Pour,

    NOT_LEVEL_STATE,
    WATER_LEVEL_LOW,
    NO_CUP,
    STOP
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
    BNO055_Init();
    TIMER_Init();
    // CAPTOUCH_Init();
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

                HAL_Delay(3000);

                OledClear(0);
                OledUpdate();
                OledDrawString("its level");
                OledUpdate();
                HAL_Delay(1000);
                QEI_ResetPosition();
                Current_state = Liquid_One;
            break;

            case Liquid_One:
                oled_sign = (char *)malloc(20 * sizeof(char));
                Liquid = 1;
                PourLevel = QEI_GetPosition();

                sprintf(oled_sign, "Vodka\n%d mml", PourLevel);
                OledClear(0);
                OledDrawString(oled_sign);
                OledUpdate();
                free(oled_sign);

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
                            QEI_ResetPosition();
                            break;
                        }
                        HAL_Delay(100);
                    }
                }
                HAL_Delay(100);
            break;

            case Liquid_Two:

                oled_sign = (char *)malloc(20 * sizeof(char));
                Liquid = 2;
                PourLevel = QEI_GetPosition();

                sprintf(oled_sign, "Water\n%d mml", PourLevel);
                OledClear(0);
                OledDrawString(oled_sign);
                OledUpdate();
                free(oled_sign);


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
                            QEI_ResetPosition();
                            break;
                        }
                        HAL_Delay(100);
                    }
                }
                HAL_Delay(100);
        
            break;

            case Check_state:
                OledClear(0);
                OledDrawString("Checking for Cup...");
                OledUpdate();
                HAL_Delay(1500);
                Current_state = Pour;

            break;

            case Pour:
                OledClear(0);
                OledDrawString("Pouring...\ntime left:\nmml Left:");
                OledUpdate();
                HAL_Delay(1500);
                Current_state = Done_Pour;
            break;


            case Done_Pour:

                OledClear(0);
                OledDrawString("Done");
                OledUpdate();
                HAL_Delay(1500);
                if(Liquid == 1){
                    Current_state = Liquid_One;
                }else{
                    Current_state = Liquid_Two;
                }
            break;
        }



    }

    return 0;
}