/*
 * text.h
 * Модуль для вывода текстовой информации, работает совместно с PCD8544.h
 * Module for text output, works together with PCD8544.h
 * 
 * Author: Погребняк Дмитрий, г. Самара, 2014
 *
 * Помещённый здесь код, включая шрифты, является свободным. Т.е. допускается его свободное использование для любых целей, включая коммерческие, при условии указания ссылки на автора (Погребняк Дмитрий, http://aterlux.ru/).
 * Code and fonts included here are free to use. It can be used for free for any purpose, including commercial use, as long as link to author (Pogrebnyak Dmitry, http://aterlux.ru/) is included.
 */ 


#ifndef TEXT_H_
#define TEXT_H_

#include "PCD8544.h"
#include <avr/io.h>
#include <avr/pgmspace.h>


// Коды шрифтов. Для отключения шрифта нужно его закомментировать 
// Font codes. To exclude font, comment out corresponding definiton
#define FONT_NORMAL 0
//#define FONT_NARROW 1
#define FONT_BOLD 2
//#define FONT_HANDWRITE 3

// Это определение отключает поддержку шрифтов у которых младший бит кодирует нижний пиксель, для экономии памяти
// Если таковые загружены, то нужно закомментировать это определение.
// This definition disables support of fonts with LSB down, for minimize memory consumption. 
// If such fonts is loaded, this defintion should be commented-out
#define ALL_FONTS_LSB_UP

/* Выбирает шрифт 
 * Selects a font */
void text_set_font(uint8_t font_code);

/* Печатает строку в заданной позиции из флеш-памяти. Строка уходящая за границу экрана спарва - обрезается.
 * Возвращает горизонтальную пощицию, следующую за последним отображённым символом.
 * Символы кириллицы заданные в UTF-8 перекодируются А..Яа..яЁё -> 128..193
 * page - номер страницы (вертикальная позиция 0..5)
 * x - горизонтальная позиция (0..83)
 * pgmStr - адрес строки во флеш-памяти
 * xorMask - применяемая операция xor, позволяет инвертировать вывод
 * 
 * Prints the string from Flash-ROM in defined position. String tail out from right edge of the screen is omitted.
 * Returns horizontal position, next after last printed character.
 * Cyrillic symbols defined in UTF-8 are recoded: А..Яа..яЁё -> 128..193
 * page - page number (vertical position 0..5)
 * x - horizontal position (0..83)
 * pgmStr - pointer to string in Flash-ROM
 * xorMask - value xored to each output byte. Allows to invert ouptut.
 */
uint8_t text_print(uint8_t page, uint8_t x, PGM_P pgmStr, uint8_t xorMask);

uint8_t buff_print(uint8_t page, uint8_t x, char * Str, uint8_t xorMask);

/* Выводит один символ заданным шрифтом. Если символ выходит за правую границу экрана - он обрезается.
 * Возвращает горизонтальную пощицию, следующую после выведенного символа.
 * page - номер страницы (вертикальная позиция 0..5)
 * x - горизонтальная позиция (0..83)
 * ch - код символа
 * xorMask - применяемая операция xor, позволяет инвертировать вывод
 * 
 * Prints one character of selected font. If character out from right edge of the screen, it is truncated.
 * Returns horizontal position, next after character.
 * page - page number (vertical position 0..5)
 * x - horizontal position (0..83)
 * ch - symbol code.
 * xorMask - value xored to each output byte. Allows to invert ouptut.
 */
uint8_t text_char(uint8_t page, uint8_t x, uint8_t ch, uint8_t xorMask);

/* Возвращает ширину символа в выбранном шрифте 
 * Returns a width of character in selected font 
 */
uint8_t text_char_width(uint8_t ch);

/* Выводит беззнаковое число с точкой. 
 * Возвращает горизонтальную пощицию, следующую за последнимвыведенным символом.
 * page - номер страницы (вертикальная позиция 0..5)
 * x - горизонтальная позиция (0..83)
 * num - число
 * decpoint - позиция десятичной точки (0..4), начиная справа 
 * xorMask - применяемая операция xor, позволяет инвертировать вывод
 * 
 * Prints one character of selected font. If character out from right edge of the screen, it is truncated.
 * Returns horizontal position, next after character.
 * page - page number (vertical position 0..5)
 * x - horizontal position (0..83)
 * num - a number
 * decpoint - decimap point position (0..4) counting from rightmost 
 * xorMask - value xored to each output byte. Allows to invert ouptut.
 */
uint8_t text_unum(uint8_t page, uint8_t x, uint16_t num, uint8_t decpoint, uint8_t xorMask);

/* Выводит знаковое число с точкой. 
 * Возвращает горизонтальную пощицию, следующую за последнимвыведенным символом.
 * page - номер страницы (вертикальная позиция 0..5)
 * x - горизонтальная позиция (0..83)
 * num - число
 * decpoint - позиция десятичной точки (0..4), начиная справа 
 * xorMask - применяемая операция xor, позволяет инвертировать вывод
 * 
 * Prints one character of selected font. If character out from right edge of the screen, it is truncated.
 * Returns horizontal position, next after character.
 * page - page number (vertical position 0..5)
 * x - horizontal position (0..83)
 * num - a number
 * decpoint - decimap point position (0..4) counting from rightmost 
 * xorMask - value xored to each output byte. Allows to invert ouptut.
 */
uint8_t text_num(uint8_t page, uint8_t x, int16_t num, uint8_t decpoint, uint8_t xorMask);

void text_clear_from(uint8_t page, uint8_t x);
#endif /* TEXT_H_ */
