#include <avr/io.h>
#include <util/delay.h> 
#include "onewire.h"


uint8_t onewire_reset(void)
{
    // ожидаем освобождение линии
    uint8_t retries = 125;
    ONEWIRE_SET_IN;
    do {
        if (--retries == 0) {
            return 0;
        }
        _delay_us(2);
    } while(!ONEWIRE_READ);
    // прижимаем линию
    ONEWIRE_LOW;
    ONEWIRE_SET_OUT;
    _delay_us(480);
    // слушаем ответный импульс
    ONEWIRE_SET_IN;
    _delay_us(70);
    // читаем состояние линии
    uint8_t state = !ONEWIRE_READ;
    _delay_us(410);
    return state;
}

void onewire_power_down(void)
{
    ONEWIRE_SET_IN;
    ONEWIRE_LOW;
}

void onewire_write_bit(uint8_t bit)
{
    if (bit & 1) {
        ONEWIRE_LOW;
        ONEWIRE_SET_OUT;
        _delay_us(10);
        ONEWIRE_HIGH;
        _delay_us(55);
    } else {
        ONEWIRE_LOW;
        ONEWIRE_SET_OUT;
        _delay_us(65);
        ONEWIRE_HIGH;
        _delay_us(5);
    }
}

uint8_t onewire_read_bit(void)
{
    // прижимаем линию
    ONEWIRE_SET_OUT;
    ONEWIRE_LOW;
    _delay_us(3);
    // ожидаем данные
    ONEWIRE_SET_IN;
    _delay_us(10);
    // читаем бит
    uint8_t bit = ONEWIRE_READ;
    _delay_us(53);
    return bit;
}

void onewire_write_byte(const uint8_t byte)
{
    // пишем каждый бит
    for (uint8_t bitmask=0x01; bitmask; bitmask<<=1) {
        onewire_write_bit((byte & bitmask) ? 1 : 0);
    }
}

inline uint8_t onewire_read_byte(void)
{
    uint8_t byte = 0;
    // читаем все биты
    for (uint8_t bitmask=0x01; bitmask; bitmask<<=1) {
        if (onewire_read_bit()) {
            byte |= bitmask;
        }
    }
    return byte;
}

inline void onewire_write(const uint8_t* buffer, uint8_t size)
{
    for (uint8_t i=0; i<size; i++) {
        onewire_write_byte(buffer[i]);
    }
}

inline void onewire_read(uint8_t* buffer, uint8_t size)
{
    for (uint8_t i=0; i<size; i++) {
        buffer[i] = onewire_read_byte();
    }
}
