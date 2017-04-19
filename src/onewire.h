#ifndef __ONEWIRE_H__
#define __ONEWIRE_H__

#include <avr/io.h>

#define ONEWIRE_PIN             PB0
#define ONEWIRE_SET_OUT         DDRB |= (1<<ONEWIRE_PIN)
#define ONEWIRE_SET_IN          DDRB &= ~(1<<ONEWIRE_PIN)
#define ONEWIRE_HIGH            PORTB |= (1<<ONEWIRE_PIN)
#define ONEWIRE_LOW             PORTB &= ~(1<<ONEWIRE_PIN)
#define ONEWIRE_READ            ((PINB & (1<<ONEWIRE_PIN)) ? 1 : 0)

uint8_t onewire_reset(void);
void onewire_power_down(void);
void onewire_write_bit(uint8_t bit);
uint8_t onewire_read_bit(void);
void onewire_write_byte(const uint8_t byte);
uint8_t onewire_read_byte(void);
void onewire_write(const uint8_t* buffer, uint8_t size);
void onewire_read(uint8_t* buffer, uint8_t size);

#endif /* __ONEWIRE_H__ */
