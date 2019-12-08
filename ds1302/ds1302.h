// Based on https://github.com/JanoVel/attiny_binary_watch/blob/master/code/binaryWatch_firmware.ino

#ifndef _ATTINY_DS1302_H_
#define _ATTINY_DS1302_H_

#include <stdint.h>

#define DS1302_SCLK_PIN   PB2
#define DS1302_IO_PIN     PB1
#define DS1302_CE_PIN     PB0

struct Time {
   uint8_t sc;
   uint8_t hr;
   uint8_t mi;
};

void DS1302_init();
void DS1302_set (Time time);
void DS1302_get (Time *time);

#endif