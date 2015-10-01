/*============================================================================================
*	Протокол обмена между БУ и БД дверей, header
*=============================================================================================*/
//#ifndef PROTOCOL_BU_H_
//#define PROTOCOL_BU_H_
#include <types.h>
#include "main.h"
/*****************************************************
	***	 Идендификаторы для дверей 	***
*****************************************************/
/*тормозной конец левая боковая*/
#define BRAKE_END_LEFT_SIDE 0x1A
/*тормозной конец правая боковая*/
#define BRAKE_END_RIGHT_SIDE 0x1B
/*тормозной конец торцевая*/
#define BRAKE_END_FRONT 0x1C

/*нетормозной конец торцевая*/
#define NONBRAKE_END_FRONT 0x1F
/*нетормозной конец левая боковая*/
#define NONBRAKE_END_LEFT_SIDE 0x1D
/*нетормозной конец правая боковая*/
#define NONBRAKE_END_RIGHT_SIDE 0x1E

/*Дебаговский пакет на приём*/
#define DEBUG_IN_ID 0x2C

/*Дебаговский пакет на передачу*/
#define DEBUG_OUT_ID 0x100
#define DEBUG_OUT_STR_MAX 2


/*****************************************************
	***	Типы дверей: боковая или торцевая	***
*****************************************************/
/*Боковая*/
#define DOOR_TYPE_SIDE	0x61
/*Торцевая*/
#define DOOR_TYPE_FRONT 0x54
/*****************************************************
	***	Идендификаторы пакетов от БУ 	***
*****************************************************/
/*пакет с командами для БУ*/
#define PACKAGE_COMMAND				0x2A
/*пакет с настройками дверей*/
#define PACKAGE_SETTINGS			0x2B
/*****************************************************
	***	Биты для флага событий 	***
*****************************************************/
/*Новая команда*/
#define NEW_CMD			0
/*Настройки приняты*/
#define STS_TAKEN		1
/*Настройки пришли*/
#define COM_STS			2
/*пишёл запрос на настройки*/
#define GET_STS			3
/*Перевод в ручной режим*/		
#define MAN_MODE		4
/*****************************************************
	***	маска команд для дверей 	***
*****************************************************/
/*cmd	-	маска команд для дверей		
		1	-	открыть
		2	-	закрыть
		3	-	разблокировать
		4	-	заблокировать
		5	-	тестировать
		6	-	передаются новые настройки двери
		7	-	запрос настроек двери
		8	-	новая прошивка
		9	-	БУ теперь настроен
*/

#define CMD_OPEN_DOOR 0x01
#define CMD_CLOSE_DOOR 0x02
#define CMD_UNLOCK 0x03
#define CMD_LOCK 0x04
#define CMD_NEW_STS 0x06
#define CMD_GET_STS 0x07
#define CMD_NEW_FIRMWARE 0x08
#define CMD_BU_NOW_SETUP 0x09

/*****************************************************
	***	HZ	***
*****************************************************/
/*Максимальный размер буффера для настроек*/
#define MAX_COUNT_OFF_SETTINGS		21

/*Максимальный размер буффера для отправки состояния двери*/
#define MAX_COUNT_OFF_STATE			7
/*интервал отправки сообщений, мс*/
#define INTERVAL_BETWEEN_SENDING	300

/*****************************************************
	***	Макросы 	***
*****************************************************/

/*Установка флага "Новая команда"*/
#define SET_FLAG_NEW_CMD	events_flags|=BIT(NEW_CMD)
/*Сброс флага "Новая команда"*/
#define CLEAR_FLAG_NEW_CMD	events_flags&=~BIT(NEW_CMD)

/*Установка флага "Настройки приняты"*/
#define SET_FLAG_STS_TAKEN	events_flags|=BIT(STS_TAKEN)
/*Сброс флага "Настройки приняты"*/
#define CLEAR_FLAG_STS_TAKEN	events_flags&=~BIT(STS_TAKEN)

/*Установка флага "Идут настройки"*/
#define SET_FLAG_COM_STS	events_flags|=BIT(COM_STS)
/*Сброс флага "Идут настройки "*/
#define CLEAR_FLAG_COM_STS	events_flags&=~BIT(COM_STS)

/*Установка флага Перевод в ручной режим*/	
#define SET_FLAG_MAN_MODE	events_flags|=BIT(MAN_MODE)	
/*Сброс флага Перевод в ручной режим*/	
#define CLEAR_FLAG_MAN_MODE	events_flags&=~BIT(MAN_MODE)


