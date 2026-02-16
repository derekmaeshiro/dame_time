#include "stm32f401xe.h"
#include "gpio.h"

#define BIT_5 (1UL << 5)

void SystemInit(void) {}
void _init(void) {}
void _fini(void) {}

void delay(volatile uint32_t count) {
    while(count--) {}
}

int main(void) {
    const struct gpio_config led_config = GPIO_CONFIG_DEFAULT;
    gpio_configure(5U, &led_config);

    while (1) {
        gpio_write(5U, HIGH);
        delay(1000000);
        gpio_write(5U, LOW);
        delay(1000000);
    }
}