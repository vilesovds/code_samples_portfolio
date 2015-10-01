
// ��������
#define D_OUT1   3//0
#define D_OUT2   5// 3
#define D_POT1   0// 1
#define D_POT2   4//4
#define D_REC1   1// 2
#define D_REC2   2// 5

//��������� ��������
#define DAMPER_CLOSE 20
#define DAMPER_P1    45
#define DAMPER_P2    50
#define DAMPER_P3    100
#define DAMPER_OPEN  100
#define DAMPER_P4 60 // 

// ��������� ��� ���������
#define Tout1_ch    1
#define Tout2_ch    5
#define Tprit1_ch   2
#define Tprit2_ch   6
#define Trec1_ch    3
#define Trec2_ch    7
#define Tin1_ch     0
#define Tin2_ch     4

// ����������
#define VVK1       0//7
#define VVK2       1//6

// ������������ ���������� (���������) DataLED
#define HEAT_LED2_2       7//0
#define HEAT_LED1_2       0//1
#define ALARM_LED_2       1//2
#define WORK_LED_2        2//3
#define HEAT_LED2_1       3//4
#define HEAT_LED1_1       4//5
#define ALARM_LED_1       5//6
#define WORK_LED_1        6//7

#define SIMULATION       7

// ������ ������ WORK_MODE
#define MANUAL_MODE   7   // ������ �����
#define FIRST_RUN     6   // ������ ����
#define AVTOM_MODE    5   // �������������� �����
#define BLASTING      4   // �������� ��������

#define HEATING2      3   // ��������� (� ������ ���� �����)
#define BAKE          2   // �������
#define HEATING       1   // ���������
#define VENTILATION   0   // ����������


// �������� ������ FlagWorkRegister
#define Flag100msec                0   // �������� ������� (������ 100 ��)
#define FlagReadDataADC            1   // 1 ��� ���� ������ �������� (TimerReadTemperature)
#define FlagRunAlgoritm            2   // 15 ��� ���� �������� ������� ��� (TimerRunAlgoritm)
#define FlagChangeMode             3   // 3 ��� �������� ������������ ��������� (TimerChangeMode)
#define FlagTimerOffBake           4   // 1 ��� ����� ������ � ������ �������� (TimerOffBake)
#define FlagDelayVvkAfterDamper    5   // 30 ��� �������� �� ��������� ��� ����� ��������� ��������
#define FlagSaveStatusVvk          10   // ������ 1 ��� ����������� ��������� �����������
#define FlagGoToHeat               7   // 
#define FlagGoToVent               8   // 
#define FlagTimerChangeVent        9   // 
#define FlagTimerChangeHeat       11  // 
//#define FlagDelayDamperPOT         6  // �������� 1 ��� ����� ��������� ��� �� ������ ��������� � ������
#define FlagDelayOffVVK1           12  // �������� 1 ��� �� ��������� ���1 ����� ��� ����������
#define FlagDelayOffVVK2           13  // �������� 1 ��� �� ��������� ���2 ����� ��� ����������
#define FlagFirstRun               14  // �������� 3 ��� ������������� ��������
#define FlagOUT                    15  // �������� 3 ��� ������������� ��������


// Flag2WorkRegisster
#define FLAG2GOTOBAKE				0 // �������� 1 ��� �������� �� ���������� � �������



// ��������� �������
#define dt1                    15   // ���� �������� ������� 15 ���.
#define DTAU2                  1    // ������ ������ �������� ����������� 1 ���.
#define DTAU4                	60   // 1 ��� �������� �� ������� �� ������ � �����
#define DTAU3                  5    // �������� 5 ��� ������������� ��������
#define dt15BP                 60   // ������ 1 ��� ����������� ��������� �����������
#define DTAU6                 30   // ��������  ����� ��������� �������� �� ������ ��������� � ������
#define DTAU8                 15   // �������� 15 ��� �� ��������� ��� ����� ��� ����������
#define dt18BP                 60   // �������� 60 ��� ����� ������ � ������ ��������
#define DTAU9					0 //�������� �������� �� ��������� � �������
#define DTAU14					180 //����� �������� ����������� ����. ������� � ������ ��� �������� ��� �� "1" � "2" �� ���
//������������
#define DTAU10 					0x3C // ������ ��������� �������� �����������
#define DTVVMAX					1 // ����������� ������� ���������� �� ������ DTAU10(���� ��������)
//#define MINUS_DTVVMAX			1//-1 
//================================================================================
#define DELTATAU11				60// 1���. ������������ ����������� �������� ����������� ������� �� ����� ������ ������.
#define DELTATAU12				180 //3��� ����� �������� ��� �������� t���� MAX
#define DTAU13					100 // 100*6 = 600 ������ =>10 �����

