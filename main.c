/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <avr/io.h>
#include <util/delay.h>

#define relay PB1
#define light_sensor PB2
#define light_regulator PB3
#define move_sensor PB4
#define switcher PB0


int main()
{
    // outputs
    DDRB |= (1<<relay);
    while (true) {
        if (PINB & (1 << move_sensor)) {
            PORTB |= (1<<relay);
            _delay_ms(500);
            PORTB &= ~(1<<relay);
            _delay_ms(500);
    }
}
