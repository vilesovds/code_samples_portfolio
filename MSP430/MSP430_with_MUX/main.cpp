/* ==================================================================
file main.cpp 
=====================================================================*/
#include <msp430x20x3.h>
/*********************************************************************
*               CONSTANTS and DEFINES
**********************************************************************/
/*�������� ��� ������� (1MHz)*/
#define BAUD_9600 104
#define BAUD_4800 208
#define BAUD_2400 417
#define BAUD_1200 833
#define BAUD_600 1667

// �������� ��� ������� (16 MHz)
//#define BAUD_2400 6667
//#define BAUD_4800 3333
//#define BAUD_9600 1667
//#define BAUD_19200 833
//#define BAUD_57600 278

/*������ ������ UART*/
#define UART_MODE_NONE 0
#define UART_MODE_STOP 0
#define UART_MODE_RX 1
#define UART_MODE_RX_END 2
#define UART_MODE_TX 3

/* ������������ �����������*/
#define TXD_PIN BIT6
#define RXD_PIN BIT7
#define UART_SPEED BAUD_2400

/*������ ������ �������*/
#define WDR  WDTCTL = WDTPW+WDTCNTCL

/*������������ ���������� �������*/
#define MAX_CHANNELS 8
/***************************************************************
*					GLOBAL DATA
****************************************************************/
int ADC_offset = 0, ADC_offset0; 	// ������ ���� ���
unsigned int adc_value = 0; 		// ��������� ���������� �������� ���
char uartmode = 0;       			// ����� ������ UART
char rxbyte = 0;         			// �������� ����
unsigned int txdata = 0; 			// ��������� ������
char bitnum = 0;         			// ������� RX/TX bit
char ADC_Setup_Complete = 0; 		// ���� �������� ��������� ���    
char adc_lock = 0;           		// ���������� ���������� ���  
char count_of_adc = 0; 				// ���������� ������� MUX
char current_adc = 0; 				// ����� �������� ������
char count_of_int = 0;				// ���-�� ����������
char flag_zamer = 0;					// ���� ��������� ��������� �� ����������

/*������ �������� ���*/
unsigned int adc_buf[16]={	adc_value, adc_value, adc_value, adc_value,
							adc_value, adc_value, adc_value, adc_value,
							adc_value, adc_value, adc_value, adc_value,
							adc_value, adc_value, adc_value, adc_value
						}; 
/*��������� ������� �������������*/
char last_init_cmd = 0x00;

/**************************************************************
*     				PROTOTYPES
***************************************************************/
void set_mux(void);

/**************************************************************
*					FUNCTIONS
***************************************************************/

/*********************************
 ����� p ���� (��� 1���)
*********************************/
void 
pause(unsigned int p){  
  unsigned int i;
  while (p>0)  {WDR; i = 300; while(i>0) i--; p--; }  
}

/************************************************
 ������� ���������� ���������� ����� = 1 � �����
*************************************************/
char 
calcBits(char data)
{
	char testbit = 1, result = 0;
	while (testbit){  
		WDR;
		if (data & testbit) result++;	// ������ ������� �� ������� ��� � ��������� �����
		testbit = testbit << 1;	/* ��� ��� testbit ��������� ����� ������, 
								�� ����� ������ �� ���� ����� �� ��������� ��������� */
	}
	return result;  
}
      
/*********************************
 ����� �� period ������ �������
**********************************/
void 
timer_pause(unsigned int period)
{  
  // ��������� ������
  CCR0 = 0xFFFF;           // mks
  if (period > 20) period -= 20;
  TACTL = TASSEL_2 + MC_1; // SMCLK, Up to CCR0 (������ ������� �� CCR0 � �������� ����������)
  // ��������� ������
  CCTL0 = 0;           	   // CCR0 interrupt disabled      
  TAR = 0;             	   // �������� ������� �������!
  while (TAR < period) WDR;// ���� ���� ������� ������� ���������
  TAR = 0;                 // �������� ������� �������!
}

/*************************
 ������ 1 ���� � UART
*************************/
void 
uart_write_bit(void)
{     
	/* ������� �� ��������� ���*/
	if (txdata & (1 << bitnum)) P1OUT |= TXD_PIN;  	// 1 �� ����
	else P1OUT &= ~TXD_PIN; 						// 0 �� ����
	bitnum++;										// ����������� ������� �����
	/* ���� ��� ��������, ��������� ����� ��������*/
	if ( bitnum > 10 ){
		/* ��������� ������*/ 
		CCTL0 = 0;          // CCR0 interrupt disabled      
		uartmode = 0;		//  c��������� ����� ������ UART
	}
}

