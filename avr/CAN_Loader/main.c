//*****************************************************************************
//* BootLoader 3.1 for MegaLoad 3.2 Mode By VilDi ,last modyfed  31.07.2008							 
//* 		
//* Devices supported at this time	  	  										 
										 
//* ATMega128											 
//* ATMega162											 
/*********************************************************/
/*                   I N C L U D E    		             */
/*********************************************************/	
#include "main.h"
#include <macros.h>
#include "mcplite.h"
#include "litecrc.h"

//*****************************************************************************
// MCU selection
// Mast have  - выставите параметры МЦУ путём макродифайна в ImageCraft`е
//*****************************************************************************
#ifdef MCU_ATMEGA128
#include "iom128v.h"
#define  PageByte 	 	 256    
#define  NSHIFTPAGE	 	 8
#define	 INTVECREG	 	 MCUCR
#define  RAMPZ_FLAG 
#define NUMPAGES 480
#define P_MCU_SET_OK
#endif

#ifdef MCU_ATMEGA162
#include "iom162v.h"
#define  PageByte 	 	 128    
#define  NSHIFTPAGE	 	7
#define	 INTVECREG	 	 GICR
#define NUMPAGES 112
#define P_MCU_SET_OK
#endif




#ifndef P_MCU_SET_OK
 #error Установите параметры MCU! 
#endif

 /*********************************************************/
/*           P R O T O T Y P E 							 */
/*********************************************************/	
void FlashLoad(void);
void GetPageNumber(void);
void ExecCode(void);
char GetPage(void);
void WriteFlash(void);
char CheckWrite(void);
void TByte(void);
void RPackage(void);
void Wait(void);
void main(void);
void Full_Erase(void);
void CPU_Reset(void);

/*****************************************************************************/
/*           G L O B A L    V A R I A B L E S			                     */
/*****************************************************************************/	
unsigned char PageBuffer[PageByte];
unsigned int PageAddress;
unsigned int RealPageAddress;
unsigned int temp;
char fat_error=0;
MCP_CAN_MESSAGE can_msg_out,can_msg_in;
//*************************************************************************
#ifdef MCU_ATMEGA162
asm( "SPMCR = 0x57\n"
	   "PAGESIZEB=128\n"// page size in byte
	   );
#else
asm( "SPMCR = 0x68\n"
	   "PAGESIZEB=256\n"
	   );
#endif	   


/*****************************************************************************/
/* Flash Programing Code								                     */
/*****************************************************************************/
void FlashLoad(void)
{

can_msg_out.data[0]=GETFLASH;
TByte();//
 
  
while (1) //в бесконечном цыкле  читаем страницы
	  {
	  GetPageNumber(); //
	  if (RealPageAddress >=NUMPAGES) 
	  {
	  fat_error=0;
	  return;// страницы кончились- выходим
	  }
	  if (GetPage())  // если страница пришла
		 {
		    WriteFlash();//- пишем во флэш
			if (CheckWrite())
			  {
		   // если записалось, то всё ништяк, давай другую страницу
		     }
		   else // болт, не записалось
		   {
		     can_msg_out.data[0]=PAGEWRITEERROR;
		    TByte();
		    }
		 }
	  else //если страница не пришла...
	   {
	    can_msg_out.data[0]=PAGELOST;
		TByte();
	   }
	  }//end while
}

/*****************************************************************************/

void GetPageNumber(void)
{
can_msg_out.data[0]=GETPAGENUMBER;
TByte();

RPackage();
                                                 // hi                              //lo
RealPageAddress = (((int)can_msg_in.data[0] << 8) + can_msg_in.data[1]);
  
PageAddress = RealPageAddress << NSHIFTPAGE;// wiew datasheet about Z pointer

#ifdef RAMPZ_FLAG
 RAMPZ = can_msg_in.data[2]; 
#endif
}

/*****************************************************************************/

char GetPage(void)
{
unsigned int i,j;
unsigned int LocalCheckSum = 0;
unsigned int CheckSum = 0;
can_msg_out.data[0]=GETPAGE; // 
TByte();
i=0;
while (i<PageByte)
 {
 	 // принимаем пакет
	 RPackage(); 
	 for (j=0;j<=7;j++) 
	  {
	    PageBuffer[i]=can_msg_in.data[j];// разбираем пакет
	    i++;
	   }
  }
		   
RPackage(); // принимаем CRC
CheckSum=crc16(PageBuffer,PageByte);	// считаем CRC
LocalCheckSum=(int)((can_msg_in.data[0]<< 8) + can_msg_in.data[1]);
 if (LocalCheckSum == CheckSum) 
 	return 1;// check CRC
 else 
   return 0;
}

