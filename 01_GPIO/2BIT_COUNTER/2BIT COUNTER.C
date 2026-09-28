//2BIT COUNTER USING TOGGLE
#include "driverlib.h"
#include "device.h"

#define LED4    23U
#define LED5    34U

void main(void)
{
    Device_init();
    Device_initGPIO();

    GPIO_setPinConfig(GPIO_23_GPIO23);
    GPIO_setPinConfig(GPIO_34_GPIO34);

    GPIO_setDirectionMode(LED4, GPIO_DIR_MODE_OUT);
    GPIO_setDirectionMode(LED5, GPIO_DIR_MODE_OUT);

    
    GPIO_writePin(LED4, 0);
    GPIO_writePin(LED5, 0);

    while(1)
    {
        
        GPIO_togglePin(LED4);
        DEVICE_DELAY_US(1000000);
        GPIO_togglePin(LED4);
        GPIO_togglePin(LED5);
        DEVICE_DELAY_US(1000000);
        GPIO_togglePin(LED4);
        DEVICE_DELAY_US(1000000);

        
        GPIO_togglePin(LED4);
        GPIO_togglePin(LED5);
        DEVICE_DELAY_US(1000000);
    }
}