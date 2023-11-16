/* USER CODE BEGIN Includes */
#include "main.h"
#include <stdbool.h>
#include <string.h>
/* USER CODE END Includes */

/* USER CODE BEGIN PV */
volatile uint32_t button1PressCount = 0;
volatile uint32_t button2PressCount = 0;
volatile uint32_t lastButton1PressTime = 0;
volatile uint32_t lastButton2PressTime = 0;
uint32_t currentTime = 0;
bool button1Pressed = false;
bool button2Pressed = false;

#define BUTTON1_PIN GPIO_PIN_X
#define BUTTON2_PIN GPIO_PIN_Y 
/* USER CODE END PV */

/* USER CODE BEGIN 2 */
// Initialize SysTick for time measurement
HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */

void LogButtonPressUART(char *button, uint32_t pressCount, uint32_t timeElapsed)
{
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Button %s pressed %lu times. Time since last press: %lu ms\r\n", button, pressCount, timeElapsed);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 1000);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == BUTTON1_PIN)
    {
        button1PressCount++;
        currentTime = HAL_GetTick();
        if(button1Pressed)
        {
            uint32_t timeElapsed = currentTime - lastButton1PressTime;
            // Log time between presses for Button 1
        }
        lastButton1PressTime = currentTime;
        button1Pressed = true;
        LogButtonPressUART("1", button1PressCount, currentTime - lastButton1PressTime);
    }
    else if(GPIO_Pin == BUTTON2_PIN)
    {
        button2PressCount++;
        currentTime = HAL_GetTick();
        if(button2Pressed)
        {
            uint32_t timeElapsed = currentTime - lastButton2PressTime;
            // Log time between presses for Button 2
        }
        lastButton2PressTime = currentTime;
        button2Pressed = true;
        LogButtonPressUART("2", button2PressCount, currentTime - lastButton2PressTime);
    }
}
/* USER CODE END 4 */

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* ... existing code ... */

    // Initialize GPIO pins for buttons with interrupt capability
    GPIO_InitStruct.Pin = BUTTON1_PIN | BUTTON2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING; // Assuming falling edge trigger for button press
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct); // Replace GPIOx with actual GPIO Port

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTIx_IRQn, 0, 0); // Replace EXTIx_IRQn with actual EXTI line
    HAL_NVIC_EnableIRQ(EXTIx_IRQn);
}
