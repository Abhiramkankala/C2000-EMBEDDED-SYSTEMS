//sci
#include "stdio.h"
#include "string.h"
#include "driverlib.h"
#include "device.h"
#include "board.h"


void main(void)
{
    Device_init();
    Device_initGPIO();
    Board_init();

    char arr[]= "HELLO WORLD\r\n";
    while(1)
    {
        SCI_writeCharArray(mySCI0_BASE,( uint16_t *)arr, sizeof(arr) );
        DEVICE_DELAY_US(2000000);
    }
}
