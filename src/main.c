#include "stm32f401xe.h"
#define BIT_5 (1UL << 5)

void SystemInit(void) {}
void _init(void) {}
void _fini(void) {}

void delay(volatile uint32_t count) {
    while(count--) {}
}

int main(void) {
    // enable the clock for GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // is this the same as RCC->AHB1ENR |= (1);

    GPIOA->MODER &= ~(3UL << (5 * 2)); // reset the 10th bit from the right
    GPIOA->MODER |= (1UL << (5 * 2)); // set PA5 to output

    while (1) {
        GPIOA->ODR ^= BIT_5;
        delay(5000000);
    }
}