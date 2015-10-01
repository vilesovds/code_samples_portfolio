/*
 * PCD8544.c
 * Р СљР С•Р Т‘РЎС“Р В»РЎРЉ Р Т‘Р В»РЎРЏ РЎР‚Р В°Р В±Р С•РЎвЂљРЎвЂ№ РЎРѓ Р Т‘Р С‘РЎРѓР С—Р В»Р ВµРЎРЏР С�Р С‘ Р Р…Р В° Р В±Р В°Р В·Р Вµ Р С”Р С•Р Р…РЎвЂљРЎР‚Р С•Р В»Р В»Р ВµРЎР‚Р В° PCD8544.
 *
 * Author: Р СџР С•Р С–РЎР‚Р ВµР В±Р Р…РЎРЏР С” Р вЂќР С�Р С‘РЎвЂљРЎР‚Р С‘Р в„–
 * 
 * Р СџР С•Р С�Р ВµРЎвЂ°РЎвЂ�Р Р…Р Р…РЎвЂ№Р в„– Р В·Р Т‘Р ВµРЎРѓРЎРЉ Р С”Р С•Р Т‘ РЎРЏР Р†Р В»РЎРЏР ВµРЎвЂљРЎРѓРЎРЏ РЎРѓР Р†Р С•Р В±Р С•Р Т‘Р Р…РЎвЂ№Р С�. Р Сћ.Р Вµ. Р Т‘Р С•Р С—РЎС“РЎРѓР С”Р В°Р ВµРЎвЂљРЎРѓРЎРЏ Р ВµР С–Р С• РЎРѓР Р†Р С•Р В±Р С•Р Т‘Р Р…Р С•Р Вµ Р С‘РЎРѓР С—Р С•Р В»РЎРЉР В·Р С•Р Р†Р В°Р Р…Р С‘Р Вµ Р Т‘Р В»РЎРЏ Р В»РЎР‹Р В±РЎвЂ№РЎвЂ¦ РЎвЂ Р ВµР В»Р ВµР в„–, Р Р†Р С”Р В»РЎР‹РЎвЂЎР В°РЎРЏ Р С”Р С•Р С�Р С�Р ВµРЎР‚РЎвЂЎР ВµРЎРѓР С”Р С‘Р Вµ, Р С—РЎР‚Р С‘ РЎС“РЎРѓР В»Р С•Р Р†Р С‘Р С‘ РЎС“Р С”Р В°Р В·Р В°Р Р…Р С‘РЎРЏ РЎРѓРЎРѓРЎвЂ№Р В»Р С”Р С‘ Р Р…Р В° Р В°Р Р†РЎвЂљР С•РЎР‚Р В° (Р СџР С•Р С–РЎР‚Р ВµР В±Р Р…РЎРЏР С” Р вЂќР С�Р С‘РЎвЂљРЎР‚Р С‘Р в„–, http://aterlux.ru/).
 * Code included here is free to use. It can be used for free for any purpose, including commercial use, as long as link to author (Pogrebnyak Dmitry, http://aterlux.ru/) is included.
 */ 

#include <avr/io.h>
#include "PCD8544.h"
#include "tools.h"
#include <util/delay.h>
#include <avr/pgmspace.h>

#ifdef PCD8544_SOFTWARE
  void inline pcd8544_sw_send(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
      PCD8544_SW_SCK_PORT &= ~PCD8544_SW_SCK;
      if (data & 0x80) {
        PCD8544_SW_MOSI_PORT |= PCD8544_SW_MOSI;
      } else {
        PCD8544_SW_MOSI_PORT &= ~PCD8544_SW_MOSI;
      }
      data <<= 1;
      PCD8544_SW_SCK_PORT |= PCD8544_SW_SCK;
    }
  }
#endif

inline void pcd8544_reset_assert() {
  PCD8544_RESET_PORT &= ~PCD8544_RESET;
}

