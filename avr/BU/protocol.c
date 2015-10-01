/*=========================================================================================================
*		Протокол обмена между БУ и БД дверей
* - v.0.1  - начало
*=========================================================================================================*/
/***********************************************************************************************************
*	***	ИНКЛУДЕСЫ	***
***********************************************************************************************************/
#include "protocol.h"
#include "main.h"
#include "algoritm.h"
/*Драйверы*/
#include "PWM.h"
#include <mcp2510.h>
#include <SPI.h>
#include <types.h>
#include <timer0.h>
#include <crc.h>
/*****************************************************
	***	Defines 	***
*****************************************************/
/*************************************************************************************************************
*	***	Global DATA	***
*************************************************************************************************************/
/*Структурки сообщений*/
USER_CAN_MESSAGE deb_msg,in_msg,out_msg;

/*Номер текущей страницы отправляемого сообщения*/
uint8_t current_out_str=0;
/*счётчик принятых страниц */
uint8_t count_in_str=0;
/*температура наружного воздуха из ЕПВС*/
uint8_t Tnar_EPVS=0;

/*Последняя полученная по кану комманда*/
uint8_t last_cmd=0;

/*Буффер для настроек*/
uint8_t buff_sts[MAX_COUNT_OFF_SETTINGS];

/*буффер для отправки состояния*/
uint8_t buff_state[MAX_COUNT_OFF_STATE]/*={1,2,3,4,5,6,7}*/;



/*Регистр состояния двери*/
uint8_t state_reg=0;
/*****************************************************
	***	Временные счётчики 	***
*****************************************************/
/*Счётчик отправки сообщений*/
uint16_t ts_sendcan=0;

/*****************************************************
	***	Флаги и регистры		***
*****************************************************/
/*Регистр событий*/
uint8_t events_flags=0;

/*Флаг ручного управления по КАНу*/
uint8_t CAN_management=FALSE;
extern TDoor Door;


/*Значения температуры*/
extern int16_t rez[2];
/*========================================================================
		Антидребезг
*=========================================================================*/
extern uint8_t debounce[N_SIGNAL_BUF_COUNT]; // состояние вводов после антидребезговой обработки


/*Регистр неисправностей*/
extern uint16_t bug_reg;

/*Регистры событий*/
extern uint16_t fatevent_buf;
extern uint16_t remevent_buf;

/*время открытия/закрытия */
extern uint16_t time_opcls;
/*Счётчик времени ручного управления по КАНу*/
extern uint16_t ts_CAN_man;

extern uint8_t door_T_close_step;

/*Наличие дополнительных дверей*/
extern uint8_t en_side_doors;


/*Флаг необходимости отсылки дебага*/
extern uint8_t send_deb;
/*Обратный отсчёт отсылки по кану...*/
extern uint16_t ts_send_deb;

extern uint16_t ts_5_0;
/*****************************************************
	***	Процедуры и функции 	***
*****************************************************/

