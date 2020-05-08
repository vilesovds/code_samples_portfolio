#pragma once // _MAX14900E_H_

#include "spi_os_bus.h"
#include "BSP_type.h"
#include <stdint.h>

class MAX14900E: public SPIDevice{

public:
    union Config_t {
        struct {
            uint8_t config1;
            uint8_t config0;
        } two_bytes;
        uint16_t config16;
        uint8_t bytes[2];
    };
    MAX14900E(SPIBus *pBus): SPIDevice(pBus,default_speed, SPIDEV_MODE2){};
    int32_t Init(uint8_t C0, uint8_t C1);
    void UpdateConfig0(uint8_t mask, BSP_type::State_t state);
    void UpdateConfig1(uint8_t mask, BSP_type::State_t state);
    void SetConfig(uint8_t C0, uint8_t C1);
    uint8_t GetStates();
    uint8_t GetDiagnostics();

private:
    Config_t m_rec;
    static const uint32_t default_speed = 20000000;
};
