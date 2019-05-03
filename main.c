#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 1000000UL
#define relay PB1
#define light_sensor PB2
#define light_regulator PB3
#define move_sensor PB4
#define switcher PB0

const unsigned long delay_time = 1500000;

unsigned int now_light;
unsigned int daylight;

int main()
{
	// outputs
	DDRB |= 1<<relay;
	//inputs
	DDRB &= ~(1 << move_sensor);
	DDRB &= ~(1 << switcher);
	// PORTB |= (1 << switcher);  //  pull-up
	
	// ADC
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);// Разрешение использования АЦП + Делитель 128 = 64 кГц
	//ADMUX |= (1<<REFS1)|(1<<REFS0);
	
	while (1) {
		if (PINB & (1 << move_sensor)) {
			if ((PINB & (1 << switcher))) {
				light();
			}
			else {
				if (now_light < daylight) {
					light();
				}
			}
		}
		
		// ADC
		now_light = getLight();
		daylight = getDaylight();
	}
}

void light() {
	PORTB |= (1<<relay);
	_delay_ms(delay_time);
	PORTB &= ~(1<<relay);
}

int getLight() {
	ADMUX &= ~((1<<MUX1)|(1<<MUX0));
	ADCSRA |= (1<<ADSC); //Начинаем преобразование
	while((ADCSRA & (1<<ADSC))); //проверим закончилось ли аналого-цифровое преобразование
	return (unsigned int) ADC;
}

int getDaylight() {
	ADMUX |= (1<<MUX1)|(1<<MUX0);
	ADCSRA |= (1<<ADSC); //Начинаем преобразование
	while((ADCSRA & (1<<ADSC))); //проверим закончилось ли аналого-цифровое преобразование
	return (unsigned int) ADC;
}
