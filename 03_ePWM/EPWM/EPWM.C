//pwm code
#include "driverlib.h"
#include "device.h"
#include "board.h"

void main(void)
{
    Device_init();
    Device_initGPIO();
    Board_init();
    EPWM_setCounterCompareValue(EPWM_BASE, EPWM_COUNTER_COMPARE_A, 1000);
}
