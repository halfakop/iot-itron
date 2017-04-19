#include <avr/io.h>
#include "const.h"
#include "tasks.h"


// Задача на отсчёт секунд.
inline void task_timer(void) {
    timer_counter++;
}
