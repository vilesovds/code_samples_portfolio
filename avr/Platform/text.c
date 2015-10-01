/*
 * text.c
 * Модуль для вывода текстовой информации, работает совместно с PCD8544.h
 * Module for text output, works together with PCD8544.h
 * 
 * Author: Погребняк Дмитрий, г. Самара, 2014
 *
 * Помещённый здесь код, включая шрифты, является свободным. Т.е. допускается его свободное использование для любых целей, включая коммерческие, при условии указания ссылки на автора (Погребняк Дмитрий, http://aterlux.ru/).
 * Code and fonts included here are free to use. It can be used for free for any purpose, including commercial use, as long as link to author (Pogrebnyak Dmitry, http://aterlux.ru/) is included.
 */ 


#include "text.h"
#include "tools.h"

/*
  Формат шрифта:
 
  Позиция   Длина     Описание
  0         1         Флаги: 
                        бит 7: 1, если старший бит вверху, младший внизу 0 - младший вверху, старший внизу; 
                        бит 1: добавлять пустую колонку перед символом 
                        бит 0: добавлять пустую колонку после символа
  1         1         Вертикальные габариты: 0xtb. Высота шрифта = b - t + 1:
                        биты 6..4: t - первая строка с которой начинается строка шрифта (у полноразмерных шрифтов - 0)
                        биты 2..0: b - последняя строка, на которой заканчивается строка шрифта (у полноразмерных - 7)
  2         1         Первый код символа, присутствующий в шрифте S
  3         1         Последний код символа, присутствующий в шрифте E (должно быть не меньше S)
  4         (E-S+1)*2 Таблица размещения символов и их ширин (сначала младший байт) c S по E:
                      биты 0..11: offset[i] - позиция от начала массива где начинаются данные символа
                      биты 12..15: width[i] - ширина символа 
  offset[i] width[i]  Данные символов
                        Символы кодируются вертикальными линиями, слева направо. Один байт - одна вертикальная линия. Порядок бит определяется значением соответствующего флага
                        Разные символы могут использовать одни и те же позиции данных, вплоть до возможности их пересечения

  Font structure:
  Position  Length    Description
  0         1         Flags:
                        bit 7: 1 - Most Significant Bit on top; 0 - Least Significant Bit on top
                        bit 1: if set, add empty column before every character
                        bit 0: if set, add empty column after every character
  1         1         Vertical size: Height of a font = b - t + 1
                        bits 6..4: t - number of first row (from top)
                        bits 2..0: b - last pixel line
  2         1         S: Code of the first character in a font
  3         1         E: Code of the last character in a font (should be >= S)
  4         (E-S+1)*2 The table of character data offsets and character widths, two bytes (LSB first) for every character from S to E, inclusive:
                        bits 0..11: offset[i] - offset of a character data, from font beginning 
                        bits 12..15: width[i] - the width of a character
  offset[i] width[i]  Font characters data
                        The characters encoded by vertical lines, each byte for each vertical line, from left to right. Bit order determined by bit 7 of Flags
                        Different character may share same character data, even overlap
*/

#define FONT_FLAG_MSBUP 0x80
#define FONT_FLAG_BLANK_BEFORE 0x02
#define FONT_FLAG_BLANK_AFTER 0x01

#ifdef FONT_NORMAL
/* Обычный шрифт. ~5 пикселей в ширину, ~14 символов в строке
 * Normal font ~5 pixel width, ~10 symbols per line */


