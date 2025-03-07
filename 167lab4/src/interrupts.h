/**
 * @file    loudred.c
 *
 * This is a test for adding four interrupts using available SYSCONFIG
 * external interrupts and available "input-able" pins from the UCSC Nucleo
 * IO Shield (used with the Nucleo F411RE dev kit).
 *
 * @deps    "Common/BOARD.h"                STM32 HAL interface from the ECE167
 *                                          Common repo.
 * @deps    "Common/framework_files/..."    STM32 HAL drivers.
 *
 * @author  nubby
 * @date    3 Mar 2025
*/
#include <stdlib.h>
#include <stdio.h>

#include <BOARD.h>

/** Module-level declarations.  **/
volatile uint8_t encA_prev = 0;
volatile uint8_t encB_prev = 0;
volatile uint8_t encC_prev = 0;
volatile uint8_t encD_prev = 0;


/** Interrupts_Init()
 * 
 * Configure PB4, PB5, PD2, and PB14 as interrupts using EXTI4, EXTI9, EXTI2,
 * and EXTI15, respectively.
 *
 * @returns (int)   SUCCESS/ERROR.
 */
int Interrupts_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    //Configure GPIO pins : PB4 PB5
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    //Configure GPIO pin  : PD2
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    //Configure GPIO pin  : PA15
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // EXTI interrupt init
    HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
    HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    encA_prev = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
    encB_prev = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
    encC_prev = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2);
    encD_prev = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);

    return SUCCESS;
}

/**setUp()
 *
 * Initialize 4 pins as interrupts.
 *
 * @returns SUCCESS/ERROR.
 */
int setUp(void)
{
    BOARD_Init();
    Interrupts_Init();
    return SUCCESS;
}

/**cleanUp()
 *
 * It should never reach this point.
 *
 * @returns SUCCESS/ERROR.
 */
int cleanUp(void)
{
    BOARD_End();
    return SUCCESS;
}

int main(void)
{
    setUp();
    while(1)
    {
        printf("Interrupt statuses:\r\n");
        printf("\t+ FLAG0 - %d\r\n", encA_prev);
        printf("\t+ FLAG1 - %d\r\n", encB_prev);
        printf("\t+ FLAG2 - %d\r\n", encC_prev);
        printf("\t+ FLAG3 - %d\r\n", encD_prev);
        HAL_Delay(100);
    }
    cleanUp();
}

// SYSCFG external interrupt ISR for pin PD2.
void EXTI2_IRQHandler(void) {
    // EXTI line interrupt detected 
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2); // Clear interrupt flag.
        encC_prev = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2);
    }
    else if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2) == RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2); // Clear interrupt flag.
        encC_prev = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2);
    }
}

