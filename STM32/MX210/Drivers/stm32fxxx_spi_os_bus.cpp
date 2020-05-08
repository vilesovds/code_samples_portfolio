#include "stm32fxxx_spi_os_bus.h"
/**
* @defgroup STM32_SPI_GROUP STM32 SPI Group
 *
 * @{
 */
/** Количество элементов массива */
#ifndef DIM
#define DIM(x) sizeof(x)/sizeof(x[0])
#endif

/** Конфигурация модуля SPI по умолчанию*/
static    const SPI_InitTypeDef _default_spi_config = {
    .Direction = SPI_DIRECTION_2LINES,
    .Mode = SPI_MODE_MASTER,
    .DataSize = SPI_DATASIZE_8BIT,
    .CLKPolarity = SPI_POLARITY_HIGH,
    .CLKPhase = SPI_PHASE_2EDGE,
    .NSS = SPI_NSS_SOFT,
    .TIMode = SPI_TIMODE_DISABLE,
    .BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8,
    .FirstBit =   SPI_FIRSTBIT_MSB,
    .CRCCalculation = SPI_CRCCALCULATION_DISABLE,
    .CRCPolynomial = 7,
};

/** Массив возможных сзначений предделителя модуля SPI*/
static const uint32_t br_prescalers[] = {
    SPI_BAUDRATEPRESCALER_2, SPI_BAUDRATEPRESCALER_4, SPI_BAUDRATEPRESCALER_8, \
    SPI_BAUDRATEPRESCALER_16, SPI_BAUDRATEPRESCALER_32, SPI_BAUDRATEPRESCALER_64, \
    SPI_BAUDRATEPRESCALER_128, SPI_BAUDRATEPRESCALER_256\
};


/** Младшая часть адреса периферии конкретного SPI модуля*/
#define SPI1_ADDRR 0x3000
#define SPI2_ADDRR 0x3800
#define SPI3_ADDRR 0x3C00
#define SPI4_ADDRR 0x3400
#define SPI5_ADDRR 0x5000
#define SPI6_ADDRR 0x5400


/**
* @brief вычисляет на какой шине висит перифирийный модуль: на APB1 или APB2
* @return True если модуль на шине APB1, false если на APB2
*/
static inline bool get_periph_base(uint32_t addr) {
    /* так можно сделать вывод, так как известно на базовые адреса шин, см.
    stm32fxxx.h
    #define APB1PERIPH_BASE        PERIPH_BASE
    #define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000U)
    */
    return (addr & 0x00010000) > 0;
}

/**
* @brief вычисляет на какой частоте работает периферийная шина, в зависимости от
  базового адреса устройства
* @return частота в Гц
*/
static uint32_t GetPeriphClock(SPI_TypeDef * instance) {
    if (get_periph_base((uint32_t)instance))
        return HAL_RCC_GetPCLK1Freq();
    else
        return HAL_RCC_GetPCLK2Freq();
}


STM32F_SPIBus::STM32F_SPIBus(SPI_TypeDef * instance) {
    OS_CREATERSEMA(&sema);
    m_opened = false;
    m_handle.Instance = instance;
};

/**
* @brief Включение тактирования периферии
* @param[in] instance, поддерживаются SPI1..6
*/
void STM32F_SPIBus::EnableRCCClock(const SPI_TypeDef * instance) {
    const uint32_t address = (uint32_t) instance & 0x0000FFFF; // get periph addr
    switch (address) {
    case SPI1_ADDRR: // SPI1
        if (READ_BIT(RCC->APB2ENR, 0x00001000) != 0x00001000)
            SET_BIT(RCC->APB2ENR, 0x00001000);
        break;
    case SPI2_ADDRR:
        if (READ_BIT(RCC->APB1ENR, 0x00004000) != 0x00004000)
            SET_BIT(RCC->APB1ENR, 0x00004000);
        break;
    case SPI3_ADDRR:
        if (READ_BIT(RCC->APB1ENR, 0x00008000) != 0x00008000)      
            SET_BIT(RCC->APB1ENR, 0x00008000);
        break;
    case SPI4_ADDRR:
        if (READ_BIT(RCC->APB2ENR, 0x00002000) != 0x00002000)
            SET_BIT(RCC->APB2ENR, 0x00002000);
        break;
    case SPI5_ADDRR:
        if (READ_BIT(RCC->APB2ENR, 0x00100000) != 0x00100000)
            SET_BIT(RCC->APB2ENR, 0x00100000);
        break;
    case SPI6_ADDRR:
        if (READ_BIT(RCC->APB2ENR, 0x00200000) != 0x00200000)
            SET_BIT(RCC->APB2ENR, 0x00200000);
        break;
    default:
        break;
    }
    __no_operation();
}


