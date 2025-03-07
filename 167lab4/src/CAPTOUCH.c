#include "CAPTOUCH.h"
#include <stdio.h>

#define NUM_TIMES 10

volatile uint32_t times[NUM_TIMES]; //array to store time between interrupts
volatile uint32_t last_time = 0; //time of last interrupt
volatile int arr_index = 0; //index of array to store next time


void CAPTOUCH_Init(void) {
    //Configure GPIO pin PB5 
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // EXTI interrupt init
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    // the rest of the function goes here
}

// external interrupt ISR for rising edge of pin PB5  
// void EXTI9_5_IRQHandler(void) {
//     // EXTI line interrupt detected 
//     if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET) {
//         // Clear interrupt flag.
//         __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);

//         uint32_t curr_time = TIMERS_GetMicroSeconds(); //get current time
//         times[arr_index] = curr_time - last_time; //store time between interrupts
//         last_time = curr_time; //update last time
//         arr_index = (arr_index + 1) % NUM_TIMES; //update index
//         // Anything that needs to happen on rising edge of PB5
//         // (ENC_B).
//     }
// }
#define ON_THRESH 500
//take average and determine
char CAPTOUCH_IsTouched(void){
    uint32_t sum = 0;
    for(int i = 0; i < NUM_TIMES;i++){ //get total time of last 10 interrupts
        sum += times[i];
    }
    if(sum / 10 > ON_THRESH){  //if average time is greater than threshold, return 1
        return 1;
    } else {
        return 0;
    }
}