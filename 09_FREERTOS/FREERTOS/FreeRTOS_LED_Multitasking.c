#include "driverlib.h"
#include "device.h"
#include "board.h"
#include "c2000_freertos.h"

//
// Main
//
void main(void)
{
    Device_init();
    Interrupt_initModule();
    Device_initGPIO();

    DINT;
    IER = 0x000;
    IFR = 0x000;
    Interrupt_initVectorTable();
    Board_init();
    FreeRTOS_init();
}

void LED_RedTask(void*pvParameters)
{
    (void)pvParameters;
    TickType_t xlastMakeTime = xTaskGetTickCount();
    while (1) 
    {
        GPIO_togglePin(LED1_GPIO);
        vTaskDelayUntil(&xlastMakeTime,pdMS_TO_TICKS(5000));
    
    }
}
//
void LED_GreenTask(void*pvParameters)
{
    (void)pvParameters;
    TickType_t xlastMakeTime = xTaskGetTickCount();
    while (1) 
    {
        GPIO_togglePin(LED2_GPIO);
        vTaskDelayUntil(&xlastMakeTime,pdMS_TO_TICKS(1000));
    
    }
}