#include "tm1637.h"
#include "ds1302.h"

void setTime(byte hr, byte mi) {
    TM1637_display_digit(0, hr / 10);   
    TM1637_display_digit(1, hr % 10); 
    TM1637_display_digit(2, mi / 10);   
    TM1637_display_digit(3, mi % 10);   
}

int main(void) {
	Time time;
	byte isPressed = 0;
	byte looper = 0;
	
	/* setup */
	pinMode(5, INPUT);
	TM1637_init(1/*enable*/, 3/*brightness*/);
	DS1302_init();  
	
	setTime(88, 88);
	TM1637_display_colon(1);
	_delay_ms(300);
	
	while(1) {
		if (analogRead(0) < 900) { /* Change in range 800..1000 if button doesn't work properly */
			isPressed = isPressed == 255 ? 255 : isPressed + 1;

			time.mi = time.mi + 1;
			if (time.mi >= 60) {
				time.mi = 0;
				time.hr = time.hr + 1;
				if (time.hr >= 24)
					time.hr = 0;
			}
			
			setTime(time.hr, time.mi);
			TM1637_display_colon(0);

			if (isPressed < 15)
				_delay_ms(100);
			else
				_delay_ms(10);

			continue;
		}
		
		if (isPressed) {
			isPressed = 0;

			DS1302_set(time);
			TM1637_display_colon(1); 
			continue;
		}
		
		DS1302_get(&time);
		
		if (time.hr >= 24 || time.mi >= 60) {
			time.hr = 0;
			time.mi = 0;
		}  
		
		setTime(time.hr, time.mi);
		looper = (looper + 1) % 16;
		TM1637_display_colon(looper > 8);
		_delay_ms(60);
	}
}
