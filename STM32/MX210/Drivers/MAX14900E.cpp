#include "MAX14900E.hpp"
#include "defaults.h"


#ifndef MAX14900_DEBUG

#define MAX14900_DEBUG 0
#endif

#if MAX14900_DEBUG
#include "SEGGER_RTT.h"
static void _Logf(const char * sFormat, ...);
#define DEBUG_LOG(s)              _Logf s
#else
#define DEBUG_LOG(s)
#endif


#if MAX14900_DEBUG
static void _Logf(const char * sFormat, ...) 
{
    va_list ParamList;
    char acBuffer[100];
    //
    // Replace place holders (%d, %x etc) by values and call output routine.
    //
    va_start(ParamList, sFormat);
    vsprintf(acBuffer, sFormat, ParamList);
    SEGGER_RTT_printf(0,"MAX14900: ");
    SEGGER_RTT_printf(0,acBuffer);
    va_end(ParamList);
}
#endif

int32_t MAX14900E::Init(uint8_t C0, uint8_t C1) {
    SetConfig(C0, C1);
    return 0;
}

void MAX14900E::UpdateConfig0(uint8_t mask, BSP_type::State_t state) {
    if (state)
        m_rec.two_bytes.config0 |= mask;
    else
        m_rec.two_bytes.config0 &= ~mask;
    Exchange(sizeof(m_rec.bytes), m_rec.bytes);
}

void MAX14900E::UpdateConfig1(uint8_t mask, BSP_type::State_t state) {
    if (state)
        m_rec.two_bytes.config1 |= mask;
    else
        m_rec.two_bytes.config1 &= ~mask;
    Exchange(sizeof(m_rec.bytes), m_rec.bytes);
}

void MAX14900E::SetConfig(uint8_t C0, uint8_t C1) {
    m_rec.two_bytes.config0 = C0;
    m_rec.two_bytes.config1 = C1;
    Exchange(sizeof(m_rec.bytes), m_rec.bytes, NULL);

}

uint8_t MAX14900E::GetDiagnostics() {
    uint8_t diag[2];
    Exchange(sizeof(diag), m_rec.bytes, diag);
    return diag[0];
}

uint8_t MAX14900E::GetStates() {
    uint8_t diag[2];
    Exchange(sizeof(diag), m_rec.bytes, diag);
    // if diag bits are set then we don't know about output state
    return diag[1]&~diag[0];
}