inline void pcd8544_reset_deassert() {
  PCD8544_RESET_PORT |= PCD8544_RESET;
}

inline void pcd8544_sce_assert() {
  PCD8544_SCE_PORT &= ~PCD8544_SCE;
}

inline void pcd8544_sce_deassert() {
  PCD8544_WAIT_DONE;
  PCD8544_SCE_PORT |= PCD8544_SCE;
}

void display_data(uint8_t data) {
  PCD8544_WAIT_DONE;
  PCD8544_DC_PORT |= PCD8544_DC;
  PCD8544_SEND(data);
}

void display_cmd(uint8_t data) {
  PCD8544_WAIT_DONE;
  pcd8544_sce_deassert(); // Р В§РЎвЂљР С•Р В±РЎвЂ№ Р Р…Р Вµ РЎвЂљР ВµРЎР‚РЎРЏР В»Р В°РЎРѓРЎРЉ РЎРѓР С‘Р Р…РЎвЂ¦РЎР‚Р С•Р Р…Р С‘Р В·Р В°РЎвЂ Р С‘РЎРЏ, Р С—Р ВµРЎР‚Р С‘Р С•Р Т‘Р С‘РЎвЂЎР ВµРЎРѓР С”Р С‘ Р В±РЎС“Р Т‘Р ВµР С� РЎРѓР В±РЎР‚Р В°РЎРѓРЎвЂ№Р Р†Р В°РЎвЂљРЎРЉ Р В»Р С‘Р Р…Р С‘РЎР‹ sce
  PCD8544_DC_PORT &= ~PCD8544_DC;
  pcd8544_sce_assert();
  PCD8544_SEND(data);
}

// Р пїЅР Р…Р С‘РЎвЂ Р С‘Р В°Р В»Р С‘Р В·Р С‘РЎР‚РЎС“Р ВµРЎвЂљ Р Т‘Р С‘РЎРѓР С—Р В»Р ВµР в„–
void display_init() {
#ifdef PCD8544_SOFTWARE
  PCD8544_SW_SCK_PORT |= PCD8544_SW_SCK;
#endif 
  pcd8544_reset_assert();
  _delay_us(10);
  pcd8544_reset_deassert();
  _delay_us(10);
  
  pcd8544_sce_assert();
  display_cmd(0x21);	// set LCD mode
  display_cmd(0x80 + 58);	// set bias voltage
  
  display_cmd(0x04);	// temperature correction
  display_cmd(0x13);	// 1:48
  display_cmd(0x20);	
  display_cmd(0x0c);	// set LCD mode,display normally
}

// Р вЂњР С•РЎвЂљР С•Р Р†Р С‘РЎвЂљ Р С”Р С•Р Р…РЎвЂљРЎР‚Р С•Р В»Р В»Р ВµРЎР‚ Р Т‘Р С‘РЎРѓР С—Р В»Р ВµРЎРЏ Р С‘ Р В·Р В°РЎвЂљР ВµР С� Р С—Р ВµРЎР‚Р ВµР Р†Р С•Р Т‘Р С‘РЎвЂљ Р ВµР С–Р С• Р Р† РЎР‚Р ВµР В¶Р С‘Р С� РЎРЊР С”Р С•Р Р…Р С•Р С�Р С‘Р С‘ РЎРЊР Р…Р ВµРЎР‚Р С–Р С‘Р С‘
void display_powerdown() {
  display_clear();
  display_cmd(0x21);
  display_cmd(0x80); // Р С•РЎвЂљР С”Р В»РЎР‹РЎвЂЎР ВµР Р…Р С‘Р Вµ Р С–Р ВµР Р…Р ВµРЎР‚Р В°РЎвЂљР С•РЎР‚Р В° Р Р…Р В°Р С—РЎР‚РЎРЏР В¶Р ВµР Р…Р С‘РЎРЏ
  display_cmd(0x24);
  display_cmd(0x08);
  PCD8544_WAIT_DONE;
  PCD8544_SW_MOSI_PORT |= PCD8544_SW_MOSI;
  pcd8544_sce_deassert();
}


