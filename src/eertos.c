#include <avr/interrupt.h>
#include "const.h"
#include "eertos.h"
#include "tasks.h"


// очередь таймеров, все задачи ОС описаны явно
volatile QITEM queue[QUEUE_SIZE] = {
    {task_timer, FREQ_TIMER + 1, FREQ_TIMER + 1},
};


// диспетчер задач, проверяет задачи в очереди, если находит
// задачу, у которой счётчик дошёл до нуля, то выполняет её
inline uint8_t eertos_dispatch(void) {
    cli();
    for (uint8_t i=0; i<QUEUE_SIZE; i++) {
        QITEM *p = (QITEM *) &queue[i];
        if (p->time == 0) {
            (p->task)();
            p->time = p->init;
        }
    }
    sei();
    return 0;
}


// обработчик прерывания по совпадению таймера, срабатывает с частотой в ~50Гц,
// реализует службу таймеров ядра, здесь уменьшаем счётчик каждого таймера
ISR(TIM0_COMPA_vect) {
    for (uint8_t i=0; i<QUEUE_SIZE; i++) {
        QITEM *p = (QITEM *) &queue[i];
        if (p->time > 0) {
            p->time--;
        }
    }
}


// запуск системного таймера
inline void eertos_run(void) {
    // предполагая F_CPU = 4800000/8
    // используем делитель 1024 (CS=101),
    // OC0n отключены от ног,
    // режим CTC (WGM=010)
    TCCR0A = (1<<WGM01) | (0<<WGM00);
    TCCR0B = (1<<CS02) | (0<<CS01) | (1<<CS00) | (0 << WGM02);
    // f_cpu / (2.0 * prescaler * hz) - 1 даёт 49.87Гц
    TCNT0 = 0;
    OCR0A = OCR0A_VALUE;
    // настраиваем прерывание по сравнению для канала A
    TIMSK0 = (0<<OCIE0B) | (1<<OCIE0A) |  (0<<TOIE0);
    // запускаем прерывания
    sei();
}
