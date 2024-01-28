#include "Buzzer.h"


void Buzzer_init()
{
	// Timer/Counter3, Fast PWM Mode, Top ICR3, Duty Cycle OCR3A
	// Prescaler 1/64, Non-Inverting
	// Frequency 1kHz, DutyCycle 50% -> 목적
	
	BUZZER_TCCRB |= ((1<<BUZZER_WGM3)|(1<<BUZZER_WGM2)); // 1110 WGM3~0 Fast PWM 14 Mode
	BUZZER_TCCRA |= ((1<<BUZZER_WGM1)|(0<<BUZZER_WGM0)); // 1110 WGM3~0 Fast PWM 14 Mode
	
	BUZZER_TCCRB |= ((0<<BUZZER_CS2)|(1<<BUZZER_CS1)|(1<<BUZZER_CS0)); // 1/64 prescaler
	
	DDRE |= (1<<3);
}
void Buzzer_makeHerz(uint16_t herz)
{
	if (herz < 100) herz = 100;
	else if(herz > 4000) herz = 4000;
	BUZZER_ICR = (250000/herz) - 1; // 1ms period의 top값 설정
	BUZZER_OCR = BUZZER_ICR / 2; // Duty Cycle
}
void Buzzer_soundOn()
{
	BUZZER_TCCRA |= ((1<<BUZZER_COM1)|(0<<BUZZER_COM0)); // Non-Inverting Mode
}
void Buzzer_soundOff()
{
	BUZZER_TCCRA &= ~((1<<BUZZER_COM1)|(1<<BUZZER_COM0)); // Off
}
void Buzzer_powerOnSound()
{
	// 6옥타브 도미솔도
	Buzzer_soundOn();
	Buzzer_makeHerz(1046);
	_delay_ms(100);
	TCNT3 = 0;
	Buzzer_makeHerz(1318);
	_delay_ms(100);
	TCNT3 = 0;
	Buzzer_makeHerz(1567);
	_delay_ms(100);
	TCNT3 = 0;
	Buzzer_makeHerz(2093);
	_delay_ms(100);
	TCNT3 = 0;
	Buzzer_soundOff();
}
void Buzzer_HHH(uint16_t *buz)
{
	Buzzer_soundOn();
	for (int i = 0; buz[i]; i++)
	{
		Buzzer_makeHerz(buz[i]);
		_delay_ms(100);
		TCNT3 = 0;
	}
	Buzzer_soundOff();
}
