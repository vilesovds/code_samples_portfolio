#ifndef PCD8544_H_
#define PCD8544_H_


#include <avr/io.h>
#include <avr/pgmspace.h>
#ifndef F_CPU
#error FPU
#endif
// РџРѕСЂС‚ Рё РЅРѕРјРµСЂ РїРёРЅР°, РЅР° РєРѕС‚РѕСЂС‹Р№ РІС‹РІРѕРґРёС‚СЃСЏ !RESET СЃРёРіРЅР°Р»
// Port and Pin number for !RESET signal
#define PCD8544_RESET_PORT PORTB
#define PCD8544_RESET_PIN_NUM 2
#define PCD8544_RESET (1 << PCD8544_RESET_PIN_NUM)

// РџРѕСЂС‚ Рё РЅРѕРјРµСЂ РїРёРЅР°, РЅР° РєРѕС‚РѕСЂС‹Р№ РІС‹РІРѕРґРёС‚СЃСЏ !SCE (CS) СЃРёРіРЅР°Р»
// Port and Pin number for !SCE (CS) signal
#define PCD8544_SCE_PORT PORTD
#define PCD8544_SCE_PIN_NUM 7
#define PCD8544_SCE (1 << PCD8544_SCE_PIN_NUM)

// РџРѕСЂС‚ Рё РЅРѕРјРµСЂ РїРёРЅР°, РЅР° РєРѕС‚РѕСЂС‹Р№ РІС‹РІРѕРґРёС‚СЃСЏ D/!C СЃРёРіРЅР°Р»
// Port and Pin number for D/!C signal
#define PCD8544_DC_PORT PORTB
#define PCD8544_DC_PIN_NUM 3
#define PCD8544_DC (1 << PCD8544_DC_PIN_NUM)

// РћРїСЂРµРґРµР»РёС‚СЊ РґР»СЏ РїСЂРѕРіСЂР°РјРјРЅРѕРіРѕ С„РѕСЂРјРёСЂРѕРІР°РЅРёСЏ
// If defined, then software output
#define PCD8544_SOFTWARE

#ifdef PCD8544_SOFTWARE
 // РџРѕСЂС‚ Рё РЅРѕРјРµСЂ РїРёРЅР°, РЅР° РєРѕС‚РѕСЂС‹Р№ РІС‹РІРѕРґРёС‚СЃСЏ СЃРёРіРЅР°Р» MOSI (DIN РЅР° РґРёСЃРїР»РµРµ)
 // Port and Pin number for software MOSI (DIN on display controller) signal
 #define PCD8544_SW_MOSI_PORT PORTB
 #define PCD8544_SW_MOSI_PIN_NUM 4
 #define PCD8544_SW_MOSI (1 << PCD8544_SW_MOSI_PIN_NUM)
 
 // РџРѕСЂС‚ Рё РЅРѕРјРµСЂ РїРёРЅР°, РЅР° РєРѕС‚РѕСЂС‹Р№ РІС‹РІРѕРґРёС‚СЃСЏ СЃРёРіРЅР°Р» SCK
 // Port and Pin number for software SCK signal
 #define PCD8544_SW_SCK_PORT PORTB
 #define PCD8544_SW_SCK_PIN_NUM 5
 #define PCD8544_SW_SCK (1 << PCD8544_SW_SCK_PIN_NUM)

 // РџСѓСЃС‚Р°СЏ С„СѓРЅРєС†РёСЏ РѕР¶РёРґР°РЅРёСЏ
 // Void wait function
 #define PCD8544_WAIT_DONE
 
 // Р¤СѓРЅРєС†РёСЏ РґР»СЏ РїРµСЂРµРґР°С‡Рё РѕРґРЅРѕРіРѕ Р±Р°Р№С‚Р°
 // A byte transmission function
 #define PCD8544_SEND(x) pcd8544_sw_send(x)
 
