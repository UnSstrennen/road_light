#include <avr/io.h>
#include <util/delay.h>

#define relay PB1
#define light_sensor PB2
#define light_regulator PB3
#define move_sensor PB4
#define switcher PB0

const unsigned long delay_time = 1500000;

int main()
{
    // outputs
    DDRB |= (1<<relay);
    DDRB |= (1<<switcher);
    //inputs
    DDRB &= ~(1 << move_sensor;

    while (true) {
        if (PINB & (1 << move_sensor)) {
            if (!(PINB & (1 << switcher))) {
                light();
            }
    }
}

void light() {
    PORTB |= (1<<relay);
    _delay_ms(delay_time);
    PORTB &= ~(1<<relay);
}