/*****************************************************
	***	 Для отправки состояния	***
******************************************************/
//{
/*=== Боковые двери ===*/
//{
/*Открыто*/
#define SET_STATE_B_OPEN	buff_state[0]|=BIT(0)
#define CLR_STATE_B_OPEN	buff_state[0]&=~BIT(0)

/*Закрыто*/
#define SET_STATE_B_CLOSE	buff_state[0]|=BIT(1)
#define CLR_STATE_B_CLOSE	buff_state[0]&=~BIT(1)

/*Дверь поджата*/
#define SET_STATE_B_INWARD	buff_state[0]|=BIT(2)
#define CLR_STATE_B_INWARD	buff_state[0]&=~BIT(2)

/*Заперто на ключ*/
#define SET_STATE_B_LOCK	buff_state[0]|=BIT(3)
#define CLR_STATE_B_LOCK	buff_state[0]&=~BIT(3)

/*Заперто "секретка"*/
#define SET_STATE_B_SEKRETKA	buff_state[0]|=BIT(4)
#define CLR_STATE_B_SEKRETKA	buff_state[0]&=~BIT(4)

/*Подножка сложена*/
#define SET_STATE_B_FOOTBOARD	buff_state[0]|=BIT(5)
#define CLR_STATE_B_FOOTBOARD	buff_state[0]&=~BIT(5)

/*Дверь заблокированна*/
#define SET_STATE_B_FREEZING	buff_state[0]|=BIT(6)
#define CLR_STATE_B_FREEZING	buff_state[0]&=~BIT(6)

/*Помеха в створе*/
#define SET_STATE_B_PARASITE	buff_state[0]|=BIT(7)
#define CLR_STATE_B_PARASITE	buff_state[0]&=~BIT(7)

/*Поддерживает режим работы - ручной*/
#define SET_STATE_B_SUPP_MODE_MANUAL	buff_state[1]|=BIT(0)
#define CLR_STATE_B_SUPP_MODE_MANUAL	buff_state[1]&=~BIT(0)

/*Поддерживает режим работы - автоматический*/
#define SET_STATE_B_SUPP_MODE_AUTO	buff_state[1]|=BIT(1)
#define CLR_STATE_B_SUPP_MODE_AUTO	buff_state[1]&=~BIT(1)

/*Скорость более 5 км/ч */
#define SET_STATE_B_V5	buff_state[1]|=BIT(2)
#define CLR_STATE_B_V5	buff_state[1]&=~BIT(2)

/*Скорость более 45 км/ч */
#define SET_STATE_B_V45	buff_state[1]|=BIT(3)
#define CLR_STATE_B_V45	buff_state[1]&=~BIT(3)

/*Текущий режим работы: 0 – ручной, 1 – автоматический*/
#define SET_STATE_B_MANUAL_MODE	buff_state[1]|=BIT(4)
#define CLR_STATE_B_MANUAL_MODE	buff_state[1]&=~BIT(4)

/*************/
/*Наличие неисправности*/
#define SET_STATE_B_REMOVABLE_ERR	buff_state[1]|=BIT(5)
#define CLR_STATE_B_REMOVABLE_ERR	buff_state[1]&=~BIT(5)
/**************/
/*Нажата кнопка открыть*/
#define SET_STATE_B_KO				buff_state[1]|=BIT(6)
#define CLR_STATE_B_KO				buff_state[1]&=~BIT(6)

/*Нажата кнопка закрыть*/
#define SET_STATE_B_KZ				buff_state[1]|=BIT(7)
#define CLR_STATE_B_KZ				buff_state[1]&=~BIT(7)




/*Исправен электродвигатель привода двери М1*/
#define CLR_STATE_B_OPERABLE_M1	buff_state[2]|=BIT(0)
#define SET_STATE_B_OPERABLE_M1	buff_state[2]&=~BIT(0)

/*Исправен электродвигатель устройства фиксации М2*/
#define CLR_STATE_B_OPERABLE_M2	buff_state[2]|=BIT(1)
#define SET_STATE_B_OPERABLE_M2	buff_state[2]&=~BIT(1)

/*Исправен электродвигатель устройства фиксации М3*/
#define CLR_STATE_B_OPERABLE_M3	buff_state[2]|=BIT(2)
#define SET_STATE_B_OPERABLE_M3	buff_state[2]&=~BIT(2)

/*Исправна муфта MF1 электродвигателя привода двери*/
#define CLR_STATE_B_OPERABLE_MF1	buff_state[2]|=BIT(3)
#define SET_STATE_B_OPERABLE_MF1	buff_state[2]&=~BIT(3)