#else 
// Р•СЃР»Рё РЅРµ Р·Р°РґР°РЅР° РїСЂРѕРіСЂР°РјРјРЅР°СЏ РїРµСЂРµРґР°С‡Р°, С‚Рѕ Р·РґРµСЃСЊ РѕРїРёСЃС‹РІР°СЋС‚СЃСЏ РјРµС‚РѕРґС‹ РѕР±СЂР°С‰РµРЅРёСЏ Рє СЂРµРіРёСЃС‚СЂР°Рј РїРµСЂРёС„РµСЂРёРё
// РџРµСЂРёС„РµСЂРёСЏ РґРѕР»Р¶РЅР° Р±С‹С‚СЊ РЅР°СЃС‚СЂРѕРµРЅР° РґР»СЏ РїРµСЂРµРґР°С‡Рё СЃРЅР°С‡Р°Р»Р° MSB, 3 СЂРµР¶РёРј SPI (POL=1 PHA=1).
// РЎРєРѕСЂРѕСЃС‚СЊ РѕР±РјРµРЅР° РЅРµ РґРѕР»Р¶РЅР° РїСЂРµРІС‹С€Р°С‚СЊ 4РњРіС†
// If software transmission not defined, here peripheral registers should be described.
// Peripheral should be configured to transmit MSB first, SPI-3 mode (POL=1, PHA=1)
// Transmission rate should not exceed 4MHz
// 
// Р”Р»СЏ РёСЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ USART РІ СЂРµР¶РёРјРµ SPI MASTER
// РћРїСЂРµРґРµР»РµРЅРёРµ РїСЂРѕС†РµРґСѓСЂС‹ РїРµСЂРµРґР°С‡Рё Р±Р°Р№С‚Р°
// Define a byte transmission procedure
 #define PCD8544_SEND(x) UDR = x
 
 // РџСЂРѕС†РµРґСѓСЂР° РѕР¶РёРґР°РЅРёСЏ РѕРєРѕРЅС‡Р°РЅРёСЏ Р°СЃРёРЅС…СЂРѕРЅРЅРѕР№ РїРµСЂРµРґР°С‡Рё
 // Procedure for awaiting of asynchronous transmission to complete
 #define PCD8544_WAIT_DONE while (!(UCSRA & (1 << TXC));

#endif

#define DISPLAY_WIDTH 84
#define DISPLAY_PAGES 6
#define DISPLAY_HEIGHT (DISPLAY_PAGES * 8)

/* Р�РЅРёС†РёР°Р»РёР·РёСЂСѓРµС‚ РґРёСЃРїР»РµР№
 * Р’СЃРµ Р·Р°РґРµР№СЃС‚РІРѕРІР°РЅРЅС‹Рµ РїРѕСЂС‚С‹ РґРѕР»Р¶РЅС‹ Р±С‹С‚СЊ РЅР°СЃС‚СЂРѕРµРЅС‹ Р·Р°СЂР°РЅРµРµ.
 * Р•СЃР»Рё РёСЃРїРѕР»СЊР·СѓРµС‚СЃСЏ Р°РїРїР°СЂР°С‚РЅР°СЏ РїРµСЂРµРґР°С‡Р°, С‚Рѕ СЃРѕРѕС‚РІРµС‚СЃС‚РІСѓСЋС‰Р°СЏ РїРµСЂРёС„РµСЂРёСЏ РґРѕР»Р¶РЅР° Р±С‹С‚СЊ РїРѕРґРіРѕС‚РѕРІР»РµРЅР° РґРѕ РІС‹Р·РѕРІР°
 * display_init
 * 
 * Initializes the display.
 * All used ports should be configured before calling of this function.
 * Also, if hardware transmission engaged, all corresponding peripherials should be configured.
 */
void display_init();

