#pragma once  // spi_os_bus.h
#include <stdint.h>
#include "RTOS.h"
//#include "device_specific.h"

/**
 * @brief ����� ��������� ������ ������������ ������
 */
enum SPI_Mode_e {
    SPIDEV_MODE0 = 0, /*!< CPOL=0 CHPHA=0 */
    SPIDEV_MODE1,     /*!< CPOL=0 CHPHA=1 */
    SPIDEV_MODE2,     /*!< CPOL=1 CHPHA=0 */
    SPIDEV_MODE3      /*!< CPOL=1 CHPHA=1 */
};

/**
 * @brief ����������� ������������ ����� ���� SPI
 *
 * ������� ����� ��� ���������� HW ��� SW ����������� ���� SPI
 */
class SPIBus {
public:
    virtual ~SPIBus() {}
    /**
     * @brief �������� ������������ ������ SPI, ����������� ���� � ������������ �
     * ��������� �������
     * @param[in] clk �������� ������� ����
     * @param[in] mode ����� ��������� ������
     * @param[in] lsb ������� ����� �����
     * @param[in] crc_calc ����� �� �������� ������� CRC
     * @return ������ ��� ��� ��������� ��������
     */
    virtual bool Open(uint32_t clk, enum SPI_Mode_e mode = SPIDEV_MODE3,
                    bool lsb = false, bool crc_calc = false) = 0;
    /** @brief ��������� ���������� ������������� ������ SPI */
    virtual void Close(){};
    /** @brief ������ ���� */
    virtual void Acquire();
    /** @brief ������������ ���� */
    virtual void Release();
    /**
     * @brief ��������� �����/�������� ������ �� ����
     * @param[in] NumBytes ���������� ���� ��� ��������/����� �� ����
     * @param[in] pWriteBuffer ��������� �� ����� ��� ��������, ����� ���� �������
     * @param[in] pReadBuffer ��������� �� ����� ��� �����, ����� ���� �������
     * @param[in] timeout ����� �������� ������������ ������
     */
    virtual size_t WriteRead(size_t NumBytes, const uint8_t *pWriteBuffer = NULL,
                            uint8_t *pReadBuffer = NULL,
                            size_t timeout = 1000) = 0;
    /**
     * @brief ���������� ��������� ����, ������� ��� ���
     */
    virtual bool IsOpened() { return m_opened; }

protected:
    /** ������� ��� ������� ���� ������� OS */
    OS_RSEMA sema;
    /** ����, ������������ ��������� ���� */
    bool m_opened;
};

/**
 * @brief ������ ����� ����� ������������ ��� ������ ���������� ������� �� ����
 * SPI
 */
class SPIDevice {
public:
    /**
     * ����������� �������, ������ ��������� ����� ������������ �� ���������
     * @param[in] pBus ��������� �� ������������ ���� SPI
     * @param[in] clk �������� ������� ����
     * @param[in] mode ����� ��������� ������
     * @param[in] lsb ������� ����� �����
     * @param[in] crc_calc ����� �� �������� ������� CRC
     * @param[in] timeout ������� ��������
     */
    SPIDevice(SPIBus *pBus, uint32_t clk, enum SPI_Mode_e mode = SPIDEV_MODE3,
                bool lsb = false, bool crc_calc = false, uint32_t timeout = 1000);
    void (*ControlCS)(bool); /*!< �������� ������ CS */
    bool Open(); /*!< ������������� ���������� � ���� SPI */
    /**
     * @brief ��������� ������ �������. ���������� CS, ��������� ����, ����������
     * �����, ��������� ����, ������������ CS
     * @param NumBytes ���������� ���� ��� ��������
     * @param pWriteBuffer ��������� �� �����, ������ ����� ������������ ������
     * @param pReadBuffer ��������� �� �����, ���� ����� ������������ ������
     * @return ���������� ����������/���������� ���� ������
     */
    size_t Exchange(size_t NumBytes, const uint8_t *pWriteBuffer = NULL,
                    uint8_t *pReadBuffer = NULL);
    /**
     * @brief ������ ������ �� ����
     * @param NumBytes ���������� ���� ��� ��������
     * @param pWriteBuffer ��������� �� �����, ������ ����� ������������ ������
     * @return ���������� ���������� ���� ������
     */
    size_t Write(size_t NumBytes, const uint8_t *pWriteBuffer);
    /**
     * @brief ������ ������ �� ����
     * @param NumBytes ���������� ���� ��� �����
     * @param pReadBuffer ��������� �� �����, ���� ����� ������������ ������
     * @return ���������� ���������� ���� ������
     */
    size_t Read(size_t NumBytes, uint8_t *pReadBuffer);

protected:
    SPIBus *m_pbus;         /*!< ��������� �� ���� SPI */
    uint32_t m_clk;         /*!< ������� */
    bool m_lsb;             /*!< ������� ��� ������ */
    uint32_t m_timeout;     /*!< ������� �������� */
    enum SPI_Mode_e m_mode; /*!< ����� SPI ���� */
    bool m_crc; /*!< ���� ������������� �������� CRC */
};