/*Исправен бесконтактный индуктивный выключатель «Открыто» S11*/
#define CLR_STATE_B_OPERABLE_S11	buff_state[2]|=BIT(4)
#define SET_STATE_B_OPERABLE_S11	buff_state[2]&=~BIT(4)

/*Исправен бесконтактный индуктивный выключатель «Положение» S9*/
#define CLR_STATE_B_OPERABLE_S9	buff_state[2]|=BIT(5)
#define SET_STATE_B_OPERABLE_S9	buff_state[2]&=~BIT(5)

/*Исправен бесконтактный индуктивный выключатель «Закрыто» S10*/
#define CLR_STATE_B_OPERABLE_S10	buff_state[2]|=BIT(6)
#define SET_STATE_B_OPERABLE_S10	buff_state[2]&=~BIT(6)

/*Исправен бесконтактный индуктивный выключатель «Поджато 1» S1*/
#define CLR_STATE_B_OPERABLE_S1	buff_state[2]|=BIT(7)
#define SET_STATE_B_OPERABLE_S1	buff_state[2]&=~BIT(7)


/*Исправен бесконтактный индуктивный выключатель «Поджато 2» S2*/
#define CLR_STATE_B_OPERABLE_S2	buff_state[3]|=BIT(0)
#define SET_STATE_B_OPERABLE_S2	buff_state[3]&=~BIT(0)

/*Исправен соленоид Y1*/
#define CLR_STATE_B_OPERABLE_Y1	buff_state[3]|=BIT(1)
#define SET_STATE_B_OPERABLE_Y1	buff_state[3]&=~BIT(1)

/*Исправен соленоид Y2*/
#define CLR_STATE_B_OPERABLE_Y2	buff_state[3]|=BIT(2)
#define SET_STATE_B_OPERABLE_Y2	buff_state[3]&=~BIT(2)

/**!!!**************!!!*************!!!**/
/*Неустранимая ошибка*/
#define SET_STATE_B_FATAL_ERR	buff_state[3]|=BIT(3)
#define CLR_STATE_B_FATAL_ERR	buff_state[3]&=~BIT(3)
/**!!!**************!!!*************!!!**/

/*Исправны цепи защитных кромок S12, S13*/
#define CLR_STATE_B_OPERABLE_S12	buff_state[3]|=BIT(4)
#define SET_STATE_B_OPERABLE_S12	buff_state[3]&=~BIT(4)

/*Исправен датчик «Мёртвая точка 1» S3*/
#define CLR_STATE_B_OPERABLE_S3	buff_state[3]|=BIT(5)
#define SET_STATE_B_OPERABLE_S3	buff_state[3]&=~BIT(5)

/*Исправен датчик «Мёртвая точка 2» S4*/
#define CLR_STATE_B_OPERABLE_S4	buff_state[3]|=BIT(6)
#define SET_STATE_B_OPERABLE_S4	buff_state[3]&=~BIT(6)

/*Ошибка закрытия на "ключ(""Секретку")*/
#define CLR_STATE_B_LOCKERR	buff_state[3]|=BIT(7)
#define SET_STATE_B_LOCKERR	buff_state[3]&=~BIT(7)
/*настройки/команда для двери удачно приняты	*/
#define SET_STATE_B_ACCEPTED	buff_state[5]|=BIT(0)
#define CLR_STATE_B_ACCEPTED	buff_state[5]&=~BIT(0)


/*Время открытия/закрытия двери*/
#define TIME_TO_OPEN_CLOSE				buff_state[6]

/*версия прошивки
берём из main.h FIRMWARE_VERSION*/
//}
/*=== Торцевые двери ===*/
//{
/*Открыто*/
#define SET_STATE_T_OPEN	buff_state[0]|=BIT(0)
#define CLR_STATE_T_OPEN	buff_state[0]&=~BIT(0)

/*Закрыто*/
#define SET_STATE_T_CLOSE	buff_state[0]|=BIT(1)
#define CLR_STATE_T_CLOSE	buff_state[0]&=~BIT(1)

/*Заперто на ключ*/
#define SET_STATE_T_LOCK	buff_state[0]|=BIT(2)
#define CLR_STATE_T_LOCK	buff_state[0]&=~BIT(2)

/*Заперто "секретка"*/
#define SET_STATE_T_SEKRETKA	buff_state[0]|=BIT(3)
#define CLR_STATE_T_SEKRETKA	buff_state[0]&=~BIT(3)

/*Текущий режим работы: 0 – ручной, 1 – автоматический*/
#define SET_STATE_T_MANUAL_MODE	buff_state[0]|=BIT(4)
#define CLR_STATE_T_MANUAL_MODE	buff_state[0]&=~BIT(4)