/* Р“РѕС‚РѕРІРёС‚ РєРѕРЅС‚СЂРѕР»Р»РµСЂ РґРёСЃРїР»РµСЏ Рё Р·Р°С‚РµРј РїРµСЂРµРІРѕРґРёС‚ РµРіРѕ РІ СЂРµР¶РёРј СЌРєРѕРЅРѕРјРёРё СЌРЅРµСЂРіРёРё.
 * РЈСЃС‚Р°РЅР°РІР»РёРІР°РµС‚ РЅР° РІСЃРµС… РїРѕСЂС‚Р°С… РґРёСЃРїР»РµСЏ РІС‹СЃРѕРєРёР№ СѓСЂРѕРІРµРЅСЊ.
 * Р”Р»СЏ РїРѕРІС‚РѕСЂРЅРѕР№ РёРЅРёС†РёР°Р»РёР·Р°С†РёРё РґРёСЃРїР»РµСЏ РЅРµРѕР±С…РѕРґРёРјРѕ РІС‹Р·РІР°С‚СЊ  display_init() РІРЅРѕРІСЊ
 * 
 * Preapares the display controller, and put it to power-down mode.
 * All used ports is set up to high level.
 * To resume from power-down, call display_init() again
 */
void display_powerdown();



/* РџРµСЂРµРґР°С‘С‚ РіСЂР°С„РёС‡РµСЃРєРёРµ РґР°РЅРЅС‹Рµ РЅР° РґРёСЃРїР»РµР№
 * Transmits a graphical data byte */
void display_data(uint8_t t);

/* РџРµСЂРµРґР°С‘С‚ РєРѕРјР°РЅРґСѓ РґРёСЃРїР»РµСЋ
 * Transmits a command to display controller */
void display_cmd(uint8_t t);

/* Р’С‹Р±РёСЂР°РµС‚ СЃС‚СЂР°РЅРёС†Сѓ (РїРѕР·РёС†РёСЋ РїРѕ РІРµСЂС‚РёРєР°Р»Рё) РґР»СЏ РІС‹РІРѕРґР°
 * page - СЃС‚СЂР°РЅРёС†Р° РѕС‚ 0 РґРѕ 5
 * 
 * Selects page (vertical postion) from 0 to 5 */
void display_setpage(uint8_t page);

/* Р’С‹Р±РёСЂР°РµС‚ РіРѕСЂРёР·РѕРЅС‚Р°Р»СЊРЅСѓСЋ РїРѕР·РёС†РёСЋ РґР»СЏ РІС‹РІРѕРґР°
 * x - РїРѕР·РёС†РёСЏ РѕС‚ 0 РґРѕ 83
 * 
 * Selects horizontal position from 0 to 83 */
void display_setx(uint8_t x);

/* Р’С‹Р±РёСЂР°РµС‚ СЃС‚СЂР°РЅРёС†Сѓ Рё РіРѕСЂРёР·РѕРЅС‚Р°Р»СЊРЅСѓСЋ РїРѕР·РёС†РёСЋ РґР»СЏ РІС‹РІРѕРґР°
 * page - СЃС‚СЂР°РЅРёС†Р° РѕС‚ 0 РґРѕ 5
 * x - РїРѕР·РёС†РёСЏ РѕС‚ 0 РґРѕ 83
 * 
 * Selects both page and horizontal position at once
 * */
void display_setpos(uint8_t page, uint8_t x);

/* Р—Р°РїРѕР»РЅСЏРµС‚ СЌРєСЂР°РЅ Р·Р°РґР°РЅРЅС‹РјРё С‡РµСЂРµРґСѓСЋС‰РёРјРёСЃСЏ Р·РЅР°С‡РµРЅРёСЏРјРё
 * Fills the screen with interleaved values */
void display_fill(uint8_t oddData, uint8_t evenData);

/* РћС‡РёС‰Р°РµС‚ СЌРєСЂР°РЅ, СѓСЃС‚Р°РЅР°РІР»РёРІР°РµС‚ РєСѓСЂСЃРѕСЂ РІ Р»РµРІС‹Р№ РІРµСЂС…РЅРёР№ СѓРіРѕР»
 * Clears the screen. Moves cursor to the left upper corner */