const uint8_t font_normal[] PROGMEM = {
  0x01, 0x07, 0x17, 0xC1, 0x5A, 0x51, 0x5F, 0x51, 0x64, 0x51, 0x69, 0x51, 0x6D, 0x51, 0x72, 0x61, 0x78, 0x71, 0x7F, 0x71, 0x86, 0x71, 0x8D, 0x11, 0x8E, 0x11, 0x8F, 0x31, 0x92, 0x51, 0x97, 0x51,
  0x9C, 0x51, 0xA1, 0x51, 0xA6, 0x21, 0xA8, 0x21, 0xA9, 0x21, 0xAB, 0x51, 0xB0, 0x51, 0xB5, 0x21, 0xB7, 0x51, 0x65, 0x11, 0xBC, 0x31, 0xBF, 0x51, 0xC4, 0x31, 0xC7, 0x51, 0xCC, 0x51, 0xD1, 0x51,
  0xD6, 0x51, 0xDB, 0x51, 0xE0, 0x51, 0xE5, 0x51, 0xEA, 0x51, 0x92, 0x11, 0xEF, 0x21, 0xF1, 0x31, 0xF4, 0x51, 0xF9, 0x31, 0xFC, 0x51, 0x01, 0x52, 0x06, 0x52, 0x0B, 0x52, 0x10, 0x52, 0x15, 0x52,
  0x1A, 0x52, 0x1F, 0x52, 0x24, 0x52, 0x29, 0x52, 0x2E, 0x32, 0x31, 0x42, 0x35, 0x52, 0x3A, 0x52, 0x3F, 0x52, 0x43, 0x52, 0xBF, 0x51, 0x47, 0x52, 0x4C, 0x52, 0x51, 0x52, 0x56, 0x52, 0x5B, 0x52,
  0x60, 0x52, 0x65, 0x52, 0x6A, 0x52, 0x6F, 0x52, 0x74, 0x52, 0x79, 0x52, 0x15, 0x32, 0x7E, 0x32, 0x81, 0x32, 0x84, 0x52, 0x89, 0x52, 0x62, 0x21, 0x8E, 0x52, 0x93, 0x52, 0x97, 0x52, 0x9C, 0x52,
  0xA1, 0x52, 0xA5, 0x42, 0xA9, 0x52, 0xAE, 0x52, 0xB3, 0x32, 0xB5, 0x32, 0xB8, 0x52, 0xBD, 0x32, 0xC0, 0x52, 0xC5, 0x52, 0xCA, 0x52, 0xCF, 0x52, 0xD3, 0x62, 0xD9, 0x52, 0xDE, 0x52, 0xE3, 0x42,
  0xE7, 0x52, 0xEC, 0x52, 0xF1, 0x52, 0xF6, 0x52, 0xFB, 0x52, 0x00, 0x53, 0x05, 0x33, 0x08, 0x13, 0x09, 0x33, 0x0B, 0x53, 0x10, 0x53, 0x06, 0x52, 0x15, 0x53, 0x0B, 0x52, 0x1A, 0x53, 0x1F, 0x53,
  0x1A, 0x52, 0x24, 0x53, 0x29, 0x53, 0x2E, 0x53, 0x33, 0x53, 0x35, 0x52, 0x38, 0x53, 0x3F, 0x52, 0x29, 0x52, 0xBF, 0x51, 0x3C, 0x53, 0x47, 0x52, 0x10, 0x52, 0x5B, 0x52, 0x41, 0x53, 0x46, 0x53,
  0x6F, 0x52, 0x4B, 0x53, 0x50, 0x53, 0x54, 0x53, 0x58, 0x63, 0x5E, 0x53, 0x5F, 0x53, 0x63, 0x53, 0x68, 0x53, 0x6D, 0x53, 0x72, 0x53, 0x8E, 0x52, 0xDB, 0x51, 0x77, 0x53, 0x7C, 0x53, 0x81, 0x53,
  0xA1, 0x52, 0x86, 0x53, 0x8B, 0x53, 0x90, 0x53, 0x94, 0x53, 0x98, 0x53, 0x9D, 0x53, 0xA1, 0x53, 0xA5, 0x53, 0xCA, 0x52, 0xA9, 0x53, 0xCF, 0x52, 0x97, 0x52, 0xAB, 0x53, 0xFB, 0x52, 0xB0, 0x53,
  0xF6, 0x52, 0xB5, 0x53, 0xBA, 0x53, 0xBE, 0x53, 0xBE, 0x63, 0xC4, 0x53, 0xC5, 0x53, 0xC9, 0x53, 0xCE, 0x53, 0xD3, 0x53, 0xD8, 0x53, 0xDD, 0x53, 0xE2, 0x53, 0x14, 0x22, 0x7F, 0x22, 0x14, 0x04,
  0x02, 0x7F, 0x02, 0x04, 0x10, 0x20, 0x7F, 0x20, 0x10, 0x08, 0x08, 0x2A, 0x1C, 0x08, 0x1C, 0x2A, 0x08, 0x08, 0x38, 0x38, 0x20, 0x20, 0x20, 0x20, 0x08, 0x1C, 0x2A, 0x08, 0x2A, 0x1C, 0x08, 0x40,
  0x70, 0x7C, 0x7F, 0x7C, 0x70, 0x40, 0x01, 0x07, 0x1F, 0x7F, 0x1F, 0x07, 0x01, 0x00, 0x5F, 0x03, 0x00, 0x03, 0x24, 0x7E, 0x24, 0x7E, 0x24, 0x26, 0x49, 0x7F, 0x49, 0x32, 0x43, 0x33, 0x08, 0x66,
  0x61, 0x30, 0x4E, 0x49, 0x36, 0x50, 0x02, 0x01, 0x3E, 0x41, 0x3E, 0x14, 0x08, 0x3E, 0x08, 0x14, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x40, 0x20, 0x08, 0x08, 0x08, 0x08, 0x08, 0x60, 0x18, 0x06, 0x3E,
  0x41, 0x41, 0x41, 0x3E, 0x42, 0x7F, 0x40, 0x42, 0x61, 0x51, 0x49, 0x46, 0x21, 0x41, 0x49, 0x4D, 0x33, 0x18, 0x14, 0x12, 0x7F, 0x10, 0x27, 0x45, 0x45, 0x45, 0x39, 0x3E, 0x45, 0x45, 0x45, 0x38,
  0x01, 0x01, 0x71, 0x0D, 0x03, 0x36, 0x49, 0x49, 0x49, 0x36, 0x06, 0x49, 0x49, 0x49, 0x3E, 0x40, 0x24, 0x08, 0x14, 0x22, 0x14, 0x14, 0x14, 0x14, 0x14, 0x22, 0x14, 0x08, 0x02, 0x01, 0x51, 0x09,
  0x06, 0x1C, 0x22, 0x49, 0x55, 0x5E, 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x7F, 0x49, 0x49, 0x4E, 0x30, 0x3E, 0x41, 0x41, 0x41, 0x22, 0x7F, 0x41, 0x41, 0x22, 0x1C, 0x7F, 0x49, 0x49, 0x49, 0x41, 0x7F,
  0x09, 0x09, 0x09, 0x01, 0x3E, 0x41, 0x41, 0x51, 0x32, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x41, 0x7F, 0x41, 0x20, 0x40, 0x40, 0x3F, 0x7F, 0x08, 0x0C, 0x12, 0x61, 0x7F, 0x40, 0x40, 0x40, 0x40, 0x7F,
  0x06, 0x18, 0x06, 0x7F, 0x02, 0x0C, 0x10, 0x7F, 0x09, 0x09, 0x09, 0x06, 0x3E, 0x41, 0x41, 0x21, 0x5E, 0x7F, 0x09, 0x19, 0x29, 0x46, 0x26, 0x49, 0x49, 0x49, 0x32, 0x01, 0x01, 0x7F, 0x01, 0x01,
  0x3F, 0x40, 0x40, 0x40, 0x3F, 0x07, 0x18, 0x60, 0x18, 0x07, 0x3F, 0x60, 0x10, 0x60, 0x3F, 0x63, 0x14, 0x08, 0x14, 0x63, 0x07, 0x08, 0x70, 0x08, 0x07, 0x61, 0x51, 0x49, 0x45, 0x43, 0x06, 0x18,
  0x60, 0x41, 0x41, 0x7F, 0x04, 0x02, 0x01, 0x02, 0x04, 0x40, 0x40, 0x40, 0x40, 0x40, 0x20, 0x54, 0x54, 0x38, 0x40, 0x7F, 0x24, 0x44, 0x44, 0x38, 0x44, 0x44, 0x44, 0x28, 0x38, 0x44, 0x44, 0x44,
  0x7F, 0x38, 0x54, 0x54, 0x54, 0x08, 0x7E, 0x09, 0x01, 0x18, 0xA4, 0xA4, 0xA8, 0x7C, 0x7F, 0x04, 0x04, 0x04, 0x78, 0x44, 0x7D, 0x40, 0x84, 0x7D, 0x7F, 0x10, 0x18, 0x24, 0x40, 0x41, 0x7F, 0x40,
  0x7C, 0x04, 0x78, 0x04, 0x78, 0x7C, 0x08, 0x04, 0x04, 0x78, 0x38, 0x44, 0x44, 0x44, 0x38, 0xFC, 0x24, 0x24, 0x24, 0x18, 0x24, 0x24, 0x24, 0xFC, 0x40, 0x7C, 0x08, 0x04, 0x04, 0x04, 0x48, 0x54,
  0x54, 0x54, 0x24, 0x04, 0x3F, 0x44, 0x40, 0x3C, 0x40, 0x40, 0x40, 0x3C, 0x0C, 0x30, 0x40, 0x30, 0x0C, 0x1C, 0x60, 0x10, 0x60, 0x1C, 0x44, 0x28, 0x10, 0x28, 0x44, 0x0C, 0x90, 0x60, 0x30, 0x0C,
  0x44, 0x64, 0x54, 0x4C, 0x44, 0x08, 0x36, 0x41, 0xFF, 0x41, 0x36, 0x08, 0x04, 0x08, 0x10, 0x08, 0x7C, 0x42, 0x41, 0x42, 0x7C, 0x7F, 0x49, 0x49, 0x49, 0x30, 0x7F, 0x01, 0x01, 0x01, 0x01, 0xC0,
  0x7E, 0x41, 0x7F, 0xC0, 0x63, 0x14, 0x7F, 0x14, 0x63, 0x22, 0x41, 0x49, 0x49, 0x36, 0x7F, 0x20, 0x10, 0x08, 0x7F, 0x7E, 0x21, 0x11, 0x09, 0x7E, 0x40, 0x3E, 0x01, 0x01, 0x7F, 0x01, 0x01, 0x01,
  0x7F, 0x03, 0x4C, 0x30, 0x0C, 0x03, 0x0C, 0x12, 0x7F, 0x12, 0x0C, 0x7F, 0x40, 0x40, 0x7F, 0xC0, 0x07, 0x08, 0x08, 0x08, 0x7F, 0x40, 0x7E, 0x40, 0x7F, 0x40, 0x7F, 0x40, 0x7F, 0xC0, 0x01, 0x7F,
  0x48, 0x48, 0x30, 0x7F, 0x48, 0x48, 0x48, 0x30, 0x22, 0x41, 0x49, 0x49, 0x3E, 0x7F, 0x08, 0x3E, 0x41, 0x3E, 0x46, 0x29, 0x19, 0x09, 0x7F, 0x7C, 0x54, 0x54, 0x58, 0x20, 0x7C, 0x04, 0x04, 0x04,
  0x04, 0xC0, 0x78, 0x44, 0x7C, 0xC0, 0x6C, 0x10, 0x7C, 0x10, 0x6C, 0x28, 0x44, 0x44, 0x54, 0x28, 0x7C, 0x20, 0x10, 0x08, 0x7C, 0x21, 0x12, 0x09, 0x7C, 0x10, 0x18, 0x14, 0x60, 0x40, 0x3C, 0x04,
  0x04, 0x7C, 0x08, 0x10, 0x08, 0x7C, 0x10, 0x10, 0x10, 0x7C, 0x04, 0x04, 0x04, 0x7C, 0x04, 0x04, 0x10, 0x28, 0x7C, 0x28, 0x10, 0x7C, 0x40, 0x40, 0x7C, 0xC0, 0x0C, 0x10, 0x10, 0x10, 0x7C, 0x40,
  0x78, 0x40, 0x7C, 0xC0, 0x04, 0x7C, 0x50, 0x50, 0x20, 0x7C, 0x50, 0x50, 0x50, 0x20, 0x28, 0x44, 0x54, 0x54, 0x38, 0x7C, 0x10, 0x38, 0x44, 0x38, 0x48, 0x34, 0x14, 0x14, 0x7C, 0x7E, 0x4B, 0x4A,
  0x4B, 0x42, 0x38, 0x55, 0x54, 0x55, 0x08
}; // 999 bytes

#endif

#ifdef FONT_BOLD
/* Жирный шрифт. ~7 пикселей в ширину, ~10 символов в строке 
 * Bold font ~7 pixel width, ~10 symbols per line */


