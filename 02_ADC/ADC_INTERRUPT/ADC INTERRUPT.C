//WITH INTERRRUPT
#include "driverlib.h"
#include "device.h"
#include "board.h"

volatile uint16_t digval = 0;
volatile float voltage = 0.0f;

void main(void)
{
    Device_init();
    Device_initGPIO();
    Board_init();

    EINT;
    ERTM;

    while(1)
    {
        // Start ADC conversion
        ADC_forceSOC(ADC0_BASE, ADC_SOC_NUMBER0);

        // Wait until conversion is complete
        while (ADC_getInterruptStatus(ADC0_BASE, ADC_INT_NUMBER1) == false);

        // Clear ADC interrupt flag
        ADC_clearInterruptStatus(ADC0_BASE, ADC_INT_NUMBER1);

        // Read ADC digital value
        digval = ADC_readResult(ADC0_RESULT_BASE, ADC_SOC_NUMBER0);

        // Convert digital value to voltage
        voltage = ((float)digval / 4095.0f) * 3.3f;

        // Wait 100 ms
        DEVICE_DELAY_US(100000);
    }
}