#ifndef __EERTOS_H__
#define __EERTOS_H__

#include <inttypes.h>

// размер очереди таймеров
#define QUEUE_SIZE 3

// указатель на выполняемую задачу
typedef void (*TPTR)(void);

// элемент очереди
typedef struct {
    TPTR task;
    uint8_t time;
    uint8_t init;
} QITEM;

// функции "операционной системы"
extern uint8_t eertos_dispatch(void);
extern void eertos_run(void);

#endif
