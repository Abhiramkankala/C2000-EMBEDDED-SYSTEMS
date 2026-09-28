//fifo blockING AND NON
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include <stdio.h>
#include <string.h>
 
void main(void)
{
    Device_init();
    Device_initGPIO();
    Board_init();
 
    char arr[] = "HI\n";
    SCI_writeCharArray(mySCI0_BASE, (uint16_t *)arr, sizeof(arr));
 
    char buf[20];
 
    while (1)
    {
        uint16_t a = SCI_readCharBlockingFIFO(mySCI0_BASE);
        //uint16_t a = SCI_readCharBlockingNonFIFO(mySCI0_BASE);
 
        snprintf(buf, sizeof(buf), "recv:%c\n", a);
        SCI_writeCharArray(mySCI0_BASE, (uint16_t *)buf, strlen(buf));
        DEVICE_DELAY_US(5000000);
    }
  }