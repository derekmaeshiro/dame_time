#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// enums
typedef enum {
    SPI_SLAVE = 0,
    SPI_MASTER = 1
} spi_mode_e;

typedef enum {
    DIV_2,
    DIV_4,
    DIV_8,
    DIV_16,
    DIV_32,
    DIV_64,
    DIV_128,
    DIV_256
} baud_rate_mode_e;

typedef enum {
    IDLE_LOW = 0,
    IDLE_HIGH = 1
} clock_polarity_e;

typedef enum {
    FIRST_EDGE = 0,
    SECOND_EDGE = 1
} clock_phase_e;

typedef enum {
    EIGHT_BIT = 0,
    SIXTEEN_BIT = 1
} data_size_e;

typedef struct {
    spi_mode_e spi_mode;
    baud_rate_mode_e baud_rate_mode;
    clock_polarity_e clock_polarity;
    clock_phase_e clock_phase;
    data_size_e data_size;
} spi_config_s;

// setting structs
const spi_config_s spi_led_config = {
    .spi_mode = SPI_MASTER,
    .baud_rate_mode = DIV_32,
    .clock_phase = FIRST_EDGE,
    .clock_polarity = IDLE_LOW,
    .data_size = EIGHT_BIT,
};

void spi_init(SPI_TypeDef *SPIx, spi_config_s *config);

#endif