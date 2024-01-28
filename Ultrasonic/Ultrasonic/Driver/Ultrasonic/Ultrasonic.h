#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define USONIC_DDR DDRD
#define USONIC_PORT PORTD
#define USONIC_PIN PIND
#define USONIC_TRIG 3
#define USONIC_ECHO 2
#define USONIC_TCNT TCNT1

enum{FALLING_EDGE, RISING_EDGE};

void Ultrasonic_switchInterrupt(uint8_t mode);
void Ultrasonic_timerStart();
void Ultrasonic_timerStop();
void Ultrasonic_init();
void Ultrasonic_trigger();
void Ultrasonic_setCompltFlag(); // seter 변수의 값을 바꾸고 싶을 때
uint8_t Ultrasonic_getCompltFlag(); // geter 변수를 사용하고 싶을 때
void Ultrasonic_clearFlag();
uint16_t Ultrasonic_getDistance();
void Ultrasonic_ISR_Process();

#endif /* ULTRASONIC_H_ */