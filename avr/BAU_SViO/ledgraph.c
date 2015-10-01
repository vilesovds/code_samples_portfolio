// ====================================================================================
// ������ ����������� ����������� ������ ����� ��������� ������� + ���������� 5�7
// ====================================================================================

#include "ledgraph.h"

#ifdef SHREG_FAST  
 #include "74HC595fast.h"
#else	
 #include "74HC595.h"
#endif


// ====================================================================================
// C O N S T A N T S
// ====================================================================================

// ���� �������� �������� �� ����-������� (LEDGRAPH_COLS - ����� �������� � �������)

// 10 led ������ �� 5 �������� 
//static char pic1[10][5] = {
//   0x00, 0x00, 0x00, 0x00, 0x00,    // LED 1
//   0x00, 0x00, 0x00, 0x00, 0x00,    // LED 2

#include "smallfont.c"


// ====================================================================================
// D A T A
// ====================================================================================

char LEDGRAPH_Screen[ LEDGRAPH_MAX_LED_NUM ][ LEDGRAPH_MAX_LED_COL_NUM ]; // ����� ������
char LEDGRAPH_xx = 0, LEDGRAPH_yy = 0;         // ������� "������" (� led ������)
char LEDGRAPH_NCOL = 1;		  				  // ������� ������� // ����� ����

#ifndef SHREG_FAST  
  TSHREG_PARAMS * pSHREG_PARAMS;
#endif

// ====================================================================================
// P R O C E D U R E S
// ====================================================================================

// -------------------------------------------
// �������������� ����� ������ x * y ��������� LED (�� �����?)
// ���� ������ ����������, ���������� 1 ����� 0
// -------------------------------------------
#ifdef SHREG_FAST  
 char LEDGRAPH_Init(char xx, char yy)
#else	
 char LEDGRAPH_Init(TSHREG_PARAMS * shpar, char xx, char yy)
#endif
{
  unsigned int x;
  char col;
  
#ifdef SHREG_FAST  
  Shift_reg_reset(); // ������� LED
#else	
  pSHREG_PARAMS = shpar; // ��������� ������ �� ��������� ���� ��������
  Shift_reg_reset(pSHREG_PARAMS); // ������� LED	
#endif

  if ( xx * yy > LEDGRAPH_MAX_LED_NUM ) return 0;
  
  LEDGRAPH_xx = xx;
  LEDGRAPH_yy = yy;
  
  for (x=0; x < LEDGRAPH_MAX_LED_NUM; x++)
   for (col=0; col < LEDGRAPH_MAX_LED_COL_NUM; col++) LEDGRAPH_Screen[ x ][col] = 0; // �������� ����������
  
  return 1;
}


// ------------------------------------------------------------
// ����� � ����������� ����������� ������������� ������� C � ������� X, Y (��������� �� 0)
// RETURN: 0 - ������, 1 - ������
// ------------------------------------------------------------
char LEDGRAPH_Print_char(char x, char y, char c)
{
  char col;
  
  if (x >= LEDGRAPH_xx) return 0;
  if (y >= LEDGRAPH_yy) return 0;
  
  for (col=0; col<5; col++) LEDGRAPH_Screen[ y * LEDGRAPH_xx + x ][col] = letters[ c ][ col ];
  
  return 1;
}

// ------------------------------------------------------------
// ����� � ����������� ����������� ������������� ������� C � ������� X, Y (��������� �� 0)
// �� �������� (����������� � ������ ������� ��������)
// RETURN: 0 - ������, 1 - ������
// ------------------------------------------------------------
char LEDGRAPH_Print_char_scroll(char x, char y, char c, signed char firstcol)
{
  char col;
  
  if (x >= LEDGRAPH_xx) return 0;
  if (y >= LEDGRAPH_yy) return 0;
    
  for (col=0; col<5; col++) 
    if (col >= firstcol) LEDGRAPH_Screen[ y * LEDGRAPH_xx + x ][col] = letters[ c ][ col-firstcol ];
  
  return 1;
}


// -----------------------------------------------------------
// �������� � ����������� �������� �� �������
// -----------------------------------------------------------
void LEDGRAPH_Put_image(char * img)
{  
  unsigned int len = LEDGRAPH_xx * LEDGRAPH_yy, col ,led;
  for (led = 0; led < len; led++) 
   for (col=0; col<5; col++) LEDGRAPH_Screen[ led ][ col ] = img[led * LEDGRAPH_MAX_LED_COL_NUM + col];
}


// -----------------------------------------------------------
// ����� � ����������� AsciiZ ������ �������� s � ������� X, Y
// RETURN: ���������� ������� ������������ ��������
// -----------------------------------------------------------
char LEDGRAPH_Print_str(char x, char y, char * s)
{
  unsigned int i = 0, p = y * LEDGRAPH_xx + x, pmax = (int)LEDGRAPH_xx * (int)LEDGRAPH_yy;
  char col;
  
  while ((p < pmax) && (s[ i ] != '\0'))
  {
    for (col=0; col<5; col++) LEDGRAPH_Screen[ p ][col] = letters[ s[i] ][ col ];
    p++;
	i++;	
  }
  return i;
}