/******************************
* ������� �������� 1 �����
******************************/
void 
TX_Byte (char data)
{
	bitnum = 0;       			// ��������� �� ������ ������������ ���
	txdata = (0xFF00 + (unsigned int)data) << 1; // 1 stop bit (1) + 8 bit data + 1 start bit (0)
	uartmode = UART_MODE_TX;  	// ��������� � ����� ��������
	/* ��������� ������ ��� �������� ����� */
	CCR0 = UART_SPEED;         	// bit/s
	TACTL = TASSEL_2 + MC_1;   	// SMCLK, Up to CCR0 (������ ������� �� CCR0 � �������� ����������)
	CCTL0 = CCIE;              	// CCR0 interrupt enabled  
	while(uartmode){
		WDR;
		SD16CCTL0 &= ~(SD16IE);
	};  // ���� ����� �����
	SD16CCTL0 |= (SD16IE);
}

/***************************
*	������ 1 ��� �� UART
****************************/
void 
uart_read_bit(void)
{
	/* ������� �������� ��� */
	char rx_bit = (P1IN & RXD_PIN);    
	if (bitnum < 8) // ��� ��� ������
	if (rx_bit) rxbyte |= (1 << bitnum); // ��������� ���

	// stop-bit (10) ������ ���� == 1
	if (bitnum == 9){
		if (rx_bit) uartmode = UART_MODE_RX_END;
		else{
			uartmode = 0;      // ���� �������, ��������� �������
			P1IE |= RXD_PIN;   // ���� ���������� ����
		}  
		/* ��������� ������*/
		CCTL0 = 0;           // CCR0 interrupt disabled      
	}          
	bitnum++;				// ����������� ���-�� �������� ������
}


/*******************************************
* Port 1 interrupt service routine
* ���������� ��� ������� ���������� ����
********************************************/
#pragma vector=PORT1_VECTOR
__interrupt void 
Port_1(void)
{  
	SD16CCTL0 &= ~(SD16IE);
	// ��� ��������� ���, �������� �����
	bitnum = 0;
	rxbyte = 0;
	P1IE = 0;          	// ��������� ���������� �� �������� ������
	P1IFG &= ~RXD_PIN; 	// ����� ����� ����������
  
	// ������ ����� � ����������,  ����� ������ ������� � �������� ��������
	//timer_pause(UART_SPEED / 2);
	//timer_pause(UART_SPEED / 3 ); // �������� �� ������
	timer_pause((UART_SPEED >> 2) - 30); // �������� �� ����� ���������� ������������� ������
  	/* �������� ����� ����� */
	uartmode = UART_MODE_RX;
	/* ��������� ������ ��� ������ ����� */
	TAR = 0; 					// reset counter
	CCR0 = UART_SPEED;			// bit/s
	CCTL0 = CCIE;				// CCR0 interrupt enabled  
}


/****************************
* ������� ������ 1 �����
****************************/
void 
RX_byte(void)
{
  /*������� ������� ���������� ���� ��� ������ ������ ��������� �����*/
	P1IFG &= ~RXD_PIN; // P1.4 IFG cleared
	P1IE |= RXD_PIN;   // P1.4 interrupt enabled
	/*���� ��������� ������ �����*/
	while(uartmode != UART_MODE_RX_END){ WDR;}; 
	uartmode = 0;
}


/*****************************************
* ���������� Timer A0 �� ���������� CCR0
******************************************/
#pragma vector=TIMERA0_VECTOR
__interrupt void 
Timer_A(void)
{  
	/*������� ����� UART�*/
	switch (uartmode){
		case UART_MODE_RX: uart_read_bit(); break;  // ��������� ��������� �������� ���
		case UART_MODE_TX: uart_write_bit(); break; // �������� ��������� ���
	}
}

/*********************************
 ��������� ���
 cmd - �������� �������������
**********************************/
void 
ADC_Setup(char cmd)
{
    ADC_offset =  ADC_offset0;				// �������� ����
    SD16INCTL0 = SD16INCH_0; 				// ����� A0  
    SD16AE = SD16AE0 + SD16AE1;     		// P1.0 & P1.1 analog inputs ON  
    SD16CCTL0 = SD16IE + SD16XOSR + BIT8;  	// 1024 OSR,interrupt enable  
    SD16CCTL0 |= SD16SC;                   	// Set bit to start conversion  
    count_of_adc=(cmd&0x0F);				// ���������� ���-�� ��������� ������������
    //current_adc = count_of_adc;
    ADC_Setup_Complete = 1;					// ������ ����, ��� ��� ���������
}

/*************************
 ������� ���� ���
**************************/
void 
calcOffset(void)
{
   /* ������� ��� �������� */
   SD16INCTL0 = SD16INCH_7;             // ��������, ��� ��������
   SD16AE = 0; 
   SD16CCTL0 |= SD16SC;                 // Set bit to start conversion  
   
   pause(500); 							// ���� ~500 ��
   SD16CCTL0 &= ~SD16SC; 				// stop ADC
   
   ADC_offset0 = adc_value - 0x8000;	// �������� ����
 
}

