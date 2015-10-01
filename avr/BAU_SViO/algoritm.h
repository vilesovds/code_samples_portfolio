
// Заслонки
#define D_OUT1   3//0
#define D_OUT2   5// 3
#define D_POT1   0// 1
#define D_POT2   4//4
#define D_REC1   1// 2
#define D_REC2   2// 5

//Положения заслонок
#define DAMPER_CLOSE 20
#define DAMPER_P1    45
#define DAMPER_P2    50
#define DAMPER_P3    100
#define DAMPER_OPEN  100
#define DAMPER_P4 60 // 

// Константы для алгоритма
#define Tout1_ch    1
#define Tout2_ch    5
#define Tprit1_ch   2
#define Tprit2_ch   6
#define Trec1_ch    3
#define Trec2_ch    7
#define Tin1_ch     0
#define Tin2_ch     4

// Контакторы
#define VVK1       0//7
#define VVK2       1//6

// Светодиодные индикаторы (единичные) DataLED
#define HEAT_LED2_2       7//0
#define HEAT_LED1_2       0//1
#define ALARM_LED_2       1//2
#define WORK_LED_2        2//3
#define HEAT_LED2_1       3//4
#define HEAT_LED1_1       4//5
#define ALARM_LED_1       5//6
#define WORK_LED_1        6//7

#define SIMULATION       7

// Режимы работы WORK_MODE
#define MANUAL_MODE   7   // Ручной режим
#define FIRST_RUN     6   // Первый пуск
#define AVTOM_MODE    5   // Автоматический режим
#define BLASTING      4   // Продувка датчиков

#define HEATING2      3   // Отопление (Т внутри выше нормы)
#define BAKE          2   // Прогрев
#define HEATING       1   // Отопление
#define VENTILATION   0   // Вентиляция


// Описание флагов FlagWorkRegister
#define Flag100msec                0   // Тактовая частота (период 100 мс)
#define FlagReadDataADC            1   // 1 сек такт опроса датчиков (TimerReadTemperature)
#define FlagRunAlgoritm            2   // 15 сек такт принятия решения БАУ (TimerRunAlgoritm)
#define FlagChangeMode             3   // 3 мин Задержка переключения подрежима (TimerChangeMode)
#define FlagTimerOffBake           4   // 1 час время работы в режиме прогрева (TimerOffBake)
#define FlagDelayVvkAfterDamper    5   // 30 сек Задержка на включение ВВК после установки заслонок
#define FlagSaveStatusVvk          10   // Период 1 мин запоминания состояния контакторов
#define FlagGoToHeat               7   // 
#define FlagGoToVent               8   // 
#define FlagTimerChangeVent        9   // 
#define FlagTimerChangeHeat       11  // 
//#define FlagDelayDamperPOT         6  // Задержка 1 мин между переводом ЗПВ из одного положения в другое
#define FlagDelayOffVVK1           12  // Задержка 1 мин на включение ВВК1 после его отключения
#define FlagDelayOffVVK2           13  // Задержка 1 мин на включение ВВК2 после его отключения
#define FlagFirstRun               14  // Задержка 3 мин предстартовая продувка
#define FlagOUT                    15  // Задержка 3 мин предстартовая продувка


// Flag2WorkRegisster
#define FLAG2GOTOBAKE				0 // Задержка 1 мин перехода из нагревания в прогрев



// Константы времени
#define dt1                    15   // Такт принятия решения 15 сек.
#define DTAU2                  1    // Период опроса датчиков температуры 1 сек.
#define DTAU4                	60   // 1 мин Задержка на переход из режима в режим
#define DTAU3                  5    // Задержка 5 сек предстартовая продувка
#define dt15BP                 60   // Период 1 мин запоминания состояния контакторов
#define DTAU6                 30   // Задержка  между переводом заслонок из одного положения в другое
#define DTAU8                 15   // Задержка 15 сек на включение ВВК после его отключения
#define dt18BP                 60   // Задержка 60 мин время работы в режиме прогрева
#define DTAU9					0 //задержка перехода из отопления в прогрев
#define DTAU14					180 //время задержки определения темп. воздуха в салоне при переводе ППВ из "1" в "2" по ДТС
//нововведения
#define DTAU10 					0x3C // Период измерения разности температуры
#define DTVVMAX					1 // оптимальная разница температур за период DTAU10(Типа скорость)
//#define MINUS_DTVVMAX			1//-1 
//================================================================================
#define DELTATAU11				60// 1мин. Дискретность определения разности температуры воздуха по длине салона вагона.
#define DELTATAU12				180 //3мин Время ожидания при контроле tприт MAX
#define DTAU13					100 // 100*6 = 600 секунд =>10 минут