// Р вЂ™РЎвЂ№Р В±Р С‘РЎР‚Р В°Р ВµРЎвЂљ РЎРѓРЎвЂљРЎР‚Р В°Р Р…Р С‘РЎвЂ РЎС“ (Р С—Р С•Р В·Р С‘РЎвЂ Р С‘РЎР‹ Р С—Р С• Р Р†Р ВµРЎР‚РЎвЂљР С‘Р С”Р В°Р В»Р С‘) Р Т‘Р В»РЎРЏ Р Р†РЎвЂ№Р Р†Р С•Р Т‘Р В°
void display_setpage(uint8_t page) {
  display_cmd(0x40 | (page & 7));
}

// Р вЂ™РЎвЂ№Р В±Р С‘РЎР‚Р В°Р ВµРЎвЂљ Р С–Р С•РЎР‚Р С‘Р В·Р С•Р Р…РЎвЂљР В°Р В»РЎРЉР Р…РЎС“РЎР‹ Р С—Р С•Р В·Р С‘РЎвЂ Р С‘РЎР‹ Р Т‘Р В»РЎРЏ Р Р†РЎвЂ№Р Р†Р С•Р Т‘Р В°
void display_setx(uint8_t x) {
  display_cmd(0x80 | x);
}

// Р вЂ™РЎвЂ№Р В±Р С‘РЎР‚Р В°Р ВµРЎвЂљ РЎРѓРЎвЂљРЎР‚Р В°Р Р…Р С‘РЎвЂ РЎС“ Р С‘ Р С–Р С•РЎР‚Р С‘Р В·Р С•Р Р…РЎвЂљР В°Р В»РЎРЉР Р…РЎС“РЎР‹ Р С—Р С•Р В·Р С‘РЎвЂ Р С‘РЎР‹ Р Т‘Р В»РЎРЏ Р Р†РЎвЂ№Р Р†Р С•Р Т‘Р В°
void display_setpos(uint8_t page, uint8_t x) {
  display_cmd(0x40 | (page & 7));
  display_cmd(0x80 | x);
}

/* Р вЂ”Р В°Р С—Р С•Р В»Р Р…РЎРЏР ВµРЎвЂљ РЎРЊР С”РЎР‚Р В°Р Р… Р В·Р В°Р Т‘Р В°Р Р…Р Р…РЎвЂ№Р С�Р С‘ РЎвЂЎР ВµРЎР‚Р ВµР Т‘РЎС“РЎР‹РЎвЂ°Р С‘Р С�Р С‘РЎРѓРЎРЏ Р В·Р Р…Р В°РЎвЂЎР ВµР Р…Р С‘РЎРЏР С�Р С‘ */
void display_fill(uint8_t oddData, uint8_t evenData) {
  display_setpos(0, 0);
  for (uint8_t y = 0; y < DISPLAY_PAGES; y++) {
    for (uint8_t x = 0; x < DISPLAY_WIDTH; x += 2) {
      display_data(oddData);
      display_data(evenData);
    }    
  }
}

/* Р С›РЎвЂЎР С‘РЎвЂ°Р В°Р ВµРЎвЂљ РЎРЊР С”РЎР‚Р В°Р Р…, РЎС“РЎРѓРЎвЂљР В°Р Р…Р В°Р Р†Р В»Р С‘Р Р†Р В°Р ВµРЎвЂљ Р С”РЎС“РЎР‚РЎРѓР С•РЎР‚ Р Р† Р В»Р ВµР Р†РЎвЂ№Р в„– Р Р†Р ВµРЎР‚РЎвЂ¦Р Р…Р С‘Р в„– РЎС“Р С–Р С•Р В» */
void display_clear() {
  display_setpos(0, 0);
  for (uint8_t y = 0; y < DISPLAY_PAGES; y++) {
    for (uint8_t x = 0; x < DISPLAY_WIDTH; x++) {
      display_data(0);
    }    
  }
  display_setpos(0, 0);
}