/*************/
/*Наличие неисправности*/
#define SET_STATE_T_REMOVABLE_ERR	buff_state[0]|=BIT(5)
#define CLR_STATE_T_REMOVABLE_ERR	buff_state[0]&=~BIT(5)
/**************/
/*Нажата кнопка открыть*/
#define SET_STATE_T_KO				buff_state[0]|=BIT(6)
#define CLR_STATE_T_KO				buff_state[0]&=~BIT(6)

/*Поддерживает режим работы - ручной*/
#define SET_STATE_T_SUPP_MODE_MANUAL	buff_state[1]|=BIT(0)
#define CLR_STATE_T_SUPP_MODE_MANUAL	buff_state[1]&=~BIT(0)

/*Поддерживает режим работы - автоматический*/
#define SET_STATE_T_SUPP_MODE_AUTO	buff_state[1]|=BIT(1)
#define CLR_STATE_T_SUPP_MODE_AUTO	buff_state[1]&=~BIT(1)

/*Помеха в створе*/
#define SET_STATE_T_PARASITE	buff_state[1]|=BIT(2)
#define CLR_STATE_T_PARASITE	buff_state[1]&=~BIT(2)

/*Дверь заблокированна*/
#define SET_STATE_T_FREEZING	buff_state[1]|=BIT(3)
#define CLR_STATE_T_FREEZING	buff_state[1]&=~BIT(3)


/*Исправен электродвигатель привода двери М1*/
#define CLR_STATE_T_OPERABLE_M1	buff_state[2]|=BIT(0)
#define SET_STATE_T_OPERABLE_M1	buff_state[2]&=~BIT(0)

/*Исправна муфта MF1 электродвигателя привода двери*/
#define CLR_STATE_T_OPERABLE_MF1	buff_state[2]|=BIT(1)
#define SET_STATE_T_OPERABLE_MF1	buff_state[2]&=~BIT(1)

/*Исправен бесконтактный индуктивный выключатель «Открыто» S3*/
#define CLR_STATE_T_OPERABLE_S3	buff_state[2]|=BIT(2)
#define SET_STATE_T_OPERABLE_S3	buff_state[2]&=~BIT(2)

/*Исправен бесконтактный индуктивный выключатель «Положение» S1*/
#define CLR_STATE_T_OPERABLE_S1	buff_state[2]|=BIT(3)
#define SET_STATE_T_OPERABLE_S1	buff_state[2]&=~BIT(3)

/*Исправен бесконтактный индуктивный выключатель «Закрыто» S2*/
#define CLR_STATE_T_OPERABLE_S2	buff_state[2]|=BIT(4)
#define SET_STATE_T_OPERABLE_S2	buff_state[2]&=~BIT(4)

/*Исправны цепи защитной кромки S6*/
#define CLR_STATE_T_OPERABLE_S6	buff_state[2]|=BIT(5)
#define SET_STATE_T_OPERABLE_S6	buff_state[2]&=~BIT(5)

/**!!!**************!!!*************!!!**/
/*Неустранимая ошибка*/
#define SET_STATE_T_FATAL_ERR	buff_state[2]|=BIT(6)
#define CLR_STATE_T_FATAL_ERR	buff_state[2]&=~BIT(6)
/**!!!**************!!!*************!!!**/


/*Температуру отправлять числом*/


/*настройки/команда для двери удачно приняты	*/
#define SET_STATE_T_ACCEPTED	buff_state[5]|=BIT(0)
#define CLR_STATE_T_ACCEPTED	buff_state[5]&=~BIT(0)

/*тест просто завершён*/
#define SET_STATE_T_TEST_COMPLETED	buff_state[5]|=BIT(1)
#define CLR_STATE_T_TEST_COMPLETED	buff_state[5]&=~BIT(1)

/*Время открытия/закрытия двери*/
#define TIME_TO_OPEN_CLOSE				buff_state[6]

/*версия прошивки
берём из main.h FIRMWARE_VERSION*/
//}
//}

/*****************************************************
	***	PROTOTYPES 	***
*****************************************************/

/********************************************************************
	Функция формирования данных для отправки
********************************************************************/
void
Formation_Of_Data_To_Send(void);
/********************************************
*	Инициализация CAN
*	door_ID - идендификатор двери
*******************************************/
void CAN_SetUp(uint8_t door_ID);
/********************************************
*	Функция отправки сообщения для БД
*******************************************/
void
CAN_Thread(void);
/***********************************************
*	Функция приёма и разбора КАН сообщений
************************************************/
void
CAN_ReadMsg(void);

/********************************************
*	Функция формирования КАН- сообщения
*******************************************/
void
CAN_FormationPackage(void);
//#endif