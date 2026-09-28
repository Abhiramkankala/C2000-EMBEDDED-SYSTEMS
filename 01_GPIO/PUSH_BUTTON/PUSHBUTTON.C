//Push Button

#include "driverlib.h"
#include "device.h"
//
// Main
//
void main(void)
{
    Device_init();
    Device_initGPIO();
    uint32_t LED=32;
    uint32_t PUSH=12;
    GPIO_setPinConfig(GPIO_32_GPIO32);
    GPIO_setDirectionMode(LED, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(LED, GPIO_PIN_TYPE_STD);
    GPIO_setPinConfig(GPIO_12_GPIO12);
    GPIO_setDirectionMode(LED, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(LED, GPIO_PIN_TYPE_STD);
    GPIO_writePin(LED, 0);
    while(1)
    {
        if (GPIO_readPin(PUSH))
        {
            GPIO_writePin(LED, 1);
            DEVICE_DELAY_US(10000);
        }
        else {
            GPIO_writePin(LED, 0);
        }
        
    }
}