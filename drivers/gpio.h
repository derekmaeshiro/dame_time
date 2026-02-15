#ifndef GPIO_H
#define GPIO_H

typedef enum {
// for shot clock
    GPIO_TX, // PA2
    GPIO_RX, // PA3
    GPIO_TEST_LED, // PB0
    GPIO_SCK_1, // PA5
    GPIO_MISO_1, // PA6
    GPIO_MOSI_1, // PA7
    GPIO_CSN_1, // PB6
    GPIO_CE_1, // PB7
    GPIO_IRQ, // PA8
    GPIO_WS_DATA, // PA0
} gpio_e;

typedef enum {
    GPIO_INPUT,
    GPIO_OUTPUT,
    GPIO_ALT,
    GPIO_ANALOG,
} gpio_mode_e;

typedef enum {
    GPIO_PULL_NONE,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
    GPIO_RESERVED, 
} gpio_resistor_e;

typedef enum {
    GPIO_LOW_SPEED,
    GPIO_MEDIUM_SPEED,
    GPIO_FAST_SPEED,
    GPIO_HIGH_SPEED,
} gpio_speed_e; //configure I/O output speed

typedef enum {
    GPIO_OTYPE_PUSH_PULL,
    GPIO_OTYPE_OPEN_DRAIN, // use for I2C
} gpio_otype_e;

typedef enum {
    ALT_FUNCTION_0,
    ALT_FUNCTION_1,
    ALT_FUNCTION_2,
    ALT_FUNCTION_3,
    ALT_FUNCTION_4,
    ALT_FUNCTION_5,
    ALT_FUNCTION_6,
    ALT_FUNCTION_7,
    ALT_FUNCTION_8,
    ALT_FUNCTION_9,
    ALT_FUNCTION_10,
    ALT_FUNCTION_11,
    ALT_FUNCTION_12,
    ALT_FUNCTION_13,
    ALT_FUNCTION_14,
    ALT_FUNCTION_15,
} gpio_alt_function_e;

typedef enum {
    GPIO_IRQ_RISING_EDGE,
    GPIO_IRQ_FALLING_EDGE,
    GPIO_IRQ_RISING_FALLING_EDGE,
} gpio_irq_edge_e;

typedef enum {
    LOW,
    HIGH,
} gpio_state_e;

// structs

// functions
void gpio_set_mode(gpio_e pin, gpio_mode_e mode);
void gpio_set_resistor(gpio_e pin, gpio_resistor_e resistor);
void gpio_set_output_type(gpio_e pin, gpio_otype_e output_type);
void gpio_set_speed(gpio_e pin, gpio_speed_e speed);
void gpio_set_alt_function(gpio_e pin, gpio_alt_function_e alt_function);
void gpio_set_irq_edge(gpio_e pin, gpio_irq_edge_e irq_edge);

#endif // GPIO_H