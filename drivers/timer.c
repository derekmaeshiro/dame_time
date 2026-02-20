#include "timer.h"
#include "gpio.h"
#include "stm32f401xe.h"

// function implementations
void timer_pwm_init(TIM_TypeDef *TIMx, uint32_t channel) {
    if (TIMx == TIM2) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    }
    else if (TIMx == TIM3) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    }
    else if (TIMx == TIM1) {
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    }

    // set the prescaler
    TIMx->PSC = 0; // --> runs at the full speed of the bus
    TIMx->ARR = 89; // (72,000,000 / 800,000) - 1 = 89

    // configure channel for pwm
    // set to pwm mode 1
    TIMx->CCMR1 = TIM_CCMR1_OC1M |= 0b110;

}
