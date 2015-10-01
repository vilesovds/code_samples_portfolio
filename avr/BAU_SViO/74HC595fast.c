// ====================================================================================
// Драйвер сдвигового регистра 74HC595
// оптимизирован по скорости дял платы вадима
// ====================================================================================

#include "74HC595fast.h"

// ====================================================================================
// FUNCTIONS
// ====================================================================================

//char led_screen1[12];


// -----------------------------
// Обнуление сдвигового регистра
// -----------------------------
void Shift_reg_reset(void)
{
  // ставим низкие уровни
  SHREG_OE_SET_LOW;
  SHREG_MR_SET_LOW;

  // поднимаем store
  SHREG_STCP_SET_LOW;
  NOP(); // пауза
  SHREG_STCP_SET_HIGH;  
}


// --------------------------------------------------
// Вывод данных из сдвигового регистра в параллельный
// --------------------------------------------------
void Shift_reg_store(void)
{
  // ставим уровни
  SHREG_OE_SET_LOW;
  SHREG_MR_SET_HIGH;
  
  SHREG_STCP_SET_HIGH;  
  NOP(); // пауза
  SHREG_STCP_SET_LOW;
}

// ---------------------------------------
// Запись данных в сдвиговый регистр
// data - указатель на массив
// len - длина в байтах
// ---------------------------------------
void Shift_reg_write_data(char * data, int len)
{

  signed char b, led;
  int i;

  // ставим уровни
  SHREG_STCP_SET_LOW;
  SHREG_OE_SET_LOW;
  SHREG_MR_SET_HIGH;
 
  for (i=(len-1); i >= 0; i--) // читаем массив задом наперед
  {
    for (b = 7; b >= 0; b--) // записываем очередной байт по битам последовательно 
	{		   
      SHREG_SHCP_SET_HIGH; // CLK high
	    
	  if ( BIT( b ) & data[i] ) SHREG_DS_SET_HIGH;  // high
	  else SHREG_DS_SET_LOW;  // low

	  // проталкиваем бит
      SHREG_SHCP_SET_LOW; // CLK low	  	
     } 	
  }

  // дополнительный бит ?  
  SHREG_SHCP_SET_HIGH; // CLK high
  NOP();
  SHREG_SHCP_SET_LOW;  // CLK low
}


// ---------------------------------------
// Запись 1 байта в сдвиговый регистр
// ---------------------------------------
void Shift_reg_write_byte(char data)
{
  signed char b;  
  for (b = 7; b >= 0; b--) // записываем очередной байт по битам последовательно 
  {		   
    SHREG_SHCP_SET_HIGH; // CLK high
	    
    if ( BIT( b ) & data ) SHREG_DS_SET_HIGH;  // high
	else SHREG_DS_SET_LOW;  // low

	  // проталкиваем бит
    SHREG_SHCP_SET_LOW;  // CLK low
  }
}

// ---------------------------------------
// Подготовка сдвиг регистра к записи
// ---------------------------------------
void Shift_reg_write_start(void)
{
  // ставим уровни
  SHREG_STCP_SET_LOW; // LOAD low
  SHREG_OE_SET_LOW;   // OE low
  SHREG_MR_SET_HIGH;  // RESET high
}

// ---------------------------------------
// Окончание записи? )
// ---------------------------------------
void Shift_reg_write_end(void)
{
  // дополнительный бит ?  
  SHREG_SHCP_SET_HIGH; // CLK high
  NOP();
  SHREG_SHCP_SET_LOW;  // CLK low
}
