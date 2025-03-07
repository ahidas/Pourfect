


/*******************************************************************************
 * INCLUDES                                                                    *
 ******************************************************************************/
#include <Board.h>
#include <QEI.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include <stm32f4xx_hal_tim.h>

static volatile int32_t encoderCount = 0; // Position count
static volatile uint8_t state = 0;        // Current state of the encoder

void QEI_Init(void) {
    //Configure GPIO pins : PB4 PB5 
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // EXTI interrupt init
    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    // the rest of the function goes here shifts the A pin to the left and the B pin to the right and combine both
    state = ((HAL_GPIO_ReadPin(GPIOB, ENC_A) << 1) | HAL_GPIO_ReadPin(GPIOB, ENC_B));
    encoderCount = 0;
}

int QEI_GetPosition(void) {
    // Convert count to degrees (assuming 360 counts per revolution)
    int count = (encoderCount/4)*15;
    return count;
}

void QEI_ResetPosition(void) {
    encoderCount = 0;
    state = ((HAL_GPIO_ReadPin(GPIOB, ENC_A) << 1) | HAL_GPIO_ReadPin(GPIOB, ENC_B));
}
//external interrupt ISR for pin PB5 
void EXTI9_5_IRQHandler(void) {
    // EXTI line interrupt detected 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5); // clear interrupt flag
        QEI_IRQ();
    }
}

// external interrupt ISR for pin PB4
void EXTI4_IRQHandler(void) {
    // EXTI line interrupt detected 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4); // clear interrupt flag
        QEI_IRQ();
    }
}
void QEI_IRQ() {
    uint8_t newState = ((HAL_GPIO_ReadPin(GPIOB, ENC_A) << 1) | HAL_GPIO_ReadPin(GPIOB, ENC_B));

    // Update encoder count based on state transition
    switch (state) {
        case 0b00:
            if (newState == 0b01) {
                encoderCount--;
            } else if (newState == 0b10) {
                encoderCount++;
            }
            break;
        case 0b01:
            if (newState == 0b11) {
                encoderCount--;
            } else if (newState == 0b00) {
                encoderCount++;
            }
            break;
        case 0b11:
            if (newState == 0b10) {
                encoderCount--;
            } else if (newState == 0b01) {
                encoderCount++;
            }
            break;
        case 0b10:
            if (newState == 0b00) {
                encoderCount--;
            } else if (newState == 0b11) {
                encoderCount++;
            }
            break;
    }

    if (encoderCount >= 96) {
        encoderCount = 0; // Reset to 0 if it reaches 360 degrees
    } else if (encoderCount <= -96) {
        encoderCount = 0; // Reset to 0 if it reaches -360 degrees
    }

    // Update state
    state = newState;
}

