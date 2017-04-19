#ifndef __CONST_H__
#define __CONST_H__

#define TICKS               50 // Гц
#define OCR0A_VALUE         F_CPU/1024/TICKS
#define BITS                8 // количество бит в байте :)
#define MASK_SENSORS        0b00111110
#define COUNT_SENSORS       5
#define DEBOUNCING          2
#define FREQ_TIMER          TICKS


// возможные состояния кнопок: не нажата, нажата MODE, нажата SET
typedef enum {OFF, ON} STATE;

// вспомогательная структура для хранения текущего состояния кнопок и приложения
typedef struct {
    int counter;              // текущее состояние счётчика
    STATE state;              // текущее состояние датчика
    uint8_t debounce;         // счётчик защиты от дребезга датчика
} SENSOR;


extern volatile uint16_t timer_counter;

#endif /* __CONST_H__ */