/*****************************************************************************/
void WriteFlash(void)
{
register unsigned int padr=PageAddress;
register unsigned char *temp;
// see the "Simple Asssembly Code Exemple for a Boot Loader"
asm(// сохраняем  значения Х поинтера
"push r26\n"
"push r27\n"
); 
temp=&PageBuffer[0];
asm(
"lds  r30,0x0121\n"//;store pointer Z\n"
"lds  r31,0x0122\n"//;store pointer Z\n"
);
asm(// грузим в X-поинтер адрес массива
"movw r26, %temp\n"
);
asm(//; re-enable the RWW section
"ldi r20, 0x11\n"
);
asm("call Do_spm\n"
//; transfer data from RAM to Flash page buffer
"ldi r24,PAGESIZEB ;init loop variable\n"
);
asm(
"Wrloop:\n"
);
asm(//
"ld r0, X+\n"
"ld r1, X+\n"
);
asm(
"ldi r20, (0x01)\n"
);
asm("PUSH r24\n"
);
asm("call Do_spm\n");
asm("POP r24\n"
);
asm(
"adiw r30, 2\n"
);
asm(
"subi r24, 2 \n"
);
asm(
"brne Wrloop\n"
);

//padr=PageAddress;

asm(
"lds  r30,0x0121\n"//;restore pointer Z\n"
"lds  r31,0x0122\n"//;restore pointer Z\n"
);
//; execute page write
asm(
"ldi r20,0x05\n"
);
asm(
"call Do_spm\n"

);
//; re-enable the RWW section
asm("ldi r20, 0x11\n"
"call Do_spm\n"
//"ret\n"
);
//; return to RWW section
//; verify that RWW section is safe to read
asm(
"Return:\n"
"lds r16, SPMCR\n"
"sbrs r16, 6 \n");// If RWWSB is set, the RWW section is not ready yet
asm("rjmp End\n"//"ret\n"
);
//; re-enable the RWW section
asm("ldi r20, 0x11\n"
"call Do_spm\n"
);
asm(
"rjmp Return\n"
);
//; check for previous SPM complete
asm("Do_spm:\n"
"Wait_spm:\n"
"lds r16, SPMCR\n"
"sbrc r16, 0\n"
"rjmp Wait_spm\n"
//; input: spmcrval determines SPM action
//; disable interrupts if enabled, store status
);
asm("lds r17, 0x5F\n"
"cli\n"
//; SPM timed sequence
"STS SPMCR, r20\n"
);
asm(
"spm\n"
);
asm(
//; restore SREG (to enable interrupts if originally enabled)
"STS 0x5F, r17\n"
"ret\n"
);
asm(// возвращаем значения Х поинтера
"End:\n"
"pop r27\n"
"pop r26\n"
); 
 
}
//**************************************************** Поверка записи****************
char CheckWrite(void)
{ register unsigned char tempbuf=0;
 unsigned char i=0;
 register unsigned int padr=PageAddress;

asm("movw r30,%padr");
while (i<PageByte){
#ifdef RAMPZ_FLAG
	asm("ELPM %tempbuf,Z+ \n"); 
#else
	asm("LPM %tempbuf,Z+ \n");
#endif
	  
	asm("PUSH r30\n"
	"PUSH r31\n");
	if (!(tempbuf==PageBuffer[i])){ 
		asm("POP r31\n"
		"POP r30\n");
		return(0);
	}
	asm("POP r31\n"
	"POP r30\n");
	i++;
}
return(1); 
 
}

//_________________________________________________________________________