const uint8_t font_bold[] PROGMEM = {
  0x01, 0x07, 0x17, 0xC1, 0x5A, 0x71, 0x61, 0x51, 0x66, 0x51, 0x6B, 0x71, 0x71, 0x71, 0x78, 0x61, 0x7E, 0x71, 0x85, 0x71, 0x8C, 0x71, 0x93, 0x11, 0x94, 0x41, 0x98, 0x51, 0x9D, 0x71, 0xA4, 0x71,
  0xAB, 0x71, 0xB2, 0x71, 0x98, 0x21, 0xB9, 0x31, 0xBB, 0x31, 0xBE, 0x51, 0xC3, 0x51, 0xC8, 0x21, 0xCA, 0x51, 0xCF, 0x21, 0xD1, 0x71, 0xD8, 0x71, 0xDF, 0x41, 0xE3, 0x71, 0xEA, 0x71, 0xF1, 0x71,
  0xF8, 0x71, 0xFF, 0x71, 0x06, 0x72, 0x0D, 0x72, 0x14, 0x72, 0x1B, 0x22, 0x1D, 0x22, 0x1F, 0x52, 0x24, 0x52, 0x29, 0x52, 0x2E, 0x72, 0x35, 0x72, 0x3C, 0x72, 0x43, 0x72, 0x4A, 0x72, 0x51, 0x72,
  0x58, 0x72, 0x5F, 0x72, 0x66, 0x72, 0x6D, 0x72, 0x74, 0x42, 0x78, 0x72, 0x7F, 0x72, 0x86, 0x72, 0x8D, 0x72, 0x92, 0x72, 0x99, 0x72, 0xA0, 0x72, 0xA7, 0x72, 0xAE, 0x72, 0xB5, 0x72, 0xBC, 0x62,
  0xC2, 0x72, 0xC9, 0x72, 0xD0, 0x72, 0xD7, 0x72, 0xDE, 0x62, 0xE4, 0x72, 0x51, 0x42, 0xEB, 0x72, 0xF2, 0x42, 0xF6, 0x52, 0xFB, 0x52, 0x00, 0x23, 0x02, 0x73, 0x09, 0x73, 0x0F, 0x73, 0x16, 0x73,
  0x1D, 0x73, 0x23, 0x53, 0x28, 0x73, 0x2F, 0x73, 0x36, 0x43, 0x39, 0x63, 0x3F, 0x73, 0x46, 0x43, 0x4A, 0x73, 0x51, 0x73, 0x58, 0x73, 0x5F, 0x73, 0x65, 0x73, 0x6C, 0x73, 0x73, 0x73, 0x7A, 0x53,
  0x7F, 0x73, 0x86, 0x73, 0x8D, 0x73, 0x94, 0x73, 0x9B, 0x73, 0xA2, 0x73, 0xA9, 0x43, 0x9E, 0x21, 0xAC, 0x43, 0xB0, 0x43, 0xB4, 0x73, 0xBA, 0x73, 0xC1, 0x73, 0x43, 0x72, 0xC8, 0x73, 0xCF, 0x73,
  0x58, 0x72, 0xD6, 0x73, 0xDD, 0x73, 0xE4, 0x73, 0xEA, 0x73, 0x7F, 0x72, 0xF1, 0x73, 0xF6, 0x73, 0x6D, 0x72, 0x99, 0x72, 0xFB, 0x73, 0xA0, 0x72, 0x4A, 0x72, 0xBC, 0x62, 0x02, 0x74, 0x09, 0x64,
  0x0F, 0x74, 0x16, 0x74, 0x1D, 0x74, 0x22, 0x74, 0x27, 0x84, 0x2F, 0x74, 0x36, 0x74, 0x3C, 0x74, 0x43, 0x74, 0x4A, 0x74, 0x51, 0x74, 0x02, 0x73, 0x58, 0x74, 0x5F, 0x74, 0x66, 0x64, 0x6C, 0x74,
  0x73, 0x74, 0x7A, 0x74, 0x81, 0x74, 0x88, 0x74, 0x8D, 0x74, 0x92, 0x74, 0x98, 0x74, 0x9D, 0x74, 0xA2, 0x74, 0x58, 0x73, 0xA7, 0x74, 0xAE, 0x74, 0x0F, 0x73, 0xB5, 0x64, 0xBB, 0x74, 0xC2, 0x64,
  0x94, 0x73, 0xC8, 0x74, 0xCF, 0x74, 0xD4, 0x74, 0xD9, 0x84, 0xE1, 0x74, 0xE8, 0x74, 0xEE, 0x74, 0xF5, 0x74, 0xFC, 0x74, 0x03, 0x75, 0x0A, 0x75, 0x11, 0x75, 0x80, 0xA4, 0xE6, 0xFF, 0xE6, 0xA4,
  0x80, 0x08, 0x0C, 0x7E, 0x0C, 0x08, 0x10, 0x30, 0x7E, 0x30, 0x10, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x1C, 0x08, 0x1C, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x38, 0x38, 0x20, 0x20, 0x20, 0x20, 0x10, 0x38,
  0x7C, 0x10, 0x7C, 0x38, 0x10, 0x40, 0x70, 0x7C, 0x7F, 0x7C, 0x70, 0x40, 0x01, 0x07, 0x1F, 0x7F, 0x1F, 0x07, 0x01, 0x00, 0x06, 0x6F, 0x6F, 0x06, 0x05, 0x03, 0x00, 0x05, 0x03, 0x22, 0x7F, 0x7F,
  0x22, 0x7F, 0x7F, 0x22, 0x24, 0x4A, 0x7F, 0x4A, 0x7F, 0x4A, 0x32, 0x42, 0x25, 0x12, 0x08, 0x24, 0x52, 0x21, 0x30, 0x4E, 0x49, 0x49, 0x56, 0x30, 0x28, 0x3E, 0x7F, 0x41, 0x7F, 0x3E, 0x14, 0x08,
  0x3E, 0x08, 0x14, 0x08, 0x08, 0x3E, 0x08, 0x08, 0xA0, 0x60, 0x08, 0x08, 0x08, 0x08, 0x08, 0x60, 0x60, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x1C, 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x1C, 0x42,
  0x7F, 0x7F, 0x40, 0x62, 0x73, 0x51, 0x59, 0x49, 0x4F, 0x66, 0x22, 0x63, 0x41, 0x49, 0x49, 0x7F, 0x36, 0x10, 0x18, 0x14, 0x52, 0x7F, 0x7F, 0x50, 0x27, 0x67, 0x45, 0x45, 0x45, 0x7D, 0x39, 0x3C,
  0x7E, 0x4B, 0x49, 0x49, 0x7B, 0x32, 0x03, 0x03, 0x01, 0x71, 0x79, 0x0F, 0x07, 0x36, 0x7F, 0x49, 0x49, 0x49, 0x7F, 0x36, 0x0E, 0x5F, 0x51, 0x51, 0x51, 0x7F, 0x3E, 0x66, 0x66, 0xA6, 0x66, 0x08,
  0x1C, 0x36, 0x63, 0x41, 0x14, 0x14, 0x14, 0x14, 0x14, 0x41, 0x63, 0x36, 0x1C, 0x08, 0x02, 0x03, 0x01, 0x51, 0x59, 0x0F, 0x06, 0x1C, 0x22, 0x49, 0x55, 0x49, 0x52, 0x0C, 0x7C, 0x7E, 0x13, 0x13,
  0x13, 0x7E, 0x7C, 0x7F, 0x7F, 0x49, 0x49, 0x4F, 0x7A, 0x30, 0x3E, 0x7F, 0x41, 0x41, 0x41, 0x63, 0x22, 0x7F, 0x7F, 0x41, 0x41, 0x63, 0x3E, 0x1C, 0x7F, 0x7F, 0x49, 0x49, 0x41, 0x63, 0x63, 0x7F,
  0x7F, 0x09, 0x09, 0x01, 0x03, 0x03, 0x3E, 0x7F, 0x41, 0x49, 0x49, 0x7B, 0x3A, 0x7F, 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x7F, 0x41, 0x7F, 0x7F, 0x41, 0x30, 0x70, 0x40, 0x40, 0x40, 0x7F, 0x3F, 0x7F,
  0x7F, 0x08, 0x0C, 0x1E, 0x73, 0x61, 0x7F, 0x7F, 0x40, 0x40, 0x40, 0x60, 0x60, 0x7F, 0x7F, 0x06, 0x18, 0x06, 0x7F, 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x7F, 0x3E, 0x7F, 0x41, 0x41, 0x41, 0x7F, 0x3E,
  0x7F, 0x7F, 0x11, 0x11, 0x11, 0x1F, 0x0E, 0x3E, 0x7F, 0x41, 0x51, 0x61, 0x3F, 0x5E, 0x7F, 0x7F, 0x11, 0x11, 0x31, 0x7F, 0x4E, 0x26, 0x6F, 0x49, 0x49, 0x49, 0x7B, 0x32, 0x01, 0x01, 0x7F, 0x7F,
  0x01, 0x01, 0x3F, 0x7F, 0x40, 0x40, 0x40, 0x7F, 0x3F, 0x0F, 0x1F, 0x30, 0x60, 0x30, 0x1F, 0x0F, 0x7F, 0x7F, 0x30, 0x0C, 0x30, 0x7F, 0x7F, 0x61, 0x73, 0x1E, 0x0C, 0x1E, 0x73, 0x61, 0x07, 0x0F,
  0x78, 0x78, 0x0F, 0x07, 0x63, 0x73, 0x59, 0x49, 0x4D, 0x67, 0x63, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x41, 0x41, 0x7F, 0x7F, 0x04, 0x02, 0x01, 0x02, 0x04, 0x40, 0x40, 0x40, 0x40, 0x40,
  0x03, 0x05, 0x20, 0x74, 0x54, 0x54, 0x7C, 0x38, 0x40, 0x7F, 0x7F, 0x44, 0x44, 0x44, 0x7C, 0x38, 0x7C, 0x44, 0x44, 0x44, 0x6C, 0x28, 0x30, 0x78, 0x48, 0x48, 0x48, 0x7F, 0x7F, 0x38, 0x7C, 0x54,
  0x54, 0x54, 0x5C, 0x08, 0x7E, 0x7F, 0x09, 0x01, 0x18, 0xBC, 0xA4, 0xA4, 0xA8, 0xFC, 0x7C, 0x7F, 0x7F, 0x04, 0x04, 0x04, 0x7C, 0x78, 0x44, 0x7D, 0x7D, 0x40, 0xC0, 0x80, 0x80, 0xFD, 0x7D, 0x7F,
  0x7F, 0x10, 0x18, 0x3C, 0x64, 0x40, 0x41, 0x7F, 0x7F, 0x40, 0x7C, 0x7C, 0x04, 0x78, 0x04, 0x7C, 0x78, 0x7C, 0x7C, 0x04, 0x04, 0x04, 0x7C, 0x78, 0x38, 0x7C, 0x44, 0x44, 0x44, 0x7C, 0x38, 0xFC,
  0xFC, 0x24, 0x24, 0x24, 0x3C, 0x18, 0x3C, 0x24, 0x24, 0xFC, 0xFC, 0x40, 0x7C, 0x7C, 0x08, 0x0C, 0x04, 0x04, 0x04, 0x08, 0x5C, 0x54, 0x54, 0x54, 0x74, 0x20, 0x04, 0x3F, 0x7F, 0x44, 0x40, 0x3C,
  0x7C, 0x40, 0x40, 0x40, 0x7C, 0x3C, 0x0C, 0x1C, 0x30, 0x60, 0x30, 0x1C, 0x0C, 0x3C, 0x7C, 0x40, 0x30, 0x40, 0x7C, 0x3C, 0x44, 0x6C, 0x38, 0x10, 0x38, 0x6C, 0x44, 0x1C, 0xBC, 0xA0, 0xA0, 0xA0,
  0xFC, 0x7C, 0x44, 0x64, 0x74, 0x54, 0x5C, 0x4C, 0x44, 0x08, 0x3E, 0x77, 0x41, 0x77, 0x3E, 0x08, 0x04, 0x02, 0x04, 0x02, 0x78, 0x44, 0x42, 0x41, 0x42, 0x44, 0x78, 0x7C, 0x16, 0x13, 0x16, 0x7C,
  0x78, 0x7F, 0x7F, 0x49, 0x49, 0x4B, 0x7B, 0x30, 0x7F, 0x7F, 0x01, 0x01, 0x01, 0x03, 0x03, 0x40, 0x7C, 0x7E, 0x41, 0x7F, 0x7F, 0x40, 0x63, 0x77, 0x1C, 0x7F, 0x1C, 0x77, 0x63, 0x22, 0x63, 0x41,
  0x41, 0x49, 0x7F, 0x36, 0x7F, 0x7F, 0x20, 0x10, 0x08, 0x7F, 0x7F, 0x7E, 0x21, 0x12, 0x09, 0x7E, 0x7F, 0x40, 0x7E, 0x3F, 0x01, 0x01, 0x7F, 0x7F, 0x0C, 0x30, 0x0C, 0x7F, 0x7F, 0x01, 0x01, 0x01,
  0x7F, 0x7F, 0x0F, 0x5F, 0x50, 0x50, 0x50, 0x7F, 0x3F, 0x0C, 0x12, 0x7F, 0x7F, 0x12, 0x0C, 0x61, 0x73, 0x1E, 0x1C, 0x1E, 0x73, 0x61, 0x7F, 0x7F, 0x40, 0x40, 0x7F, 0xFF, 0xC0, 0x0F, 0x1F, 0x10,
  0x10, 0x10, 0x7F, 0x7F, 0x40, 0x7C, 0x40, 0x7F, 0x7F, 0x40, 0x7C, 0x40, 0x7F, 0xFF, 0xC0, 0x01, 0x7F, 0x7F, 0x48, 0x48, 0x78, 0x30, 0x7F, 0x7F, 0x48, 0x78, 0x30, 0x4F, 0x7F, 0x7F, 0x48, 0x48,
  0x48, 0x78, 0x30, 0x22, 0x63, 0x41, 0x49, 0x49, 0x7F, 0x3E, 0x7F, 0x7F, 0x08, 0x3E, 0x41, 0x7F, 0x3E, 0x0E, 0x5F, 0x71, 0x31, 0x11, 0x7F, 0x7F, 0x38, 0x7C, 0x4E, 0x4A, 0x4B, 0x79, 0x30, 0x7C,
  0x7C, 0x54, 0x54, 0x5C, 0x78, 0x20, 0x7C, 0x7C, 0x04, 0x04, 0x0C, 0x0C, 0x40, 0x78, 0x7C, 0x44, 0x7C, 0x7C, 0x40, 0x38, 0x7C, 0x54, 0x54, 0x54, 0x5C, 0x18, 0x6C, 0x7C, 0x10, 0x7C, 0x10, 0x7C,
  0x6C, 0x28, 0x6C, 0x44, 0x54, 0x54, 0x7C, 0x28, 0x7C, 0x7C, 0x20, 0x10, 0x08, 0x7C, 0x7C, 0x21, 0x12, 0x09, 0x7C, 0x7C, 0x10, 0x18, 0x3C, 0x64, 0x40, 0x78, 0x3C, 0x04, 0x04, 0x7C, 0x7C, 0x08,
  0x10, 0x08, 0x7C, 0x7C, 0x10, 0x10, 0x10, 0x7C, 0x7C, 0x04, 0x04, 0x04, 0x7C, 0x7C, 0xFC, 0xFC, 0x44, 0x44, 0x44, 0x7C, 0x38, 0x04, 0x04, 0x7C, 0x7C, 0x04, 0x04, 0x0C, 0x9C, 0xB0, 0xE0, 0x70,
  0x3C, 0x0C, 0x10, 0x28, 0x7C, 0x7C, 0x28, 0x10, 0x7C, 0x7C, 0x40, 0x40, 0x7C, 0xFC, 0xC0, 0x0C, 0x1C, 0x10, 0x10, 0x10, 0x7C, 0x7C, 0x40, 0x78, 0x40, 0x7C, 0x7C, 0x40, 0x78, 0x40, 0x7C, 0xFC,
  0xC0, 0x04, 0x7C, 0x7C, 0x50, 0x50, 0x70, 0x20, 0x7C, 0x7C, 0x50, 0x50, 0x70, 0x2C, 0x7C, 0x7C, 0x50, 0x50, 0x50, 0x70, 0x20, 0x28, 0x6C, 0x44, 0x54, 0x54, 0x7C, 0x38, 0x7C, 0x7C, 0x10, 0x38,
  0x44, 0x7C, 0x38, 0x08, 0x5C, 0x74, 0x34, 0x14, 0x7C, 0x7C, 0x7E, 0x7F, 0x4B, 0x4A, 0x43, 0x67, 0x66, 0x38, 0x7D, 0x55, 0x54, 0x55, 0x5D, 0x18
}; // 1304 bytes

