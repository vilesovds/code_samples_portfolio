#include "main.h"
//************************  ������ ********************************
//*								   								   													*
//*******************************************************************
char debounce[N_SIGNAL_BUF_COUNT]; // ��������� ������ ����� ��������������� ���������
char bufval[N_SIGNAL_BUF_COUNT];   // �������� ������������ 


USER_CAN_MESSAGE can_msg_out,can_msg_in;
unsigned int timestamp1=0, timestamp2=0, hbit =0;
char err_flag = 0;
int timeout = 0, kolvo = 0;



//*********************** PROTOTYPES *****************************
void checkbuf(char sig_num, char sig_val, char debounce_time);
void check_pins(void);
void Process_data(void);
void packages(void);
void port_init(void);
void watchdog_init(void);

//***********************  BEGIN**********************************
// -------------------------------------------
// ������ ������������
// debounce_time - ����� �������� ������������  (��)
// -------------------------------------------
void checkbuf(char sig_num, char sig_val, char debounce_time)
{
  char tmpbufval = bufval[sig_num]; 
  
  if (sig_val)
  {
    tmpbufval++;
  }
  else
  {
    if (tmpbufval > 0) tmpbufval--;
  }

  if (tmpbufval == 0) debounce[sig_num] = 0;
  if (tmpbufval >= debounce_time)
  {
    debounce[sig_num] = 1;
	tmpbufval = debounce_time;
  }
  
  bufval[sig_num] = tmpbufval; 
}

// -----------------------------------
// ����� � ����������� ��� ���� ������ 
// ���������� �� ������� (1 ms)
// -----------------------------------
void check_pins(void)
{
  checkbuf(N_SIGNAL_DFK1,SIGNAL_DFK1,50);
  checkbuf(N_SIGNAL_DFK2,SIGNAL_DFK2,50);
  checkbuf(N_SIGNAL_DFK3,SIGNAL_DFK3,50);
  checkbuf(N_SIGNAL_DFK4,SIGNAL_DFK4,50);
  checkbuf(N_SIGNAL_DFK5,SIGNAL_DFK5,50);
  checkbuf(N_SIGNAL_UZD1OUT,SIGNAL_UZD1OUT,50);
  checkbuf(N_SIGNAL_UZD2OUT,SIGNAL_UZD2OUT,50);
  checkbuf(N_SIGNAL_B1OUT,SIGNAL_B1OUT,50);
  checkbuf(N_SIGNAL_B2OUT,SIGNAL_B2OUT,50);
  checkbuf(N_SIGNAL_IKOUT,SIGNAL_IKOUT,50);
}
//*******************************************************************
// ----------------------------------------------------------------------------------------------
// ������������� ������
// ----------------------------------------------------------------------------------------------
void port_init(void)
{
PORTA = 0x00;
 DDRA  = 0x00;

 PORTB = 0x00;
 DDRB  = 0x08;

 PORTC = 0x0F; 
 DDRC  = 0x0F;
 
 PORTD = 0x0A;
 DDRD  = 0xCA;
 
 
 PORTE = 0x00;
 DDRE  = 0x01;

}

//*******************************************************************
//Watchdog initialize
// prescale: 16K 
void watchdog_init(void)
{
 WDR(); //this prevents a timout on enabling
 WDTCR= 0x08; //WATCHDOG ENABLED - dont forget to issue WDRs
}
//----------------------------------------------------------------------------------------------
void Timer0_user_int(void)
{
check_pins();
}

// ----------------------------------------------------------------------------------------------
//  M A I N
// ----------------------------------------------------------------------------------------------
void main(void)
{   
    
   port_init(); 
    timer0_init();
   timer0_Delay(1000);
   SPI_masterInit(); 
   LED_RUN_ON;
   LED_ERROR_ON;
  
   CAN_Init(CAN_PARAMS,  CAN_BAUD_500);  
   CAN_setAcceptFilter(CAN_PARAMS, 2, 0x200);
   LED_ERROR_OFF;
   timer0_Delay(1000);
   
  can_msg_out.ID = 0x100;


  can_msg_out.DLEN = 2;

  //������� ����
  while(1)
  { 
  //-------------------------------------
  // �������� �����
  packages();
  
  //------------------------------------------------------
   //�������� �����
   if ( (timer0_getCounter() - hbit) > 200) { CAN_Msg_Send(CAN_PARAMS,&can_msg_out); hbit = timer0_getCounter(); LED_RUN_SWITCH; }
   
   //�������� ��� ���������
   if (CAN_msg_Read(CAN_PARAMS, &can_msg_in)) 
    { 
	 //�������� ���� ������
	 if (err_flag) {LED_ERROR_OFF; err_flag = 0;}
	 //������������
	 Process_data(); 
	 //����������, ����� ��������� ��� ��������	  
	 timestamp1 =  timer0_getCounter();
	}
 
   //���� �� ��������, ��������� ���� ���
   if ( (timer0_getCounter() - timestamp1) > MAX_timeout )
   {
       err_flag = 1;
	
	   //�������, ����� �� error
	   //���� �� ����� - �������� � ���������� ����� ��������
		
		if ( (timer0_getCounter() - timestamp2) > timeout) 
		  if ( (PORTD & BIT(6)) )
		   {
		    LED_ERROR_ON; 
			B1LED_ON;
			B2LED_ON;
		    timestamp2 = timer0_getCounter();
		    if (!kolvo) {timeout = 600;}
			 else 
			  {timeout = 500; }
		   }
		   else
		   {
		    LED_ERROR_OFF;
			B1LED_OFF;
			B2LED_OFF; 
			timestamp2 = timer0_getCounter();
		    if (kolvo) {timeout = 600;kolvo=0;}
			 else 
			  {timeout = 500;kolvo=1; }
		   }
		   
		   
	  } 
 }
}


