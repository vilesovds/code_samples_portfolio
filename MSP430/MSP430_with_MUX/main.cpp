/* ==================================================================
file main.cpp 
=====================================================================*/
#include <msp430x20x3.h>
/*********************************************************************
*               CONSTANTS and DEFINES
**********************************************************************/
/*счетчики для таймера (1MHz)*/
#define BAUD_9600 104
#define BAUD_4800 208
#define BAUD_2400 417
#define BAUD_1200 833
#define BAUD_600 1667

// счетчики для таймера (16 MHz)
//#define BAUD_2400 6667
//#define BAUD_4800 3333
//#define BAUD_9600 1667
//#define BAUD_19200 833
//#define BAUD_57600 278

/*режимы работы UART*/
#define UART_MODE_NONE 0
#define UART_MODE_STOP 0
#define UART_MODE_RX 1
#define UART_MODE_RX_END 2
#define UART_MODE_TX 3

/* конфигурация контроллера*/
#define TXD_PIN BIT6
#define RXD_PIN BIT7
#define UART_SPEED BAUD_2400

/*макрос сброса вачдога*/
#define WDR  WDTCTL = WDTPW+WDTCNTCL

/*Максимальное количество каналов*/
#define MAX_CHANNELS 8
/***************************************************************
*					GLOBAL DATA
****************************************************************/
int ADC_offset = 0, ADC_offset0; 	// сдвиги нуля АЦП
unsigned int adc_value = 0; 		// последнее измеренное значение АЦП
char uartmode = 0;       			// режим работы UART
char rxbyte = 0;         			// принятый байт
unsigned int txdata = 0; 			// исходящие данные
char bitnum = 0;         			// текущий RX/TX bit
char ADC_Setup_Complete = 0; 		// флаг успешной настройки АЦП    
char adc_lock = 0;           		// блокировка обновления АЦП  
char count_of_adc = 0; 				// количество каналов MUX
char current_adc = 0; 				// номер текущего канала
char count_of_int = 0;				// кол-во прерываний
char flag_zamer = 0;					// флаг окончания измерения по прерыванию

/*буффер значений ацп*/
unsigned int adc_buf[16]={	adc_value, adc_value, adc_value, adc_value,
							adc_value, adc_value, adc_value, adc_value,
							adc_value, adc_value, adc_value, adc_value,
							adc_value, adc_value, adc_value, adc_value
						}; 
/*Последняя команда инициализации*/
char last_init_cmd = 0x00;

/**************************************************************
*     				PROTOTYPES
***************************************************************/
void set_mux(void);

/**************************************************************
*					FUNCTIONS
***************************************************************/

/*********************************
 пауза p мсек (при 1МГц)
*********************************/
void 
pause(unsigned int p){  
  unsigned int i;
  while (p>0)  {WDR; i = 300; while(i>0) i--; p--; }  
}

/************************************************
 Функция возвращает количество битов = 1 в байте
*************************************************/
char 
calcBits(char data)
{
	char testbit = 1, result = 0;
	while (testbit){  
		WDR;
		if (data & testbit) result++;	// просто смотрим на текущий бит в изучаемом байте
		testbit = testbit << 1;	/* так как testbit ограничен одним байтом, 
								то после обхода по всем битам он неминуемо обнулится */
	}
	return result;  
}
      
/*********************************
 Пауза на period циклов таймера
**********************************/
void 
timer_pause(unsigned int period)
{  
  // запускаем таймер
  CCR0 = 0xFFFF;           // mks
  if (period > 20) period -= 20;
  TACTL = TASSEL_2 + MC_1; // SMCLK, Up to CCR0 (таймер считает до CCR0 и вызывает прерывание)
  // выключаем таймер
  CCTL0 = 0;           	   // CCR0 interrupt disabled      
  TAR = 0;             	   // обнуляем счетчик таймера!
  while (TAR < period) WDR;// ждем пока счетчик таймера досчитает
  TAR = 0;                 // обнуляем счетчик таймера!
}

/*************************
 запись 1 бита в UART
*************************/
void 
uart_write_bit(void)
{     
	/* смотрим на очередной бит*/
	if (txdata & (1 << bitnum)) P1OUT |= TXD_PIN;  	// 1 на ноге
	else P1OUT &= ~TXD_PIN; 						// 0 на ноге
	bitnum++;										// увеличиваем счётчик битов
	/* если все передали, выключаем режим передачи*/
	if ( bitnum > 10 ){
		/* выключаем таймер*/ 
		CCTL0 = 0;          // CCR0 interrupt disabled      
		uartmode = 0;		//  cбрасываем режим работы UART
	}
}