#endif


#ifdef FONT_NARROW 
/* Узкий шрифт ~3-4 пикселя в ширину, ~16-21 символ в строке 
 * Narrow font ~3-4 pixel width, ~16-21 symbols per line */

const uint8_t font_narrow[] PROGMEM = {
  0x01, 0x07, 0x17, 0xC1, 0x5A, 0x51, 0x5F, 0x51, 0x64, 0x51, 0x69, 0x51, 0x6D, 0x51, 0x72, 0x61, 0x78, 0x71, 0x7F, 0x71, 0x86, 0x71, 0x8D, 0x11, 0x8E, 0x11, 0x8F, 0x31, 0x92, 0x51, 0x97, 0x41,
  0x9B, 0x41, 0x9F, 0x41, 0x8F, 0x11, 0xA3, 0x21, 0xA4, 0x21, 0xA6, 0x41, 0xAA, 0x31, 0xAD, 0x11, 0xAE, 0x31, 0x65, 0x11, 0xB1, 0x31, 0xB4, 0x41, 0xB8, 0x31, 0xBB, 0x41, 0xBF, 0x41, 0xC3, 0x41,
  0xC7, 0x41, 0xCB, 0x41, 0xCF, 0x41, 0xD3, 0x41, 0xD7, 0x41, 0x92, 0x11, 0xDB, 0x11, 0xDC, 0x31, 0xDF, 0x31, 0xE2, 0x31, 0xE5, 0x31, 0xE8, 0x41, 0xEC, 0x41, 0xF0, 0x41, 0xF4, 0x41, 0xF8, 0x41,
  0xFC, 0x41, 0x00, 0x42, 0x04, 0x42, 0x08, 0x42, 0x5C, 0x11, 0x0C, 0x32, 0x0F, 0x42, 0x13, 0x42, 0x17, 0x52, 0x1B, 0x42, 0xB4, 0x41, 0x1E, 0x42, 0x22, 0x42, 0x26, 0x42, 0x2A, 0x42, 0x2E, 0x32,
  0x31, 0x42, 0x35, 0x42, 0x39, 0x52, 0x3E, 0x42, 0x42, 0x42, 0x46, 0x42, 0xF8, 0x21, 0x4A, 0x32, 0xFF, 0x21, 0x4D, 0x32, 0x50, 0x42, 0x62, 0x21, 0x54, 0x32, 0x57, 0x32, 0x59, 0x32, 0x5C, 0x32,
  0x5F, 0x32, 0x62, 0x32, 0x65, 0x32, 0x68, 0x32, 0x6B, 0x12, 0x6C, 0x22, 0x6E, 0x32, 0x5C, 0x11, 0x71, 0x32, 0x74, 0x32, 0x77, 0x32, 0x7A, 0x32, 0x7C, 0x32, 0x7F, 0x32, 0x82, 0x32, 0x85, 0x32,
  0x88, 0x32, 0x8B, 0x32, 0x8E, 0x32, 0x91, 0x32, 0x94, 0x32, 0x97, 0x32, 0x9A, 0x32, 0x9D, 0x12, 0x9E, 0x32, 0xA0, 0x32, 0xA3, 0x52, 0xEC, 0x41, 0xA8, 0x42, 0xF0, 0x41, 0xAC, 0x42, 0xB0, 0x52,
  0xFC, 0x41, 0xB5, 0x52, 0xBA, 0x42, 0xBE, 0x42, 0xC2, 0x42, 0x0F, 0x42, 0xC6, 0x42, 0x17, 0x52, 0x08, 0x42, 0xB4, 0x41, 0xC9, 0x42, 0x1E, 0x42, 0xF4, 0x41, 0x2E, 0x32, 0xCD, 0x42, 0xD1, 0x52,
  0x3E, 0x42, 0xD6, 0x42, 0xDA, 0x42, 0xDD, 0x52, 0xE1, 0x52, 0xE6, 0x42, 0xE7, 0x42, 0xEA, 0x42, 0xEE, 0x42, 0xF2, 0x42, 0xF6, 0x42, 0x54, 0x32, 0xFA, 0x32, 0xFD, 0x32, 0x00, 0x33, 0x03, 0x33,
  0x06, 0x33, 0x09, 0x33, 0x0C, 0x33, 0x8E, 0x32, 0x0F, 0x33, 0x11, 0x33, 0x8C, 0x32, 0x14, 0x33, 0x16, 0x33, 0x77, 0x32, 0x18, 0x33, 0x7A, 0x32, 0x59, 0x32, 0x19, 0x33, 0x94, 0x32, 0x1C, 0x33,
  0x91, 0x32, 0x1F, 0x33, 0x22, 0x33, 0x24, 0x33, 0x26, 0x33, 0x29, 0x33, 0x2C, 0x43, 0x2C, 0x33, 0x30, 0x33, 0x33, 0x43, 0x37, 0x33, 0x3A, 0x43, 0x3E, 0x33, 0x14, 0x22, 0x7F, 0x22, 0x14, 0x04,
  0x02, 0x7F, 0x02, 0x04, 0x10, 0x20, 0x7F, 0x20, 0x10, 0x08, 0x08, 0x2A, 0x1C, 0x08, 0x1C, 0x2A, 0x08, 0x08, 0x38, 0x38, 0x20, 0x20, 0x20, 0x20, 0x08, 0x1C, 0x2A, 0x08, 0x2A, 0x1C, 0x08, 0x40,
  0x70, 0x7C, 0x7F, 0x7C, 0x70, 0x40, 0x01, 0x07, 0x1F, 0x7F, 0x1F, 0x07, 0x01, 0x00, 0x5F, 0x03, 0x00, 0x03, 0x24, 0x7E, 0x24, 0x7E, 0x24, 0x46, 0x79, 0x4F, 0x31, 0x42, 0x30, 0x0C, 0x42, 0x36,
  0x49, 0x36, 0x50, 0x3E, 0x41, 0x3E, 0x2A, 0x1C, 0x1C, 0x2A, 0x08, 0x1C, 0x08, 0x60, 0x08, 0x08, 0x08, 0x60, 0x18, 0x06, 0x3E, 0x41, 0x41, 0x3E, 0x42, 0x7F, 0x40, 0x62, 0x51, 0x49, 0x46, 0x41,
  0x49, 0x4D, 0x33, 0x18, 0x16, 0x7F, 0x10, 0x27, 0x45, 0x45, 0x39, 0x3E, 0x45, 0x45, 0x38, 0x01, 0x01, 0x79, 0x07, 0x36, 0x49, 0x49, 0x36, 0x06, 0x49, 0x49, 0x3E, 0x64, 0x08, 0x14, 0x22, 0x14,
  0x14, 0x14, 0x22, 0x14, 0x08, 0x01, 0x59, 0x06, 0x3E, 0x49, 0x55, 0x5E, 0x7E, 0x11, 0x11, 0x7E, 0x7F, 0x49, 0x4E, 0x30, 0x3E, 0x41, 0x41, 0x22, 0x7F, 0x41, 0x41, 0x3E, 0x7F, 0x49, 0x49, 0x41,
  0x7F, 0x09, 0x09, 0x01, 0x3E, 0x41, 0x51, 0x32, 0x7F, 0x08, 0x08, 0x7F, 0x20, 0x40, 0x3F, 0x7F, 0x0C, 0x0A, 0x71, 0x7F, 0x40, 0x40, 0x40, 0x7F, 0x06, 0x18, 0x06, 0x7F, 0x06, 0x18, 0x7F, 0x09,
  0x09, 0x06, 0x3E, 0x41, 0x31, 0x5E, 0x7F, 0x09, 0x19, 0x66, 0x26, 0x49, 0x49, 0x32, 0x01, 0x7F, 0x01, 0x3F, 0x40, 0x40, 0x3F, 0x1F, 0x60, 0x60, 0x1F, 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x73, 0x0C,
  0x0C, 0x73, 0x07, 0x78, 0x78, 0x07, 0x71, 0x49, 0x45, 0x43, 0x06, 0x18, 0x60, 0x02, 0x01, 0x02, 0x40, 0x40, 0x40, 0x40, 0x20, 0x54, 0x78, 0x7F, 0x44, 0x38, 0x44, 0x44, 0x38, 0x44, 0x7F, 0x38,
  0x54, 0x58, 0x08, 0x7E, 0x09, 0x18, 0xA4, 0x7C, 0x7F, 0x04, 0x78, 0x7D, 0x80, 0x7D, 0x7F, 0x18, 0x64, 0x7C, 0x3C, 0x78, 0x7C, 0x04, 0x78, 0x38, 0x44, 0x38, 0xFC, 0x24, 0x18, 0x24, 0xFC, 0x7C,
  0x08, 0x04, 0x48, 0x54, 0x24, 0x04, 0x3F, 0x44, 0x3C, 0x40, 0x7C, 0x3C, 0x40, 0x3C, 0x7C, 0x30, 0x7C, 0x6C, 0x10, 0x6C, 0x1C, 0xA0, 0x7C, 0x64, 0x54, 0x4C, 0x08, 0x36, 0x41, 0xFF, 0x41, 0x36,
  0x08, 0x0C, 0x04, 0x7C, 0x42, 0x41, 0x42, 0x7C, 0x7F, 0x49, 0x49, 0x30, 0x7F, 0x01, 0x01, 0x01, 0xC0, 0x7E, 0x41, 0x7F, 0xC0, 0x63, 0x14, 0x7F, 0x14, 0x63, 0x22, 0x41, 0x49, 0x36, 0x7F, 0x20,
  0x10, 0x7F, 0x7E, 0x21, 0x11, 0x7E, 0x40, 0x3E, 0x01, 0x7F, 0x01, 0x01, 0x7F, 0x03, 0x4C, 0x30, 0x0F, 0x0C, 0x12, 0x7F, 0x12, 0x0C, 0x7F, 0x40, 0x7F, 0xC0, 0x07, 0x08, 0x08, 0x7F, 0x40, 0x7E,
  0x40, 0x7F, 0x40, 0x7F, 0x40, 0xFF, 0x01, 0x7F, 0x48, 0x30, 0x7F, 0x48, 0x48, 0x30, 0x22, 0x49, 0x49, 0x3E, 0x7F, 0x3E, 0x41, 0x3E, 0x46, 0x29, 0x19, 0x7F, 0x3E, 0x45, 0x38, 0x7C, 0x54, 0x28,
  0x7C, 0x04, 0x04, 0x39, 0x45, 0x3E, 0x38, 0x54, 0x48, 0x6C, 0x7C, 0x6C, 0x54, 0x54, 0x28, 0x7C, 0x31, 0x7C, 0x18, 0x64, 0x7C, 0x18, 0x7C, 0x10, 0x7C, 0x04, 0x7C, 0x04, 0x38, 0x7C, 0x38, 0x7C,
  0x40, 0xFC, 0x0C, 0x10, 0x7C, 0x78, 0x7C, 0x78, 0xFC, 0x04, 0x7C, 0x70, 0x7C, 0x50, 0x20, 0x7C, 0x44, 0x54, 0x38, 0x7C, 0x38, 0x44, 0x38, 0x68, 0x14, 0x7C, 0x7E, 0x4B, 0x4A, 0x43, 0x39, 0x54,
  0x49
}; // 833 bytes

