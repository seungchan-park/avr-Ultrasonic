#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "Driver/I2C_LCD/I2C_LCD.h"
#include "Driver/Ultrasonic/Ultrasonic.h"
#include "periph/I2C/I2C.h"

ISR(INT2_vect)
{
	Ultrasonic_ISR_Process();
}

int main(void)
{
	Ultrasonic_init();
	LCD_init();
	sei();
	
	char buff[30];
	uint16_t distance;
	
    while (1) 
    {
		// 시나리오
		Ultrasonic_trigger();
		_delay_ms(500);
		
		if (Ultrasonic_getCompltFlag())
		{
			Ultrasonic_clearFlag();
			distance = Ultrasonic_getDistance();
			
			sprintf(buff, "distance : %03dcm", distance);
			LCD_writeStringXY(1, 0, buff);
		}
    }
}

