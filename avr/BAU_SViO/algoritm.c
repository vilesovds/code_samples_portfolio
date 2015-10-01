
/*
*<= 0 =>	14.04.2009 �. ����� ����� ������� � ������� ��������� ��� :
		 �� ������ �������� ��� ������ ������ �� ��������� ��������. ��������. ����� ������ 2.6
*/
#include <PLATFORM.h>

#include "algoritm.h"
#include <types.h>
#include <macros.h>
#include "main.h"

// ������� ����������� ��������
#define Speed1
//{
uint16_t	FlagWorkRegister=0;
uint8_t 			Flag2WorkRegister=0;

uint8_t	FlagRegVacoom=0;
uint16_t	TimerOpenDampVagon=0;
uint16_t	TimerCloseDampVagon=0;



uint8_t WORK_MODE=0x10;
uint8_t ALARM_REG=0;


uint8_t STEP_COUNT=0;
uint8_t STEP_PRIT=0;
uint8_t TIMER_START_VENTILATION=0;
uint8_t TIMER_STOP_VENTILATION=0;
uint8_t Timer_Start_Bake=0;

uint8_t TimerOUTDamper[8];
uint8_t REG_FlagOUTDamper=0x00;

uint8_t TimerOUT=0x0;

uint8_t TimerDelayDamper[8];
uint8_t REG_FlagDelayDamper=0xFF;

int16_t  T_OUTSIDE1=0;
int16_t  T_OUTSIDE2=0;
int16_t  T_OUTSIDE=0;

int16_t  T_INSIDE1=0;
int16_t  T_INSIDE2=0;
extern int16_t  T_INSIDE=0;

int16_t  T_PRIT1=0;
int16_t  T_PRIT2=0;
int16_t  T_PRIT=0;

int16_t  Tprit1=0;
int16_t  Tprit9=0;

int16_t  DTS1=0;
int16_t  DTS2=0;
int16_t  DTNV1=0;
int16_t  DTNV2=0;
int16_t  DTRV1=0;
int16_t  DTRV2=0;
int16_t  DTPRV1=0;
int16_t  DTPRV2=0;

const int16_t minus_50=0xFE0C;   // - 50 ��������
const int16_t minus_20=0xFF38;   // - 20 ��������
const int16_t minus_5=0xFFCE;   // - 5 ��������
const int16_t minus_2=0xFFEC;   // - 2 ������a

uint8_t TimerReadTemperature=0;
uint8_t TimerRunAlgoritm=0;
uint8_t TimerDelayVvkAfterDamper=0;
uint8_t TimerSaveStatusVvk=0;
uint8_t TimerDelayOffVVK1=0;
uint8_t TimerDelayOffVVK2=0;
uint8_t TimerFirstRun=0;


uint8_t TimerOffBake=0;
uint8_t Timer_perevod_v_pol=0;
uint16_t CountVVK1=0;
uint16_t CountVVK2=0;



int16_t M_DataADC[8];              //������� �������� ���� ��� (�� 15���)
int16_t Array15_DataADC[8][dt1];


extern uint8_t DataIN;             // ���������� �����
extern uint8_t DataLED;            // ������������ ���������� (���������)
extern uint8_t DataOUT;            // ���������� ������
extern uint8_t QuantityADC;        // ���������� ������� ���
extern uint8_t ErrorReg;           // ������� ������

extern int16_t T_USTAVKI;               // �������� ����������� � ������
extern uint16_t TimerButtonClickOff;

extern uint8_t EnaStandByInfo;     // ������� ���������



extern int16_t MT_DataADC[8];       // �������� ���������� (������� �� 15 ��� ��� CAN)
extern int16_t T_DataADC[8];        // �������� ���������� (����������)
extern int16_t SIM_T_DATA[8];       // �������� ���������� �������������� � ��

extern uint8_t DamperDataIN[6];  // ��������� ��� ��������� ��������� ��������
extern uint8_t DamperDataOUT[6]; // ��������� ��� ������� ��������� ��������

uint32_t B_L=0x800E41F6; // 1000*B= -934390
uint16_t  K_L=0x8E7;     // 1000*K= 2279


uint16_t Tdamp[6];
uint8_t ErrorDamper=0;

extern uint8_t ErrorReg;

//������� �����������������������( the level of heaing)
uint8_t the_loh=0;
// ��. ������������ ��������
uint8_t NLoh=0;
//������� ������������������ �� ������� ��������� �����������
uint8_t the_loh_before_down=0;
uint8_t prit_down=0x00;
//������� ��������� ��������
uint8_t DamperZRV1=0;
uint8_t DamperZRV2=0;
// �������� �� ����������
int8_t dZRV1=0; 
int8_t dZRV2=0;

//������ ���������� ���������� �� ������� ������� ������������������
uint16_t Timer_may_up_loh=0;
uint16_t Timer_upr_prit_min_down=0;
uint16_t Timer_upr_prit_min_up=0;
uint16_t Timer_wait_dampers=DTAU_WD;
uint16_t Speed_Timer=0;

uint8_t start_close_prit=0;
uint8_t end_close_prit=0;
//
uint8_t work_tab7=1;
uint8_t Timer_work_tab7=0;
//
uint16_t Timer_close_damper=DTAU16;
uint8_t Timer_smeny_loh_up=0;
uint8_t Timer_smeny_loh_down=0;

// ��� � ��������� "1"
uint8_t PPVin1=0x00;
uint8_t workmin=0x00;
uint8_t cooler=0x00;
uint8_t begin_close=0x00;
// ����� ���������� ��������� �� �������/������� �������
uint8_t mayup=1;
uint8_t maydown=1;
uint8_t mayup_min=1;
uint8_t maydown_min=1;
// �������� ��������� ����������� ������
int16_t OLD_T_INSIDE=0;
int16_t SpeedTimer=0;
int16_t Timer_rec_data=0;
uint8_t Loh_Array[DTAU13+1];// ������ ��� ����������� �������� ������������� 
								//������ ������������������ �� ����� ������ ��� 10*6 ���
