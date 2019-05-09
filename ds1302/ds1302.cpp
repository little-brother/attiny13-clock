#include "ds1302.h"

#include <util/delay.h>
#include "Arduino.h"

#define DS1302_READBIT 0
#define DS1302_ENABLE  0x8E
#define DS1302_SECONDS 0x80
#define DS1302_MINUTES 0x82
#define DS1302_HOURS   0x84

void start (void) {
	digitalWrite(DS1302_CE_PIN, LOW);
	pinMode(DS1302_CE_PIN, OUTPUT);  
	
	digitalWrite(DS1302_SCLK_PIN, LOW);
	pinMode(DS1302_SCLK_PIN, OUTPUT);
	
	pinMode(DS1302_IO_PIN, OUTPUT);
	
	digitalWrite(DS1302_CE_PIN, HIGH);
	delayMicroseconds(4);
}

void stop (void) {
	digitalWrite(DS1302_CE_PIN, LOW);
	delayMicroseconds(4); 
}

byte toggleread(void) {
	byte i, data;
	
	data = 0;
	for( i = 0; i <= 7; i++) {
		digitalWrite(DS1302_SCLK_PIN, HIGH);
		delayMicroseconds(1);
	
		digitalWrite(DS1302_SCLK_PIN, LOW);
		delayMicroseconds(1);
		
		bitWrite( data, i, digitalRead(DS1302_IO_PIN)); 
	}

	return data;
}

void togglewrite (byte data, byte release) {
	int i;
	
	for( i = 0; i <= 7; i++) { 
		digitalWrite(DS1302_IO_PIN, bitRead(data, i));  
		delayMicroseconds(1);
		
		digitalWrite(DS1302_SCLK_PIN, HIGH);     
		delayMicroseconds(1);
		
		if( release && i == 7) {
			pinMode(DS1302_IO_PIN, INPUT);
		} else {
			digitalWrite(DS1302_SCLK_PIN, LOW);
			delayMicroseconds(1); 
		}
	}
}

byte read (int address) {
	byte data;
	bitSet(address, DS1302_READBIT);  
	
	start();
	togglewrite(address, true);  
	data = toggleread();
	stop();
	
	return data;
}

void write (int address, byte data) {
	bitClear(address, DS1302_READBIT);   
	
	start();
	togglewrite(address, false); 
	togglewrite(data, false); 
	stop();  
}

void DS1302_init() {
	write(DS1302_ENABLE,  0);
	write(DS1302_SECONDS, 0); 
}

byte decToBcd(byte dec) {
	return (dec + 6 * (dec / 10));
}

byte bcdToDec(byte bcd) {
	return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
}

void DS1302_get (Time *time) {
	time->mi = bcdToDec(read(DS1302_MINUTES));
	time->hr = bcdToDec(read(DS1302_HOURS));
	time->sc = bcdToDec(read(DS1302_SECONDS));
	
	pinMode(DS1302_CE_PIN, OUTPUT);
	digitalWrite(DS1302_CE_PIN, LOW);
}

void DS1302_set (Time time) {
	write(DS1302_HOURS, decToBcd(time.hr));
	write(DS1302_MINUTES, decToBcd(time.mi));	
}