/******************************
* Функция передачи 1 байта
******************************/
void 
TX_Byte (char data)
{
	bitnum = 0;       			// указатель на первый передаваемый бит
	txdata = (0xFF00 + (unsigned int)data) << 1; // 1 stop bit (1) + 8 bit data + 1 start bit (0)
	uartmode = UART_MODE_TX;  	// переходим в режим передачи
	/* запускаем таймер для передачи байта */
	CCR0 = UART_SPEED;         	// bit/s
	TACTL = TASSEL_2 + MC_1;   	// SMCLK, Up to CCR0 (таймер считает до CCR0 и вызывает прерывание)
	CCTL0 = CCIE;              	// CCR0 interrupt enabled  
	while(uartmode){
		WDR;
		SD16CCTL0 &= ~(SD16IE);
	};  // ждем ухода байта
	SD16CCTL0 |= (SD16IE);
}

/***************************
*	Читаем 1 бит из UART
****************************/
void 
uart_read_bit(void)
{
	/* смотрим принятый бит */
	char rx_bit = (P1IN & RXD_PIN);    
	if (bitnum < 8) // это бит данных
	if (rx_bit) rxbyte |= (1 << bitnum); // Добавляем бит

	// stop-bit (10) должен быть == 1
	if (bitnum == 9){
		if (rx_bit) uartmode = UART_MODE_RX_END;
		else{
			uartmode = 0;      // глюк какойто, убиваемся апстену
			P1IE |= RXD_PIN;   // ждем стартового бита
		}  
		/* выключаем таймер*/
		CCTL0 = 0;           // CCR0 interrupt disabled      
	}          
	bitnum++;				// увеличиваем кол-во принятых байтов
}


/*******************************************
* Port 1 interrupt service routine
* вызывается при приходе стартового бита
********************************************/
#pragma vector=PORT1_VECTOR
__interrupt void 
Port_1(void)
{  
	SD16CCTL0 &= ~(SD16IE);
	// это стартовый бит, начинаем прием
	bitnum = 0;
	rxbyte = 0;
	P1IE = 0;          	// выключаем прерывания от перепада уровня
	P1IFG &= ~RXD_PIN; 	// сброс флага прерывания
  
	// делаем паузу в полупериод,  чтобы таймер попадал в середину импульса
	//timer_pause(UART_SPEED / 2);
	//timer_pause(UART_SPEED / 3 ); // поправки на косяки
	timer_pause((UART_SPEED >> 2) - 30); // поправка на время выполнения промежуточных команд
  	/* начинаем прием байта */
	uartmode = UART_MODE_RX;
	/* запускаем таймер для приема байта */
	TAR = 0; 					// reset counter
	CCR0 = UART_SPEED;			// bit/s
	CCTL0 = CCIE;				// CCR0 interrupt enabled  
}


/****************************
* Функция приема 1 байта
****************************/
void 
RX_byte(void)
{
  /*ожидаем прихода стартового бита для начала чтения входящего байта*/
	P1IFG &= ~RXD_PIN; // P1.4 IFG cleared
	P1IE |= RXD_PIN;   // P1.4 interrupt enabled
	/*ждем окончания приема байта*/
	while(uartmode != UART_MODE_RX_END){ WDR;}; 
	uartmode = 0;
}


/*****************************************
* Прерывание Timer A0 по достижению CCR0
******************************************/
#pragma vector=TIMERA0_VECTOR
__interrupt void 
Timer_A(void)
{  
	/*смотрим режим UARTа*/
	switch (uartmode){
		case UART_MODE_RX: uart_read_bit(); break;  // принимаем очередной входящий бит
		case UART_MODE_TX: uart_write_bit(); break; // передаем очередной бит
	}
}

/*********************************
 Настройка АЦП
 cmd - комманда инициализации
**********************************/
void 
ADC_Setup(char cmd)
{
    ADC_offset =  ADC_offset0;				// смешение нуля
    SD16INCTL0 = SD16INCH_0; 				// канал A0  
    SD16AE = SD16AE0 + SD16AE1;     		// P1.0 & P1.1 analog inputs ON  
    SD16CCTL0 = SD16IE + SD16XOSR + BIT8;  	// 1024 OSR,interrupt enable  
    SD16CCTL0 |= SD16SC;                   	// Set bit to start conversion  
    count_of_adc=(cmd&0x0F);				// запоминаем кол-во возможных переключений
    //current_adc = count_of_adc;
    ADC_Setup_Complete = 1;					// Ставим флаг, что АЦП настроено
}

