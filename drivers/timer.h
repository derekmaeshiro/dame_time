#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef enum {
    GAME_CLOCK_DATA,
    POS_CLOCK_DATA
} timer_e;

void timer_pwm_init(void);
void set_duty_cycle(timer_e timer, uint8_t duty_cycle_percent);

#endif // TIMER_H