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

// function implementations
void gpio_enable_clock(uint8_t port_idx) {
    RCC->AHB1ENR |= (1 << port_idx);
}

void gpio_configure(gpio_e pin, const struct gpio_config* config) {
    gpio_enable_clock(pin / 16);
    gpio_set_mode(pin, config->mode);
    gpio_set_resistor(pin, config->resistor);
    gpio_set_output_type(pin, config->o_type);
    gpio_set_speed(pin, config->speed);
    gpio_set_alt_function(pin, config->alt_function);
    gpio_set_irq_edge(pin, config->irq_edge);
}

void gpio_set_mode(gpio_e pin, gpio_mode_e mode) {
    const uint8_t port_idx = gpio_port_idx(pin);
    const uint8_t pin_idx = gpio_pin_idx(pin);
    volatile uint32_t* MODER_Address = &GPIO_PORTS[port_idx]->MODER;
    uint32_t local = *MODER_Address;
    uint32_t mask = (3U << (pin_idx * 2U));

    local &= ~mask;
    local |= ((uint32_t)mode << pin_idx * 2U);
    *MODER_Address = local;
}

void gpio_set_resistor(gpio_e pin, gpio_resistor_e resistor) {
    const uint8_t port_idx = gpio_port_idx(pin);
    const uint8_t pin_idx = gpio_pin_idx(pin);
    volatile uint32_t* PUPDR_Address = &GPIO_PORTS[port_idx]->PUPDR;
    uint32_t local = *PUPDR_Address;
    uint32_t mask = (3U << (pin_idx * 2U));

    local &= ~mask;
    local |= ((uint32_t)resistor << pin_idx * 2U);
    *PUPDR_Address = local;
}

void gpio_set_output_type(gpio_e pin, gpio_otype_e output_type) {
    const uint8_t port_idx = gpio_port_idx(pin);
    const uint8_t pin_idx = gpio_pin_idx(pin);
    volatile uint32_t* OTYPER_Address = &GPIO_PORTS[port_idx]->OTYPER;
    uint32_t local = *OTYPER_Address;
    uint32_t mask = (1U << (pin_idx * 1U));

    local &= ~mask;
    local |= (output_type << pin_idx * 1U);
    *OTYPER_Address = local;
}

void gpio_set_speed(gpio_e pin, gpio_speed_e speed) {
    const uint8_t port_idx = gpio_port_idx(pin);
    const uint8_t pin_idx = gpio_pin_idx(pin);
    volatile uint32_t* OSPEEDR_Address = &GPIO_PORTS[port_idx]->OSPEEDR;
    uint32_t local = *OSPEEDR_Address;
    uint32_t mask = (3U << (pin_idx * 2U));

    local &= ~mask;
    local |= ((uint32_t)speed << pin_idx * 2U);
    *OSPEEDR_Address = local;
}

void gpio_set_alt_function(gpio_e pin, gpio_alt_function_e alt_function) {
    const uint8_t port_idx = gpio_port_idx(pin);
    const uint8_t pin_idx = gpio_pin_idx(pin);
    volatile uint32_t* AFR_Address = &GPIO_PORTS[port_idx]->AFR[pin_idx / 8];
    uint32_t local = *AFR_Address;
    uint32_t mask = (15U << ((pin_idx % 8) * 4U));

    local &= ~mask;
    local |= ((uint32_t)alt_function << (pin_idx % 8) * 4U);
    *AFR_Address = local;
}

void gpio_set_irq_edge(gpio_e pin, gpio_irq_edge_e irq_edge) {
    if (irq_edge == GPIO_IRQ_NONE) {
        return; // Don't configure IRQ if not requested
    }

    // Enable SYSCFG Clock (Connected to APB2 bus)
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    const uint8_t port_idx = gpio_port_idx(pin);
    const uint8_t pin_idx = gpio_pin_idx(pin);
    volatile uint32_t* EXTI_Address = &SYSCFG->EXTICR[pin_idx / 4];
    uint32_t local = *EXTI_Address;
    uint32_t mask = (15U << ((pin_idx % 4) * 4));

    local &= ~mask;
    local |= (port_idx << ((pin_idx % 4) * 4));
    *EXTI_Address = local;

    // initialize the irq_edge
    if (irq_edge == GPIO_IRQ_RISING_EDGE) {
        EXTI->FTSR &= ~(1U << pin_idx);
        EXTI->RTSR |= (1U << pin_idx);
    }
    else if (irq_edge == GPIO_IRQ_FALLING_EDGE) {
        EXTI->FTSR |= (1U << pin_idx);
        EXTI->RTSR &= ~(1U << pin_idx);
    }
    else if (irq_edge == GPIO_IRQ_RISING_FALLING_EDGE) {
        EXTI->FTSR |= (1U << pin_idx);
        EXTI->RTSR |= (1U << pin_idx);
    }

    // interrupt mask register
    EXTI->IMR |= (1U << pin_idx);

    // enable interrupts
    switch(pin_idx) {
        case 0: NVIC_EnableIRQ(EXTI0_IRQn); break;
        case 1: NVIC_EnableIRQ(EXTI1_IRQn); break;
        case 2: NVIC_EnableIRQ(EXTI2_IRQn); break;
        case 3: NVIC_EnableIRQ(EXTI3_IRQn); break;
        case 4: NVIC_EnableIRQ(EXTI4_IRQn); break;
        case 5: NVIC_EnableIRQ(EXTI9_5_IRQn); break;
        case 6: NVIC_EnableIRQ(EXTI9_5_IRQn); break;
        case 7: NVIC_EnableIRQ(EXTI9_5_IRQn); break;
        case 8: NVIC_EnableIRQ(EXTI9_5_IRQn); break;
        case 9: NVIC_EnableIRQ(EXTI9_5_IRQn); break;
        case 10: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
        case 11: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
        case 12: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
        case 13: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
        case 14: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
        case 15: NVIC_EnableIRQ(EXTI15_10_IRQn); break;
        default: NVIC_EnableIRQ(EXTI0_IRQn); break;
    }
}