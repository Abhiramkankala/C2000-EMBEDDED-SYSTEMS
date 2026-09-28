 //modbus
#include "driverlib.h"
#include "device.h"
#include "board.h"
#include <errno.h>
#include "libmodbus/modbus.h"
#include "libmodbus/logging.h"

int test();

void main(void)
{
    Device_init();
    Device_initGPIO();
    Board_init();
    LOGGING_Init(LOGGER_BASE);
    modbus_SCI_init(MODBUS_BASE);
    test();
}

int test()
{
    modbus_t *ctx;
    uint8_t tab_bits[32];
    int rc;
    int i;

   
    ctx = modbus_new_rtu("SCIC", 9600, 'N', 8, 1);

    if (ctx == NULL) {
        LOGE("Unable to create the libmodbus context\n");
        return -1;
    }

    modbus_set_debug(ctx, FALSE);

    modbus_set_response_timeout(ctx, 2, 0); 

    if (modbus_connect(ctx) == -1) {
        LOGE("Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    modbus_set_slave(ctx, 1);

    while(1)
     {
        uint8_t even_bits[] = {0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
        uint8_t odd_bits[] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};
        modbus_write_bits(ctx, 0, 16, even_bits);
        DEVICE_DELAY_US(5000000); 
        modbus_write_bits(ctx, 0, 16, odd_bits);
        DEVICE_DELAY_US(5000000);
        uint8_t dest[16];
        if(modbus_read_bits(ctx, 0, 16, dest) == -1)
        {
            LOGE("Cannot read coils");
        } 
        else 
        {
            int i;
            for(i=0; i<16; i++)
            {
                LOGI("Coil %d=%d\n", i, dest[i]);
            }
        }
        DEVICE_DELAY_US(5000000); 
    }

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
  }