/********************************************************************
*	***	Проверка ошибок	***
*********************************************************************/
void
CheckAlarms(void)
{
	/*Торцевая дверь*/
	if (fatevent_buf&0xFF00){
		SET_STATE_T_FATAL_ERR;
	}
	else {
		CLR_STATE_T_FATAL_ERR;
	}
	if (remevent_buf&0xFF00){
		SET_STATE_T_REMOVABLE_ERR;
	}
	else{
		CLR_STATE_T_REMOVABLE_ERR;
	}
	/*Боковая дверь*/
	if (fatevent_buf&0x00FF){
		SET_STATE_B_FATAL_ERR;
	}
	else {
		CLR_STATE_B_FATAL_ERR;
	}
	if (remevent_buf&0x00FF){
		SET_STATE_B_REMOVABLE_ERR;
	}
	else{
		CLR_STATE_B_REMOVABLE_ERR;
	}	
}
/********************************************************************
	Функция формирования данных для отправки
********************************************************************/
void
Formation_Of_Data_To_Send(void)
{
	CheckAlarms();
	/*вначале смотрим тип двери*/
	if(Door.type==DOOR_TYPE_SIDE){
		/*
		//Открыто
		if (Door.state==MODE_OPEN)  SET_STATE_B_OPEN;
		else						CLR_STATE_B_OPEN;
		
		//Закрыто
		if ((Door.state==MODE_CLOSE)
			||(Door.state==MODE_BLOKING)
			||(Door.state==MODE_BLOKED)) 	SET_STATE_B_CLOSE;
		else						 		CLR_STATE_B_CLOSE;
		*/
		
		 //При необходимости заменить на это
		//Открыто
		if (S_OPEN)  	SET_STATE_B_OPEN;
		else			CLR_STATE_B_OPEN;
		
		//Закрыто 
		if ((S_CLOSED)&&(S_COMPRESSED_2)&&(S_COMPRESSED_1) )	SET_STATE_B_CLOSE;
		else			CLR_STATE_B_CLOSE;
		
		
		
		/*Дверь поджата*/
		if((S_COMPRESSED_2)&&(S_COMPRESSED_1))  SET_STATE_B_INWARD;
		else 								 	CLR_STATE_B_INWARD;
		
		/*Заперто на ключ*/
		if(S_LOCK)	SET_STATE_B_LOCK;
		else		CLR_STATE_B_LOCK;
		
		/*Заперто "секретка"*/
		if(S_SEKRETKA)	SET_STATE_B_SEKRETKA;
		else			CLR_STATE_B_SEKRETKA;
		
		/*Подножка сложена*/
		if(S_FOOTBOARD)	SET_STATE_B_FOOTBOARD;
		else			CLR_STATE_B_FOOTBOARD;
		
		/*Дверь заблокированна*/
		if ((Door.state==MODE_BLOKED)||(Door.state==MODE_KEYLOCK))//||(S_LOCK)||(S_SEKRETKA))	
				SET_STATE_B_FREEZING;
		else	CLR_STATE_B_FREEZING;
		
		/*Помеха в створе*/
		if((S_DIST_AL_1)||(S_DIST_AL_2))	SET_STATE_B_PARASITE;
		else								CLR_STATE_B_PARASITE;
		
		#warning Доделать
		/*Поддерживает режим работы - ручной*/
		
		/*Поддерживает режим работы - автоматический*/
		
		/*Скорость более 5 км/ч */
		if(S_V5)	SET_STATE_B_V5;
		else		CLR_STATE_B_V5;
		
		/*Скорость более 45 км/ч */
		if(S_V45)	SET_STATE_B_V45;
		else 		CLR_STATE_B_V45;
		
		/*Текущий режим работы: 0 – ручной, 1 – автоматический*/
		if(Door.state!=MODE_MANUAL)	SET_STATE_B_MANUAL_MODE;
		else						CLR_STATE_B_MANUAL_MODE;
		
		/*Нажата кнопка открыть*/
		if(S_KO)	SET_STATE_B_KO;
		else 		CLR_STATE_B_KO;
		
		/*Нажата кнопка закрыть*/
		if(S_KZ)	SET_STATE_B_KZ;
		else 		CLR_STATE_B_KZ;
		
		/********/
		/*НЕИСПРВНОСТИ */
		/*Двигатель М1*/
		if(bug_reg&BIT(BUG_M1))	SET_STATE_B_OPERABLE_M1;
		else 					CLR_STATE_B_OPERABLE_M1;
		
		/*Двигатель М2*/
		if(bug_reg&BIT(BUG_M2))	SET_STATE_B_OPERABLE_M2;
		else 					CLR_STATE_B_OPERABLE_M2;
		
		/*Двигатель М3*/
		if(bug_reg&BIT(BUG_M3))	SET_STATE_B_OPERABLE_M3;
		else 					CLR_STATE_B_OPERABLE_M3;
		
		/*Исправна муфта MF1 электродвигателя привода двери*/
		if(bug_reg&BIT(BUG_MF1))	SET_STATE_B_OPERABLE_MF1;
		else 						CLR_STATE_B_OPERABLE_MF1;
		
		/*Исправен бесконтактный индуктивный выключатель «Открыто» S11*/
		if(bug_reg&BIT(BUG_OPEN))	SET_STATE_B_OPERABLE_S11;
		else 						CLR_STATE_B_OPERABLE_S11;
		
		/*Исправен бесконтактный индуктивный выключатель «Положение» S9*/
		if(bug_reg&BIT(BUG_POS))	SET_STATE_B_OPERABLE_S9;
		else 						CLR_STATE_B_OPERABLE_S9;
		
		/*Исправен бесконтактный индуктивный выключатель «Закрыто» S10*/
		if(bug_reg&BIT(BUG_CLOSE))	SET_STATE_B_OPERABLE_S10;
		else 						CLR_STATE_B_OPERABLE_S10;
		
		/*Исправен бесконтактный индуктивный выключатель «Поджато 1» S1*/
		if(bug_reg&BIT(BUG_COMPRES1))	SET_STATE_B_OPERABLE_S1;
		else 							CLR_STATE_B_OPERABLE_S1;
		
		/*Исправен бесконтактный индуктивный выключатель «Поджато 2» S2*/
		if(bug_reg&BIT(BUG_COMPRES1))	SET_STATE_B_OPERABLE_S2;
		else 							CLR_STATE_B_OPERABLE_S2;
		
		/*Исправен соленоид Y1*/
		if(bug_reg&BIT(BUG_Y1))	SET_STATE_B_OPERABLE_Y1;
		else 					CLR_STATE_B_OPERABLE_Y1;
		
		/*Исправен соленоид Y2*/
		if(bug_reg&BIT(BUG_Y2))	SET_STATE_B_OPERABLE_Y2;
		else 					CLR_STATE_B_OPERABLE_Y2;
		
		/*Исправны цепи защитных кромок S12, S13*/
		if(bug_reg&BIT(BUG_PROTECTIVE_EDGE))	SET_STATE_B_OPERABLE_S12;
		else 									CLR_STATE_B_OPERABLE_S12;
				
		/*Исправен датчик «Мёртвая точка 1» S3*/
		if(bug_reg&BIT(BUG_LOGJAM1))	SET_STATE_B_OPERABLE_S3;
		else 							CLR_STATE_B_OPERABLE_S3;
		
		/*Исправен датчик «Мёртвая точка 2» S4*/
		if(bug_reg&BIT(BUG_LOGJAM2))	SET_STATE_B_OPERABLE_S4;
		else 							CLR_STATE_B_OPERABLE_S4;
			
		/*Ошибка закрытия на ключ*/
	/*	if(bug_reg&BIT(BUG_LOCK))		SET_STATE_B_LOCKERR;
		else 							CLR_STATE_B_LOCKERR;
	*/
		
		/*Время открытия/закрытия двери*/
		buff_state[4]=time_opcls;

		
		/*Версия прошивки*/
		buff_state[6]=FIRMWARE_VERSION;
	}
	/*===Для торцевой двери===*/
	else{
		/*
		//Открыто
		if ((Door.state==MODE_OPEN)
			||((Door.state==MODE_CLOSING)&&(door_T_close_step==0)))  SET_STATE_T_OPEN;
		else						CLR_STATE_T_OPEN;
		
		//Закрыто
		if (Door.state==MODE_CLOSE)  SET_STATE_T_CLOSE;
		else						 CLR_STATE_T_CLOSE;		
		*/
		
		// При необходимости заменить на это
		//Открыто
		if (S_OPEN)  	SET_STATE_T_OPEN;
		else			CLR_STATE_T_OPEN;
		
		//Закрыто
		if (S_CLOSED )	SET_STATE_T_CLOSE;
		else			CLR_STATE_T_CLOSE;
		
		
		
		/*Заперто на ключ*/
		if(S_LOCK)	SET_STATE_T_LOCK;
		else		CLR_STATE_T_LOCK;
		
		/*Заперто "секретка"*/
		if(S_SEKRETKA)	SET_STATE_T_SEKRETKA;
		else			CLR_STATE_T_SEKRETKA;
		
		/*Текущий режим работы: 0 – ручной, 1 – автоматический*/
		if(Door.state!=MODE_MANUAL)	SET_STATE_T_MANUAL_MODE;
		else						CLR_STATE_T_MANUAL_MODE;
		
		/*Наличие неисправности*/
		#warning TODO
		/*Нажата кнопка открыть*/
		if(S_KO)	SET_STATE_T_KO;
		else 		CLR_STATE_T_KO;
		
		#warning Доделать
		/*Поддерживает режим работы - ручной*/
		
		/*Поддерживает режим работы - автоматический*/
		
		/*Помеха в створе*/
		if((S_DIST_AL_1)||(S_DIST_AL_2))	SET_STATE_T_PARASITE;
		else								CLR_STATE_T_PARASITE;
		
		/*Дверь заблокированна*/
		if (Door.state==MODE_BLOKED)	SET_STATE_T_FREEZING;
		else						 	CLR_STATE_T_FREEZING;
		
		/*Неисправности*/
		/*Исправен электродвигатель привода двери М1*/
		if(bug_reg&BIT(BUG_M1))	SET_STATE_T_OPERABLE_M1;
		else					CLR_STATE_T_OPERABLE_M1;
		
		/*Исправна муфта MF1 электродвигателя привода двери*/
		if(bug_reg&BIT(BUG_MF1))	SET_STATE_T_OPERABLE_MF1;
		else						CLR_STATE_T_OPERABLE_MF1;
		
		/*Исправен бесконтактный индуктивный выключатель «Открыто» S3*/
		if(bug_reg&BIT(BUG_OPEN))	SET_STATE_T_OPERABLE_S3;
		else						CLR_STATE_T_OPERABLE_S3;
		
		/*Исправен бесконтактный индуктивный выключатель «Положение» S1*/
		if(bug_reg&BIT(BUG_POS))	SET_STATE_T_OPERABLE_S1;
		else						CLR_STATE_T_OPERABLE_S1;
		
		/*Исправен бесконтактный индуктивный выключатель «Закрыто» S2*/
		if(bug_reg&BIT(BUG_CLOSE))	SET_STATE_T_OPERABLE_S2;
		else						CLR_STATE_T_OPERABLE_S2;
		
		/*Исправны цепи защитной кромки S6*/
		if(bug_reg&BIT(BUG_PROTECTIVE_EDGE))	SET_STATE_T_OPERABLE_S6;
		else									CLR_STATE_T_OPERABLE_S6;
		
		/*Температура в тамбуре*/
	//	if (rez[0]<0 ){
	//		buff_state[3]=-((int8_t)(rez[0]/10));
	//	}
	//	else{
			buff_state[3]=(int8_t)(rez[0]/10);
	//	}
		
		//buff_state[3]=-10;
		/*Время открытия/закрытия*/
		buff_state[4]=time_opcls;
		/*Версия прошивки*/
		buff_state[6]=FIRMWARE_VERSION;
	}
}