#endif

#ifdef FONT_HANDWRITE
/* Фигурный "рукописный" шрифт. 5-7 пикселей в ширину 
 * "Handwrite" font ~5-7 pixel width */

const uint8_t font_handwrite[] PROGMEM = {
  0x01, 0x07, 0x17, 0xC1, 0x5A, 0x51, 0x5F, 0x51, 0x64, 0x51, 0x69, 0x51, 0x6D, 0x51, 0x72, 0x61, 0x78, 0x71, 0x7F, 0x71, 0x86, 0x71, 0x8D, 0x11, 0x8E, 0x11, 0x8F, 0x31, 0x92, 0x51, 0x97, 0x51,
  0x9C, 0x51, 0xA1, 0x51, 0xA6, 0x21, 0xA8, 0x21, 0xA9, 0x21, 0xAB, 0x51, 0xB0, 0x51, 0xB5, 0x21, 0xB7, 0x51, 0x65, 0x11, 0xBC, 0x31, 0xBF, 0x61, 0x5F, 0x31, 0xC5, 0x51, 0xCA, 0x51, 0xCF, 0x51,
  0xD4, 0x51, 0xD9, 0x51, 0xDE, 0x51, 0xE3, 0x51, 0xE8, 0x51, 0x92, 0x11, 0xED, 0x21, 0xEF, 0x31, 0xF2, 0x51, 0xF7, 0x31, 0xFA, 0x51, 0xFF, 0x51, 0x04, 0x72, 0x0B, 0x62, 0x11, 0x62, 0x17, 0x72,
  0x1E, 0x72, 0x24, 0x72, 0x2B, 0x62, 0x30, 0x72, 0x37, 0x42, 0x3B, 0x52, 0x40, 0x72, 0x47, 0x62, 0x4D, 0x72, 0x54, 0x72, 0xBF, 0x61, 0x5A, 0x72, 0x61, 0x62, 0x67, 0x72, 0x6E, 0x62, 0x74, 0x62,
  0x79, 0x72, 0x80, 0x72, 0x87, 0x72, 0x8E, 0x72, 0x95, 0x72, 0x9C, 0x62, 0xA2, 0x32, 0xA5, 0x32, 0xA8, 0x32, 0xAB, 0x52, 0xB0, 0x52, 0x62, 0x21, 0xB5, 0x62, 0xBB, 0x42, 0xBE, 0x42, 0xC2, 0x52,
  0xC7, 0x52, 0xCB, 0x42, 0xCF, 0x52, 0xD4, 0x52, 0xD9, 0x22, 0xDA, 0x42, 0xDE, 0x52, 0xE3, 0x42, 0xE7, 0x52, 0xEC, 0x52, 0xF1, 0x52, 0xF6, 0x52, 0xFB, 0x52, 0x00, 0x43, 0x04, 0x53, 0x09, 0x43,
  0x0D, 0x53, 0x12, 0x53, 0x17, 0x53, 0x1C, 0x53, 0x21, 0x53, 0x26, 0x53, 0x2B, 0x33, 0x2E, 0x13, 0x2F, 0x33, 0x31, 0x53, 0x36, 0x53, 0x04, 0x72, 0x3B, 0x73, 0x0B, 0x62, 0x42, 0x63, 0x17, 0x72,
  0x1E, 0x72, 0x48, 0x73, 0x4F, 0x63, 0x79, 0x72, 0x55, 0x73, 0x40, 0x72, 0x5C, 0x73, 0x4D, 0x72, 0x30, 0x72, 0xBF, 0x61, 0x63, 0x73, 0x5A, 0x72, 0x11, 0x62, 0x6A, 0x73, 0x95, 0x72, 0x71, 0x73,
  0x8E, 0x72, 0x78, 0x73, 0x7F, 0x73, 0x86, 0x73, 0x8D, 0x73, 0x94, 0x73, 0x9A, 0x73, 0xA1, 0x73, 0xA7, 0x63, 0xAD, 0x73, 0xB4, 0x73, 0xB5, 0x62, 0xBB, 0x53, 0xBB, 0x42, 0xC0, 0x53, 0xC5, 0x53,
  0xC7, 0x52, 0xCA, 0x53, 0xCF, 0x53, 0x0D, 0x53, 0xD4, 0x53, 0xD9, 0x43, 0xDD, 0x53, 0xE1, 0x53, 0xE5, 0x53, 0xF1, 0x52, 0xEA, 0x53, 0xEF, 0x53, 0xBE, 0x42, 0xF4, 0x53, 0x21, 0x53, 0xF9, 0x53,
  0x1C, 0x53, 0xFE, 0x53, 0x03, 0x54, 0x06, 0x54, 0x0B, 0x64, 0x11, 0x54, 0x13, 0x54, 0x18, 0x44, 0x1B, 0x54, 0x20, 0x54, 0x25, 0x54, 0x2A, 0x74, 0x31, 0x54, 0x14, 0x22, 0x7F, 0x22, 0x14, 0x04,
  0x02, 0x7F, 0x02, 0x04, 0x10, 0x20, 0x7F, 0x20, 0x10, 0x08, 0x08, 0x2A, 0x1C, 0x08, 0x1C, 0x2A, 0x08, 0x08, 0x38, 0x38, 0x20, 0x20, 0x20, 0x20, 0x08, 0x1C, 0x2A, 0x08, 0x2A, 0x1C, 0x08, 0x40,
  0x70, 0x7C, 0x7F, 0x7C, 0x70, 0x40, 0x01, 0x07, 0x1F, 0x7F, 0x1F, 0x07, 0x01, 0x00, 0x5F, 0x03, 0x00, 0x03, 0x24, 0x7E, 0x24, 0x7E, 0x24, 0x26, 0x49, 0x7F, 0x49, 0x32, 0x43, 0x33, 0x08, 0x66,
  0x61, 0x30, 0x4E, 0x49, 0x36, 0x50, 0x02, 0x01, 0x3E, 0x41, 0x3E, 0x14, 0x08, 0x3E, 0x08, 0x14, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x40, 0x20, 0x08, 0x08, 0x08, 0x08, 0x08, 0x60, 0x18, 0x06, 0x3C,
  0x42, 0x41, 0x41, 0x21, 0x1E, 0x42, 0x61, 0x51, 0x49, 0x46, 0x20, 0x42, 0x49, 0x49, 0x36, 0x18, 0x16, 0x10, 0x7F, 0x10, 0x20, 0x4F, 0x45, 0x45, 0x39, 0x30, 0x4C, 0x46, 0x45, 0x38, 0x01, 0x41,
  0x31, 0x0D, 0x03, 0x30, 0x4E, 0x49, 0x4D, 0x32, 0x0C, 0x52, 0x31, 0x19, 0x06, 0x40, 0x24, 0x08, 0x14, 0x22, 0x14, 0x14, 0x14, 0x14, 0x14, 0x22, 0x14, 0x08, 0x02, 0x01, 0x51, 0x09, 0x06, 0x1C,
  0x22, 0x49, 0x55, 0x5E, 0x48, 0x30, 0x1C, 0x12, 0x11, 0x7E, 0x10, 0x38, 0x44, 0x3F, 0x4A, 0x49, 0x36, 0x3C, 0x42, 0x41, 0x41, 0x21, 0x12, 0x04, 0x22, 0x51, 0x3F, 0x41, 0x42, 0x3C, 0x30, 0x4A,
  0x4D, 0x49, 0x49, 0x41, 0x22, 0x49, 0x3F, 0x09, 0x09, 0x01, 0x01, 0x3C, 0x42, 0x41, 0x59, 0x49, 0x32, 0x49, 0x3F, 0x08, 0x08, 0x3F, 0x46, 0x20, 0x52, 0x3F, 0x40, 0x20, 0x52, 0x51, 0x3F, 0x09,
  0x32, 0x49, 0x3E, 0x0C, 0x0A, 0x31, 0x40, 0x20, 0x52, 0x3F, 0x40, 0x40, 0x20, 0x40, 0x38, 0x06, 0x7F, 0x20, 0x3F, 0x40, 0x02, 0x61, 0x1F, 0x0C, 0x30, 0x5F, 0x22, 0x41, 0x3F, 0x01, 0x09, 0x09,
  0x06, 0x3C, 0x42, 0x41, 0x59, 0x21, 0x5E, 0x22, 0x41, 0x3F, 0x05, 0x39, 0x49, 0x26, 0x20, 0x46, 0x49, 0x49, 0x49, 0x32, 0x02, 0x01, 0x3D, 0x42, 0x22, 0x01, 0x3E, 0x40, 0x20, 0x10, 0x3F, 0x40,
  0x01, 0x1E, 0x60, 0x20, 0x18, 0x07, 0x02, 0x01, 0x3E, 0x40, 0x38, 0x40, 0x3F, 0x40, 0x42, 0x41, 0x32, 0x1C, 0x26, 0x41, 0x21, 0x02, 0x21, 0x46, 0x48, 0x48, 0x38, 0x0F, 0x22, 0x51, 0x31, 0x4E,
  0x42, 0x21, 0x7F, 0x41, 0x41, 0x06, 0x18, 0x60, 0x41, 0x41, 0x7F, 0x04, 0x02, 0x01, 0x02, 0x04, 0x40, 0x40, 0x40, 0x40, 0x40, 0x38, 0x44, 0x44, 0x38, 0x44, 0x20, 0x3E, 0x49, 0x46, 0x38, 0x44,
  0x44, 0x24, 0x30, 0x48, 0x50, 0x3F, 0x40, 0x38, 0x54, 0x54, 0x54, 0x08, 0x7E, 0x15, 0x02, 0x18, 0x64, 0xB4, 0x78, 0x04, 0x40, 0x3E, 0x11, 0x0E, 0x70, 0x3D, 0x40, 0xA0, 0x7D, 0x10, 0x40, 0x3E,
  0x19, 0x2E, 0x48, 0x20, 0x3E, 0x51, 0x4E, 0x7C, 0x08, 0x7C, 0x04, 0x78, 0x74, 0x08, 0x04, 0x04, 0x78, 0x38, 0x44, 0x44, 0x44, 0x38, 0xFC, 0x48, 0x44, 0x24, 0x58, 0x38, 0x44, 0xFC, 0xA0, 0x40,
  0x04, 0x28, 0x58, 0x4C, 0x08, 0x54, 0x54, 0x54, 0x20, 0x04, 0x3F, 0x44, 0x20, 0x3C, 0x40, 0x20, 0x3C, 0x40, 0x1C, 0x60, 0x20, 0x1C, 0x08, 0x1C, 0x60, 0x30, 0x40, 0x3C, 0x44, 0x24, 0x38, 0x48,
  0x44, 0x0C, 0x50, 0xA0, 0x7C, 0x10, 0x64, 0x54, 0x54, 0x4C, 0x24, 0x08, 0x36, 0x41, 0xFF, 0x41, 0x36, 0x08, 0x04, 0x08, 0x10, 0x08, 0x7C, 0x42, 0x41, 0x42, 0x7C, 0x32, 0x49, 0x3F, 0x09, 0x49,
  0x49, 0x30, 0x20, 0x42, 0x3D, 0x01, 0x01, 0x01, 0x41, 0x32, 0x0C, 0x7F, 0x18, 0x26, 0x41, 0x30, 0x42, 0x41, 0x49, 0x4D, 0x32, 0x01, 0x3E, 0x41, 0x22, 0x11, 0x3F, 0x40, 0x20, 0x40, 0x38, 0x06,
  0x01, 0x3F, 0x40, 0x22, 0x41, 0x3D, 0x01, 0x01, 0x3F, 0x41, 0x02, 0x01, 0x01, 0x3F, 0x42, 0x22, 0x01, 0x0E, 0x11, 0x4A, 0x3F, 0x0A, 0x11, 0x0E, 0x01, 0x3E, 0x40, 0x20, 0x50, 0xAF, 0x50, 0x02,
  0x01, 0x0E, 0x10, 0x10, 0x38, 0x47, 0x01, 0x3E, 0x40, 0x3E, 0x40, 0x3F, 0x40, 0x01, 0x3E, 0x40, 0x3E, 0x40, 0xBF, 0x50, 0x32, 0x49, 0x3F, 0x08, 0x48, 0x48, 0x30, 0x7F, 0x08, 0x48, 0x30, 0x00,
  0x7F, 0x30, 0x48, 0x3F, 0x08, 0x48, 0x48, 0x30, 0x42, 0x41, 0x49, 0x29, 0x1E, 0x72, 0x3F, 0x08, 0x3E, 0x41, 0x41, 0x3E, 0x20, 0x46, 0x29, 0x19, 0x09, 0x3A, 0x47, 0x30, 0x4A, 0x45, 0x39, 0x01,
  0x08, 0x24, 0x54, 0x48, 0x20, 0x18, 0x64, 0xA8, 0x7C, 0x10, 0x64, 0x18, 0x7C, 0x30, 0x4C, 0x20, 0x44, 0x54, 0x54, 0x28, 0x3C, 0x41, 0x21, 0x3C, 0x40, 0x7C, 0x18, 0x14, 0x64, 0x40, 0x20, 0x18,
  0x3C, 0x40, 0x38, 0x7C, 0x30, 0x7C, 0x10, 0x10, 0x7C, 0x20, 0x7C, 0x08, 0x04, 0x38, 0x40, 0xFC, 0x08, 0x24, 0x38, 0x10, 0x08, 0x04, 0x7C, 0x04, 0x04, 0x38, 0x44, 0xFC, 0x64, 0x38, 0x3C, 0x40,
  0x40, 0xBC, 0x50, 0x04, 0x08, 0x08, 0x3C, 0x40, 0x38, 0x40, 0x7C, 0x3C, 0x40, 0x38, 0x40, 0xBC, 0x50, 0x08, 0x04, 0x7C, 0x50, 0x20, 0x3C, 0x40, 0x7C, 0x50, 0x50, 0x20, 0x44, 0x54, 0x54, 0x38,
  0x7C, 0x10, 0x78, 0x44, 0x3C, 0x48, 0x34, 0x14, 0x38, 0x44, 0x30, 0x54, 0x4B, 0x4A, 0x4B, 0x42, 0x24, 0x38, 0x55, 0x54, 0x55, 0x08
}; //1078 bytes