bool STM32F_SPIBus::Open(const SPI_InitTypeDef * pSPI_config) {
    // check if needed reopen
    if ((m_opened == false) || (0 != memcmp((const void *)&m_handle.Init, (const void *)pSPI_config, sizeof(SPI_InitTypeDef)))) {
        HAL_SPI_DeInit(&m_handle);
        EnableRCCClock(m_handle.Instance);
        m_handle.Init = *pSPI_config;
        if (HAL_SPI_Init(&m_handle) == HAL_OK) {
            // to avoid overlap in simple Open function
            m_mode = SPIDEV_MODE0;
            m_crc = pSPI_config->CRCCalculation;
            m_lsb = pSPI_config->FirstBit == SPI_FIRSTBIT_LSB ? true : false;
            m_clk = 0xDEADCAFE;
            m_opened = true;
            __HAL_SPI_ENABLE(&m_handle); // Включаем SPI до установки CS!!!
        } else
            m_opened = false;
    }
    return m_opened;
}


bool STM32F_SPIBus::Open(uint32_t clk, enum SPI_Mode_e mode, bool lsb, bool crc_calc) {
    uint32_t periph_clk, tmp_pre, div = 2;
    // check if needed reopen
    if ((m_opened == false) || (m_clk != clk) || (m_mode != mode) || (m_lsb != lsb) || (m_crc != crc_calc)) {
        HAL_SPI_DeInit(&m_handle);
        EnableRCCClock(m_handle.Instance);

        m_handle.Init = _default_spi_config;
        m_handle.Init.CLKPhase       = (mode & 0x01) ? SPI_PHASE_2EDGE   : SPI_PHASE_1EDGE;
        m_handle.Init.CLKPolarity    = (mode & 0x02) ? SPI_POLARITY_HIGH : SPI_POLARITY_LOW;
        m_handle.Init.CRCCalculation = crc_calc;
        m_handle.Init.FirstBit       = lsb ? SPI_FIRSTBIT_LSB : SPI_FIRSTBIT_MSB;
        // calculate prescaler
        periph_clk = GetPeriphClock(m_handle.Instance);
        for (uint32_t i = 0; i < DIM(br_prescalers); i++) {
            tmp_pre = br_prescalers[i];
            // if calculated clk less than needed, then we can stop
            if (periph_clk / div <= clk)
                break;
            div *= 2;
        }

        m_handle.Init.BaudRatePrescaler = tmp_pre;
        if (HAL_SPI_Init(&m_handle) == HAL_OK) {
            m_crc = crc_calc;
            m_mode = mode;
            m_clk = clk;
            m_lsb = lsb;
            m_opened = true;
            __HAL_SPI_ENABLE(&m_handle); // Включаем SPI до установки CS!!!
        } else
            m_opened = false;
    }
    return m_opened;
}

/**
*   @brief
*/
size_t STM32F_SPIBus::WriteRead(size_t NumBytes, const uint8_t *pWriteBuffer, uint8_t *pReadBuffer, size_t timeout) {
    if (NumBytes == 0)
        return 0;
    if ((pWriteBuffer) && (pReadBuffer)) {
        if (HAL_OK == HAL_SPI_TransmitReceive(&m_handle, (uint8_t *)pWriteBuffer, pReadBuffer, NumBytes, timeout))
            return NumBytes;
    } else if ((pWriteBuffer) && (NULL == pReadBuffer)) {
        if (HAL_OK == HAL_SPI_Transmit(&m_handle, (uint8_t *)pWriteBuffer, NumBytes, timeout))
            return NumBytes;
    } else if ((NULL == pWriteBuffer) && (pReadBuffer)) {
        if (HAL_OK == HAL_SPI_Receive(&m_handle, pReadBuffer, NumBytes, timeout))
            return NumBytes;
    }
    return 0;
}

/** @} */