/* Р вЂ™РЎвЂ№Р Р†Р С•Р Т‘Р С‘РЎвЂљ Р Р…Р В° РЎРЊР С”РЎР‚Р В°Р Р… РЎРѓРЎвЂљРЎР‚Р С•Р С”РЎС“ Р С‘Р В· РЎвЂћР В»Р ВµРЎв‚¬-Р С—Р В°Р С�РЎРЏРЎвЂљР С‘
 * page - РЎРѓРЎвЂљРЎР‚Р С•Р С”Р В° Р Р…Р В° Р С”Р С•РЎвЂљР С•РЎР‚РЎС“РЎР‹ Р Р†РЎвЂ№Р Р†Р С•Р Т‘Р С‘РЎвЂљРЎРѓРЎРЏ Р С‘Р В·Р С•Р В±РЎР‚Р В°Р В¶Р ВµР Р…Р С‘Р Вµ
 * x - Р С—Р С•Р В·Р С‘РЎвЂ Р С‘РЎРЏ, Р Р…Р В°РЎвЂЎР С‘Р Р…Р В°РЎРЏ РЎРѓ Р С”Р С•РЎвЂљР С•РЎР‚Р С•Р в„– Р Р†РЎвЂ№Р Р†Р С•Р Т‘Р С‘РЎвЂљРЎРѓРЎРЏ Р С‘Р В·Р С•Р В±РЎР‚Р В°Р В¶Р ВµР Р…Р С‘Р Вµ
 * pData - РЎРѓРЎРѓРЎвЂ№Р В»Р С”Р В° Р Р…Р В° Р Т‘Р В°Р Р…Р Р…РЎвЂ№Р Вµ Р Р†Р С• РЎвЂћР В»Р ВµРЎв‚¬-Р С—Р В°Р С�РЎРЏРЎвЂљР С‘
 * width - Р С”Р С•Р В»Р С‘РЎвЂЎР ВµРЎРѓРЎвЂљР Р†Р С• Р Р†РЎвЂ№Р Р†Р С•Р Т‘Р С‘Р С�РЎвЂ№РЎвЂ¦ Р Т‘Р В°Р Р…Р Р…РЎвЂ№РЎвЂ¦, РЎв‚¬Р С‘РЎР‚Р С‘Р Р…Р В°
 * xorMask - Р С‘Р Р…Р Р†Р ВµРЎР‚РЎРѓР С‘РЎРЏ, Р С—РЎР‚Р С‘Р С�Р ВµР Р…РЎРЏР ВµР С�Р В°РЎРЏ Р С” Р Р†РЎвЂ№Р Р†Р С•Р Т‘Р С‘Р С�РЎвЂ№Р С� Р Т‘Р В°Р Р…Р Р…РЎвЂ№Р С�.
 * yinvert - Р ВµРЎРѓР В»Р С‘ Р Р…Р Вµ Р Р…Р С•Р В»РЎРЉ, Р С—Р ВµРЎР‚Р ВµР Р†Р С•РЎР‚Р В°РЎвЂЎР С‘Р Р†Р В°Р ВµРЎвЂљ Р С‘Р В·Р С•Р В±РЎР‚Р В°Р В¶Р ВµР Р…Р С‘Р Вµ Р Р†Р Р†Р ВµРЎР‚РЎвЂ¦-Р Р…Р С•Р С–Р В°Р С�Р С‘
 * 
 * Р вЂўРЎРѓР В»Р С‘ Р С‘Р В·Р С•Р В±РЎР‚Р В°Р В¶Р ВµР Р…Р С‘Р Вµ Р Р…Р Вµ Р С—Р С•Р С�Р ВµРЎвЂ°Р В°Р ВµРЎвЂљРЎРѓРЎРЏ Р Р…Р В° РЎРЊР С”РЎР‚Р В°Р Р… Р С—Р С• РЎв‚¬Р С‘РЎР‚Р С‘Р Р…Р Вµ, Р Р†РЎвЂ№Р Р†Р С•Р Т‘Р С‘РЎвЂљ РЎвЂљР С•Р В»РЎРЉР С”Р С• Р ВµР С–Р С• РЎвЂЎР В°РЎРѓРЎвЂљРЎРЉ. Р вЂ™Р С•Р В·Р Р†РЎР‚Р В°РЎвЂ°Р В°Р ВµРЎвЂљ РЎРѓР С”Р С•Р В»РЎРЉР С”Р С• Р С”Р С•Р В»Р С•Р Р…Р С•Р С” Р В±РЎвЂ№Р В»Р С• Р Р†РЎвЂ№Р Р†Р ВµР Т‘Р ВµР Р…Р С•
 */