#endif

PGM_VOID_P text_current_font = 0;

void text_set_font(uint8_t font_code) {
  switch (font_code) {
    #ifdef FONT_NORMAL
    case FONT_NORMAL: text_current_font = &font_normal; break;
    #endif
    #ifdef FONT_BOLD
    case FONT_BOLD: text_current_font = &font_bold; break;
    #endif
    #ifdef FONT_NARROW
    case FONT_NARROW: text_current_font = &font_narrow; break;
    #endif
    #ifdef FONT_HANDWRITE
    case FONT_HANDWRITE: text_current_font = &font_handwrite; break;
    #endif
  }  
}

#define _TEXT_UTF8_MAP_FROM 0xD0
inline uint8_t _map_UTF8_to_font(unsigned char first, unsigned char second) {
  if (first == 0xD0) {
    if ((second >= 0x90) && (second <= 0xBF)) {
      return second - 0x10;
    } else if (second == 0x81) { // Ё
      return 0xC0;
    }
  } else if (first == 0xD1) {
    if ((second >= 0x80) && (second <= 0x8F)) {
      return second + 0x30;
    } else if (second == 0x91) {
      return 0xC1; // ё
    }
  }
  return 0;
}

uint8_t _text_output_char(uint8_t x, uint8_t idx, PGM_VOID_P pgmFont, uint8_t fontFlags, uint8_t fontStart, uint8_t fontMax, uint8_t xorMask)  {
  if (fontFlags & FONT_FLAG_BLANK_BEFORE) {
    display_data(xorMask);
    x++;
    if (x >= DISPLAY_WIDTH)
      return x;
  }
  if ((idx >= fontStart) && (idx <= fontMax)) {
    uint16_t fw = pgm_read_word(pgmFont + 4 + (idx - fontStart) * 2);
    PGM_VOID_P p = pgmFont + (fw & 0xFFF);
    uint8_t w = (fw >> 12);
    if (w > (DISPLAY_WIDTH - x))
      w = DISPLAY_WIDTH - x;
    x += w;
    #ifndef ALL_FONTS_LSB_UP
    if (fontFlags & FONT_FLAG_MSBUP) {
      while (w--) {
        display_data(reversebits(pgm_read_byte(p++) ^ xorMask));
      }
    } else {
    #endif
      while (w--) {
        display_data(pgm_read_byte(p++) ^ xorMask);
      }
    #ifndef ALL_FONTS_LSB_UP
    }
    #endif
    if (x >= DISPLAY_WIDTH) 
      return x;
  }
  if (fontFlags & FONT_FLAG_BLANK_AFTER) {
    display_data(xorMask);
    x++;
  }
  return x;
}

