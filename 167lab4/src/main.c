#include <stdio.h>
#include <stdlib.h>
#include <Board.h>
#include <BNO055.h>
#include "timers.h"
#include "CAPTOUCH.h"
#include "Oled.h"
#include "buttons.h"
#include "QEI.h"
#include "sensors.h"
#include "DFRobot_RGBLCD1602.h"
#include "pwm.h"
typedef enum {
    Level_check,
    Button,
    Check_state,
    Pour,
    Done_Pour
}State;

typedef enum {
    Auto_pour,
    Manual_pour
} Pour_mode;

void startPump(){
    // Start the pump
    printf("Starting pump...\r\n");
    PWM_SetDutyCycle(PWM_4, 100);
}

void stopPump(){
    // Stop the pump
    printf("Stopping pump...\r\n");
    PWM_SetDutyCycle(PWM_4, 0);
}

int checks(){
    if(!SENSORS_checkLevel()){
        printf("level is uneven\r\n");
        return 0;
    }
    if(!SENSORS_cupPresent()){
        printf("no cup present\r\n");
        return 0;
    }
    if(!SENSORS_checkWaterLevel()){
        printf("water level is low\r\n");
        return 0;
    }
    return 1;
}
//start on level check 
State Current_state =  Level_check;
Pour_mode Pour_type = Auto_pour;

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
    BUTTONS_Init();
    QEI_Init();
    SENSORS_Init();
    DFRobot_Init();
    PWM_Init();

    uint8_t buttons = 0;
    int pour_amount = 0;
    char buffer[32];  // Buffer to hold the formatted string
    while(1){
        switch(Current_state){
            case Level_check:
                if(!checks()){
                    HAL_Delay(100);
                    continue;
                }

                HAL_Delay(1000);
                setCursor(0, 0);
                sprintf(buffer, "Water Level: %d%%", SENSORS_getWaterLevel() * 10);
                printstr(buffer);
                if(!SENSORS_cupPresent()){
                    setCursor(0, 1);
                    printstr("No cup present level check");
                    HAL_Delay(1000);
                    continue;
                }
                if(!SENSORS_checkWaterLevel()){
                    setCursor(0, 1);
                    printstr("Water level low");
                    HAL_Delay(1000);
                    continue;
                }
                if(!SENSORS_checkLevel()){
                    setCursor(0, 1);
                    printstr("Level is uneven");
                    HAL_Delay(1000);
                    continue;
                }
                HAL_Delay(100);
                printf("moving to button\n");
                Current_state = Button;
            break;

            case Button:
                pour_amount = SENSORS_getPosition();
                if(!checks()){
                    Current_state = Level_check;
                    continue;
                }
                setCursor(0, 1);
                printstr("                      ");
                sprintf(buffer, "Amount: %d ", pour_amount);
                setCursor(0, 1);
                printstr(buffer);
                buttons = buttons_state();
                if((buttons & 0x1) == 0){
                    printf("press detected\r\n");
                    for(int i = 0; i <= 100; i++){
                        pour_amount = SENSORS_getPosition();
                        setCursor(0, 1);
                        sprintf(buffer, "Amount: %d", pour_amount);
                        printstr(buffer);
                        buttons = buttons_state();
                        if((( buttons & 0x1) == 1) && (i >= 7)){
                            printf("Long press \r\n");
                            printf("moving to Check state\n");
                            Pour_type = Manual_pour;
                            Current_state = Check_state;
                            break;
                        }
                        if(( buttons & 0x1) == 1){
                            printf("short press\r\n");
                            printf("moving to check state\n");
                            Pour_type = Auto_pour;
                            Current_state = Check_state;
                            break;
                        }
                        HAL_Delay(100);
                    }
                }
                HAL_Delay(100);
            break;


            case Check_state:
                printf("in check state\r\n");
                if(!checks()){
                    Current_state = Button;
                    continue;
                }
                HAL_Delay(1500);
                Current_state = Pour;

            break;

            case Pour:
            printf("in pour state\r\n");
            if(Pour_type == Auto_pour){
                printf("pouring automatically\r\n");
                startPump();
                while(SENSORS_getWaterHeight() < SENSORS_getCupHeight()){
                    //pour
                    printf("water height: %d cup height: %d\r\n", SENSORS_getWaterHeight(), SENSORS_getCupHeight());
                    printf("pouring auto...\r\n");

                    if(!checks()){
                        break;
                    }
                    HAL_Delay(100);
                }
            } else {
                startPump();
                printf("pouring manually: %d\r\n", pour_amount);
                int count = 0;
                while(count < pour_amount){
                    //pour

                    printf("pouring man...\r\n");
                    count++;
                    if(!checks()){
                        break;
                    }
                }
            }
            stopPump();
            printf("water height: %d cup height: %d\r\n", SENSORS_getWaterHeight(), SENSORS_getCupHeight());
            Current_state = Done_Pour;
            break;


            case Done_Pour:
            printf("done pouring pour\n moving to check level\n");
            Current_state = Level_check;
            break;
        }



    }

    return 0;
}