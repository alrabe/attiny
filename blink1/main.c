#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

#define SIMULATOR

int main() {
	DDRB |= (1 << PB0);
	while (1) {
		PORTB |= (1 << PB0);   // turn on led

#ifndef SIMULATOR
		_delay_ms(750);
#endif

		PORTB &= ~(1 << PB0);

#ifndef SIMULATOR		
		_delay_ms(250);
#endif
	}
	return 0;
}
