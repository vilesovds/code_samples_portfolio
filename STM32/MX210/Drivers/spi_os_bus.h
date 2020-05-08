#pragma once  // spi_os_bus.h
#include <stdint.h>
#include "RTOS.h"
//#include "device_specific.h"

/**
 * @brief Режим генерации клоков относительно данных
 */
enum SPI_Mode_e {
    SPIDEV_MODE0 = 0, /*!< CPOL=0 CHPHA=0 */
    SPIDEV_MODE1,     /*!< CPOL=0 CHPHA=1 */
    SPIDEV_MODE2,     /*!< CPOL=1 CHPHA=0 */
    SPIDEV_MODE3      /*!< CPOL=1 CHPHA=1 */
};

/**
 * @brief Абстрактный интерфейсный класс шины SPI
 *
 * Базовый класс для реализации HW или SW интерфейсов шины SPI
 */
class SPIBus {
public:
    virtual ~SPIBus() {}
    /**
     * @brief Включает переферийный модуль SPI, настраивает шину в соответствии с
     * выбранным режимом
     * @param[in] clk желаемая частота шины
     * @param[in] mode режим генерации клоков
     * @param[in] lsb младшим битом вперёд
     * @param[in] crc_calc нужно ли включать подсчёт CRC
     * @return успешо или нет произошло открытие
     */
    virtual bool Open(uint32_t clk, enum SPI_Mode_e mode = SPIDEV_MODE3,
                    bool lsb = false, bool crc_calc = false) = 0;
    /** @brief Процедура отключения переферийного модуля SPI */
    virtual void Close(){};
    /** @brief Захват шины */
    virtual void Acquire();
    /** @brief Освобождение шины */
    virtual void Release();
    /**
     * @brief Процедура приёма/передачи данных по шине
     * @param[in] NumBytes количество байт для передачи/приёма по шине
     * @param[in] pWriteBuffer указатель на буфер для передачи, может быть нулевым
     * @param[in] pReadBuffer указатель на буфер для приёма, может быть нулевым
     * @param[in] timeout время ожидания перефеийного модуля
     */
    virtual size_t WriteRead(size_t NumBytes, const uint8_t *pWriteBuffer = NULL,
                            uint8_t *pReadBuffer = NULL,
                            size_t timeout = 1000) = 0;
    /**
     * @brief Возвращает состояние шины, открыта или нет
     */
    virtual bool IsOpened() { return m_opened; }

protected:
    /** Семафор для захвата шины задачей OS */
    OS_RSEMA sema;
    /** Флаг, отображающий состояние шины */
    bool m_opened;
};

/**
 * @brief Данный класс можно использовать как шаблон реализации девайса на шине
 * SPI
 */
class SPIDevice {
public:
    /**
     * Конструктор девайса, многие параметры можно использовать по умолчанию
     * @param[in] pBus Указатель на родительскую шину SPI
     * @param[in] clk Желаемая частота шины
     * @param[in] mode Режим генерации клоков
     * @param[in] lsb Младшим битом вперёд
     * @param[in] crc_calc Нужно ли включать подсчёт CRC
     * @param[in] timeout Таймаут ожидания
     */
    SPIDevice(SPIBus *pBus, uint32_t clk, enum SPI_Mode_e mode = SPIDEV_MODE3,
                bool lsb = false, bool crc_calc = false, uint32_t timeout = 1000);
    void (*ControlCS)(bool); /*!< Контроль вывода CS */
    bool Open(); /*!< Инициализация параметров и шины SPI */
    /**
     * @brief Процедура обмена данными. Активирует CS, открывает шину, поризводит
     * обмен, закрывает шуну, деактивирует CS
     * @param NumBytes Количество байт для передачи
     * @param pWriteBuffer Указатель на буфер, откуда будут передаваться данные
     * @param pReadBuffer Указатель на буфер, куда будут складываться данные
     * @return Количество переданных/полученных байт данных
     */
    size_t Exchange(size_t NumBytes, const uint8_t *pWriteBuffer = NULL,
                    uint8_t *pReadBuffer = NULL);
    /**
     * @brief Запись данных по шине
     * @param NumBytes Количество байт для передачи
     * @param pWriteBuffer Указатель на буфер, откуда будут передаваться данные
     * @return Количество переданных байт данных
     */
    size_t Write(size_t NumBytes, const uint8_t *pWriteBuffer);
    /**
     * @brief Чтение данных из шины
     * @param NumBytes Количество байт для приёма
     * @param pReadBuffer Указатель на буфер, куда будут складываться данные
     * @return Количество полученных байт данных
     */
    size_t Read(size_t NumBytes, uint8_t *pReadBuffer);

protected:
    SPIBus *m_pbus;         /*!< указатель на шину SPI */
    uint32_t m_clk;         /*!< частота */
    bool m_lsb;             /*!< младший бит первый */
    uint32_t m_timeout;     /*!< таймаут ожидания */
    enum SPI_Mode_e m_mode; /*!< режим SPI шины */
    bool m_crc; /*!< флаг необходимости подсчёта CRC */
};
