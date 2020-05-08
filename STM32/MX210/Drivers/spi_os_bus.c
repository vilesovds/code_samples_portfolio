#include "spi_os_bus.h"

static void Dummy(bool) {
}


SPIDevice::SPIDevice(SPIBus *pBus, uint32_t clk, enum SPI_Mode_e mode, bool lsb, bool crc_calc, uint32_t timeout)
 :ControlCS(Dummy)
 ,m_pbus(pBus)
 ,m_clk(clk)
 ,m_lsb(lsb)
 ,m_timeout(timeout)
 ,m_mode(mode)
 ,m_crc(crc_calc) {

}

void SPIBus::Acquire()
{
    OS_Use(&sema);
}

void SPIBus::Release()
{
    if (OS_GetSemaValue(&sema)) // chk if used
        OS_Unuse(&sema);
}

size_t SPIDevice::Exchange(size_t NumBytes, const uint8_t *pWriteBuffer, uint8_t *pReadBuffer)
{
    m_pbus->Open(m_clk,m_mode,m_lsb,m_crc);

    ControlCS(true);
    const size_t transferred = m_pbus->WriteRead(NumBytes, pWriteBuffer, pReadBuffer, m_timeout);
    ControlCS(false);

    m_pbus->Close();

    return transferred;
}

size_t SPIDevice::Read(size_t NumBytes, uint8_t *pReadBuffer)
{
    if (m_pbus->IsOpened())
        return m_pbus->WriteRead(NumBytes, NULL, pReadBuffer, m_timeout);

    return 0;
}

bool SPIDevice::Open()
{
    return m_pbus->Open(m_clk, m_mode, m_lsb, m_crc);
}

size_t SPIDevice::Write(size_t NumBytes, const uint8_t *pWriteBuffer)
{
    if (m_pbus->IsOpened())
        return m_pbus->WriteRead(NumBytes, pWriteBuffer, NULL, m_timeout);

    return 0;
}
