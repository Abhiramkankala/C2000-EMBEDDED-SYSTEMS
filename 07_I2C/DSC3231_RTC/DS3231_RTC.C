#include "driverlib.h"
#include "device.h"
#include "board.h"
#include <stdio.h>
#include <string.h>

#define DS3231_I2C_ADDR 0x68

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t dayOfWeek;
    uint8_t date;
    uint8_t month;
    uint16_t year;
} DS3231_Time;

static inline uint8_t BCDToDec(uint8_t val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}

static inline uint8_t DecToBCD(uint8_t val)
{
    return ((val / 10) << 4) | (val % 10);
}

static void SCI_print(uint32_t base, const char *str)
{
    SCI_writeCharArray(base, (const uint16_t * const)str, (uint16_t)strlen(str));
}

static bool DS3231_CheckNack(uint32_t base, const char *where)
{
    if (I2C_getStatus(base) & I2C_STS_NO_ACK)
    {
        I2C_clearStatus(base, I2C_STS_NO_ACK);
        SCI_print(mySCI0_BASE, where);
        return true;
    }
    return false;
}

bool DS3231_ReadTime(DS3231_Time *time)
{
    uint8_t rawData[7];
    uint16_t i;

    while (I2C_getStopConditionStatus(i2c_BASE)) {}

    I2C_setSlaveAddress(i2c_BASE, DS3231_I2C_ADDR);
    I2C_setDataCount(i2c_BASE, 1);
    I2C_putData(i2c_BASE, 0x00);
    I2C_setConfig(i2c_BASE, I2C_MASTER_SEND_MODE);

    I2C_sendStartCondition(i2c_BASE);
    I2C_sendStopCondition(i2c_BASE);

    while (I2C_getStopConditionStatus(i2c_BASE)) {}

    if (DS3231_CheckNack(i2c_BASE, "RD ptr NACK\r\n"))
    {
        return false;
    }

    I2C_setDataCount(i2c_BASE, 7);
    I2C_setConfig(i2c_BASE, I2C_MASTER_RECEIVE_MODE);
    I2C_sendStartCondition(i2c_BASE);
    I2C_sendStopCondition(i2c_BASE);

    for (i = 0; i < 7; i++)
    {
        while (I2C_getRxFIFOStatus(i2c_BASE) == I2C_FIFO_RXEMPTY) {}
        rawData[i] = (uint8_t)I2C_getData(i2c_BASE);
    }

    while (I2C_getStopConditionStatus(i2c_BASE)) {}

    if (DS3231_CheckNack(i2c_BASE, "RD data NACK\r\n"))
    {
        return false;
    }

    time->seconds = BCDToDec(rawData[0] & 0x7F);
    time->minutes = BCDToDec(rawData[1] & 0x7F);
    time->hours = BCDToDec(rawData[2] & 0x3F);
    time->dayOfWeek = BCDToDec(rawData[3] & 0x07);
    time->date = BCDToDec(rawData[4] & 0x3F);
    time->month = BCDToDec(rawData[5] & 0x1F);
    time->year = (uint16_t)BCDToDec(rawData[6]) + 2000;

    return true;
}

bool DS3231_SetTime(DS3231_Time *time)
{
    uint8_t txData[8];
    uint16_t i;
    uint8_t shortYear;

    while (I2C_getStopConditionStatus(i2c_BASE)) {}

    txData[0] = 0x00;
    txData[1] = DecToBCD(time->seconds);
    txData[2] = DecToBCD(time->minutes);
    txData[3] = DecToBCD(time->hours);

    txData[4] = DecToBCD(time->dayOfWeek);
    txData[5] = DecToBCD(time->date);
    txData[6] = DecToBCD(time->month);

    shortYear = (time->year >= 2000) ? (uint8_t)(time->year - 2000)
                : (uint8_t)time->year;
    txData[7] = DecToBCD(shortYear);

    I2C_setSlaveAddress(i2c_BASE, DS3231_I2C_ADDR);
    I2C_setDataCount(i2c_BASE, 8);

    for (i = 0; i < 8; i++)
    {
        while (I2C_getTxFIFOStatus(i2c_BASE) == I2C_FIFO_TX16) {}
        I2C_putData(i2c_BASE, txData[i]);
    }

    I2C_setConfig(i2c_BASE, I2C_MASTER_SEND_MODE);
    I2C_sendStartCondition(i2c_BASE);
    I2C_sendStopCondition(i2c_BASE);

    while (I2C_getStopConditionStatus(i2c_BASE)) {}

    if (DS3231_CheckNack(i2c_BASE, "WR NACK\r\n"))
    {
        return false;
    }

    return true;
}

void main(void)
{
    DS3231_Time now;
    DS3231_Time curr_time;
    char msg[64];

    Device_init();
    Device_initGPIO();
    Board_init();

    curr_time.date = 11;
    curr_time.month = 9;
    curr_time.year = 2020;
    curr_time.hours = 13;
    curr_time.minutes = 30;
    curr_time.seconds = 0;
    curr_time.dayOfWeek = 5;

    SCI_print(mySCI0_BASE, "Setting DS3231 time...\r\n");

    if (DS3231_SetTime(&curr_time))
    {
        SCI_print(mySCI0_BASE, "Set OK!\r\n");
    }
    else
    {
        SCI_print(mySCI0_BASE, "Set FAILED\r\n");
    }

    while (1)
    {
        if (DS3231_ReadTime(&now))
        {
            snprintf(msg, sizeof(msg), "%04u-%02u-%02u %02u:%02u:%02u\r\n",
                     now.year, now.month, now.date,
                     now.hours, now.minutes, now.seconds);
            SCI_print(mySCI0_BASE, msg);
        }
        else
        {
            SCI_print(mySCI0_BASE, "Read FAILED\r\n");
        }

        DEVICE_DELAY_US(1000000);
    }
}
// End of File