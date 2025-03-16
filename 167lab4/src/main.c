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
   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);  //set the pin high
}

void stopPump(){
    // Stop the pump
    printf("Stopping pump...\r\n");
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7, GPIO_PIN_RESET);  //set the pin high
}

int checks(){
   // printf("in checks\n");
    // if(!SENSORS_checkLevel()){
    //     printf("level is uneven\r\n");
    //     clear();
    //     printstr("Uneven level");
    //     return 0;
    // }
    if(!SENSORS_cupPresent()){
        printf("no cup present\r\n");
        clear();
        printstr("No cup");
        return 0;
    }
    if(!SENSORS_checkWaterLevel()){
        printf("water level is low\r\n");
        clear();
        printstr("Not enough water");
        return 0;
    }
   // printf("done checks\n");
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

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7, GPIO_PIN_RESET);  //set the pin high

    uint8_t buttons = 0;
    int pour_amount = 0;
    char buffer[32];  // Buffer to hold the formatted string
    while(1){
        switch(Current_state){
            case Level_check:
                HAL_Delay(1000);

                if(!checks()){
                    printf("here\n");
                    continue;
                }
                HAL_Delay(100);
                setCursor(0, 0);
                sprintf(buffer, "Water Level: %.0f%%", SENSORS_getWaterLevel() / 2.0 * 10);
                clearPrint(buffer,0);
                printf("moving to button\n");
                Current_state = Button;
            break;

            case Button:
                pour_amount = SENSORS_getPosition();
                if(!checks()){
                    Current_state = Level_check;
                    continue;
                }
                setCursor(0, 0);
                sprintf(buffer, "Water Level: %.0f%%", SENSORS_getWaterLevel() / 2.0 * 10);
                clearPrint(buffer,0);

                sprintf(buffer, "Amount: %d ", pour_amount);
                setCursor(0, 1);
                clearPrint(buffer,1);
                buttons = buttons_state();
                if((buttons & 0x1) == 0){
                    printf("press detected\r\n");
                    for(int i = 0; i <= 100; i++){
                        pour_amount = SENSORS_getPosition();
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
            float last_percent = 0;
            if(Pour_type == Auto_pour){
                printf("pouring automatically\r\n");
                int getCupHeight = SENSORS_getCupHeight();
                int water_height = 0;
                startPump();
                while(water_height < getCupHeight * 3.5){
                    water_height = SENSORS_getWaterHeight();
                    if(water_height <= 0){
                        water_height = 1;
                    }
                    printf("pouring auto...\r\n");
                    float percent = 100.0 * water_height / (getCupHeight * 3.5);
                    if (((percent > last_percent) && percent <= 100)  || last_percent == 0){
                    sprintf(buffer, "%.0f%%",  percent);
                    setCursor(0, 1);
                    clearPrint(buffer,1);
                    last_percent = percent;
                    }
                    if(!checks()){
                        break;
                    }
                    HAL_Delay(200);
                }
                printf("broke out\n");
            } else {
                startPump();
                printf("pouring manually: %d\r\n", pour_amount);
                int count = 0;
                while(count < pour_amount){
                    //pour
                    sprintf(buffer, "Amount: %d ", pour_amount - count - 1);
                    setCursor(0, 1);
                    clearPrint(buffer,1);
                    printf("pouring man...\r\n");
                    count++;
                    if(!checks()){
                        break;
                    }
                    HAL_Delay(200);
                }
            }
            stopPump();
          //  printf("water height: %d cup height: %d\r\n", SENSORS_getWaterHeight(), SENSORS_getCupHeight());
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