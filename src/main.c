#include <stdbool.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 1000000
#define relay PB2
#define light_sensor PB4
#define light_regulator PB3
#define move_sensor PB1
#define night_led PB0

const unsigned long delay_time = 1500000;

bool night;

ISR(INT0_vect)
{
	if (night) {
		light();
	}
}

int main()
{
	// outputs
	DDRB |= 1<<relay;
	DDRB |= 1<<night_led;
	//inputs
	DDRB &= ~(1 << move_sensor);

	// ADC
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);// Разрешение использования АЦП + Делитель 128 = 64 кГц
	ADMUX |= (1<<MUX1);

	// EXINT
	GIMSK |= (1<<INT0);
	MCUCR |= (1<<ISC00)|(1<<ISC01);
	sei();

	while (1) {
		unsigned int now_light = getLight();
		unsigned int daylight = getDaylight();
		night = now_light < daylight;
		if (night) {
			PORTB |= 1<<night_led;
		}
		else {
			PORTB &= ~(1<<night_led);
		}
	}
}

void light() {
	PORTB |= (1<<relay);
	_delay_ms(delay_time);
	PORTB &= ~(1<<relay);
}

int getLight() {
	ADMUX &= ~(1<<MUX0);
	ADCSRA |= (1<<ADSC); //Начинаем преобразование
	while((ADCSRA & (1<<ADSC))); //проверим закончилось ли аналого-цифровое преобразование
	return (unsigned int) ADC;
}

int getDaylight() {
	ADMUX |= (1<<MUX0);
	ADCSRA |= (1<<ADSC); //Начинаем преобразование
	while((ADCSRA & (1<<ADSC))); //проверим закончилось ли аналого-цифровое преобразование
	return (unsigned int) ADC;
}
