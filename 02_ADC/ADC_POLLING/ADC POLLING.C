//ADC POLLING

#include "driverlib.h"
#include "device.h"
#include "board.h"


void main(void)
{
    Device_init();
    Device_initGPIO();
    Board_init();

    while(1)
    {
         ADC_forceSOC(ADC0_BASE, ADC0_SOC0);
        DEVICE_DELAY_US(20);
        uint16_t res =ADC_readResult(ADC0_RESULT_BASE, ADC0_SOC0);
        uint16_t a = res;
        DEVICE_DELAY_US(20000);
    }


    }
