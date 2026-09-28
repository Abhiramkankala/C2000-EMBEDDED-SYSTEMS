//Key Debounce

#include "driverlib.h"
#include "device.h"
#include "board.h"
//
// Main
//
void main(void)
{
    Device_init();
    Device_initGPIO();
    Board_init();
    __uint32_t cnt=0;
    
    uint32_t PUSH=12;
    GPIO_setPinConfig(GPIO_12_GPIO12);
    GPIO_setDirectionMode(PUSH, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(PUSH, GPIO_PIN_TYPE_STD);

   

    while(1)
    {
        __uint32_t state=GPIO_readPin(PUSH);
        if(state==1){
            if(cnt<=500000){
                GPIO_writePin(GREEN_LED_GPIO, 0);
                cnt+=10000;
                DEVICE_DELAY_US(10000);
                
            }
            else {
                GPIO_writePin(GREEN_LED_GPIO, 1);
                DEVICE_DELAY_US(5000000);
                GPIO_writePin(RED_LED_GPIO, 0);
                cnt=0;
                DEVICE_DELAY_US(10000);
            }
        }
        else {
            GPIO_writePin(GREEN_LED_GPIO, 1);
            GPIO_writePin(RED_LED_GPIO, 1);
            cnt=0;
        }
        DEVICE_DELAY_US(20000);
    }
} 