uint8_t Counter_6sec=0;
uint16_t Loh_Timer=0;
#ifdef Speed1
uint8_t Flag_SpeedDTV=0;
int16_t Speed_Array[DTAU10+1];
#endif
int16_t SPEEDDTV=0;
//
uint8_t count_10=0;
uint8_t FlagBlinkingIndikator=0;
//}
//===============================================================================================================================================================
//||					PROCEDURES																							||
//===============================================================================================================================================================
void 
WORK_DUBLE(void)
{
	CheckErrorAndAlarm();
	CheckErrorDamper();
	if (FlagWorkRegister&BIT(Flag100msec)) {
		TimersAlgoritm();
	}
	if (FlagWorkRegister&BIT(FlagReadDataADC))  ReadTemperature();
	T_OUTSIDE=CalkulationToutMDL(); // ����������� ����������� ��������� �������
	
	// ����������� ������ ������ ������/��������������
	if (0x03&DataIN){
		WORK_MODE|=BIT(MANUAL_MODE);
		WORK_MODE&=~BIT(AVTOM_MODE);
	}
	else{
		WORK_MODE&=~BIT(MANUAL_MODE);
		WORK_MODE|=BIT(AVTOM_MODE);
	}
	//----------------------------------------------------
    if (WORK_MODE&BIT(MANUAL_MODE)){   // ������ ����� ������ ��� (������)
		VVK_OFF(VVK1);
		VVK_OFF(VVK2);
		// ������� 2
		if (T_OUTSIDE<=minus_20){
			RunDamper(D_OUT1,DAMPER_P1);
			RunDamper(D_OUT2,DAMPER_P1);
			RunDamper(D_REC1,DAMPER_OPEN);
			RunDamper(D_REC2,DAMPER_OPEN);
		}
		else if ((T_OUTSIDE>minus_20)&&(T_OUTSIDE<=minus_5)){
			RunDamper(D_OUT1,DAMPER_P2);
			RunDamper(D_OUT2,DAMPER_P2);
			RunDamper(D_REC1,DAMPER_OPEN);
			RunDamper(D_REC2,DAMPER_OPEN);
		}
		else if (T_OUTSIDE>minus_5){
			RunDamper(D_OUT1,DAMPER_P3);
			RunDamper(D_OUT2,DAMPER_P3);
			RunDamper(D_REC1,DAMPER_P4);
			RunDamper(D_REC2,DAMPER_P4);
		}
	    RunDamper(D_POT1,DAMPER_CLOSE);
	    RunDamper(D_POT2,DAMPER_CLOSE);
		WORK_MODE|=BIT(FIRST_RUN);
	}
  //------------------------------------// ������ ����� ������ ��� (�����)
  
	else if (WORK_MODE&BIT(AVTOM_MODE)) {  // �������������� ����� ������ ��� (������)
      
		if (WORK_MODE&BIT(BLASTING)){ // �������� ��������
			/*�� ������*/
		}
		else if( FlagWorkRegister&BIT(FlagRunAlgoritm)){
			if (WORK_MODE&BIT(FIRST_RUN))			FirstRun();         // ����������� ��������� ��� ������ �����
			else if (WORK_MODE&BIT(BAKE))			BakeRun();          // ��������: ������� ������
			else if (WORK_MODE&BIT(HEATING))		HeatingRun();       // ��������: ����������� �������� �����������
			else if (WORK_MODE&BIT(HEATING2))		HeatingRun();		// ��������: ����������� �������� �����������	
			else if (WORK_MODE&BIT(VENTILATION))	VentilationRun();   // ��������: ����������
		}
	}
  
  //------------------------------------// �������������� ����� ������ ��� (�����)
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void BlinkingIndikator(uint8_t IND)
{
 if (FlagBlinkingIndikator) DataLED|=BIT(IND);
 else                        DataLED&=~BIT(IND);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void ReadTemperature(void)
{
   uint8_t i=0;
   uint8_t j=0;
   uint8_t k=0;
   int32_t tmp=0;
  // �������� ����� ��������� ���������� 
	for (j=0;j<8;j++){
		for (i=1;i<dt1;i++){
			if (WORK_MODE&BIT(BLASTING)){
				Array15_DataADC[j][i-1]=T_DataADC[j];
			}
			else{
				Array15_DataADC[j][i-1]=Array15_DataADC[j][i];
			}	
		}
		Array15_DataADC[j][dt1-1]=T_DataADC[j];
	}

   // ��������� �������� �������� (� ���� ����)
	for (j=0;j<8;j++){
		tmp=0;
		for (k=0;k<dt1;k++){
			tmp+=Array15_DataADC[j][k];
		}
	    M_DataADC[j]=(int16_t)(tmp/dt1);
		if (EnaStandByInfo&BIT(SIMULATION)){
			MT_DataADC[j]=T_DataADC[j];
		}
		else{
			MT_DataADC[j]=M_DataADC[j];
		}
	}	
     
	FlagWorkRegister&=~BIT(FlagReadDataADC);
 	DTS1=	MT_DataADC[Tin1_ch];
	DTS2=	MT_DataADC[Tin2_ch];
	DTNV1=	MT_DataADC[Tout1_ch];
	DTNV2=	MT_DataADC[Tout2_ch];
	DTRV1=	MT_DataADC[Trec1_ch];
	DTRV2=	MT_DataADC[Trec2_ch];
	DTPRV1=	MT_DataADC[Tprit1_ch];
	DTPRV2=	MT_DataADC[Tprit2_ch];
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void FirstRun(void)
{uint8_t k;
	T_OUTSIDE=CalkulationToutMDL(); // ����������� ����������� ��������� �������
	T_INSIDE=CalkulationTinMDL ();  // ����������� ����������� ������� ������
	OLD_T_INSIDE=T_INSIDE;
	workmin=0x00;
	cooler=0x00;
	Timer_close_damper=DTAU16;
		// �������������
#ifdef Speed1	
	for (k=0;k<DTAU10+1;k++){
		Speed_Array[k]=T_INSIDE;
	}
	Flag_SpeedDTV=0x01;
#endif	
	
	if (T_OUTSIDE>=T_USTAVKI){
	    if (ALARM_REG&BIT(VENTILATION)){}
		else{
			WORK_MODE&=0xF0;                // �������� ������������� ��������
			WORK_MODE|=BIT(VENTILATION);    // ������������� ����� ����������
			STEP_COUNT=0;                   // �������� ������� �����
			WORK_MODE&=~BIT(FIRST_RUN);
	    } 
	}
	else{
	    if ((T_INSIDE+DTVZ )>=T_USTAVKI){
			if (ALARM_REG&BIT(HEATING)){}
			else { 
				WORK_MODE&=0xF0;             // �������� ������������� ��������
				WORK_MODE|=BIT(HEATING);     // ������������� ����� ���������
				STEP_COUNT=0;                // �������� ������� �����
				WORK_MODE&=~BIT(FIRST_RUN);
			}	
		}
		else{
			if (ALARM_REG&BIT(BAKE)){}
			else{
				WORK_MODE&=0xF0;             // �������� ������������� ��������
				WORK_MODE|=BIT(BAKE);        // ������������� ����� �������
				STEP_COUNT=0;                // �������� ������� �����
				WORK_MODE&=~BIT(FIRST_RUN);
			}	
		}
	}

} 
////////////////////////////////////////////////////////////////////////////////////////////////////
void BakeRun(void)
{
	T_INSIDE=CalkulationTinMDL ();    // ����������� ����������� ������� ������
	T_PRIT=CalkulationTpritMAX ();    // ����������� ����������� ���������� �������
	T_OUTSIDE=CalkulationToutMDL ();  // ����������� ����������� ��������� �������

	if (ALARM_REG&BIT(BAKE)){
		WORK_MODE|=BIT(FIRST_RUN); // ��������� ������ ����
		WORK_MODE&=~BIT(BAKE);
		VVK_OFF(VVK1);
		VVK_OFF(VVK2);
	}
	else{  
		if (T_INSIDE<(T_USTAVKI-DTVZ)){
			switch (STEP_COUNT){
				case 0:
					RunDamper(D_OUT1,DAMPER_CLOSE);
					RunDamper(D_OUT2,DAMPER_CLOSE);
					RunDamper(D_REC1,DAMPER_OPEN);
					RunDamper(D_REC2,DAMPER_OPEN);
					RunDamper(D_POT1,DAMPER_CLOSE);
				    RunDamper(D_POT2,DAMPER_CLOSE);
					
				    STEP_COUNT++;  // ������� � ���������� ����
				break;
				case 1:
					RegulationBake();  // ��������� ���������� ���� � ������ ��������
				break;
			} 
		}
		else if ((T_INSIDE>=(T_USTAVKI-DTVZ))&&(T_INSIDE<(T_USTAVKI+DTVZ))){ //������� � ���������
				ChangeMode(HEATING); 
				STEP_COUNT=0;
				STEP_PRIT=0;
			
		}  
		/*else if ((T_INSIDE>=(T_USTAVKI+DTVZ))) {///������� � ����������?
				ChangeMode(VENTILATION);
				STEP_COUNT=0;
				STEP_PRIT=0;
		}*/
	}  
}
void RegulationBake(void)
{
	switch (STEP_PRIT){
	case 0:
		RunDamper(D_OUT1,DAMPER_CLOSE);
	    RunDamper(D_OUT2,DAMPER_CLOSE);
	    RunDamper(D_REC1,DAMPER_OPEN);
	    RunDamper(D_REC2,DAMPER_OPEN);
	    RunDamper(D_POT1,DAMPER_CLOSE);
	    RunDamper(D_POT2,DAMPER_CLOSE);
		
		if (T_PRIT<Tprit2){
			VVK_ON(VVK1);VVK_ON(VVK2);//+ +
		}
		else STEP_PRIT++;
	break;
	case 1:
	    RunDamper(D_OUT1,DAMPER_CLOSE);
	    RunDamper(D_OUT2,DAMPER_CLOSE);
	    RunDamper(D_REC1,DAMPER_OPEN);
	    RunDamper(D_REC2,DAMPER_OPEN);
	    RunDamper(D_POT1,DAMPER_CLOSE);
	    RunDamper(D_POT2,DAMPER_CLOSE);
		
		if (T_PRIT<(Tprit2-dTprit2)) STEP_PRIT--;
		else if (T_PRIT>=Tprit3)     STEP_PRIT++;
			else{
				VVK_OFF(VVK1);VVK_ON(VVK2);//-+
			}
	break;
	case 2:
	    RunDamper(D_OUT1,DAMPER_CLOSE);
	    RunDamper(D_OUT2,DAMPER_CLOSE);
	    RunDamper(D_REC1,DAMPER_OPEN);
	    RunDamper(D_REC2,DAMPER_OPEN);
	    RunDamper(D_POT1,DAMPER_CLOSE);
	    RunDamper(D_POT2,DAMPER_CLOSE);
		
		if (T_PRIT<(Tprit3-dTprit3)) STEP_PRIT--;
		else if (T_PRIT>=Tprit4)     STEP_PRIT++;
		else{
			VVK_ON(VVK1);VVK_OFF(VVK2);	//+-
		}
	 break;
	case 3:
		if (T_PRIT<(Tprit4-dTprit4)) STEP_PRIT--;
		else if (T_PRIT>=Tprit5)     STEP_PRIT++;
		else{
			VVK_ON(VVK1);VVK_OFF(VVK2);//+-
			
			
			if (T_PRIT<=Tprit4){
				RunDamper(D_OUT1,DAMPER_CLOSE);
				RunDamper(D_OUT2,DAMPER_CLOSE);	
			}
			else if ((T_PRIT>Tprit4)&&(T_PRIT<Tprit5)) {
				RunDamper(D_OUT1,(uint8_t)(DAMPER_CLOSE+ ((T_PRIT-600)*0x08)/0x0A));
				RunDamper(D_OUT2,(uint8_t)(DAMPER_CLOSE +((T_PRIT-600)*0x08)/0x0A));
			}
			RunDamper(D_REC1,DAMPER_OPEN);
			RunDamper(D_REC2,DAMPER_OPEN);
			RunDamper(D_POT1,DAMPER_CLOSE);
			RunDamper(D_POT2,DAMPER_CLOSE);
			
		}
	break;
	case 4:
		if (T_PRIT<(Tprit5-dTprit5)) STEP_PRIT--;
		else{
	        if (T_PRIT>=Tprit5) {
				RunDamper(D_OUT1,DAMPER_CLOSE);
				RunDamper(D_OUT2,DAMPER_CLOSE);	
			}	
			RunDamper(D_REC1,DAMPER_OPEN);
			RunDamper(D_REC2,DAMPER_OPEN);
			RunDamper(D_POT1,DAMPER_CLOSE);
			RunDamper(D_POT2,DAMPER_CLOSE);
			
			VVK_OFF(VVK1);VVK_OFF(VVK2);//--
			
		}
	break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////
// ������������� ������� ����������������������� loh : 0..3
//ok
void change_loh(uint8_t loh)
{
	if (loh>3) return;
	switch (loh){
		case 0:
			VVK_OFF(VVK1);VVK_OFF(VVK2);//--
		break;
		case 1:
			VVK_ON(VVK1);VVK_OFF(VVK2);//+-
		break;
		case 2:
			VVK_OFF(VVK1);VVK_ON(VVK2);//-+
		break;
		case 3:
			VVK_ON(VVK1);VVK_ON(VVK2);//++
		break;
	}
	//the_loh=loh;
}
//ok
uint8_t Loh_Up(void)
{uint8_t temp=the_loh;
	if (the_loh>=0x03) return 0;
	else if (the_loh<0x03){
			temp++;
			change_loh(temp);
			if (temp==the_loh) return 1;
			else return 0;
	}
	return 0;
}
	

//ok
uint8_t Loh_Down(void)
{uint8_t temp=the_loh;
	if (the_loh<=0) return 0;
	else if (the_loh>0){
			temp--;
			change_loh(temp);
			if (temp==the_loh) return 1;
			else return 0;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////
//�������� ��� ok
void ProcessDumpers(void)
{	int16_t dTvdl=0;
	uint8_t UREC;
	double temp_k=0;
	CalkulationTinMDL();
	dTvdl=T_INSIDE1-T_INSIDE2;
	
	//������� 2
	if(the_loh>0x00){
		cooler=0x00;
	}
	if (cooler==0x00){
		if (T_OUTSIDE<=minus_20){
			RunDamper(D_OUT1,DAMPER_P1);// ���������� ������� ���������
			RunDamper(D_OUT2,DAMPER_P1);
			RunDamper(D_REC1,DAMPER_OPEN+dZRV1);
			RunDamper(D_REC2,DAMPER_OPEN+dZRV2);
			UREC=DAMPER_OPEN;
		}
		else if ((T_OUTSIDE>minus_20)&&(T_OUTSIDE<=minus_5)){
			RunDamper(D_OUT1,DAMPER_P2);
			RunDamper(D_OUT2,DAMPER_P2);
			RunDamper(D_REC1,DAMPER_OPEN+dZRV1);
			RunDamper(D_REC2,DAMPER_OPEN+dZRV2);
			UREC=DAMPER_OPEN;
		}
		else if (T_OUTSIDE>minus_5){
			RunDamper(D_OUT1,DAMPER_P3);
			RunDamper(D_OUT2,DAMPER_P3);
			RunDamper(D_REC1,DAMPER_P4+dZRV1);
			RunDamper(D_REC2,DAMPER_P4+dZRV2);
			UREC=DAMPER_P4;
		}
	}
	else if((Timer_close_damper==DTAU16)&&(cooler)&&(begin_close==0)) { //������ 
		RunDamper(D_OUT1,DAMPER_CLOSE);
		RunDamper(D_OUT2,DAMPER_CLOSE);
		RunDamper(D_REC1,DAMPER_OPEN);
		RunDamper(D_REC2,DAMPER_OPEN);
		if(start_close_prit==0){
			start_close_prit=0x01;
		}
		if(((TestingDamper(D_OUT1)==DAMPER_CLOSE)&&(TestingDamper(D_OUT2)==DAMPER_CLOSE)
		&&(TestingDamper(D_REC1)==DAMPER_OPEN)&&(TestingDamper(D_REC2)==DAMPER_OPEN))||(Timer_wait_dampers==0)){// ���������
			
			Timer_close_damper=0;// ���� ����������� - ��������� ������
			begin_close=0x01;
		}
	}
	//
	if((begin_close)&&(cooler)){
		temp_k=(double)Timer_close_damper/DTAU16;
		if (T_OUTSIDE<=minus_20){			// K Uupr
			RunDamper(D_OUT1,(DAMPER_CLOSE+(uint8_t)((DAMPER_P1-DAMPER_CLOSE)*temp_k  )));// ���������� ������� ���������
			RunDamper(D_OUT2,(DAMPER_CLOSE+(uint8_t)((DAMPER_P1-DAMPER_CLOSE)*temp_k  )));
			RunDamper(D_REC1,(DAMPER_OPEN-(uint8_t)((DAMPER_OPEN-DAMPER_OPEN)*temp_k 	)));
			RunDamper(D_REC2,(DAMPER_OPEN-(uint8_t)((DAMPER_OPEN-DAMPER_OPEN)*temp_k  )));
		}
		else if ((T_OUTSIDE>minus_20)&&(T_OUTSIDE<=minus_5)){
			RunDamper(D_OUT1,(DAMPER_CLOSE+(uint8_t)((DAMPER_P2-DAMPER_CLOSE)*temp_k  )));
			RunDamper(D_OUT2,(DAMPER_CLOSE+(uint8_t)((DAMPER_P2-DAMPER_CLOSE)*temp_k  )));
			RunDamper(D_REC1,(DAMPER_OPEN-(uint8_t)((DAMPER_OPEN-DAMPER_OPEN)*temp_k  )));
			RunDamper(D_REC2,(DAMPER_OPEN-(uint8_t)((DAMPER_OPEN-DAMPER_OPEN)*temp_k  )));
		}
		else if (T_OUTSIDE>minus_5){
			RunDamper(D_OUT1,(DAMPER_CLOSE+(uint8_t)((DAMPER_P3-DAMPER_CLOSE)*temp_k )));
			RunDamper(D_OUT2,(DAMPER_CLOSE+(uint8_t)((DAMPER_P3-DAMPER_CLOSE)*temp_k )));
			RunDamper(D_REC1,(DAMPER_OPEN-(uint8_t)((DAMPER_OPEN-DAMPER_P4)*temp_k )));
			RunDamper(D_REC2,(DAMPER_OPEN-(uint8_t)((DAMPER_OPEN-DAMPER_P4)*temp_k )));
		}
		if (Timer_close_damper==DTAU16){
			end_close_prit=0x01;
		}
	}
	else if (cooler==0){
		begin_close=0;
		Timer_wait_dampers=DTAU14;
		Timer_close_damper=DTAU16;
		end_close_prit=0x01;
	}

	// ������� 7
	
	if (((WORK_MODE&BIT(HEATING))||(WORK_MODE&BIT(HEATING2)))&&(cooler==0x00)&&(work_tab7)) {
		if (dTvdl>DELTATVDLD){
			if (DamperZRV2<UREC)	dZRV2=(DamperZRV2+10)-UREC;
			else if((DamperZRV2==UREC)&&(DamperZRV1>DAMPER_CLOSE))	dZRV1=(DamperZRV1-10)-UREC;
		}
		else if(((dTvdl+DELTATVDLD)>=0)&&(dTvdl<=DELTATVDLD)){
		// ������ �� ������
		}
		else if((dTvdl+DELTATVDLD)<0){
			if (DamperZRV1<UREC){
				dZRV1=(DamperZRV1+10)-UREC;
			}
			else if((DamperZRV1==UREC)&&(DamperZRV2>DAMPER_CLOSE)){
				dZRV2=(DamperZRV2-10)-UREC;
			}
			else if(DamperZRV2==DAMPER_CLOSE){ // �� ��������
			
			}
		}
		work_tab7=0x00;
	}
	
	
	
}
/*
*
*
*/
void 
HeatingRun(void)
{ 	uint8_t need_loh=0;
	T_INSIDE=CalkulationTinMDL ();    // ����������� ����������� ������� ������
	T_PRIT=CalkulationTpritMAX ();    // ����������� ����������� ���������� �������
	T_OUTSIDE=CalkulationToutMDL ();  // ����������� ����������� ��������� �������
	if (ALARM_REG&BIT(HEATING)){
		WORK_MODE|=BIT(FIRST_RUN); // ��������� ������ ����
		WORK_MODE&=~BIT(HEATING);
		change_loh(0x00);//--
	} 
	else{  
		switch (STEP_COUNT)	{
			case 0: // �������� �������� ��������� ������� � ��������� �������� ������� 2
				ProcessDumpers();
				RunDamper(D_POT1,DAMPER_CLOSE);
				RunDamper(D_POT2,DAMPER_CLOSE);
				STEP_COUNT++;  // ������� � ���������� ����
			break;
			case 1:// ��� 2 - �������� ��������� ������, ����� ������� ������������������ ��� � ������������ � �������� 4
				if (T_OUTSIDE>=T_USTAVKI)						need_loh=0x00;//change_loh(0x00);//--
				else if ((T_OUTSIDE<T_USTAVKI)&&(T_OUTSIDE>=TNGR2))	need_loh=0x01;//change_loh(0x01);//+-
				else if ((T_OUTSIDE<TNGR2)&&(T_OUTSIDE+TNGR3>=0))	need_loh=0x02;//change_loh(0x02);//-+
				else if (T_OUTSIDE+TNGR3<0)							need_loh=0x03;//change_loh(0x03);//++
				change_loh(need_loh);
				if (the_loh==need_loh){
					STEP_COUNT++;
					TIMER_START_VENTILATION=0;
					Timer_Start_Bake=0;
					Timer_smeny_loh_up=DELTATAU15;
					Timer_smeny_loh_down=DELTATAU15;
				}
			break;
			case 2://�������� � ������������ � ��������� 5 � 6 � ������� ����������, ���������� � ������ �������. � �� ������� ��� ��� ���������� �����.
				STEP_PRIT=the_loh;
				//������� � "����������"
				if ((T_OUTSIDE>=T_USTAVKI)&&(FlagWorkRegister&BIT(FlagGoToVent))){
						change_loh(0x00);	//�:  	- � ��� ���?
											//�����:	- �� ������ ������!
						ChangeMode(VENTILATION);
						workmin=0x00;
						STEP_COUNT=0x00;
						STEP_PRIT=0x00;
						cooler=0x00;
						Timer_close_damper=DTAU16;
						TIMER_START_VENTILATION=0x00;//  ����� ��������-�������
						return;
				}				
				// ������� � ����� "�������"
				if ((T_INSIDE<(T_USTAVKI-DTVZ-dTvz_min))&&(Flag2WorkRegister&BIT(FLAG2GOTOBAKE))){ // <13
						ChangeMode(BAKE); 
						STEP_COUNT=0x00;
						STEP_PRIT=0x00;
						cooler=0x00;
						Timer_close_damper=DTAU16;
						workmin=0x00;
						Timer_Start_Bake=0x00;
						return;
				}
				//===========================================================================================
				//		
				//			��������� ��������
				//
				//===========================================================================================
				if (T_INSIDE>=(T_USTAVKI+DELTATVZ1)) { // >=17s - ������� 6
					ChangeMode(HEATING2);
					RegulationHeating2();
				}			//<17								//>15
				else if((T_INSIDE<(T_USTAVKI+DELTATVZ1))&&((T_INSIDE+DELTATVZ1)>T_USTAVKI)) {// 15 <    	<17
					if (ALARM_REG&BIT(HEATING)) {// �� ������
						WORK_MODE|=BIT(FIRST_RUN); // ��������� ������ ����
						WORK_MODE&=~BIT(HEATING);
						change_loh(0x00);
					}
					else{
						// ���������� ����������������������� ����������
						ProcessDumpers();
						// �������� �� ��������
						WorkingWithPritMax();
						WorkingWithPritMin();
						ChangeMode(HEATING);// ���� ��� ��������
					}
				}
				else if((T_INSIDE+DELTATVZ1)<=T_USTAVKI) {//<=15
					cooler=0x00;
					Timer_close_damper=DTAU16;
					ChangeMode(HEATING);
					RegulationHeating();// ������� 5
				}
			break;
		}
	}   
}

/*
* ����������� ��������
*
*/
void 
WorkingWithPritMax(void)
{	int16_t pritMax=CalkulationTpritMAX ();
	//���� ����������� ������� .. �� ������� �� ��������� ������� ������� ����������������������� 
	//� �� ��������� ���������� �� �������, ���� �� ���������� ������� TpritMAX<=TRITGR2
	if(pritMax>TPRITGR4){
		if (the_loh>0){
			change_loh(0x00);
			if (the_loh==0x00){
				STEP_PRIT=the_loh;
				mayup=0x00;
				Timer_smeny_loh_up=DELTATAU15;
			}
		}
		return;
	}
	//���� .. �� ������� �� ��������� ������� ������� ����������������������� 
	//� �� ��������� ���������� �� �������, ���� �� ���������� ������� TpritMAX<=TRITGR2
	else if ((pritMax>TPRITGR1)&&(Timer_may_up_loh==0)&&(Timer_smeny_loh_down==0)){
			if (Loh_Down()) {
				STEP_PRIT=the_loh;
				mayup=0x00;
				Timer_smeny_loh_down=DELTATAU15;
				Timer_may_up_loh=DELTATAU12;// ������� ������
			}
		return;
	}
	else if (pritMax<=TPRITGR2){
		mayup=(0x01);
	}
	// � ������������ � �����������
	mayup=mayup&mayup_min;
}
//ok

/*  
* �������� �� ����������� ��������
* 
*/
void 
WorkingWithPritMin(void)
{	int16_t pritMin=CalkulationTpritMIN ();
	uint8_t temp=the_loh;
	int8_t delta_loh=0;
	//���� ����� ��� <TNGR1  ��� workmin=0x01
	if((pritMin<(TPRITMIN1))){
		//�������������� ����������� ���������� �������:
		if ((the_loh<3)&&(Timer_smeny_loh_up==0)&&(mayup)){
			if(Loh_Up()){
				STEP_PRIT=the_loh;	
				// ���������� ����� ��� �������
				if (prit_down==0) the_loh_before_down=the_loh-0x01;
				maydown_min=0x00;// ��������� ��������� ������ ������������������
				prit_down=0x01; // ������ ����
				Timer_smeny_loh_up=DELTATAU15;
			}
		}
	}
	else if((pritMin>=(TPRITMIN2))&&(prit_down)){//&&(Timer_upr_prit_min_down==0)){
		if (Timer_smeny_loh_down==0){
			temp=the_loh_before_down;//NLoh;
			delta_loh=temp-the_loh;// ������� ��� �������?
			change_loh(temp);
			if (the_loh==temp){// ������������ �������
				prit_down=0x00;
				STEP_PRIT=the_loh;
				
				maydown_min=0x01;
				if(delta_loh>0){// ���������
					Timer_smeny_loh_up=DELTATAU15;
				}
				else if(delta_loh<0){// �����
					Timer_smeny_loh_down=DELTATAU15;
				}
			}
		}
	}
	mayup=mayup&mayup_min;
	maydown=maydown_min;
}
/*
*  �������� �� ������� 5 + ���. ��������(������� 7 � ������ ������)
*
*/
void 
RegulationHeating(void)
{	
	if (ALARM_REG&BIT(HEATING)) {
		WORK_MODE|=BIT(FIRST_RUN); // ��������� ������ ����
		WORK_MODE&=~BIT(HEATING);
		change_loh(0x00);
	}
	else {
		ProcessDumpers();
		// �������� �� ��������
		WorkingWithPritMax();
		WorkingWithPritMin();
		// >13											//<=14
		if(((T_INSIDE>(T_USTAVKI-DTVZ-dTvz_min)))&&((T_INSIDE+DELTATVZ2)<=T_USTAVKI)){
		if ((mayup)&&(the_loh<0x03)&&(Timer_smeny_loh_up==0)){
				change_loh(0x03); // ���������� ������������ �������
				if (the_loh==0x03){
					STEP_PRIT=0x03;
					//�������� �� �����������
					Timer_smeny_loh_up=DELTATAU15;
				}
			}
		}										
					//<15								//>14
		else if (( (T_INSIDE+DELTATVZ1<T_USTAVKI)&&(T_INSIDE+DELTATVZ2>T_USTAVKI) )/*&&(Flag_SpeedDTV)*/){
			// ���� dt�/dtau10 <=0
			if (SPEEDDTV<=0){
				if ((mayup)&&(Timer_smeny_loh_up==0)){
					if (Loh_Up()){ // ������� �� ��������� ������� �������
						STEP_PRIT=the_loh;
						Timer_smeny_loh_up=DELTATAU15;
					}
				}
			}
			// ���� dt�/dtau10 >0.1/tau10
			else if(SPEEDDTV>DTVVMAX){
				if ((maydown)&&(Timer_smeny_loh_down==0)){
					if (Loh_Down()){
						STEP_PRIT=the_loh;
						Timer_smeny_loh_down=DELTATAU15;
					}
				}
			}
			else {
				//Flag_SpeedDTV=0x00;// ���������� � ��������� ����
			}
		}
		
	}
}
/*
*
*/
void 
RegulationHeating2(void) // ������� 6
{	uint8_t temp=the_loh;
  	if (ALARM_REG&BIT(HEATING2)) {
		WORK_MODE|=BIT(FIRST_RUN); // ��������� ������ ����
		WORK_MODE&=~BIT(HEATING2);
		change_loh(0x00);
	}
	else {
		ProcessDumpers();
		// �������� �� ��������
		WorkingWithPritMax();
		WorkingWithPritMin();
		if ( ((T_INSIDE>=(T_USTAVKI+DELTATVZ1))&&(T_INSIDE<(T_USTAVKI+DELTATVZ2)))){
			//>=17
			if (SPEEDDTV>=0){
				if(the_loh==0x00){
					cooler=0x01;
				}
				//else cooler=0x00;
				
				if ((the_loh>0)&&(maydown)&&(Timer_smeny_loh_down==0)){
					temp--;
					change_loh(temp);
					if (the_loh==temp){
						STEP_PRIT=the_loh;
						Timer_smeny_loh_down=DELTATAU15;
					}
				}
			}
			else if ((SPEEDDTV+DTVVMAX)<0){// ������� ������, ����� ����������
				if ((the_loh<3)&&(mayup)&&(Timer_smeny_loh_up==0)){
					temp++;
					change_loh(temp);
					if (the_loh==temp){
						STEP_PRIT=the_loh;
						//Timer_upr_prit_min_up=DELTATAU13;// � ������� dtau13
						Timer_smeny_loh_up=DELTATAU15;
					}
				}
			}
			else{
			}
		}		//>=18
		else if(T_INSIDE>=(T_USTAVKI+DELTATVZ2)){
			cooler=0x00;
			if ((the_loh>0)&&(maydown)){
				change_loh(0x00);
				if (the_loh==0){
					STEP_PRIT=the_loh;
					Timer_close_damper=DTAU16;
					Timer_smeny_loh_down=DELTATAU15;
				}
			}
		}
		
	}
}

/*


*/
void 
VentilationRun(void)
{
				  
	change_loh(0x00);// ������� �� �����������  �������
	T_INSIDE=CalkulationTinMDL ();    // ����������� ����������� ������� ������
	T_PRIT=CalkulationTpritMAX ();    // ����������� ����������� ���������� �������
	T_OUTSIDE=CalkulationToutMDL ();  // ����������� ����������� ��������� �������
   
	if (ALARM_REG&BIT(VENTILATION)) {
		WORK_MODE|=BIT(FIRST_RUN);      // ��������� ������ ����
		WORK_MODE&=~BIT(VENTILATION);
		change_loh(0x00);// ������� �� �����������  �������
		//DataLED&=~BIT(WORK_LED_1);
		//DataLED&=~BIT(WORK_LED_2);
	} 
	else{  
     //DataLED|=BIT(WORK_LED_1);
	 //DataLED|=BIT(WORK_LED_2);
		switch (STEP_COUNT){
			case 0:
				RunDamper(D_OUT1,DAMPER_OPEN);
				RunDamper(D_OUT2,DAMPER_OPEN);
				RunDamper(D_REC1,DAMPER_CLOSE);
				RunDamper(D_REC2,DAMPER_CLOSE);
				RunDamper(D_POT1,DAMPER_OPEN);
				RunDamper(D_POT2,DAMPER_OPEN);
				STEP_COUNT++;  // ������� � ���������� ����
				FlagWorkRegister&=~BIT(FlagTimerChangeVent);
			break;
			case 1:
				change_loh(0x00);// ������� �� �����������  �������
				if ((T_INSIDE<=(T_USTAVKI-(DTVZ/2)))||(T_OUTSIDE<=(T_USTAVKI-DTVZ))) {
					FlagWorkRegister|=BIT(FlagTimerChangeVent);
					if (FlagWorkRegister&BIT(FlagGoToHeat)){
						ChangeMode(HEATING); 
						STEP_COUNT=0;
						STEP_PRIT=0;
						FlagWorkRegister&=~BIT(FlagTimerChangeVent);
					}
				}
				else {
					FlagWorkRegister&=~BIT(FlagTimerChangeVent);
					FlagWorkRegister&=~BIT(FlagGoToHeat);
				}
			break;
		}
	}
}

/*

*/
void 
VVK_ON(uint8_t VVK)
{
	if (VVK==VVK1){
		if (FlagWorkRegister&BIT(FlagDelayOffVVK1)){
			/*if (!(DataOUT&BIT(VVK))) {
				DataOUT|=BIT(VVK);
			*/
			if (!(PORTG&BIT(VVK))) {
				PORTG|=BIT(VVK);
				//
				the_loh|=BIT(0);
				DataLED|=BIT(HEAT_LED1_1);
				DataLED|=BIT(HEAT_LED1_2);
				CountVVK1++;
				EEPROM_WriteChar(ADDR_VVK1,(CountVVK1>>8));
				EEPROM_WriteChar(ADDR_VVK1+1,((uint8_t)CountVVK1));
			}
		}
	}
	else if (VVK==VVK2) {
		if (FlagWorkRegister&BIT(FlagDelayOffVVK2)) {
			/*
			if (!(DataOUT&BIT(VVK))){
				DataOUT|=BIT(VVK);
			*/
			if (!(PORTG&BIT(VVK))){
				PORTG|=BIT(VVK);
				the_loh|=BIT(1);
				DataLED|=BIT(HEAT_LED2_2);
				DataLED|=BIT(HEAT_LED2_1);
				CountVVK2++;
				EEPROM_WriteChar(ADDR_VVK2,(CountVVK2>>8));
				EEPROM_WriteChar(ADDR_VVK2+1,((uint8_t)CountVVK2));
			}
		}
	}	 
}
/*
*
*/
void 
VVK_OFF(uint8_t VVK)
{
	if (VVK==VVK1){
		if (PORTG&BIT(VVK)){
			PORTG&=~BIT(VVK);
			//
			the_loh&=~BIT(0);
			DataLED&=~BIT(HEAT_LED1_2);
			DataLED&=~BIT(HEAT_LED1_1);
			FlagWorkRegister&=~BIT(FlagDelayOffVVK1);
			TimerDelayOffVVK1=0;
		}
	}
	else if (VVK==VVK2) {
		if (PORTG&BIT(VVK)){
			PORTG&=~BIT(VVK);
			//
			the_loh&=~BIT(1);
			DataLED&=~BIT(HEAT_LED2_2);
			DataLED&=~BIT(HEAT_LED2_1);
			FlagWorkRegister&=~BIT(FlagDelayOffVVK2);
			TimerDelayOffVVK2=0;
		}
	}
}
// ok
/*
*
*/
uint8_t 
TestingDamper (uint8_t NDamper)
{
	uint8_t P_Damper=0;

	if (DamperDataIN[NDamper]<15){
		ErrorDamper|=BIT(NDamper);
		P_Damper=DAMPER_CLOSE;
		Tdamp[NDamper]=0x00;
	}
	else if ((DamperDataIN[NDamper]>=15)&&(DamperDataIN[NDamper]<25)){
			P_Damper=DAMPER_CLOSE;
			Tdamp[NDamper]=0;
			ErrorDamper&=~BIT(NDamper);
	}
	else if ((DamperDataIN[NDamper]>=25)&&(DamperDataIN[NDamper]<95)) {
		ErrorDamper&=~BIT(NDamper);
		P_Damper=DamperDataIN[NDamper];
		Tdamp[NDamper]=(uint16_t)DamperDataIN[NDamper];
	}
	else if ((DamperDataIN[NDamper]>=95)) {
		ErrorDamper&=~BIT(NDamper);
		P_Damper=DAMPER_OPEN;
		if (DamperDataIN[NDamper]<=100)
			Tdamp[NDamper]=(uint16_t)DamperDataIN[NDamper];
		else Tdamp[NDamper]=100;
	}
	return P_Damper;
}
//Ok
/*
*  ����������� ���������� ����������
*/
void 
RunDamper (uint8_t NDamper, uint8_t data)
{ 
	/* ������ ��������� �������� */
	DamperDataOUT[NDamper]=data;     
	if (NDamper==D_REC1){
		DamperZRV1=data;
	}
	else if (NDamper==D_REC2){
		DamperZRV2=data;
	}
	else if ((NDamper==D_POT1)||(NDamper==D_POT2)){
		if(data==DAMPER_OPEN)	PPVin1=0x01;
		else{	
			if (PPVin1) Timer_perevod_v_pol=DTAU14;
			PPVin1=0x00;
		}
	}
}

/*
*
*
*/
void 
ChangeMode (uint8_t mode)
{
   WORK_MODE&=0xF0;                 // �������� ������������� ��������
   WORK_MODE|=BIT(mode);
}
/*
*
*/
int16_t 
CalkulationTpritMAX (void)
{	int16_t t=0;
	if ((Timer_perevod_v_pol==0)&&(PPVin1==0x00)) {// ���� � ���
		if (!(ErrorReg&BIT(Tprit1_ch)))        T_PRIT1=DTPRV1;
		else if (!(ErrorReg&BIT(Tprit2_ch)))   T_PRIT1=DTPRV2;
	 
		if (!(ErrorReg&BIT(Tprit2_ch)))        T_PRIT2=DTPRV2;
		else if (!(ErrorReg&BIT(Tprit1_ch)))   T_PRIT2=DTPRV1;
	 
		if (T_PRIT1>T_PRIT2) t=T_PRIT1;
		else                 t=T_PRIT2;
	}
	else{	// ���� � �������
		if (!(ErrorReg&BIT(Trec1_ch)))        T_PRIT1=DTRV1;
		else if (!(ErrorReg&BIT(Trec2_ch)))   T_PRIT1=DTRV2;
		else 	 							   T_PRIT1=800;				 
	 
		if (!(ErrorReg&BIT(Trec2_ch)))        T_PRIT2=DTRV2;
		else if (!(ErrorReg&BIT(Trec1_ch)))   T_PRIT2=DTRV1;
		else 	 							   T_PRIT2=800; 
		if (T_PRIT1<T_PRIT2) t=T_PRIT1;
		else                 t=T_PRIT2;
	}
    return t;
}

/*
*
*
*/
int16_t 
CalkulationTpritMIN (void)
{
   int16_t t=0;
	if ((Timer_perevod_v_pol==0)&&(PPVin1==0x00)){
		if (!(ErrorReg&BIT(Tprit1_ch)))        T_PRIT1=DTPRV1;
		else if (!(ErrorReg&BIT(Tprit2_ch)))   T_PRIT1=DTPRV2;
		
		if (!(ErrorReg&BIT(Tprit2_ch)))        T_PRIT2=DTPRV2;
		else if (!(ErrorReg&BIT(Tprit1_ch)))   T_PRIT2=DTPRV1;
		
		if (T_PRIT1>T_PRIT2) t=T_PRIT2;
		else                 t=T_PRIT1;
	}
	else{// ���� � �������
		if (!(ErrorReg&BIT(Trec1_ch)))        T_PRIT1=DTRV1;
		else if (!(ErrorReg&BIT(Trec2_ch)))   T_PRIT1=DTRV2;
		else 	 							   T_PRIT1=800;				 
	 
		if (!(ErrorReg&BIT(Trec2_ch)))        T_PRIT2=DTRV2;
		else if (!(ErrorReg&BIT(Trec1_ch)))   T_PRIT2=DTRV1;
		else 	 							   T_PRIT2=800; 
		if (T_PRIT1<T_PRIT2) t=T_PRIT2;
		else                 t=T_PRIT1;
	}
   return t;
}  

/*
*
*
*/
int16_t 
CalkulationToutMDL (void)
{
   int16_t t=0;
    if (ErrorReg&BIT(Tout1_ch)) {     // ������ 1 �������� ����������� ����������
        if (ErrorReg&BIT(Tout2_ch)){ // ������� 1 � 2 �������� ����������� ����������
		    T_OUTSIDE1=minus_50;
			T_OUTSIDE2=minus_50;
		}
		else{                        // �������� ������ ������ 2 �������� �����������
		    T_OUTSIDE1=DTNV2;
			T_OUTSIDE2=DTNV2;
		}
	}
	else if (ErrorReg&BIT(Tout2_ch)) {// ������ 2 �������� ����������� ����������
	    if (ErrorReg&BIT(Tout1_ch)) { // ������� 1 � 2 �������� ����������� ����������
		    T_OUTSIDE1=minus_50;
			T_OUTSIDE2=minus_50;
		}
		else{                         // �������� ������ ������ 1 �������� �����������
		    T_OUTSIDE1=DTNV1;
			T_OUTSIDE2=DTNV1;
		}
	}
	else{                           // ��� ������� �������� ����������� ��������
	    T_OUTSIDE1=DTNV1;
		T_OUTSIDE2=DTNV2; 
	}
	  
	  t=(T_OUTSIDE1+T_OUTSIDE2)/2;
	  
    return t;
}
//ok
#ifdef Speed1
void 
Speedometr(void)
{	uint8_t k;
	int16_t t=CalkulationTinMDL ();;
	for (k=(DTAU10-0x01);k>0x00;k--){
		Speed_Array[k]=Speed_Array[k-0x01];
	}
	Speed_Array[0x00]=t;
	SPEEDDTV=Speed_Array[0x00]-Speed_Array[(DTAU10-0x01)];
	//SPEEDDTV=ConvertToSignedData(SPEEDDTV);
}
#else
void 
Speedometr_2(void)
{	int16_t t=CalkulationTinMDL ();;
	if ((/*(!(OLD_T_INSIDE==t))||*/(Speed_Timer>0))){// ����������� ���������� ��� ������ ����� �������� ��������� �����������
		SPEEDDTV=((signed long)(t-OLD_T_INSIDE)*100)/Speed_Timer;
		OLD_T_INSIDE=t;
		if (!(OLD_T_INSIDE==t)||(Speed_Timer==DTAU10))	Speed_Timer=0;
			
	}
	//else SPEEDDTV=0;
}
#endif
/*
*
*
**/
int16_t 
CalkulationTinMDL (void)
{ // int16_t t=0;
   // ����������� ����������� ������ ��� ������ ���������
	if (WORK_MODE&BIT(FIRST_RUN))  {
		if ((ErrorReg&BIT(Tin1_ch))&&(ErrorReg&BIT(Tin2_ch))) {
			if (T_OUTSIDE>=T_USTAVKI) {
				if (!(ErrorReg&BIT(Tprit1_ch))){
													T_INSIDE1=DTPRV1;
					if (ErrorReg&BIT(Tprit2_ch)) 	T_INSIDE2=DTPRV1;
					else                         	T_INSIDE2=DTPRV2;
				}
				else if (!(ErrorReg&BIT(Tprit2_ch))){
													T_INSIDE2=DTPRV2;
					if (ErrorReg&BIT(Tprit1_ch)) 	T_INSIDE1=DTPRV2;
					else                         	T_INSIDE1=DTPRV1;
				}
			}
			else { }
		}
		else if (ErrorReg&BIT(Tin1_ch)){
			if (!(ErrorReg&BIT(Tin2_ch))) {
				T_INSIDE1=DTS2;
				T_INSIDE2=DTS2;
			}
		}
		else if (ErrorReg&BIT(Tin2_ch)){
			if (!(ErrorReg&BIT(Tin1_ch))) {
				T_INSIDE1=DTS1;
				T_INSIDE2=DTS1;
			}
		}
		else{
			T_INSIDE1=DTS1;
			T_INSIDE2=DTS2;
		}
	}
	else if (WORK_MODE&BIT(AVTOM_MODE)) {
		// ����������� ����������� ������ � ������ ������� � ���������
		if ((Timer_perevod_v_pol==0)&&(PPVin1==0x00)) {
			if ((ErrorReg&BIT(Trec1_ch))&&(ErrorReg&BIT(Trec2_ch))) {// ���� ���������������� ����������
				if ((ErrorReg&BIT(Tin1_ch))&&(ErrorReg&BIT(Tin2_ch))){}
				else if (ErrorReg&BIT(Tin1_ch)){
					if (!(ErrorReg&BIT(Tin2_ch))) {
						T_INSIDE1=DTS2;
						T_INSIDE2=DTS2;
					}
				}
				else if (ErrorReg&BIT(Tin2_ch)){
					if (!(ErrorReg&BIT(Tin1_ch))){
						T_INSIDE1=DTS1;
						T_INSIDE2=DTS1;
					}
				}
				else{
					T_INSIDE1=DTS1;
					T_INSIDE2=DTS2;
				}  
			}
			else{
				if (!(ErrorReg&BIT(Trec1_ch))){
													T_INSIDE1=DTRV1;
					if (ErrorReg&BIT(Trec2_ch))		T_INSIDE2=DTRV1;
					else							T_INSIDE2=DTRV2;
				}
				else if (!(ErrorReg&BIT(Trec2_ch))){
													T_INSIDE2=DTRV2;
					if (ErrorReg&BIT(Trec1_ch))		T_INSIDE1=DTRV2;
					else							T_INSIDE1=DTRV1;
				}
			}
		}
		// ����������� ����������� ������ � ������ ����������
		else if ((Timer_perevod_v_pol>0)||(PPVin1)) {
			if ((ErrorReg&BIT(Tin1_ch))&&(ErrorReg&BIT(Tin2_ch))){// ��� ��� ���@������ - ������ �� ����������������
				if (!(ErrorReg&BIT(Trec1_ch))){// ������ ���
													T_INSIDE1=DTRV1;
					if (ErrorReg&BIT(Trec2_ch))	T_INSIDE2=DTRV1;// ���� ������ ����
					else							T_INSIDE2=DTRV2;// ���� ������ ���
				}
				else if (!(ErrorReg&BIT(Trec2_ch))){// ����� ���� ������ ���
													T_INSIDE2=DTRV2;
					if (ErrorReg&BIT(Trec1_ch))	T_INSIDE1=DTRV2;// � ������ ����
					else							T_INSIDE1=DTRV1;// � ������ ���
				}
			}// ����� ������ ���� ���� ��� �� ���
			else if (ErrorReg&BIT(Tin1_ch)){// ������ ������
				if (!(ErrorReg&BIT(Tin2_ch))){// � ������ ���
					T_INSIDE1=DTS2;
					T_INSIDE2=DTS2;
				}
			}
			else if (ErrorReg&BIT(Tin2_ch)){// ������ ������
				if (!(ErrorReg&BIT(Tin1_ch))){// � ������ ���
					T_INSIDE1=DTS1;
					T_INSIDE2=DTS1;
				}
			}
			else{// ��� � �����
				T_INSIDE1=DTS1;
				T_INSIDE2=DTS2;
			}
		}
	}
   
   //t=;
   return (T_INSIDE1+T_INSIDE2)/2;
}

/*
*
*
*
*/
uint8_t 
Ostatok(uint16_t d)
{
	return d-0x0A*(d/0x0A);
}
/*========================================================================================
* ������� ������� ������������ ������ ������������������ ��� �������� �������� ���� 16.1 
*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void 
Lohometr(void)
{	uint16_t k;// ������ �������
	uint8_t l=the_loh;
	uint32_t temp=0;
	if (Loh_Timer==DTAU13){// ���� ��� ������ ���� 13
		for (k=(DTAU13-0x01);k>0x00;k--){
			Loh_Array[k]=Loh_Array[k-0x01];
		}
		Loh_Array[0x00]=l;
	}	
	else{ // ���� �� ������...
		Loh_Array[Loh_Timer]=l;
	}
	// ������ ������� �����
	for (k=0;k<(Loh_Timer-1);k++){
		temp+=Loh_Array[k];
	}
	temp= (temp*0x0A)/(Loh_Timer);
	// ���������
	if (Ostatok(temp)>=0x05) NLoh=((uint8_t)(temp/0x0A))+0x01;
	else NLoh=((uint8_t)(temp/0x0A));
}
//=========================================================================================
/*
*
*
*/
void 
CheckErrorAndAlarm(void)
{
  if (WORK_MODE&BIT(BLASTING))
  {
        TimerButtonClickOff=25;
		
           DataLED&=~BIT(ALARM_LED_1);
		   DataLED&=~BIT(ALARM_LED_2);
		   
		   BlinkingIndikator(WORK_LED_1);
		   BlinkingIndikator(WORK_LED_2);
		   
  }
  else
  {
	    if ((ErrorReg&BIT(Tin1_ch))&&(ErrorReg&BIT(Tin2_ch))&&(ErrorReg&BIT(Trec1_ch))&&(ErrorReg&BIT(Trec2_ch)))/*������ �� ������ � ���.*/
		{
		   ALARM_REG|=BIT(BAKE);
		   ALARM_REG|=BIT(HEATING);
		   ALARM_REG|=BIT(HEATING2);
    	   ALARM_REG|=BIT(VENTILATION);
		   
		   DataLED|=BIT(ALARM_LED_1);
		   DataLED|=BIT(ALARM_LED_2);
		   
		   DataLED&=~BIT(WORK_LED_1);
		   DataLED&=~BIT(WORK_LED_2);
		}   
    	else if ((ErrorReg&BIT(Tprit1_ch))&&(ErrorReg&BIT(Tprit2_ch)))/* ������ �� ��������*/
		{
    	   ALARM_REG|=BIT(BAKE);
		   ALARM_REG|=BIT(HEATING);
		   ALARM_REG|=BIT(HEATING2);
		   ALARM_REG&=~BIT(VENTILATION);
		   
		   if (T_OUTSIDE>=T_USTAVKI)  // ����������� ������� ���������� ��� ����������
		   {
		      DataLED&=~BIT(ALARM_LED_1);
		      DataLED&=~BIT(ALARM_LED_2);
			  
			 // if (ErrorReg&BIT(Tin_ch))
		     // {
		     //    BlinkingIndikator(WORK_LED_1);
		     //    BlinkingIndikator(WORK_LED_2);
		     // }
		     // else
		     // {
		          if ((ErrorReg&BIT(Tin1_ch))||(ErrorReg&BIT(Trec1_ch))||(ErrorReg&BIT(Tout1_ch))||(ErrorReg&BIT(Tprit1_ch))||
		              (ErrorDamper&BIT(D_OUT1))||(ErrorDamper&BIT(D_POT1))||(ErrorDamper&BIT(D_REC1)))
		          {
		               BlinkingIndikator(WORK_LED_1);
		          }
		          else DataLED|=BIT(WORK_LED_1);
		   
		          if ((ErrorReg&BIT(Tin2_ch))||(ErrorReg&BIT(Trec2_ch))||(ErrorReg&BIT(Tout2_ch))||(ErrorReg&BIT(Tprit2_ch))||
		              (ErrorDamper&BIT(D_OUT2))||(ErrorDamper&BIT(D_POT2))||(ErrorDamper&BIT(D_REC2)))
		          {
		               BlinkingIndikator(WORK_LED_2);
		          }
		          else DataLED|=BIT(WORK_LED_2);
		     // }
		   }	  
		   else
		   {
		      DataLED|=BIT(ALARM_LED_1);
		      DataLED|=BIT(ALARM_LED_2);
			  DataLED&=~BIT(WORK_LED_1);
		      DataLED&=~BIT(WORK_LED_2);
		   }
		}// ��� ������,�� �������� ������  �������
        else 
	    {
		   ALARM_REG&=~BIT(BAKE);
		   ALARM_REG&=~BIT(HEATING);
		   ALARM_REG&=~BIT(HEATING2);
		   ALARM_REG&=~BIT(VENTILATION);
		   
		   DataLED&=~BIT(ALARM_LED_1);
		   DataLED&=~BIT(ALARM_LED_2);
		   
		   //if (ErrorReg&BIT(Tin_ch))
		   //{
		   //BlinkingIndikator(WORK_LED_1);
		   //BlinkingIndikator(WORK_LED_2);
		   //}
		   //else
		   //{
		      if ((ErrorReg&BIT(Tin1_ch))||(ErrorReg&BIT(Trec1_ch))||(ErrorReg&BIT(Tout1_ch))||(ErrorReg&BIT(Tprit1_ch))||
		          (ErrorDamper&BIT(D_OUT1))||(ErrorDamper&BIT(D_POT1))||(ErrorDamper&BIT(D_REC1)))
		      {
		           BlinkingIndikator(WORK_LED_1);
		      }
		      else DataLED|=BIT(WORK_LED_1);
		   
		      if ((ErrorReg&BIT(Tin2_ch))||(ErrorReg&BIT(Trec2_ch))||(ErrorReg&BIT(Tout2_ch))||(ErrorReg&BIT(Tprit2_ch))||
		          (ErrorDamper&BIT(D_OUT2))||(ErrorDamper&BIT(D_POT2))||(ErrorDamper&BIT(D_REC2)))
		      {
		           BlinkingIndikator(WORK_LED_2);
		      }
		      else DataLED|=BIT(WORK_LED_2);
		   //}
	    }
	}	
}


/*
*
*/
void 
CheckErrorDamper(void)
{
	uint8_t i=0;
	for (i=0;i<6;i++){
		TestingDamper (i);
	}
}

/*
*
*/
void TimersAlgoritm(void)
{
	/*if (PIND&BIT(7)) PORTD&=~BIT(7);
	else             PORTD|=BIT(7);
	*/
	
	if (count_10<0x0A){
		count_10++;
		if (count_10<0x05)  FlagBlinkingIndikator=0x01;
		else FlagBlinkingIndikator=0;
	}
	else{
		// ������ ������ �������� ����������� 1 ���.
		if (TimerReadTemperature<DTAU2) TimerReadTemperature++;
		else{
			TimerReadTemperature=0;
			FlagWorkRegister|=BIT(FlagReadDataADC);
		}
//================================================================================	   
#ifdef Speed1
		if(Flag_SpeedDTV) Speedometr(); // ����������� �������� �����������
#else
		Speedometr_2();
#endif
//================================================================================	   
		// �������� ������������� ��������
		if (TimerFirstRun<DTAU3) TimerFirstRun++;
		else {
			if (WORK_MODE&BIT(AVTOM_MODE)){
				if (WORK_MODE&BIT(BLASTING)){
					WORK_MODE|=BIT(FIRST_RUN);
					WORK_MODE&=~BIT(BLASTING);
				}
			}
			else	WORK_MODE&=~BIT(BLASTING);
		}
//=======================================================================================================	   
		// ���� �������� ������� 1 ���.
		if (TimerRunAlgoritm<DTAU2) TimerRunAlgoritm++;
		else{
			TimerRunAlgoritm=0;
			FlagWorkRegister|=BIT(FlagRunAlgoritm);
		}
//=======================================================================================================	   
//{	   
		// TIMEOUT  �������� ���1 �� ������ ��������� � ������
		if (TimerOUTDamper[D_OUT1]<DTAU6)	TimerOUTDamper[D_OUT1]++;
		else								REG_FlagOUTDamper|=BIT(D_OUT1);
	   
		// TIMEOUT 2 ��� �������� ���2 �� ������ ��������� � ������
		if (TimerOUTDamper[D_OUT2]<DTAU6) 	TimerOUTDamper[D_OUT2]++;
		else								REG_FlagOUTDamper|=BIT(D_OUT2);
	   
		// TIMEOUT 2 ��� �������� ���1 �� ������ ��������� � ������
		if (TimerOUTDamper[D_REC1]<DTAU6) 	TimerOUTDamper[D_REC1]++;
		else                               	REG_FlagOUTDamper|=BIT(D_REC1);
	   
		// TIMEOUT 2 ��� �������� ���2 �� ������ ��������� � ������
		if (TimerOUTDamper[D_REC2]<DTAU6) 	TimerOUTDamper[D_REC2]++;
		else                               	REG_FlagOUTDamper|=BIT(D_REC2);
	   
		// TIMEOUT 2 ��� �������� ���1 �� ������ ��������� � ������
		if (TimerOUTDamper[D_POT1]<DTAU6) 	TimerOUTDamper[D_POT1]++;
		else                               	REG_FlagOUTDamper|=BIT(D_POT1);
	   
		// TIMEOUT 2 ��� �������� ���2 �� ������ ��������� � ������
		if (TimerOUTDamper[D_POT2]<DTAU6) 	TimerOUTDamper[D_POT2]++;
		else                               	REG_FlagOUTDamper|=BIT(D_POT2);
		// ��������  ����� ������������� ���1 �� ������ ��������� � ������ (����� ���������)
		if (TimerDelayDamper[D_OUT1]<DTAU6)	TimerDelayDamper[D_OUT1]++;
		else                               	REG_FlagDelayDamper|=BIT(D_OUT1);
	   
		// ��������  ����� ������������� ���2 �� ������ ��������� � ������ (����� ���������)
		if (TimerDelayDamper[D_OUT2]<DTAU6)	TimerDelayDamper[D_OUT2]++;
		else                              	REG_FlagDelayDamper|=BIT(D_OUT2);
		// ��������  ����� ������������� ���1 �� ������ ��������� � ������ (����� ���������)
		if (TimerDelayDamper[D_REC1]<DTAU6)	TimerDelayDamper[D_REC1]++;
		else                               	REG_FlagDelayDamper|=BIT(D_REC1);
		// �������� ����� ������������� ���2 �� ������ ��������� � ������ (����� ���������)
		if (TimerDelayDamper[D_REC2]<DTAU6)	TimerDelayDamper[D_REC2]++;
		else                                REG_FlagDelayDamper|=BIT(D_REC2);
	   
		// ��������  ����� ������������� ���1 �� ������ ��������� � ������ (����� ���������)
		if (TimerDelayDamper[D_POT1]<DTAU6) TimerDelayDamper[D_POT1]++;
		else                                REG_FlagDelayDamper|=BIT(D_POT1);
	   
		// ��������  ����� ������������� ���2 �� ������ ��������� � ������ (����� ���������)
		if (TimerDelayDamper[D_POT2]<DTAU6) TimerDelayDamper[D_POT2]++;
		else                                REG_FlagDelayDamper|=BIT(D_POT2);
//}
		/* ������ ��� �������� T����MAX */
		if (Timer_may_up_loh>0)Timer_may_up_loh--;
//===========================================================================
		if ((Timer_wait_dampers>0)&&(start_close_prit)){
			Timer_wait_dampers--;
		}
		/* ������ ����������� �������� ����������� ������ */
		if (SpeedTimer>0) {
			SpeedTimer--;
		}
		else{
			
		}
		/* ������ ����������� �������� ����������� ������ 2 */
		if (Speed_Timer<DTAU10) {
			Speed_Timer++;
		}
		else{
			//Speed_Timer=0;
		}
		/*������ �������� �������� */
		if(Timer_close_damper<DTAU16){
			Timer_close_damper++;
		}
		else if((end_close_prit)&&(begin_close)){
			cooler=0x00;
			begin_close=0x00;
			start_close_prit=0x00;
			Timer_wait_dampers=DTAU_WD;
			end_close_prit=0x00;
		}
		/*��� ������� 7*/
		if ((Timer_work_tab7>0)&&(work_tab7==0)){
			Timer_work_tab7--;
		}
		else if((Timer_work_tab7==0)&&(work_tab7==0)){
			work_tab7=0x01;
			Timer_work_tab7=DELTATAU11;
		}
//=====================================================================================
		if (Timer_smeny_loh_up>0){
			Timer_smeny_loh_up--;
		}
		if (Timer_smeny_loh_down>0){
			Timer_smeny_loh_down--;
		}
		else{
		}
//=====================================================================================	   
		if (Timer_rec_data>0){
			Timer_rec_data--;
		}
		/* ��������  �� ��������� ���1 ����� ��� ����������*/
		if (TimerDelayOffVVK1<DTAU8) TimerDelayOffVVK1++;
		else {
			FlagWorkRegister|=BIT(FlagDelayOffVVK1);
		}
		/* ��������  �� ��������� ���2 ����� ��� ����������*/
	    if (TimerDelayOffVVK2<DTAU8) TimerDelayOffVVK2++;
		else{
			FlagWorkRegister|=BIT(FlagDelayOffVVK2);
		}
//=====================================================================================================
		if (Timer_perevod_v_pol>0)	Timer_perevod_v_pol--;
//=====================================================================================================   
	   /* 1 ��� �������� �������� �� ���������� � ���������*/
		if (FlagWorkRegister&BIT(FlagTimerChangeVent)){
			if (TIMER_STOP_VENTILATION<DTAU4){
				TIMER_STOP_VENTILATION++;
				FlagWorkRegister&=~BIT(FlagGoToHeat);
			}
			else{
				TIMER_STOP_VENTILATION=0;
				FlagWorkRegister|=BIT(FlagGoToHeat);
			}
		}
		else{
				TIMER_STOP_VENTILATION=0;
				FlagWorkRegister&=~BIT(FlagGoToHeat);
		}
//=================================================================================================
		/* ������� ��� ����������� ������������� �������� ������ ������������������*/
		if (Counter_6sec<5) Counter_6sec++;
		else{
			if ( WORK_MODE&BIT(HEATING)||WORK_MODE&BIT(HEATING2) ){
				if (Loh_Timer<DTAU13){
					Loh_Timer++;
					Lohometr();
				}
			}
			else Loh_Timer=0x00;
			Counter_6sec=0;
		}
//=================================================================================================	   
	   /* �������� �������� �� ��������� � ����������*/
		if (( WORK_MODE&BIT(HEATING)||WORK_MODE&BIT(HEATING2) )&&(STEP_COUNT==2)){
			if (the_loh==0){
				if ((TIMER_START_VENTILATION<DTAU4))
					TIMER_START_VENTILATION++;
				else {
					FlagWorkRegister|=BIT(FlagGoToVent);
				}
			}
			else{
				TIMER_START_VENTILATION=0;
				FlagWorkRegister&=~BIT(FlagGoToVent);
			}
			/*�������� �������� �� ���������  � �������	*/
			if ((Timer_Start_Bake<DTAU9)){
					Timer_Start_Bake++;
					Flag2WorkRegister&=~BIT(FLAG2GOTOBAKE);
			}
			else {
				Flag2WorkRegister|=BIT(FLAG2GOTOBAKE);
			}
		}
		else {
			Timer_Start_Bake=0;
			TIMER_START_VENTILATION=0;
			FlagWorkRegister&=~BIT(FlagGoToVent);
			Flag2WorkRegister&=~BIT(FLAG2GOTOBAKE);
		}
//===========================================================================================================		   	   
	   // ������ ����������� ��������� �����������
		if (TimerSaveStatusVvk<dt15BP) TimerSaveStatusVvk++;
		else {
			TimerSaveStatusVvk=0;
			FlagWorkRegister|=BIT(FlagSaveStatusVvk);
		}
		count_10=0;	
	}	
	FlagWorkRegister&=~BIT(Flag100msec);
}

