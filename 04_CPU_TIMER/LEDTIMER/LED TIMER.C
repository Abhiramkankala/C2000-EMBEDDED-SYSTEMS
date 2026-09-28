//timer 1
#include "driverlib.h"
#include "device.h"
#include "board.h"


void main(void)
{
    
    Device_init();
    Device_initGPIO();
    Board_init();
    CPUTimer_startTimer(myCPUTIMER0_BASE);
    
    while(1)
        if(CPUTimer_getTimerOverflowStatus(myCPUTIMER0_BASE))
        {
            CPUTimer_clearOverflowFlag(myCPUTIMER0_BASE);
            GPIO_togglePin(RED_LED_GPIO);
            GPIO_togglePin(GREEN_LED_GPIO);

       
    }
}