// ====================================================================================
// ������� ���������� �������� 74HC595
// ������������� �� �������� ��� ����� ������
// ====================================================================================

#include "74HC595fast.h"

// ====================================================================================
// FUNCTIONS
// ====================================================================================

//char led_screen1[12];


// -----------------------------
// ��������� ���������� ��������
// -----------------------------
void Shift_reg_reset(void)
{
  // ������ ������ ������
  SHREG_OE_SET_LOW;
  SHREG_MR_SET_LOW;

  // ��������� store
  SHREG_STCP_SET_LOW;
  NOP(); // �����
  SHREG_STCP_SET_HIGH;  
}


// --------------------------------------------------
// ����� ������ �� ���������� �������� � ������������
// --------------------------------------------------
void Shift_reg_store(void)
{
  // ������ ������
  SHREG_OE_SET_LOW;
  SHREG_MR_SET_HIGH;
  
  SHREG_STCP_SET_HIGH;  
  NOP(); // �����
  SHREG_STCP_SET_LOW;
}

// ---------------------------------------
// ������ ������ � ��������� �������
// data - ��������� �� ������
// len - ����� � ������
// ---------------------------------------
void Shift_reg_write_data(char * data, int len)
{

  signed char b, led;
  int i;

  // ������ ������
  SHREG_STCP_SET_LOW;
  SHREG_OE_SET_LOW;
  SHREG_MR_SET_HIGH;
 
  for (i=(len-1); i >= 0; i--) // ������ ������ ����� �������
  {
    for (b = 7; b >= 0; b--) // ���������� ��������� ���� �� ����� ��������������� 
	{		   
      SHREG_SHCP_SET_HIGH; // CLK high
	    
	  if ( BIT( b ) & data[i] ) SHREG_DS_SET_HIGH;  // high
	  else SHREG_DS_SET_LOW;  // low

	  // ������������ ���
      SHREG_SHCP_SET_LOW; // CLK low	  	
     } 	
  }

  // �������������� ��� ?  
  SHREG_SHCP_SET_HIGH; // CLK high
  NOP();
  SHREG_SHCP_SET_LOW;  // CLK low
}


// ---------------------------------------
// ������ 1 ����� � ��������� �������
// ---------------------------------------
void Shift_reg_write_byte(char data)
{
  signed char b;  
  for (b = 7; b >= 0; b--) // ���������� ��������� ���� �� ����� ��������������� 
  {		   
    SHREG_SHCP_SET_HIGH; // CLK high
	    
    if ( BIT( b ) & data ) SHREG_DS_SET_HIGH;  // high
	else SHREG_DS_SET_LOW;  // low

	  // ������������ ���
    SHREG_SHCP_SET_LOW;  // CLK low
  }
}

// ---------------------------------------
// ���������� ����� �������� � ������
// ---------------------------------------
void Shift_reg_write_start(void)
{
  // ������ ������
  SHREG_STCP_SET_LOW; // LOAD low
  SHREG_OE_SET_LOW;   // OE low
  SHREG_MR_SET_HIGH;  // RESET high
}

// ---------------------------------------
// ��������� ������? )
// ---------------------------------------
void Shift_reg_write_end(void)
{
  // �������������� ��� ?  
  SHREG_SHCP_SET_HIGH; // CLK high
  NOP();
  SHREG_SHCP_SET_LOW;  // CLK low
}
