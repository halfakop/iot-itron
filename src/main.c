#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include "const.h"
#include "eertos.h"
#include "onewire.h"

// глобальные переменные
volatile uint16_t timer_counter = 0;


// обрабатываем изменение состояния датчиков
ISR(PCINT0_vect)
{
    // uint8_t sensors = PINB & MASK_SENSORS;
}


int main(void)
{
    // настройка порта
    // PB0 - 1 wire, настраивается в соответвующем модуле
    // PB1:5 - вход с оптического датчика
    DDRB = 0b00000000;

    // разрешаем общее прерывание по изменению состояния выводов
    GIMSK |= (1<<PCIE);
    // отслеживаем изменение на выводе PB1
    PCMSK |= (1<PCINT0);

    // Экономия энергии: было потребление 0.608-0.610мА
    //  - отключаем аналоговый компаратор, стало 0.543-0.545мА
    //  - отключаем АЦП, стало 0.542-0544мА
    //  - дрёма снизила потребление до 0.207-0.210мА
    ACSR |= (1<<ACD);
    PRR &= (~(1<<PRADC));
    set_sleep_mode(SLEEP_MODE_IDLE);

    // запускаем выполнение задач
    eertos_run();

    while(!eertos_dispatch()) {
        // основной поток тут

        // уходим в спячку до срабатывания прерывания по таймеру
        sleep_enable();
        sleep_cpu();
    }

    return 0;
}
