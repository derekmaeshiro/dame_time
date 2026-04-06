#include "spi.h"
#include "stm32f401xe.h"
#include "ring_buffer.h"

#define TX_SIZE 256
#define RX_SIZE 256

typedef struct {
    SPI_TypeDef *hw;
    ring_buffer_s tx_rb;
    ring_buffer_s rx_rb;
} spi_device_s;

// global tx/rx buffers
static uint8_t spi1_tx_buffer[TX_SIZE];
static uint8_t spi1_rx_buffer[RX_SIZE];
spi_device_s spi1_dev = { .hw = SPI1 };

static uint8_t spi2_tx_buffer[TX_SIZE];
static uint8_t spi2_rx_buffer[RX_SIZE];
spi_device_s spi2_dev = { .hw = SPI2 };

void spi_init(spi_device_s *dev, spi_config_s *config) {
    SPI_TypeDef *SPIx = dev->hw;
    uint32_t temp_cr1 = 0;

    temp_cr1 |= master_selection(config->spi_mode);
    temp_cr1 |= configure_baud_rate(config->baud_rate_mode);
    temp_cr1 |= configure_clock_phase(config->clock_phase);
    temp_cr1 |= configure_clock_polarity(config->clock_polarity);
    temp_cr1 |= configure_data_size(config->data_size);

    SPIx->CR1 = temp_cr1;

    // enabling spi interrupts
    // 1. spi control register 2
    SPIx->CR2 |= SPI_CR2_RXNEIE;
    // 2. nvic enable
    if (SPIx == SPI1) {
        NVIC_EnableIRQ(SPI1_IRQn);
        // ring buffer initializations
        rb_init(&dev->tx_rb, spi1_tx_buffer, TX_SIZE, RB_BLOCK_IF_FULL);
        rb_init(&dev->rx_rb, spi1_rx_buffer, RX_SIZE, RB_BLOCK_IF_FULL);
    }
    else if (SPIx == SPI2) {
        NVIC_EnableIRQ(SPI2_IRQn);
        // ring buffer initializations
        rb_init(&dev->tx_rb, spi2_tx_buffer, TX_SIZE, RB_BLOCK_IF_FULL);
        rb_init(&dev->rx_rb, spi2_rx_buffer, RX_SIZE, RB_BLOCK_IF_FULL);
    }

    // start the hardware
    SPIx->CR1 |= SPI_CR1_SPE;
}

static void spi_interrupt_handler(spi_device_s *dev) {
    SPI_TypeDef *SPIx = dev->hw;

    // if TXE flag is set, handle TX logic
    if (SPIx->SR & SPI_SR_TXE && SPIx->CR2 & SPI_CR2_TXEIE) {
        uint8_t data;
        bool found_byte = rb_pop(&dev->tx_rb, &data);
        if (found_byte) {
            SPIx->DR = data;
        }
        else {
            // disable TXEIE interrupt
            SPIx->CR2 &= ~SPI_CR2_TXEIE;
        }
    }

    // if RXNE flag is set, read SPI1->DR and push into rx_rb
    // hardware automatically sets this bit whenever a full byte has been shifted in from the MISO line
    if (SPIx->SR & SPI_SR_RXNE) { // if the bit has been set --> a full byte has been sent
        rb_push(&dev->rx_rb, SPIx->DR);
    }
}

void SPI1_IRQHandler(void) {
    spi_interrupt_handler(&spi1_dev);
}

void SPI2_IRQHandler(void) {
    spi_interrupt_handler(&spi2_dev);
}

uint32_t static inline master_selection(spi_mode_e spi_mode) {
    return (spi_mode == SPI_MASTER) ? SPI_CR1_MSTR : 0;
}

uint32_t static inline configure_baud_rate(baud_rate_mode_e baud_rate_mode) {
    return (uint32_t) ((baud_rate_mode & 0x7) << SPI_CR1_BR_Pos);
}

uint32_t static inline configure_clock_phase(clock_phase_e clock_phase) {
    return (uint32_t) (clock_phase << SPI_CR1_CPHA_Pos);
}

uint32_t static inline configure_clock_polarity(clock_polarity_e clock_polarity) {
    return (uint32_t) (clock_polarity << SPI_CR1_CPOL_Pos);
}

uint32_t static inline configure_data_size(data_size_e data_size) {
    return (data_size == SIXTEEN_BIT) ? SPI_CR1_DFF : 0;
}

void spi_send_byte(spi_device_s* dev, uint8_t data) {
    // 1. Push the byte into the tx_rb
    rb_push(&dev->tx_rb, data);
    // 2. Kickstart: Check if the TXEIE bit is currently 0
    if (!(dev->hw->CR2 & SPI_CR2_TXEIE)) {
        dev->hw->CR2 |= SPI_CR2_TXEIE;
    }
}

uint8_t spi_receive_byte(spi_device_s* dev) {
    uint8_t data;
    bool receive_byte = rb_pop(&dev->rx_rb, &data);
    if (receive_byte) {
        return data;
    }
    else {
        return 0;
    }
}

/* POLLING IMPLEMENTATION */
// void spi_send_byte(SPI_TypeDef *SPIx, uint8_t data) {
//     // 1. Wait for the transmit buffer to be empty (TXE flag)
//     // if the flag is 1, the dock is empty and you can drop your byte
//     // if the flag is 0, the dock is being used
//     while ((SPIx->SR & SPI_SR_TXE) == 0) {}

//     // 2. Write byte to DR register
//     SPIx->DR = data;

//     // 3. Wait for BUSY (BSY) flag to clear to ensure bits left wire
//     while (SPIx->SR & SPI_SR_BSY) {}

//     // 4. If we send another byte without receiving one, we will get an OVR error, so we have to dump out a byte first
//     (void)SPIx->DR;
// }

// uint8_t spi_receive_byte(SPI_TypeDef *SPIx) {
//     // 1. Send dummy data (master needs to generate clock pulses)
//     SPIx->DR = 0xFF;

//     // 2. Wait for the receive buffer to be not empty (RXNE flag) (full byte has arrived)
//     while (SPIx->SR & SPI_SR_RXNE == 0) {}

//     // 3. Read the DR register to get data
//     return (uint8_t)SPIx->DR;
// }