// ----------------------------------------------------------------------------------------------
// ��������� ��������� �������
// ----------------------------------------------------------------------------------------------
void Process_data(void)
{char tmp=0;

// ������� �� �����
  if ( (can_msg_in.data[0]==0x33)&&(can_msg_in.data[1]==0x22)&&(can_msg_in.data[2]==0x11) )
 {
  	watchdog_init();
	while(1);
 }
else
{
 	tmp=(can_msg_in.data[0]);

	// 0-� ��� - ��������� B1
	// 1 -� ��� - ��������� B2
    if ((tmp&BIT(0))==0x01) B1LED_ON;
 	 else B1LED_OFF; 
 	if ((tmp&BIT(1))==0x02) B2LED_ON;
 	else B2LED_OFF;
	/*Buzzer*/
	if ((can_msg_in.data[1])&BIT(0))	   PORTE|= BIT(0);//BUZZER ON
	else  								   PORTE&=~BIT(0);// BUZZER OFF
 }
 
}
//===========================================================
 
// ��������� ��������� �����
void packages(void)
{char i=0;
     // first byte - is DFK1-5
	/*���� �������� ��� ���������...*/
	if ((debounce[N_SIGNAL_DFK1])&&(debounce[N_SIGNAL_DFK2])){
		/*��������, ��� �� ��������� �� ���1 �� ���2*/
		can_msg_out.data[0]&=~BIT(0);
		can_msg_out.data[0]&=~BIT(1);
		/*�� �������� ���, ��� ��� ��������� */
		if  (debounce[N_SIGNAL_UZD1OUT]>0) can_msg_out.data[1]|=BIT(0); 
		else  can_msg_out.data[1]&=~BIT(0);
		if  (debounce[N_SIGNAL_UZD2OUT]>0) can_msg_out.data[1]|=BIT(1); 
		else  can_msg_out.data[1]&=~BIT(1);
	}
	/*���� �������� ��������� �� ���1...*/
	else if((debounce[N_SIGNAL_DFK1])&&(debounce[N_SIGNAL_DFK2]==0)){
		/*��������, ��� �� �������� ���1 */
		can_msg_out.data[0]&=~BIT(1);
		//can_msg_out.data[0]&=~BIT(0);
		/*������ ������� ���2 ����������� ������ � ���2*/
		if  (debounce[N_SIGNAL_UZD2OUT]==0) can_msg_out.data[0]|=BIT(0); 
		else  can_msg_out.data[0]&=~BIT(0);
		/*��1 ���������� ��� ����...*/
		if  (debounce[N_SIGNAL_UZD1OUT]>0) can_msg_out.data[1]|=BIT(0); 
		else  can_msg_out.data[1]&=~BIT(0);
		/*� ��2 ���� �� ��������...*/
		can_msg_out.data[1]&=~BIT(1);
	}
	/*���� �� ������� ���������...*/
	else if((debounce[N_SIGNAL_DFK1]==0)&&(debounce[N_SIGNAL_DFK2]==0)){
		can_msg_out.data[0]&=~BIT(1);
		/*������ ���1 � ���2 �������� ���1 � ���2 ��������������*/
		if  ((debounce[N_SIGNAL_UZD1OUT]==0)||(debounce[N_SIGNAL_UZD2OUT]==0)) can_msg_out.data[0]|=BIT(0); 
		else  can_msg_out.data[0]&=~BIT(0);
		/*if  (debounce[N_SIGNAL_UZD2OUT]>0) can_msg_out.data[0]|=BIT(1); 
		else  can_msg_out.data[0]&=~BIT(1);
		*/
		/*� �������������� ���� ������ �� ���������*/
		can_msg_out.data[1]&=~BIT(0);
		can_msg_out.data[1]&=~BIT(1);
	}
	// first byte - is DFK1-5 
	for (i=2;i<=4;i++) {
		if  (debounce[i]>0) can_msg_out.data[0]|=BIT(i); 
		else  can_msg_out.data[0]&=~BIT(i);
	}
	for (i=7;i<=9;i++) {
		if  (debounce[i]>0) can_msg_out.data[1]|=BIT(i-5); 
		else  can_msg_out.data[1]&=~BIT(i-5);
	}
}
//*******************  END ******************************************