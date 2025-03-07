#include <Board.h>
#include <timers.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include <stm32f4xx_hal_tim.h>

#define BUFFER_SIZE 10      // Size for moving average buffer

static volatile uint32_t buffer[BUFFER_SIZE] = {0};
static volatile uint8_t bufferIndex = 0;
static volatile uint32_t sum = 0;
static volatile uint8_t count = 0;

static volatile uint32_t last_time = 0;  // Last rising edge time
static volatile uint32_t avg_period = 0;  // Averaged period

uint32_t average(uint32_t num){
    sum -= buffer[bufferIndex];
    buffer[bufferIndex] = num;
    sum += num;

    bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;
    return sum/BUFFER_SIZE;
}

void CAPTOUCH_Init(void) {
    TIMER_Init();  // Initialize timer
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    // EXTI interrupt init
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    last_time = TIMERS_GetMicroSeconds();
}

// External interrupt ISR
void EXTI9_5_IRQHandler(void) {
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);  // Clear interrupt flag

        uint32_t current_time = TIMERS_GetMicroSeconds();  // Get current time

        uint32_t new_period = current_time - last_time;  // Calculate period

        avg_period= average(new_period);
        
    
        last_time = TIMERS_GetMicroSeconds();
    }
}

char CAPTOUCH_IsTouched(void) {
    if(avg_period > 230){
        return TRUE;
    }else{
        return FALSE;
    }
    
}