void display_clear();

/* Р’С‹РІРѕРґРёС‚ РЅР° СЌРєСЂР°РЅ СЃС‚СЂРѕРєСѓ РёР· С„Р»РµС€-РїР°РјСЏС‚Рё
 * page - СЃС‚СЂРѕРєР° РЅР° РєРѕС‚РѕСЂСѓСЋ РІС‹РІРѕРґРёС‚СЃСЏ РёР·РѕР±СЂР°Р¶РµРЅРёРµ
 * x - РїРѕР·РёС†РёСЏ, РЅР°С‡РёРЅР°СЏ СЃ РєРѕС‚РѕСЂРѕР№ РІС‹РІРѕРґРёС‚СЃСЏ РёР·РѕР±СЂР°Р¶РµРЅРёРµ
 * pData - СЃСЃС‹Р»РєР° РЅР° РґР°РЅРЅС‹Рµ РІРѕ С„Р»РµС€-РїР°РјСЏС‚Рё
 * width - РєРѕР»РёС‡РµСЃС‚РІРѕ РІС‹РІРѕРґРёРјС‹С… РґР°РЅРЅС‹С…, С€РёСЂРёРЅР°
 * xorMask - РёРЅРІРµСЂСЃРёСЏ, РїСЂРёРјРµРЅСЏРµРјР°СЏ Рє РІС‹РІРѕРґРёРјС‹Рј РґР°РЅРЅС‹Рј.
 * yinvert - РµСЃР»Рё РЅРµ РЅРѕР»СЊ, РїРµСЂРµРІРѕСЂР°С‡РёРІР°РµС‚ РёР·РѕР±СЂР°Р¶РµРЅРёРµ РІРІРµСЂС…-РЅРѕРіР°РјРё
 * 
 * Р•СЃР»Рё РёР·РѕР±СЂР°Р¶РµРЅРёРµ РЅРµ РїРѕРјРµС‰Р°РµС‚СЃСЏ РЅР° СЌРєСЂР°РЅ РїРѕ С€РёСЂРёРЅРµ, РІС‹РІРѕРґРёС‚ С‚РѕР»СЊРєРѕ РµРіРѕ С‡Р°СЃС‚СЊ. Р’РѕР·РІСЂР°С‰Р°РµС‚ СЃРєРѕР»СЊРєРѕ РєРѕР»РѕРЅРѕРє Р±С‹Р»Рѕ РІС‹РІРµРґРµРЅРѕ
 */
uint8_t display_sprite(uint8_t page, uint8_t x, PGM_VOID_P pData, uint8_t width, uint8_t xorMask, uint8_t yinvert);

/* РљРѕРїРёСЂСѓРµС‚ РёР· Р±СѓС„РµСЂР° РёР·РѕР±СЂР°Р¶РµРЅРёРµ РЅР° СЌРєСЂР°РЅ
 * page, x - РЅР°С‡Р°Р»СЊРЅР°СЏ РїРѕР·РёС†РёСЏ РґР»СЏ РІС‹РІРѕРґР° Р±СѓС„РµСЂР°
 * pData - СѓРєР°Р·Р°С‚РµР»СЊ РЅР° Р±СѓС„РµСЂ
 * pages_height - РІС‹СЃРѕС‚Р° Р±СѓС„РµСЂР° (РІ СЃС‚СЂР°РЅРёС†Р°С…)
 * width - С€РёСЂРёРЅР° Р±СѓС„РµСЂР°
 * 
 * Transmits a data from the offscreen buffer to display region.
 * page, x - initial position for buffer output
 * pData - pointer to the buffer
 * pages_height - height of the buffer in pages
 * width - width of the buffer area
 */
void display_buffer(uint8_t page, uint8_t x, uint8_t * pData, uint8_t pages_height, uint8_t width);


#endif /* PCD8544_H_ */
