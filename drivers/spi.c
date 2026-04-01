#include "spi.h"
#include "stm32f401xe.h"

void spi_init(SPI_TypeDef *SPIx, spi_config_s *config) {
    uint32_t temp_cr1 = 0;

    temp_cr1 |= master_selection(config->spi_mode);

    SPIx->CR1 = temp_cr1;
}

static inline master_selection(spi_mode_e spi_mode) {
    if (spi_mode == SPI_MASTER) {
        return SPI_CR1_MSTR;
    }
    else {
        return 0;
    }
}

static inline configure_baud_rate(baud_rate_mode_e baud_rate_mode) {

}

static inline configure_clock_phase(clock_phase_e clock_phase) {

}

static inline configure_clock_polarity(clock_polarity_e clock_polarity) {

}

static inline configure_data_size(data_size_e data_size) {

}