uint8_t display_sprite(uint8_t page, uint8_t x, PGM_VOID_P pData, uint8_t width, uint8_t xorMask, uint8_t yinvert) {
  if ((page >= DISPLAY_PAGES) || (x >= DISPLAY_WIDTH))
    return 0;
  display_setpos(page, x);
  uint8_t r = DISPLAY_WIDTH - x;
  if (width > r)
    width = r;
  else 
    r = width;
  if (yinvert) {
    while (width--) {
      display_data(reversebits(pgm_read_byte(pData++)) ^ xorMask);
    }  
  } else {  
    while (width--) {
      display_data(pgm_read_byte(pData++) ^ xorMask);
    }  
  }  
  return r;
}

/* Р С™Р С•Р С—Р С‘РЎР‚РЎС“Р ВµРЎвЂљ Р С‘Р В· Р В±РЎС“РЎвЂћР ВµРЎР‚Р В° Р С‘Р В·Р С•Р В±РЎР‚Р В°Р В¶Р ВµР Р…Р С‘Р Вµ Р Р…Р В° РЎРЊР С”РЎР‚Р В°Р Р…
 * page, x - Р Р…Р В°РЎвЂЎР В°Р В»РЎРЉР Р…Р В°РЎРЏ Р С—Р С•Р В·Р С‘РЎвЂ Р С‘РЎРЏ Р Т‘Р В»РЎРЏ Р Р†РЎвЂ№Р Р†Р С•Р Т‘Р В° Р В±РЎС“РЎвЂћР ВµРЎР‚Р В°
 * pData - РЎС“Р С”Р В°Р В·Р В°РЎвЂљР ВµР В»РЎРЉ Р Р…Р В° Р В±РЎС“РЎвЂћР ВµРЎР‚
 * pages_height - Р Р†РЎвЂ№РЎРѓР С•РЎвЂљР В° Р В±РЎС“РЎвЂћР ВµРЎР‚Р В° (Р Р† РЎРѓРЎвЂљРЎР‚Р В°Р Р…Р С‘РЎвЂ Р В°РЎвЂ¦)
 * width - РЎв‚¬Р С‘РЎР‚Р С‘Р Р…Р В° Р В±РЎС“РЎвЂћР ВµРЎР‚Р В°
 */
void display_buffer(uint8_t page, uint8_t x, uint8_t * pData, uint8_t pages_height, uint8_t width) {
  if ((x >= DISPLAY_WIDTH) || (page >= DISPLAY_PAGES))
    return;
  uint8_t w = DISPLAY_WIDTH - x;
  if (w > width)
    w = width;
  uint8_t t = page + ((pages_height > DISPLAY_PAGES) ? DISPLAY_PAGES : pages_height);
  if (t > DISPLAY_PAGES)
    t = DISPLAY_PAGES;
  while (page < t) {
    display_setpos(page++, x);
    for (uint8_t i = 0; i < w; i++) {
      display_data(pData[i]);
    }
    pData += width;
  }
}


