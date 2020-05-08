#pragma once
#include "spi_os_bus.h"

class STM32F_SPIBus: public SPIBus {
public:
    STM32F_SPIBus(SPI_TypeDef * instance);
    size_t WriteRead(size_t NumBytes, const uint8_t *pWriteBuffer = NULL, uint8_t *pReadBuffer = NULL, size_t timeout = 1000);
    bool Open(uint32_t clk, enum SPI_Mode_e mode = SPIDEV_MODE3, bool lsb = false, bool crc_calc = false);
    bool Open(const SPI_InitTypeDef * pSPI_config);

private:
    static void EnableRCCClock(const SPI_TypeDef * instance);

    SPI_HandleTypeDef m_handle;
    uint32_t m_clk; /*!< SPI Clock Value*/
    bool m_lsb;
    enum SPI_Mode_e m_mode; /*!< SPI Clock mode*/
    bool m_crc; /*!< SPI Clock Polarity. High polarity if true, else low polarity*/
};