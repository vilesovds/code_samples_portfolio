/*Алгоритм работы БУ*/
#ifndef	_ALGORITM_BU_H_
#define _ALGORITM_BU_H_

#include <types.h>
#include <macros.h>
#include "protocol.h"

/*****************************************************
	***	DEFINES 	***
*****************************************************/
#warning Эту хрень надо переделать(в алгоритме реплэс...)
/*максимальное кол-во импульсов*/
#define MAX_IMPULS_COUNT  Door.sts_max_impuls

/*Регистры для ШИМ*/
#define M1A 'A',7 
#define M1B 'A',6 

// Задержка измерения тока, мс
#define I_DEL  100

/*Темпрература перегрева БУ*/
# define OVERHEATING_T	800

/*Биты лампочек в регистре lamps_reg*/
#define LED_O_GREEN	1
#define LED_Z_GREEN	0

// Питание кнопок
#define LED_PWR_ON	PORTD|=BIT(5) 
#define LED_PWR_OFF	PORTD&=~BIT(5)

/*Биты для регистра команд*/
#define C_OPEN 0
#define C_CLOSE 1
#define C_LOCK 2
#define C_UNLOCK 3

#define ON	1
#define OFF	0

/*Режимы работы*/
enum {
	MODE_FIRST_BLOOD=0,// первый запуск
	MODE_MANUAL,// ручной режим
	MODE_OPENING, // открытие
	MODE_CLOSING,// закрытие
	MODE_OPEN, //отрылись
	MODE_CLOSE,// закрылись
	MODE_BREAKAGE, //сломались 
	MODE_BLOKING, // блокировка
	MODE_BLOKED, // заблокированно
	MODE_INTERMEDIATELY, // в промежуточном положении
	MODE_KEYLOCK, // Заперто на ключ или секретку
	MODE_SMOOTHBRAKING // Плавное торможение
};

/*Алгоритмические ошибки*/
enum {
	FAT_BUG_B1,
	FAT_BUG_B2,
	FAT_BUG_B3,
	FAT_BUG_B4,
	FAT_BUG_B5,
	FAT_BUG_B6,
	FAT_BUG_B7,
	FAT_BUG_B8,
	FAT_BUG_T1,
	FAT_BUG_T2,
	FAT_BUG_T3,
	FAT_BUG_T4,
	FAT_BUG_T5,
	FAT_BUG_T6,
	FAT_BUG_T7,
	FAT_BUG_T8
};
enum {
	REM_BUG_B1,
	REM_BUG_B2,
	REM_BUG_B3,
	REM_BUG_B4,
	REM_BUG_B5,
	REM_BUG_B6,
	REM_BUG_B7,
	REM_BUG_B8,
	REM_BUG_T1,
	REM_BUG_T2,
	REM_BUG_T3,
	REM_BUG_T4,
	REM_BUG_T5,
	REM_BUG_T6,
	REM_BUG_T7,
	REM_BUG_T8
};
/*Пороговое значение тока двигателя М1, ниже которого можно утверждать о неисправноти */
#define MIN_IM1	10
/*Задержка между включением муфты и двигателя М1, мс*/
#define PAUSE_MF	300

/*период мигания подсветкой кнопок*/
#define PERIOD_ZMIG 10000

/*===  МАКРОСЫ===*/
//{
/********************************************
	Управляющие
*******************************************/
/*Двигатель М2*/
#define CM2_ON	PORTA|=BIT(4)
#define CM2_OFF	PORTA&=~BIT(4)

/*Двигатель М3*/
#define CM3_ON	PORTA|=BIT(5)
#define CM3_OFF	PORTA&=~BIT(5)

/*Управляющая катушка соленоида Y1*/
#define CY1U_ON	 PORTD|=BIT(2)
#define CY1U_OFF PORTD&=~BIT(2)

/*Пусковая катушка соленоида Y1*/
#define CY1P_ON	 PORTD|=BIT(4)
#define CY1P_OFF PORTD&=~BIT(4)

/*Управляющая катушка соленоида Y2*/
#define CY2U_ON	 PORTD|=BIT(3)
#define CY2U_OFF PORTD&=~BIT(3)

/*Пусковая катушка соленоида Y2*/
#define CY2P_ON	 PORTD|=BIT(5)
#define CY2P_OFF PORTD&=~BIT(5)

/*Муфта*/
#define CM_ON	{PORTG|=BIT(3);ts_5_0=0;}
#define CM_OFF	PORTG&=~BIT(3)

/*Внутренний баззер*/
#define CB_INSIDE_ON	PORTG|=BIT(2)
#define CB_INSIDE_OFF	PORTG&=~BIT(2)

/*Внешний баззер*/
#define CB_OUTSIDE_ON	PORTG|=BIT(0)
#define CB_OUTSIDE_OFF	PORTG&=~BIT(0)


/*Остановитьт двигатель М1*/
#define M1_OFF Stop_PWM()
//}

/*****************************************************
	***	Прототипы 	***
*****************************************************/
/***********************************************************************************************
	Функция пересчёта кол-ва импульсов от датчика положение в расстояние, измеряемое в процентах
************************************************************************************************/
void
Recalc_Position(void);

/****************************************************************
	***	Мега главная функция, разруливает дверями 	***
*****************************************************************/
void 
Door_Manager(void);
/***********************************************************************
	Подпрограмма Открытия/Закрытия  двери по сигналам датчика "Положение"
RETURN:	0 - ещё не всё
		1 - всё ништяк, открылась
*************************************************************************/
uint8_t
Door_WorkPosition(void);

/*************************************************************
 Подпрограмма открытия/закрытия двери при отсутствии датчика "положение"
 Return: 	0 - ещё не всё
		1- всё,открылась
**************************************************************/
uint8_t 
Door_WorkWithoutPosition(void);

uint8_t
Door_RecVd(void);
void
Chech_Bugs(void);
#endif