// -----------------------------------------------------------
// ����� � ����������� AsciiZ ������ �������� s � ������� X, Y
// RETURN: ���������� ������� ������������ ��������
// -----------------------------------------------------------
char LEDGRAPH_Print_str_scroll(char x, char y, char * s, signed char firstcol)
{
  unsigned int i = 0;
  unsigned int pmax = (int)LEDGRAPH_xx * (int)LEDGRAPH_yy;
  int p = y * LEDGRAPH_xx + x; // ������� � ������� �������� (����� LED)
  char col;
  
  while ((p < pmax) && (s[ i ] != '\0'))
  {    
    for (col=0; col<5; col++) LEDGRAPH_Screen[ p ][col + firstcol ] = letters[ s[i] ][ col ];
    p++;
    i++; // ���������� ���� ������, ��������� � ����������
  }
  return i;
}


// -----------------------------------------------------------
// ����� � ����������� Ascii ������ �������� s � ������� X, Y ������ len
// RETURN: ���������� ������� ������������ ��������
// -----------------------------------------------------------
char LEDGRAPH_Print_str_len(char x, char y, char * s, char len)
{
  unsigned int i = 0, p = y * LEDGRAPH_xx + x, pmax = (int)LEDGRAPH_xx * (int)LEDGRAPH_yy;
  char col;
  
  while ((p < pmax) && (i < len))
  {
    for (col=0; col<5; col++) LEDGRAPH_Screen[ p ][col] = letters[ s[i] ][ col ];
    p++;
	i++;	
  }
  return i;
}



// --------------------------------------------------------------
// ��������� ����� (���������� �������� ������������ �� �������!)
// --------------------------------------------------------------
void LEDGRAPH_Refresh(void)
{
  signed char led;//, len = LEDTXT_xx * LEDTXT_yy;
  
//static char letters[256][5] = {
//   0x00, 0x00, 0x00, 0x00, 0x00,    // 0 (?)
//   0x00, 0x00, 0x00, 0x00, 0x00,    // 1 (?)

	// ����� �������� ������� LED
	if (LEDGRAPH_NCOL > LEDGRAPH_MAX_LED_COL_NUM) LEDGRAPH_NCOL = 1;
	
#ifdef SHREG_FAST  
    // ���������� ����� �������� � ������
    Shift_reg_write_start();

	// ����� ��������������� ��������� ������� ������������ ������
	//Shift_reg_write_data(pSHREG_PARAMS, &LEDGRAPH_Screen[ (LEDGRAPH_NCOL-1) * 10 ], 10);

    for (led = LEDGRAPH_MAX_LED_NUM - 1; led >= 0; led--) 
	  Shift_reg_write_byte(0xFF ^ LEDGRAPH_Screen[led][LEDGRAPH_NCOL-1] );

	// ���������� ���� ������ ������� ��� LED (����� ������� LED �������������� �����)
	Shift_reg_write_byte(0xFF - BIT( LEDGRAPH_NCOL-1 ));	

    // ��������� ������? )
    Shift_reg_write_end();
	
	// ����������
    Shift_reg_store();	 
#else	
    // ���������� ����� �������� � ������
    Shift_reg_write_start(pSHREG_PARAMS);

	// ����� ��������������� ��������� ������� ������������ ������
	//Shift_reg_write_data(pSHREG_PARAMS, &LEDGRAPH_Screen[ (LEDGRAPH_NCOL-1) * 10 ], 10);

    for (led = LEDGRAPH_MAX_LED_NUM - 1; led >= 0; led--) 
	  Shift_reg_write_byte(pSHREG_PARAMS, 0xFF ^ LEDGRAPH_Screen[led][LEDGRAPH_NCOL-1] );

	// ���������� ���� ������ ������� ��� LED (����� ������� LED �������������� �����)
	Shift_reg_write_byte(pSHREG_PARAMS, 0xFF - BIT( LEDGRAPH_NCOL-1 ));	

    // ��������� ������? )
    Shift_reg_write_end(pSHREG_PARAMS);
	
	// ����������
    Shift_reg_store(pSHREG_PARAMS);	 
#endif

	LEDGRAPH_NCOL++;	
}



// --------------------------------------------------------------
// ������ ������ �����
// --------------------------------------------------------------
void LEDGRAPH_CLRSCR(void)
{
  signed char led;//, len = LEDTXT_xx * LEDTXT_yy;
	
#ifdef SHREG_FAST  
    // ���������� ����� �������� � ������
    Shift_reg_write_start();

	// ���������� ���� ������ ������� ��� LED (����� ������� LED �������������� �����)
	Shift_reg_write_byte(0xFF);	

    // ��������� ������? )
    Shift_reg_write_end();
	
	// ����������
    Shift_reg_store();	 
#else	
    // ���������� ����� �������� � ������
    Shift_reg_write_start(pSHREG_PARAMS);

	// ���������� ���� ������ ������� ��� LED (����� ������� LED �������������� �����)
	Shift_reg_write_byte(pSHREG_PARAMS, 0xFF);	

    // ��������� ������? )
    Shift_reg_write_end(pSHREG_PARAMS);
	
	// ����������
    Shift_reg_store(pSHREG_PARAMS);	 
#endif

}