/**********************************
 ������������ �������� �������
***********************************/
void 
process_cmd(void) 
{
	unsigned char dat=0x00;		//��������� ����������
	/* 0x60 - ������ �������� ��� �� ������� */
	if ((rxbyte&0xF0) == 0x60){
		// ��������� ��� ���� ���������?
		if (!(ADC_Setup_Complete)) return;
		/*�������� �� ����������*/
		dat=(rxbyte&0x0F);
		if (dat>count_of_adc)  return;
        adc_lock = 1; 		// ��������� ��������� �������� ���        
		unsigned int tmp = adc_buf[dat];	//adc_value;    
		adc_lock = 0; // ��������� ��������� �������� ���     
       	// �������� ������������ �������� ��� �� �������       
		TX_Byte(char(tmp >> 8)); // HIGH  
		TX_Byte(char(tmp));      // LOW 
    	/* ����������� ����� */
		TX_Byte( (~ char(tmp >> 8)) ^ char(tmp));
		/*��� ��������� ������� �������������*/
		TX_Byte(last_init_cmd);
  	}  
	else if ((rxbyte & 0xF0) == 0x90) {// ��������� ���
		/*�������� �� ����������*/
		dat=(rxbyte & 0x0F);
		if (dat<=MAX_CHANNELS){
			TX_Byte(~rxbyte);  		// ���������� ���� �������� ���� (� ���������)
			ADC_Setup(dat); 		// ����������� ���
			last_init_cmd=rxbyte;	// ���������� ������� �������������
		}else return;
	}    
   
}

/*****************************************************
*	 ���������� �� ��� 16 ���
******************************************************/
#pragma vector = SD16_VECTOR
__interrupt void 
SD16ISR(void)
{ 
	unsigned int tmp;						// ��������� ����������
	tmp = SD16MEM0;  						// ����������� ���� ������ ��� � ���������� ����� ������!!
	if (adc_lock==0){						// �� ��������� ��������� �������� ���
		if ((tmp > 0) && (tmp < 0xffff)) 	// �������� �� ������������
			adc_value = tmp - ADC_offset;  	// ��������� �������� ������
		else adc_value = tmp;
		/*���������� � ������*/
		adc_buf[current_adc]=adc_value;
		/* ���� ���� ����������� ��������� ���,
		�� ����������� �������������*/
		if (ADC_Setup_Complete) set_mux();	// ����������� ������������� 
	}
}
/***********************************************
*    ������������� MUX
***********************************************/
void 
set_mux(void)
{ 
    SD16CCTL0 &= ~SD16SC; 	// stop ADC
	/*����������� ������ ���*/
	current_adc++;
    if (current_adc>=count_of_adc) current_adc=0;
    /*����� � ����(� ���������)*/
    switch(current_adc){
		case 0:
			P1OUT |= ((1<<2)|(1<<4)); 
		break;
		case 1:
			P1OUT &= ~(1<<2); P1OUT |= (1<<4); 
		break;
		case 2:
			P1OUT |= (1<<2); P1OUT &= ~(1<<4); 
		break;      
    }
	pause(1);				// ����� ��� ��������������
    SD16CCTL0 |= SD16SC;	// start ADC
}
  


/*********************************************************
		MAIN
*********************************************************/
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;   // Stop WDT
    /* ������������� ������� ���������� 1MHz (����������) */
	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;

	/* timer mode */
	TACTL = TASSEL_2 + MC_1;   // SMCLK, Up to CCR0 (������ ������� �� CCR0 � �������� ����������)

	/* i/o setup*/
	P2DIR =0x40; 
	P2SEL = 0;
	P2OUT = 0;
	P1SEL |= (1<<3); // Vref on
	P1DIR = (TXD_PIN|0x3C/*|(1<<3)*/);  //  P1.6 - TXD, P1.2 - P1.5 - output to operate MUX
	P1IES |= RXD_PIN;  					// P1.7 Hi/lo edge
	P1OUT = TXD_PIN; 					// TX high
	/* AntiBUG "SDA2" */
	(*(volatile unsigned char *)0xBF) = 0x61;  
  
	/* ��������������� ��������� ���    */
	//SD16CTL =SD16VMIDON+SD16REFON+ SD16SSEL_1 + SD16XDIV_2 + SD16DIV_2; // 1.2V ref, buffer, SMCLK,  /16 /4 

	SD16CTL = SD16VMIDON + SD16REFON + SD16SSEL_1 + SD16XDIV_2/* + SD16DIV_3*/; // 1.2V ref, buffer, SMCLK,  /16 /1
	SD16CCTL0 = SD16IE + SD16XOSR+ BIT8;  // 1024 OSR, bipolar, interrupt enable  

  	_BIS_SR(GIE);       // interrupt enable  
	calcOffset();       // ���������� ����  
	WDTCTL=WDT_MRST_32; // ������ �� 32 ��
	WDR;				// ���������� ������
	/*main cycle*/
	while(1){
		RX_byte();		// ��������� ���� �� UART�
		WDR;			
		process_cmd();  // ������������ �������� �������
		WDR;			
	}
}