void text_clear_from(uint8_t page, uint8_t x)
{
	  if ((x >= DISPLAY_WIDTH) || (page >= DISPLAY_PAGES) || !text_current_font)
	    return ;
	  display_setpos(page, x);
	  do  {
		  display_data(0);
		  x++;
	  } while (x < DISPLAY_WIDTH);

}
uint8_t text_print(uint8_t page, uint8_t x, PGM_P pgmStr, uint8_t xorMask) {
  if ((x >= DISPLAY_WIDTH) || (page >= DISPLAY_PAGES) || !text_current_font)
    return x;
  display_setpos(page, x);
  uint8_t fontFlags = pgm_read_byte(text_current_font);

  uint8_t fontStart = pgm_read_byte(text_current_font + 2);
  uint8_t fontMax = pgm_read_byte(text_current_font + 3);
  do  {
    uint8_t ch = pgm_read_byte(pgmStr++);
    if (!ch) break;
    if (ch >= _TEXT_UTF8_MAP_FROM)
      ch = _map_UTF8_to_font(ch, pgm_read_byte(pgmStr++));
    x = _text_output_char(x, ch, text_current_font, fontFlags, fontStart, fontMax, xorMask);
  } while (x < DISPLAY_WIDTH);
  return x;
}

uint8_t buff_print(uint8_t page, uint8_t x, char * Str, uint8_t xorMask) {
  if ((x >= DISPLAY_WIDTH) || (page >= DISPLAY_PAGES) || !text_current_font)
    return x;
  display_setpos(page, x);
  uint8_t fontFlags = pgm_read_byte(text_current_font);

  uint8_t fontStart = pgm_read_byte(text_current_font + 2);
  uint8_t fontMax = pgm_read_byte(text_current_font + 3);
  do  {
    uint8_t ch = *Str++;
    if (!ch) break;
    if (ch >= _TEXT_UTF8_MAP_FROM)
      ch = _map_UTF8_to_font(ch, *Str++);
    x = _text_output_char(x, ch, text_current_font, fontFlags, fontStart, fontMax, xorMask);
  } while (x < DISPLAY_WIDTH);
  return x;
}