/********************************************
*	Инициализация CAN
*	door_ID - идендификатор двери
*******************************************/
void 
CAN_SetUp(uint8_t door_ID)
{
	/*Инициализируем SPI*/
	SPI_masterInit(); 
	/*Инициализируем mcp2510*/
	CAN_Init(CAN_PARAMS, CAN_BAUD_125);  
	/*Принимаем всё*/
	CAN_setAcceptFilter(CAN_PARAMS, 1, 0x7FF);
	CAN_setAcceptMask(CAN_PARAMS,0,0x0000);
	/*Поднастраиваем структурку сообщения*/
	out_msg.ID=door_ID;
	/*Сразу определяем тип двери*/
	if((out_msg.ID==BRAKE_END_LEFT_SIDE)||(out_msg.ID==BRAKE_END_RIGHT_SIDE)
							||(out_msg.ID==NONBRAKE_END_LEFT_SIDE)||(out_msg.ID==NONBRAKE_END_RIGHT_SIDE)){
		/*Боковая дверь*/
		Door.type=DOOR_TYPE_SIDE;
	}
	else if((out_msg.ID==BRAKE_END_FRONT)||(out_msg.ID==NONBRAKE_END_FRONT)){
		/* торцевая дверь*/
		Door.type=DOOR_TYPE_FRONT;
	}
	out_msg.DLEN=8;
	
	deb_msg.ID=DEBUG_OUT_ID;
	deb_msg.DLEN=8;
}
/***********************************************
*	Функция приёма и разбора КАН сообщений
************************************************/
void
CAN_ReadMsg(void)
{
	/*Локальный счётчик*/
	uint8_t i;
	/*Смотрим, а пришло ли сообщение?*/
	if(CAN_msg_Read(CAN_PARAMS, &in_msg)){
		/*Начинаем разбирать по полочкам*/
		switch(in_msg.ID){
			/*Комманда от БД*/
			case PACKAGE_COMMAND:{
				/*Хаваем только команду, отосланную нам*/
				//if(last_cmd!=in_msg.data[out_msg.ID-BRAKE_END_LEFT_SIDE]){
					last_cmd=in_msg.data[out_msg.ID-BRAKE_END_LEFT_SIDE];
					SET_FLAG_NEW_CMD;
				//}
				/*Принимаем температуру*/
				Tnar_EPVS=in_msg.data[6];
				/*Выставляем флаг*/
				
			}break;
			/*Настройки от БД*/
			case PACKAGE_SETTINGS:{
				/*Если настройки приходят для нас*/	
				if(events_flags&BIT(COM_STS)){
						
					/*Смотрим № страницы*/
					if (in_msg.data[0]==0x00){
						/*Забираем настройки*/
						for (i=0;i<7;i++){	
							buff_sts[i]=in_msg.data[i+1];
						}
						/*Ставим битик, что приняли нулевую страницу*/
						count_in_str|=BIT(0);
					}
					else if(in_msg.data[0]==0x01){
						/*Забираем настройки*/
						for (i=0;i<7;i++){	
							buff_sts[i+7]=in_msg.data[i+1];
						}
						/*Ставим битик, что приняли первую страницу*/
						count_in_str|=BIT(1);
					}
					else if(in_msg.data[0]==0x02){
						/*Забираем настройки*/
						for (i=0;i<7;i++){	
							buff_sts[i+14]=in_msg.data[i+1];
						}
						/*Ставим битик, что приняли первую страницу*/
						count_in_str|=BIT(2);
					}
					/*выставляем флаг, если приняли все страницы*/
					if (count_in_str==0x07){
						SET_FLAG_STS_TAKEN;
						/*Сбрасываем счётчик*/
						count_in_str=0x00;
					}
				}
			}break;
			/*CAN_MAN*/
			case DEBUG_IN_ID:{
				/*Если этот пакет не нам, то выходим*/
				if(in_msg.data[0]!=out_msg.ID) return;
				else{
						/*Ставим флаг, что нужно отправлять дебаг-пакеты*/
						send_deb=TRUE;
						/*Засекаем обратный отсчёт, по истечении этого времени флаг, который выше станет =0*/
						ts_send_deb=1000;
						/*Если от нас хотят ручное управление по кану, то...*/
						if(in_msg.data[1]&BIT(0)){
							/*Засекаем счётчик управления по кану. Когда он истечёт, следущий флаг станет = 0*/
							ts_CAN_man=1000;
							/*Ставим флаг, что управляемся по кану...*/
							CAN_management=TRUE;
							/* Направление и ШИМ*/
							Set_PWM(in_msg.data[1]&BIT(1),in_msg.data[2]);
							/*Муфта*/
							if(in_msg.data[1]&BIT(2)) 	CM_ON
							else 						CM_OFF;
							/*Двигатель М2*/
							if(in_msg.data[1]&BIT(3))	CM2_ON;
							else						CM2_OFF;
							/*Двигатель М3*/
							if(in_msg.data[1]&BIT(4))	CM3_ON;
							else						CM3_OFF;
							/*Управляющая катушка соленоида Y1*/
							if(in_msg.data[1]&BIT(5))	CY1U_ON;
							else						CY1U_OFF;
							/*Пусковая катушка соленоида Y1*/
							if(in_msg.data[1]&BIT(6))	CY1P_ON;
							else						CY1P_OFF;
							/*Пусковая катушка соленоида Y1*/
							if(in_msg.data[1]&BIT(6))	CY1P_ON;
							else						CY1P_OFF;
							/*Удерживающая катушка соленоида Y2*/
							if(in_msg.data[1]&BIT(7))	CY2U_ON;
							else						CY2U_OFF;
						}
						else CAN_management=FALSE;
					}
			}break;
			/*Неизвестные пакеты не обрабатываем*/
			default:return;
		}
	}
	//else CLEAR_FLAG_NEW_CMD;
}
/********************************************
*	Функция обнуления КАН- сообщения
*******************************************/
void
CAN_ResetMSG(void)
{	uint8_t i;
	for(i=1;i<out_msg.DLEN;i++)
		out_msg.data[i]=0x00;

}
/********************************************
*	Функция формирования КАН- сообщения
*******************************************/
void
CAN_FormationPackage(void)
{
	uint8_t i;
	uint16_t temp;
	CAN_ResetMSG();
//	out_msg.data[0]=current_out_str;
	switch(out_msg.data[0]){
		case 0:{
			for(i=0;i<4;i++){
				out_msg.data[i+1]=buff_state[i];
			}
			/*Алгоритмические ошибки*/
			/* 5й байт под неустранимые*/
			out_msg.data[5] = (uint8_t)((fatevent_buf&0x00FF)|( (fatevent_buf&0xFF00)>>8 ));
			/* 6й байт под устранимые*/
			out_msg.data[6] = (uint8_t)((remevent_buf&0x00FF)|( (remevent_buf&0xFF00)>>8 ));
			out_msg.data[7] =0x00;

		} break;
		case 1:{
			/*Контрольная сумма настроек для двери*/
			temp=crc16(&buff_sts[0],21);
			/*CRC.h*/
			out_msg.data[1]=(uint8_t)(temp>>8);
			/*CRC.l*/
			out_msg.data[2]=(uint8_t)(temp&0x00FF);
			/*Настроен ли БУ*/
			out_msg.data[3]=state_reg;
			
			/*Дополнительные двери*/
			if(en_side_doors) {
				out_msg.data[4]=en_side_doors;
				if (S_HAND_LS_LOCK) 	out_msg.data[4]|=BIT(2);
				else					out_msg.data[4]&=~BIT(2);
				if (S_HAND_LS_SECRET)	out_msg.data[4]|=BIT(3);
				else					out_msg.data[4]&=~BIT(3);
				if (S_HAND_RS_LOCK)		out_msg.data[4]|=BIT(4);
				else					out_msg.data[4]&=~BIT(4);
				if (S_HAND_RS_SECRET) 	out_msg.data[4]|=BIT(5);
				else 					out_msg.data[4]&=~BIT(5);
			}
			out_msg.data[5] = buff_state[4];
			out_msg.data[6] = buff_state[5];
			out_msg.data[7] = buff_state[6];

		} break;
		/*Настройки*/
		case 2:{
			for (i=0;i<7;i++){	
				out_msg.data[i+1]=buff_sts[i];
			}
		}break;
		case 3:{
			for (i=0;i<7;i++){	
				out_msg.data[i+1]=buff_sts[i+7];
			}
		}break;
		case 4:{
			for (i=0;i<7;i++){	
				out_msg.data[i+1]=buff_sts[i+14];
			}
		}break;
		default: return;
	}
}
/********************************************
*	Функция отправки сообщения для БД
*******************************************/
void
CAN_Thread(void)
{
	/*Приём сообщения от БД(а может и не только от БД*/
	CAN_ReadMsg();
	/*Выжидаем интервал отправки сообщений*/
	if(time_interval(ts_sendcan)<INTERVAL_BETWEEN_SENDING) return; 

	/*Собираем данные для формирования очередного пакета*/
	Formation_Of_Data_To_Send();
	/*Формируем очередной пакет*/
	CAN_FormationPackage();
	/*Ну и пускаем его в сеть*/

	CAN_Msg_Send(CAN_PARAMS,&out_msg);
	ts_sendcan=timer0_getCounter();
	/*Меняем номер отправляемой страницы*/
//	current_out_str++;
out_msg.data[0]++;
	/*МегаБубен*/
	if(events_flags&BIT(GET_STS)){
		//if (current_out_str>4)	current_out_str=0;
		if (out_msg.data[0]>4)	out_msg.data[0]=0;
	}
	//else if(current_out_str>1) current_out_str=0;
	else if(out_msg.data[0]>1) out_msg.data[0]=0;
	
}