#define DELTATAU15				60 // 1 ��� ����� �������� �� ��������� ��������� ������ ����������������������� 
									//��� ��� ��������� ����������� ������� � ������
#define DTAU16 					960// 16 ����� �� ��������/��������  ��������
#define DTAU_WD 				150 // ����� �������� ������������ �������� 
#define DTAU18					30//
//=================================================================================================================================
#define FIVESECOND				5 // ������������ ����������� �������� ��������� �������� �����������. 
// ��������� ����������
#define DTVZ                   20       // 2 ������a
#define dTvz_min               10       // 1 ������
#define dTvz_max               10       // 1 ������
#define dTprit1                10       // 1 ������
#define dTprit1_1              10       // 1 ������
#define dTprit1_2              20      // 2 ������

#define dTprit2                30       // 3 ������a
#define dTprit3                100      // 10 ��������
#define dTprit4                50       // 5 ��������
#define dTprit5                100      // 10 ��������

//#define Tprit1                 200      // 20 ��������
#define Tprit1_1               220      // 22 �������
#define Tprit1_2               240      // 24 �������
#define Tprit1_3               260      // 26 ��������
#define Tprit2                 350      // 35 ��������
#define Tprit3                 500      // 50 ��������
#define Tprit4                 600      // 60 ��������
#define Tprit4_1               620      // 62 ��������
#define Tprit4_2               640      // 64 ��������
#define Tprit4_3               660      // 66 ��������
#define Tprit4_4               680      // 68 ��������

#define Tprit5                 700      // 70 ��������
//==========================================================================================
//��������� ����������� ��������� �������
#define TNGR1					160
#define TNGR2					0
#define TNGR3					280	//-28 ��������
//
#define DELTATVZ1				0//1//0 // 0.1/1/ ������ ���������� ���������� �� �������� ����������� ������� � ������ t�.�. 
									// � � �������, � � ������� ������� ��� �������������� �������������
#define DELTATVZ2				20 // 2 ������� ���������� ���������� �� �������� ����������� ������� � ������ t�.�. 
									// � � �������, � � ������� ������� ��� �������������� �������������
#define DELTATVZ3				25 // 2.5 ������� ���������� ���������� �� �������� ����������� ������� � ������ t�.�. 
									// � � �������, � � ������� ������� ��� �������������� �������������
//
// ��������� ����������� ���������� �������
#define TPRITGR1				400
#define TPRITGR2				350
#define TPRITGR3				500
#define TPRITGR4				600
#define TPRITGR5				700
////////////////////////////////////////////////////////////////////////////////////////
//
//                                                                T���� MIN
//
//==================================================================================
//���������� ���������� �� ����������� ����������� ���������� �������  � ������� �������
#define TPRITMIN1			160
//���������� ���������� �� ����������� ����������� ���������� �������  � ������� �������
#define TPRITMIN2			161
//���������� �������� ���������� ������� �� ����� ������ ������
#define DELTATVDLD				10
#define MINUS_DELTATVDLD		0xFFFB //-5 ��������

#define DTV02	2 // 0.2 ������� �
#define MINUSDTV02 0xFFFB // - 0.2 ������� 
//===========================================================================================



#define BR_dT9                 300      //5*60 ������ ��������� �������� ��������
#define BR_dT10                 300      //5*60 ������ ��������� �������� ��������


#define STARTTIMECLOSEDAMP  0
#define STARTTIMEOPENDAMP   1

//������� ���������
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