uint8_t text_char(uint8_t page, uint8_t x, uint8_t ch, uint8_t xorMask) {
  if ((x >= DISPLAY_WIDTH) || (page >= DISPLAY_PAGES) || !text_current_font)
    return x;
  display_setpos(page, x);
  uint8_t fontFlags = pgm_read_byte(text_current_font);

  uint8_t fontStart = pgm_read_byte(text_current_font + 2);
  uint8_t fontMax = pgm_read_byte(text_current_font + 3);
  return _text_output_char(x, ch, text_current_font, fontFlags, fontStart, fontMax, xorMask);
}

uint8_t text_char_width(uint8_t ch) {
  if (!text_current_font)
    return 0;
  uint8_t fontFlags = pgm_read_byte(text_current_font);

  uint8_t fontStart = pgm_read_byte(text_current_font + 2);
  uint8_t fontMax = pgm_read_byte(text_current_font + 3);
  
  uint8_t w = 0;
  if (fontFlags & FONT_FLAG_BLANK_BEFORE) 
    w++;
  if (fontFlags & FONT_FLAG_BLANK_AFTER) 
    w++;
  if ((ch >= fontStart) && (ch <= fontMax)) 
    w += (pgm_read_byte(text_current_font + 5 + (ch - fontStart) * 2) >> 4);
  return w;
}

const PROGMEM uint16_t _text_num_scalers[] = {1, 10, 100, 1000, 10000};
  
uint8_t text_unum(uint8_t page, uint8_t x, uint16_t num, uint8_t decpoint, uint8_t xorMask) {
  if ((x >= DISPLAY_WIDTH) || (page >= DISPLAY_PAGES) || !text_current_font)
    return x;
  display_setpos(page, x);
  uint8_t fontFlags = pgm_read_byte(text_current_font);

  uint8_t fontStart = pgm_read_byte(text_current_font + 2);
  uint8_t fontMax = pgm_read_byte(text_current_font + 3);
  uint8_t outme = 0;
  for (int8_t d = 4; d >= 0; d--) {
    uint8_t r = 0;
    uint16_t scaler = pgm_read_word(&_text_num_scalers[d]);
    while (num >= scaler) {
      r++;
      num -= scaler;
    }
    if ((r > 0) || (decpoint >= d)) 
      outme = 1;
    if (outme) 
      x = _text_output_char(x, r + 48, text_current_font, fontFlags, fontStart, fontMax, xorMask);
    if (decpoint && (decpoint == d))
      x = _text_output_char(x, '.', text_current_font, fontFlags, fontStart, fontMax, xorMask);
  }
  return x;
}

uint8_t text_num(uint8_t page, uint8_t x, int16_t num, uint8_t decpoint, uint8_t xorMask) {
  if (num < 0) {
    x = text_char(page, x, '-', xorMask);
    num = -num;
  }  
  return text_unum(page, x, num, decpoint, xorMask);
}