/*************************
 Рассчет нуля АЦП
**************************/
void 
calcOffset(void)
{
   /* считаем без усиления */
   SD16INCTL0 = SD16INCH_7;             // замкнуто, без усиления
   SD16AE = 0; 
   SD16CCTL0 |= SD16SC;                 // Set bit to start conversion  
   
   pause(500); 							// ждем ~500 мс
   SD16CCTL0 &= ~SD16SC; 				// stop ADC
   
   ADC_offset0 = adc_value - 0x8000;	// смещение нуля
 
}

/**********************************
 обрабатываем принятую команду
***********************************/
void 
process_cmd(void) 
{
	unsigned char dat=0x00;		//Временная переменная
	/* 0x60 - запрос значения АЦП из буффера */
	if ((rxbyte&0xF0) == 0x60){
		// настройка АЦП была выполнена?
		if (!(ADC_Setup_Complete)) return;
		/*Проверка на валидность*/
		dat=(rxbyte&0x0F);
		if (dat>count_of_adc)  return;
        adc_lock = 1; 		// запрещаем обновлять значения АЦП        
		unsigned int tmp = adc_buf[dat];	//adc_value;    
		adc_lock = 0; // разрешаем обновлять значения АЦП     
       	// посылаем рассчитанные значения АЦП из буффера       
		TX_Byte(char(tmp >> 8)); // HIGH  
		TX_Byte(char(tmp));      // LOW 
    	/* контрольная сумма */
		TX_Byte( (~ char(tmp >> 8)) ^ char(tmp));
		/*Шлём последнюю команду инициализации*/
		TX_Byte(last_init_cmd);
  	}  
	else if ((rxbyte & 0xF0) == 0x90) {// настройка АЦП
		/*Проверка на валидность*/
		dat=(rxbyte & 0x0F);
		if (dat<=MAX_CHANNELS){
			TX_Byte(~rxbyte);  		// возвращаем эхом принятый байт (с инверсией)
			ADC_Setup(dat); 		// настраиваем АЦП
			last_init_cmd=rxbyte;	// запоминаем команду инициализации
		}else return;
	}    
   
}

/*****************************************************
*	 Прерывание от АЦП 16 бит
******************************************************/
#pragma vector = SD16_VECTOR
__interrupt void 
SD16ISR(void)
{ 
	unsigned int tmp;						// временная переменная
	tmp = SD16MEM0;  						// Обязательно надо читать АЦП в прерывании иначе кирдык!!
	if (adc_lock==0){						// не запрещено обновлять значения АЦП
		if ((tmp > 0) && (tmp < 0xffff)) 	// проверка на зашкаливание
			adc_value = tmp - ADC_offset;  	// коррекция нулевого уровня
		else adc_value = tmp;
		/*Записываем в буффер*/
		adc_buf[current_adc]=adc_value;
		/* если была произведена настройка АЦП,
		то переключаем мультиплексор*/
		if (ADC_Setup_Complete) set_mux();	// переключаем мультиплексор 
	}
}
/***********************************************
*    Переключатель MUX
***********************************************/
void 
set_mux(void)
{ 
    SD16CCTL0 &= ~SD16SC; 	// stop ADC
	/*переключаем индекс ацп*/
	current_adc++;
    if (current_adc>=count_of_adc) current_adc=0;
    /*вывод в порт(с инверсией)*/
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
	pause(1);				// пауза для мультиплексора
    SD16CCTL0 |= SD16SC;	// start ADC
}
  


/*********************************************************
		MAIN
*********************************************************/
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;   // Stop WDT
    /* устанавливаем частоту процессора 1MHz (калибровка) */
	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;

	/* timer mode */
	TACTL = TASSEL_2 + MC_1;   // SMCLK, Up to CCR0 (таймер считает до CCR0 и вызывает прерывание)

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
  
	/* предварительная настройка АЦП    */
	//SD16CTL =SD16VMIDON+SD16REFON+ SD16SSEL_1 + SD16XDIV_2 + SD16DIV_2; // 1.2V ref, buffer, SMCLK,  /16 /4 

	SD16CTL = SD16VMIDON + SD16REFON + SD16SSEL_1 + SD16XDIV_2/* + SD16DIV_3*/; // 1.2V ref, buffer, SMCLK,  /16 /1
	SD16CCTL0 = SD16IE + SD16XOSR+ BIT8;  // 1024 OSR, bipolar, interrupt enable  

  	_BIS_SR(GIE);       // interrupt enable  
	calcOffset();       // калибровка нуля  
	WDTCTL=WDT_MRST_32; // Вачдог на 32 мс
	WDR;				// сбрасываем вачдог
	/*main cycle*/
	while(1){
		RX_byte();		// Принимаем байт по UARTу
		WDR;			
		process_cmd();  // обрабатываем принятую команду
		WDR;			
	}
}


