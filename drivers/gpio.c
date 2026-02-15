#include "gpio.h"
#include "stm32f401xe.h"

#define GPIO_PORT_OFFSET (4u)
#define GPIO_PORT_MASK (0x3u << GPIO_PORT_OFFSET)
#define GPIO_PIN_MASK (0xFu)
#define GPIO_PORT_CNT (3u)
#define GPIO_PIN_CNT_PER_PORT (16u)

static inline uint8_t gpio_port_idx(gpio_e pin) {
    return (pin & GPIO_PORT_MASK) >> GPIO_PORT_OFFSET;
}

static inline uint8_t gpio_pin_idx(gpio_e pin) {
    return pin & GPIO_PIN_MASK;
}

static inline uint8_t gpio_pin_bit(gpio_e pin) {
    return 1 << gpio_pin_idx(pin);
}

/* Port Registers */
static GPIO_TypeDef *const GPIO_PORTS[] = {
    GPIOA,
    GPIOB,
    GPIOC,
};

void gpio_set_mode(gpio_e pin, gpio_mode_e mode) {
    const uint8_t port_idx = gpio_port_idx(pin);
    const uint8_t pin_idx = gpio_pin_idx(pin);
    volatile uint32_t* MODER_Address = &GPIO_PORTS[port_idx]->MODER;
    uint32_t local = *MODER_Address;
    uint32_t mask = (3U << (pin_idx * 2U));

    local &= ~mask;
    local |= (mode << pin_idx * 2U);
    *MODER_Address = local;
}

void gpio_set_resistor(gpio_e pin, gpio_resistor_e resistor) {

}

void gpio_set_output_type(gpio_e pin, gpio_otype_e output_type) {
    
}

void gpio_set_speed(gpio_e pin, gpio_speed_e speed) {

}

void gpio_set_alt_function(gpio_e pin, gpio_alt_function_e alt_function) {

}

void gpio_set_irq_edge(gpio_e pin, gpio_irq_edge_e irq_edge) {

}