#define DELTATAU15				60 // 1 мин Время задержки на повторное изменение уровня теплопроизводительности 
									//СВО при изменении температуры воздуха в салоне
#define DTAU16 					960// 16 минут на закрытие/открытие  заслонок
#define DTAU_WD 				150 // время ожидания установления заслонок 
#define DTAU18					30//
//=================================================================================================================================
#define FIVESECOND				5 // Дискретность определения скорости изменения салонной температуры. 
// Константы температур
#define DTVZ                   20       // 2 градусa
#define dTvz_min               10       // 1 градус
#define dTvz_max               10       // 1 градус
#define dTprit1                10       // 1 градус
#define dTprit1_1              10       // 1 градус
#define dTprit1_2              20      // 2 градус

#define dTprit2                30       // 3 градусa
#define dTprit3                100      // 10 градусов
#define dTprit4                50       // 5 градусов
#define dTprit5                100      // 10 градусов

//#define Tprit1                 200      // 20 градусов
#define Tprit1_1               220      // 22 градуса
#define Tprit1_2               240      // 24 градуса
#define Tprit1_3               260      // 26 градусов
#define Tprit2                 350      // 35 градусов
#define Tprit3                 500      // 50 градусов
#define Tprit4                 600      // 60 градусов
#define Tprit4_1               620      // 62 градусов
#define Tprit4_2               640      // 64 градусов
#define Tprit4_3               660      // 66 градусов
#define Tprit4_4               680      // 68 градусов

#define Tprit5                 700      // 70 градусов
//==========================================================================================
//Граничные температуры наружного воздуха
#define TNGR1					160
#define TNGR2					0
#define TNGR3					280	//-28 градусов
//
#define DELTATVZ1				0//1//0 // 0.1/1/ градус Допустимое отклонение от заданной температуры воздуха в салоне tв.з. 
									// и в большую, и в меньшую сторону при автоматическом регулировании
#define DELTATVZ2				20 // 2 градуса Допустимое отклонение от заданной температуры воздуха в салоне tв.з. 
									// и в большую, и в меньшую сторону при автоматическом регулировании
#define DELTATVZ3				25 // 2.5 градуса Допустимое отклонение от заданной температуры воздуха в салоне tв.з. 
									// и в большую, и в меньшую сторону при автоматическом регулировании
//
// Граничные температуры приточного воздуха
#define TPRITGR1				400
#define TPRITGR2				350
#define TPRITGR3				500
#define TPRITGR4				600
#define TPRITGR5				700
////////////////////////////////////////////////////////////////////////////////////////
//
//                                                                Tприт MIN
//
//==================================================================================
//допустимое отклонение от минимальной температуры приточного воздуха  в меньшую сторону
#define TPRITMIN1			160
//допустимое отклонение от минимальной температуры приточного воздуха  в большую сторону
#define TPRITMIN2			161
//допустимая разность температур воздуха по длине салона вагона
#define DELTATVDLD				10
#define MINUS_DELTATVDLD		0xFFFB //-5 градусов

#define DTV02	2 // 0.2 градуса и
#define MINUSDTV02 0xFFFB // - 0.2 градуса 
//===========================================================================================



#define BR_dT9                 300      //5*60 секунд удержание закрытых заслонок
#define BR_dT10                 300      //5*60 секунд удержание открытых заслонок


#define STARTTIMECLOSEDAMP  0
#define STARTTIMEOPENDAMP   1

//рабочие константы
#define VOZRASTAET   1
#define NEMENYAETSYA 0
#define UBYVAET		 2

void WORK_DUBLE(void);
void TimersAlgoritm(void);
void ReadTemperature(void);
void FirstRun(void);
void BakeRun(void);
void HeatingRun(void);
void VentilationRun(void);

void BlinkingIndikator(unsigned char IND);
signed int CalkulationToutMDL (void);
signed int CalkulationTinMDL (void);
signed int CalkulationTpritMAX (void);
signed int CalkulationTpritMIN (void);
unsigned char TestingDamper (unsigned char NDamper);
void RunDamper (unsigned char NDamper, unsigned char data);
void ChangeMode (unsigned char mode);
void RegulationBake(void);
void RegulationHeating(void);
void RegulationHeating2(void);
void VVK_ON(unsigned char VVK);
void VVK_OFF(unsigned char VVK);
void CheckErrorAndAlarm(void);
void CheckErrorDamper(void);
void change_loh(char loh);
char Loh_Up(void);
char Loh_Down(void);
void WorkingWithPritMax(void);
void WorkingWithPritMin(void);
void Speedometr(void);