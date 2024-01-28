#include "Ultrasonic.h"

uint8_t ultrasonicDistanceFlag;

void Ultrasonic_switchInterrupt(uint8_t mode)
{
	if (mode == FALLING_EDGE)
	{
		EICRA &= ~((1<<ISC21)|(1<<ISC20));
		EICRA |= ((1<<ISC21)|(0<<ISC20)); // Falling edge
	}
	else
	{
		EICRA &= ~((1<<ISC21)|(1<<ISC20));
		EICRA |= ((1<<ISC21)|(1<<ISC20)); // Rising edge
	}
}
void Ultrasonic_timerStart()
{
	USONIC_TCNT = 0; // TCNT 초기화
	TCCR1B |= ((0<<CS12)|(1<<CS11)|(1<<CS10)); // 시간측정 시작
}
void Ultrasonic_timerStop()
{
	TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10)); // 시간측정 시작
}
void Ultrasonic_init()
{
	USONIC_DDR &= ~(1<<USONIC_ECHO); // Echo Pin
	USONIC_DDR |= (1<<USONIC_TRIG); // Trigger Pin
	EICRA |= ((1<<ISC21)|(1<<ISC20)); // Rising Edge Interrupt
	EIMSK |= (1<<INT2); // INT2 External Interrupt
	
	//TCCR1B |= ((0<<CS12)|(1<<CS11)|(1<<CS10)); // Timer1 prescaler 64
	
	ultrasonicDistanceFlag = 0;
}
void Ultrasonic_trigger()
{
	USONIC_PORT |= (1<<USONIC_TRIG);
	_delay_us(10);
	USONIC_PORT &= ~(1<<USONIC_TRIG);
}
void Ultrasonic_setCompltFlag() // seter 변수의 값을 바꾸고 싶을 때
{
	ultrasonicDistanceFlag = 1;
}
uint8_t Ultrasonic_getCompltFlag() // geter 변수를 사용하고 싶을 때
{
	return ultrasonicDistanceFlag;
}
void Ultrasonic_clearFlag()
{
	ultrasonicDistanceFlag = 0;
}
uint16_t Ultrasonic_getDistance()
{
	return USONIC_TCNT * 0.068;
}
void Ultrasonic_ISR_Process()
{
	if (USONIC_PIN & (1<<USONIC_ECHO))
	{
		Ultrasonic_timerStart();
		Ultrasonic_switchInterrupt(FALLING_EDGE);
	}
	else
	{
		Ultrasonic_timerStop();
		Ultrasonic_switchInterrupt(RISING_EDGE);
		Ultrasonic_setCompltFlag();
	}
}