///********************  Полное стирание Flash(кроме бутлодера)********************************
void Full_Erase(void)
{//register unsigned int temp=0; 
 register unsigned int padr;
 temp=0; 
  while (temp<NUMPAGES) 
  { 
 #ifdef RAMPZ_FLAG

if (temp>240)
{
 padr=(temp%240 )<< NSHIFTPAGE;
 RAMPZ = 1;
 }
 else
 {
   padr=temp << NSHIFTPAGE;
   RAMPZ = 0;
 }
#else
   padr=temp << NSHIFTPAGE; 
#endif    
    asm(
      "movw r30,%padr\n"// грузим в Z-поинтер номер страницы
    );
	// стираем страницу
//	asm("push %temp\n");
    asm(
    "ldi r20,0x03\n"
    );
    asm(
    "call Do_spm\n"
    );
//	asm("pop %temp\n");
	temp++;
  }	
 
	
}
//****************************************************************************


//+++++++++++++++++++++++++++Принимаем КАН сообщение+++++++++++++++++++=
void RPackage(void)
{ char i;
 char j;

     for (i=0;i<=7;i++)
	 {
	   for (j=0;j<=244;j++) // ~100 msec
	     if (CAN_msg_Read( &can_msg_in) ) return;	   
	
     }
	 if (fat_error)
	  CPU_Reset();// стираем всё и уходим в перезагрузку
	 else
	 ExecCode();// выполняем программу
	return; 
 
    
}
/*****************************************************************************/


//******************** Отправляем КАН сообщение******
void TByte(void)
{char freez=0;
while ( CAN_Msg_Send(&can_msg_out)==0)
 {
  if (freez>=CAN_FREEZ_TIME) 
  {
   #ifdef RAMPZ_FLAG 
   RAMPZ = 0;
   #endif
   asm("jmp 0x0000");       // Run application code   
  } 
  freez++;
 }
 
}
//()()()()()()()()()()()()()()()()()()()()())()()()()()()()()()()()())


//*******************Переход на начало загруженной программы ***********************************

void ExecCode(void)
{

can_msg_out.data[0]=STARTPROGRAMM;//посылаем сообщение об выполнении программы
TByte();
 
#ifdef RAMPZ_FLAG 
RAMPZ = 0;
#endif
#ifdef INTVECREG
 INTVECREG = 0x01;	   	 // Enable interrupt vector select
 INTVECREG = 0x00;	   	 // Move interrupt vector to flash
#endif
asm("jmp 0x0000");       // Run application code   
}


//*************************************************************
void main(void)
{
//unsigned char i;
// инициализация

  port_init(); // *** startup defined in mcplite.h ***

 SPI_masterInit(); 

 CAN_init();  

//
// Отправляем  идендификационные данные платы
 //can_msg_out.ID=CANOUT_ID;

    can_msg_out.CNTRL |= BIT(3); // TXREQ
	can_msg_out.EID8 = 0; 
	can_msg_out.EID0 = 0; 
	can_msg_out.SIDH = (char)((CANOUT_ID >> 3) & 0x00FF); 
	can_msg_out.SIDL = (char)((CANOUT_ID << 5) & 0x00E0);
 

	can_msg_out.DLEN=7;
	can_msg_out.data[0]=START_BOOT;// приглашение к работе ...
	can_msg_out.data[1]=_IS_;
	can_msg_out.data[2]=_S_;
	can_msg_out.data[3]=_M_;
	can_msg_out.data[4]=_P_;
	can_msg_out.data[5]=_K_;
	can_msg_out.data[6]=KEY;
	TByte();
	can_msg_out.DLEN=1;
	
	RPackage();
	if ((can_msg_in.data[0] == WORK)
	&&(can_msg_in.data[1]==_IS_)
	&&(can_msg_in.data[2]==_S_)
	&&(can_msg_in.data[3]==_M_)
	&&(can_msg_in.data[4]==_P_)
	&&(can_msg_in.data[5]==_K_)
	&&(can_msg_in.data[6]==KEY)) {
	
		//RPackage();
		fat_error=1;
		Full_Erase();
		FlashLoad();
		ExecCode();
	}
	else {
//		can_msg_out.DLEN=1;
		ExecCode();
	}
		   
}

/******************************************
*Стирание и перезагрузка проца
*******************************************/
void 
CPU_Reset(void)
{
	/*отсылаем сообщение об ошибке*/
	can_msg_out.data[0]=FATALERROR;
	TByte();
	Full_Erase();
	WDTCR = 0x08;	// Заводим вачдог...
	while(1);		// и запускаем бесконечный цикл... 
					// по вачдогу проц перезагрузится
}




