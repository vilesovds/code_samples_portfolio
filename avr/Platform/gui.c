#include "gui.h"

#include "buttons.h"
#include "IR.h"
#include "A4988.h"
#include "settings.h"
#include "timer2.h"
//File types definitions
#define T_FOLDER         0 //Folder file
#define T_DFOLDER        2 //Dynamic Folder
#define T_SFOLDER        3 //Select Folder
#define T_APP            4 //Application
#define T_CONF           5 //Config
#define T_SCONF          6 //Select Config
#define T_DCONF          7 //Dynamic Config
#define FILENUMB 33 //Set the total number of files in array
#define FILEREW  3//number of bytes for each file.
#define MAXDEPTH 4//Set the maximum depth of the node
#define DISPSTR 4//quantity of lines used for write menu. Display may have more lines!
#define LCDBITS (DISPSTR-1) //cursor mask 
#define LCDINVBITS ~LCDBITS //page mask
#define LCDCOL	16

//

enum {
	GUI_MOVE_TO_NEXT=0,
	GUI_MOVE_TO_PREV,
	GUI_MOVE_TO_HOME
};

static uint8_t level; //store current node level
volatile uint8_t fileNumb = 0; //store current file number
static int workType = 0; //used for locking up in applications.
volatile uint16_t configLim[4]; //used fr string parameters and current value of config file
static uint8_t brCrumbs[MAXDEPTH][3]; //bread Crumbs Array. Used for navigation, especially with dynamic and selecting files.
/*
 brCrumbs[level][0] - store number of last selected file. usually - number of folder file
 brCrumbs[level][1] - store current position of file list in the folder
 brCrumbs[level][2] - FLAG. Used for reading number of selected file
 */
//some buffers^
PGM_P header;
//static char buff[LCDCOL - 3];
static uint8_t fileData[4]; //used for storing information of current file

struct pt gui_pt;
struct pt config_pt;
struct pt fileselect_pt;
struct pt menubuttons_pt;
struct pt programselect;
struct pt resetsettings_pt;
struct pt auto_pt;
struct pt calib_pt;
struct pt manual_pt;

struct pt moveto_pt;

struct pt_sem semaphore;
static uint16_t ts_tmp;

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

static uint8_t fileStruct[FILENUMB * FILEREW] PROGMEM = { T_FOLDER, 1, 2/*2*/, //Меню//0
		T_FOLDER, 4, 2, 	//Управление //1
		T_FOLDER, 8, 10, 	//Уставки //2
		T_APP, 0, 1, 		//О системе //3

		T_FOLDER,28,4,//T_APP, 0, 2, 		//Автоматическое //4
		T_APP, 0, 3, 		//Ручное //5
		T_FOLDER, 7, 0, 	//Настройки //6
		T_CONF, 0, 0, 		//Кол-во остановок //7
		T_FOLDER, 24, 1, 	//Калибровка //8
		T_CONF, 0, 1, 		//PPS старт//9
		T_CONF, 0, 2, 		//PPS стоп//10
		T_CONF, 0, 3, 		//PPS маршевое//11
		T_CONF, 0, 4, 		//Время разгона//12
		T_CONF, 0, 5, 		//Время торможения//13
		T_CONF, 0, 6, 		//Пауза до снимка//14
		T_CONF, 0, 7, 		//Пауза после снимка//15
		T_CONF, 0, 8, 		//Инверсия напр.//16
		T_SFOLDER, 19, 4, 	//Микрошаг//17
		T_APP, 0, 5, 		//Сброс настроек //18
		T_SCONF, SETTINGS_MICROSTEP_INDX, 0, 		//Полный шаг"//19
		T_SCONF, SETTINGS_MICROSTEP_INDX, 1, 		// 1/2 //20
		T_SCONF, SETTINGS_MICROSTEP_INDX, 2, 		// 1/4 //21
		T_SCONF, SETTINGS_MICROSTEP_INDX, 3, 		// 1/8 //22
		T_SCONF, SETTINGS_MICROSTEP_INDX, 4, 		// 1/16 //23
		T_APP, 0, 4, 		//Калибровать //24
		T_FOLDER, 26, 0, 	//Настройки	//25
		T_CONF, 0, 10, 		//PPS калибровки //26
		/**/
		T_FOLDER, 28, 4,	// 27
		T_APP, 0, 2, 		// 28 Запуск
		T_APP, 0, 7, 		// 29 Снимок
		T_APP, 0, 8, 		// 30 След. точка
		T_APP, 0, 9, 		// 31 Пред. точка
		T_APP, 0, 10, 		// 32 Домой
		};
#define AUTO_SUB_MENU	27
/*===================================================================
 Array: fileNames
 Contain: names  of graph nodes
 =====================================================================*/
prog_char file_0[] PROGMEM = "              Меню";
prog_char file_1[] PROGMEM = "Управление";
prog_char file_2[] PROGMEM = "Уставки";
prog_char file_3[] PROGMEM = "О системе";
prog_char file_4[] PROGMEM = "Автоматическое";
prog_char file_5[] PROGMEM = "Ручное";
prog_char file_6[] PROGMEM = "Настройки";
prog_char file_7[] PROGMEM = "Кол-во остан.";
prog_char file_8[] PROGMEM = "Калибровка";
prog_char file_9[] PROGMEM = "PPS старт";
prog_char file_10[] PROGMEM = "PPS стоп";
prog_char file_11[] PROGMEM = "PPS маршевое";
prog_char file_12[] PROGMEM = "Время разгона";
prog_char file_13[] PROGMEM = "Время остановки";
prog_char file_14[] PROGMEM = "Пауза до снимка";
prog_char file_15[] PROGMEM = "Пауза после снимка";
prog_char file_16[] PROGMEM = "Инверсия напр.";
prog_char file_17[] PROGMEM = "Микрошаг";
prog_char file_18[] PROGMEM = "Сброс настроек";
prog_char file_19[] PROGMEM = "Полный шаг";
prog_char file_20[] PROGMEM = "1/2  шага";
prog_char file_21[] PROGMEM = "1/4  шага";
prog_char file_22[] PROGMEM = "1/8  шага";
prog_char file_23[] PROGMEM = "1/16 шага";
prog_char file_24[] PROGMEM = "Калибровать";
//prog_char file_25[] PROGMEM = "Настройки";
prog_char file_26[] PROGMEM = "PPS калибровки";


prog_char file_28[] PROGMEM = "Запуск";
prog_char file_29[] PROGMEM = "Снимок";
prog_char file_30[] PROGMEM = "След. точка";
prog_char file_31[] PROGMEM = "Пред. точка";
prog_char file_32[] PROGMEM = "Домой";
PROGMEM const char *fileNames[] = { file_0, file_1, file_2, file_3, file_4,
		file_5, file_6, file_7, file_8, file_9, file_10, file_11, file_12,
		file_13, file_14, file_15, file_16, file_17, file_18, file_19, file_20,
		file_21, file_22, file_23, file_24, file_6/*file_25*/, file_26,file_1, file_28,
		file_29, file_30, file_31, file_32};
/*===================================================================
 Array: configsLimit
 Contain: Config setup parameters
 Config may be:
 Boolean, only for Yes/No r
 Numeric, with limits, or without It.
 Every config described by three values:
 1. minimum value
 2. maximum value
 3. settings index
 If max value ==0 - there is unrated config
 If max value = 1 - there is boolean value
 Else there is limited config.
 =====================================================================*/PROGMEM static uint16_t configsLimit[] =
		{ 2, 400, SETTINGS_BPC_INDX, //0
				1, 65000, SETTINGS_START_PPS_INDX, //1
				1, 65000, SETTINGS_STOP_PPS_INDX, //2
				1, 65000, SETTINGS_MARCH_PPS_INDX, //3
				0, 5000, SETTINGS_START_TIME_INDX, //4 start time in ms
				0, 5000, SETTINGS_STOP_TIME_INDX, //5 stop time in ms
				0, 5000, SETTINGS_PAUSE_BEFORE_SHOT_INDX, //6 time in ms
				0, 0/*5000*/, SETTINGS_PAUSE_AFTER_SHOT_INDX, //7 time in ms
				0, 1, SETTINGS_DIR_INV_INDX, //8 unrated for dynamic
				0, 0, SETTINGS_TOTAL_STEPS_INDX, //9
				1, 65000, SETTINGS_CAL_PPS_INDX //10
		};

prog_char saved[] PROGMEM = "Сохранено!";

/*Prototypes*/
#if 0
static void ConfigUpButton(void);
static void ConfigDownButton(void);
#endif
PT_THREAD(Auto(void));
static void ConfigDispStatic(void);
static void ConfigDispSetup(uint8_t conftype);
static void fileReturn(void);
/******************************/
/**
 * данные о конфиге. Входной парамтер - номер конфига
 */
static void ConfigReadLimits(uint8_t cnumb)
{
	for (int i = 0; i < 3; i++)
		configLim[i] = pgm_read_word(&configsLimit[cnumb * 3 + i]);
}

/**/
static void dispStaticDraw(uint8_t buttons) //Р РёСЃСѓРµС‚ СЃРµРїР°СЂР°С‚РѕСЂС‹, Рё РєРЅРѕРїРєРё. РѕР±С‰Р°СЏ С„СѓРЅРєС†РёСЏ РґР»СЏ РІСЃРµС… РјРѕРґСѓР»РµР№.
{
	display_clear();

	switch (buttons) {
	case 0: //обычный набор кнопок
		text_set_font(FONT_BOLD);
		text_print(5, 15, PSTR("выбрать"), 0);
		text_set_font(FONT_NORMAL);
		break;
	case 1: //Надпись Сохранить
		text_set_font(FONT_BOLD);
		text_print(5, 6, PSTR("сохранить"), 0);
		text_set_font(FONT_NORMAL);
		break;
	}
}

//РџРѕР»СѓС‡РµРЅРёРµ РґР°РЅРЅС‹С… Рѕ С„Р°Р№Р»Рµ
static void fileGet(uint8_t fnumb) //РґР°РЅРЅС‹Рµ Рѕ С„Р°Р№Р»Рµ. Р’С…РѕРґРЅРѕР№ РїР°СЂР°РјРµС‚СЂ - РЅРѕРјРµСЂ С„Р°Р№Р»Р° РЎР�РЎРўР•РњРќР«Р™
{
	for (uint8_t i = 0; i < FILEREW; i++)
		fileData[i] = pgm_read_byte(&fileStruct[FILEREW * fnumb + i]); //РЎС‡РёС‚С‹РІР°РµРј РёСЃР»РѕРІС‹Рµ Р·РЅР°С‡РµРЅРёСЏ
	//strcpy_P(buffer, (char*)pgm_read_word(&(fileNames[fnumb])));//РЎС‡РёС‚С‹РІР°РµРј РЅР°Р·РІР°РЅРёРµ
	header = (PGM_P) pgm_read_word(&fileNames[fnumb]);
}

/**
 *
 */
uint16_t ConfigRead(uint16_t indx) {
	uint16_t tmp = 0;
	uint8_t ret;
	ret = SettingsGetCurrent(indx, (void*) &tmp);
	printf("Cfgread indx %u; ret %u\r\n", indx, tmp);
	if (ret == 1)
		return (uint8_t) tmp;
	return tmp;
}

/**
 *
 */
void ConfigWrite(uint16_t indx, uint16_t val) {
	//eeprom_write_byte ((unsigned char *) _param1, _param2);
	SettingsUpdateValue(indx, (void*) &val);
	printf("ConfigWrite indx %u, val %u\r\n", indx, val);
}

/**
 *
 */
static void dispHead(PGM_P head) {
	text_set_font(FONT_BOLD);
	text_print(0, 4, head, 0);
	text_set_font(FONT_NORMAL);
}

static void pageList(void) {

	char buff[(LCDCOL - 3) * 2];
	char buff2[LCDCOL * 2];
	uint8_t currs;
	uint8_t fstart;

	dispStaticDraw(0);	//рисуем статику

	switch (fileData[0]) {
	case T_FOLDER:	//обычная папка..
		if ((brCrumbs[level][1] & LCDINVBITS)== ((fileData[2]) & LCDINVBITS))currs = (fileData[2]) & LCDBITS;//Это максимальное положение курсора на последней странице
		else
		currs = LCDBITS;
		fstart = fileData[1] + (brCrumbs[level][1] & LCDINVBITS);//номер файла для старта вывода.
		for (uint8_t i = 0; i <= currs; i++) {//пишем из массива для статических папок
			text_print(i + 1, 0, (PGM_P) pgm_read_word(&fileNames[i + fstart]),
					((brCrumbs[level][1] & LCDBITS) == (i)) * 0xFF);
		}
		break;

		case T_DFOLDER:		//динамическая папка
		if ((brCrumbs[level][1] & LCDINVBITS) == ((fileData[2]) & LCDINVBITS))
		currs = (fileData[2]) & LCDBITS;//Это максимальное положение курсора на последней странице
		else
		currs = LCDBITS;
		strncpy_P(buff, (char*) pgm_read_word(&(fileNames[fileData[1]])),
				sizeof(buff));//читаем общее название
		buff[sizeof(buff) - 1] = '\0';
		for (uint8_t i = 0; i <= currs; i++) {
			snprintf(buff2, sizeof(buff2), "%s %d", buff,
					(brCrumbs[level][1] & LCDINVBITS) + i);
			buff_print(i + 1, 0, buff2,
					((brCrumbs[level][1] & LCDBITS) == (i)) * 0xFF);
		}
		break;

		case T_SFOLDER:		//папка выбора параметра
		{
			uint8_t config_folder_view = pgm_read_byte(
					&fileStruct[FILEREW * fileData[1] + 1]);//читаем байт сдвига.
			uint8_t config_folder_data = ConfigRead(
					/*brCrumbs[level][0]+*/config_folder_view);//чтение значения выбора
			if (config_folder_data > fileData[2])
			config_folder_data = fileData[2];
			if (config_folder_view < 0)
			config_folder_view = 0;
			if (brCrumbs[level][2] == 0) {
				brCrumbs[level][1] = config_folder_data;//ставим курсор в выбранную позицию.
				brCrumbs[level][2] = 1;
			}
			if ((brCrumbs[level][1] & LCDINVBITS) == ((fileData[2]) & LCDINVBITS))
			currs = (fileData[2]) & LCDBITS;//Это максимальное положение курсора на последней странице
			else
			currs = LCDBITS;
			fstart = fileData[1] + (brCrumbs[level][1] & LCDINVBITS);//номер файла для старта вывода.
			for (uint8_t i = 0; i <= currs; i++) { //пишем из массива для статических папок
				text_print(i + 1, 0, (PGM_P) pgm_read_word(&fileNames[i + fstart]),
						((brCrumbs[level][1] & LCDBITS) == (i)) * 0xFF);
			}
		}
		break;

	}
	dispHead(header);
	// stringSelect();
}

static void displayPosition(uint8_t p) {
	uint8_t x;
	x = text_print(p, 0, PSTR("Поз. :"), 0);
	text_clear_from(p, x);
	text_unum(p, 30, A4988GetPos(), 0, 0);
}

PT_THREAD( Manual(void) ){
	static int8_t butt;
PT_BEGIN(&manual_pt);
	display_clear();
	while (workType) {
		displayPosition(2);
		ts_tmp = timer2_getCounter();
		PT_WAIT_UNTIL(&manual_pt,(((butt = get_buttons())>0)||(time_interval(ts_tmp)>20)));
		if (butt > 0) { //если обновили информацию
			switch (butt) {
			case F_BUTOK:
				IR_shot(3);
				break;
			case F_BUTUP:
				A4988SetSteps(1);
				A4988start(0, &manual_pt,&semaphore);
				PT_SEM_WAIT(&manual_pt,&semaphore);
				break;
			case F_BUTDOWN:
				A4988SetSteps(48 * 5);
				A4988start(1, &manual_pt, &semaphore);
				PT_SEM_WAIT(&manual_pt,&semaphore);
				break;
			case F_BUTCANCEL:
				workType = 0;
				break;
			}
		}
	}
PT_END(&manual_pt);
}

/**
 *
 */
PT_THREAD(MoveTo(uint8_t to))
{
	static int8_t butt;
	PT_BEGIN(&moveto_pt);
	display_clear();
	switch(to){
		case GUI_MOVE_TO_NEXT:
			A4988MoveNext();
			break;
		case GUI_MOVE_TO_PREV:
			A4988MovePrev();
			break;
		case GUI_MOVE_TO_HOME:
			A4988MoveHome();
			break;
		default:
			workType = 0;
			break;
	}
	while(workType){
		//TODO progressbar
		displayPosition(2);
		ts_tmp = timer2_getCounter();
		PT_WAIT_UNTIL(&moveto_pt,(((butt = get_buttons())>0)||(time_interval(ts_tmp)>20)));
		if(butt){
			switch (butt) {
				case F_BUTOK:
					A4988StopAutoRotate();
					display_clear();
					//TODO Print Остановка
					PT_WAIT_UNTIL(&moveto_pt,(A4988GetSM()==A4988_SPEEDSM_COMPLETED));
					workType = 0;
					break;
			}
		}
		if(A4988GetSM()==A4988_SPEEDSM_COMPLETED){
			workType = 0;
			break;
		}
	}
	PT_END(&moveto_pt);
}

/**
 *
 */
static void takePhoto(void)
{
	display_clear();
	text_print(3, 20, PSTR("Птичка!"), 0);
	IR_shot(3);
}

/**
 *
 */
static int PositionToStep(uint16_t position, uint8_t *step )
{
	uint8_t s;
	uint16_t p;
	s = (uint32_t)position * A4988bpc/A4988totalSteps;
	p = (uint32_t)s*A4988totalSteps/A4988bpc;
	if(step) *step = s;
	if(p==position) return 1;
	else return 0;
}

/**
 *
 */
PT_THREAD( Auto(void)) {
	// TODO global butt
	static int8_t butt;
	static uint8_t substep = 0;
PT_BEGIN(&auto_pt);
	display_clear();

	while (workType) {
		displayPosition(2);
		ts_tmp = timer2_getCounter();
		PT_WAIT_UNTIL(&auto_pt,(((butt = get_buttons())>0)||(time_interval(ts_tmp)>20)));
		if (butt > 0) { //если обновили информацию
			switch (butt) {
			case F_BUTOK:
			case F_BUTCANCEL:
				A4988StopAutoRotate();
				display_clear();
				//TODO Print Остановка
				PT_WAIT_UNTIL(&auto_pt,(A4988GetSM()==A4988_SPEEDSM_COMPLETED));
				workType = 0;
				break;
			}
		}
		switch(substep){
		case 0:
			if(PositionToStep(A4988GetPos(),NULL)==0){
				substep = 2;
				break;
			}
			takePhoto();
			ts_tmp = timer2_getCounter();
			PT_WAIT_UNTIL(&auto_pt,(time_interval(ts_tmp)>=A4988pauseAfterShot));
			display_clear();
			substep ++;
			break;
		case 1:
			//PT_WAIT_UNTIL(&auto_pt,(time_interval(ts_tmp)>=A4988pauseAfterShot));
			//display_clear();
			substep ++;
			break;
		case 2:
			A4988MoveNext();
			substep++;
			break;
		case 3:
			if(A4988GetSM()!=A4988_SPEEDSM_COMPLETED) break;
			if(A4988GetPos()==0){ // END
				// TODO Beep
				ts_tmp = timer2_getCounter();
				text_print(3, 15, PSTR("Закончено!"), 0);
				PT_WAIT_UNTIL(&auto_pt,(time_interval(ts_tmp)>=3000));
				substep  = 0;
				workType = 0;
			}else{
				substep++;
			}
			break;
		case 4:
			ts_tmp = timer2_getCounter();
			substep++;
			break;
		case 5:
			PT_WAIT_UNTIL(&auto_pt,(time_interval(ts_tmp)>=A4988pauseBeforeShot));
			substep = 0;
			break;
		}
	}
PT_END(&auto_pt);
}

inline void dipslayCalib(uint8_t start) {
	text_set_font(FONT_BOLD);
	text_clear_from(5, 0);
	text_print(5, 25, start ? PSTR("старт") : PSTR("стоп"), 0);
	text_set_font(FONT_NORMAL);
}

PT_THREAD( Calib(void)) {
	static int8_t butt;
	static uint8_t play = 0;
PT_BEGIN(&calib_pt);
	A4988SetPPS(A4988calibPPS);
	A4988SetPos(ConfigRead(SETTINGS_TOTAL_STEPS_INDX));
	display_clear();
	text_print(0, 0, PSTR("Калибровка"), 0); //пишем*
	text_print(3, 0, PSTR("Нажмите \"ОК\" для"), 0);
	text_print(4, 0, PSTR("начала калибров"), 0);
	text_print(5, 0, PSTR("ки"), 0);
	while (workType) {
		displayPosition(2);
		ts_tmp = timer2_getCounter();
		PT_WAIT_UNTIL(&calib_pt,(((butt = get_buttons())>0)||(time_interval(ts_tmp)>20)));
		if (butt > 0) { //если обновили информацию
			switch (butt) {
			case F_BUTOK:
				switch (play) {
				case 0:
					A4988SetPos(0);
					// set maximum total steps
					ConfigWrite(SETTINGS_TOTAL_STEPS_INDX, 0xFFFF);
					text_clear_from(3, 0);
					text_clear_from(4, 0);
					dipslayCalib(1);
					play = 1;
					break;
				case 1:
					A4988SetSteps(0xFFFF);
					A4988start(0, 0,0);
					dipslayCalib(0);
					play = 2;
					break;
				case 2:
					A4988stop();
					dipslayCalib(1);
					play = 1;
					break;
				}
				break;
			case F_BUTUP:
				A4988SetSteps(1);
				A4988start(0, &calib_pt,&semaphore);
				PT_SEM_WAIT(&calib_pt,&semaphore);
				if (play) {
					dipslayCalib(1);
					play = 1;
				}
				break;
			case F_BUTDOWN:
				A4988SetSteps(1);
				A4988start(1, &calib_pt,&semaphore);
				PT_SEM_WAIT(&calib_pt,&semaphore);
				if (play) {
					dipslayCalib(1);
					play = 1;
				}
				break;
			case F_BUTCANCEL:
				A4988stop();
				play = 0;
				ConfigWrite(SETTINGS_TOTAL_STEPS_INDX, A4988GetPos()); // save total steps
				A4988SetPos(0); // reset position
				workType = 0;

				break;
			}
		}
	}
PT_END(&calib_pt);
}

PT_THREAD(ResetSettings(void)) {
	static int8_t butt;
	PT_BEGIN(&resetsettings_pt);
	display_clear();
	text_print(2, 0, PSTR("Нажмите \"+\" "), 0); //пишем*
	text_print(3, 0, PSTR("для сброса"), 0); //пишем*
	while (workType) {
		PT_WAIT_UNTIL(&resetsettings_pt,((butt = get_buttons())>0));
		if (butt > 0) { //если обновили информацию
			switch (butt) {
			case F_BUTOK:

				break;
			case F_BUTUP:
				SettingsLoadDefaults();
				SettingsSaveAll();
				display_clear();
				text_set_font(FONT_BOLD);
				text_print(2, 0, PSTR("Готово!"), 0); //пишем*
				text_set_font(FONT_NORMAL);
				ts_tmp = timer2_getCounter();
				PT_WAIT_UNTIL(&resetsettings_pt,(time_interval(ts_tmp)>1500));
				//vTaskDelay(1500);
				workType = 0;
				break;
			case F_BUTDOWN:
				break;
			case F_BUTCANCEL:
				workType = 0;
				break;
			}
		}
	}
	PT_END(&resetsettings_pt);
}

//обработчики типов файлов
PT_THREAD(programSelect(void)) //тип файла 0x01
{
	PT_BEGIN(&programselect);
	ts_tmp = timer2_getCounter();
	switch (fileData[2]) {
	case 1: //Звывод инфо о программе
		display_clear();
		dispHead(PSTR("О программе"));
		text_print(1, 0, PSTR("Platform:"), 0);
		text_print(2, 0, PSTR("v1.1"), 0);
		text_print(3, 0, PSTR("Created by"), 0);
		text_print(4, 0, PSTR("Vildi     2014"), 0);
		PT_WAIT_UNTIL(&programselect,(time_interval(ts_tmp)>=3000));
		//vTaskDelay(3000);
		workType = 0;
		break;
	case 2:
		workType = 1; //включили зацикливание программы
		PT_WAIT_THREAD(&programselect, Auto());
		break;
	case 3:
		workType = 1; //включили зацикливание программы
		PT_WAIT_THREAD(&programselect, Manual());
		break;
	case 4: //Калибровка
		workType = 1; //включили зацикливание программы
		PT_WAIT_THREAD(&programselect, Calib());
		break;
	case 5:
		workType = 1; //включили зацикливание программы
		PT_WAIT_THREAD(&programselect, ResetSettings());
		break;
	case 7:
		takePhoto();
		PT_WAIT_UNTIL(&programselect,(time_interval(ts_tmp)>=1000));
		break;
	case 8:
		workType = 1;
		PT_WAIT_THREAD(&programselect, MoveTo(GUI_MOVE_TO_NEXT));
		break;
	case 9:
		workType = 1;
		PT_WAIT_THREAD(&programselect, MoveTo(GUI_MOVE_TO_PREV));
		break;
	case 10:
		workType = 1;
		PT_WAIT_THREAD(&programselect, MoveTo(GUI_MOVE_TO_HOME));
		break;
	}

	fileReturn();
	PT_END(&programselect);
}

#define GUI_BUTTONS_L1	50
#define GUI_BUTTONS_L2	500
#define GUI_BUTTONS_L3	5000

PT_THREAD(configSelect(uint8_t cnumb, uint8_t conftype)) {
	static uint8_t DConf_Shift = 0;
	static uint8_t butt;
	static uint16_t ts_;
	static uint16_t changedcnt = 0;
	static uint16_t d = 1;

	PT_BEGIN(&config_pt)
	;
	workType = 2;
	ConfigReadLimits(cnumb);
	if (conftype)
		DConf_Shift = brCrumbs[level - 1][1];
	if (configLim[1] == 0)
		configLim[3] = ConfigRead(configLim[2] + DConf_Shift * 2); //прочитали 2 байта.
	else
		configLim[3] = ConfigRead(configLim[2] + DConf_Shift);
	ConfigDispSetup(conftype);

	while (workType == 2) { //запускаем программу редактирования конфигов
		ts_ = timer2_getCounter();
		PT_WAIT_UNTIL(&config_pt,
				(((butt = get_buttons()) > 0) || (time_interval(ts_) >= 500))); //500ms timeout
		if (butt > 0) { //если обновили информацию
			switch (butt) {
			case F_BUTUP: //кнопка  +
				//ConfigUpButton();
				if ((changedcnt >= GUI_BUTTONS_L1)
						&& (changedcnt < GUI_BUTTONS_L2)
						&& ((configLim[3] % 10) == 0) && (d < 10)) {
					d = 10;
				} else if ((changedcnt >= GUI_BUTTONS_L2)
						&& (changedcnt < GUI_BUTTONS_L3)
						&& ((configLim[3] % 100) == 0) && (d < 100)) {
					d = 100;
				} else if ((changedcnt >= GUI_BUTTONS_L3)
						&& ((configLim[3] % 500) == 0) && (d < 1000)) {
					d = 500;
				}

				if (configLim[1] == 0) { //значит что без ограничений
					if (configLim[3] < (0xFFFF - d)) {
						configLim[3] += d;
						changedcnt += d;
					} else if (configLim[3] != 0xFFFF) {
						configLim[3] = 0xFFFF;
						changedcnt = 0;
					}
				} else if (configLim[1] == 1) { //значит что вырианты только да или нет
					configLim[3] = !configLim[3];
				} else { //изменяющееся:
					if (configLim[3] < (configLim[1] - d)) {
						configLim[3] += d;
						changedcnt += d;
					} else if (configLim[3] != configLim[1]) {
						configLim[3] = configLim[1];
						changedcnt = 0;
					}
				}
				break;
			case F_BUTDOWN:
				if ((changedcnt >= GUI_BUTTONS_L1)
						&& (changedcnt < GUI_BUTTONS_L2)
						&& ((configLim[3] % 10) == 0) && (d < 10)) {
					d = 10;
				} else if ((changedcnt >= GUI_BUTTONS_L2)
						&& (changedcnt < GUI_BUTTONS_L3)
						&& ((configLim[3] % 100) == 0) && (d < 100)) {
					d = 100;
				} else if ((changedcnt >= GUI_BUTTONS_L3)
						&& ((configLim[3] % 500) == 0) && (d < 1000)) {
					d = 500;
				}

				if (configLim[1] == 0) { //значит что без ограничений
					if (configLim[3] > d) {
						configLim[3] -= d;
						changedcnt += d;
					} else if (configLim[3] != 0) {
						configLim[3] = 0;
						changedcnt = 0;
					}
				} else if (configLim[1] == 1) { //значит что вырианты только да или нет
					configLim[3] = !configLim[3];
				} else { //изменяющееся:
					if ((configLim[3] - configLim[0]) > d) {
						configLim[3] -= d;
						changedcnt += d;
					} else if (configLim[3] != configLim[0]) {
						configLim[3] = configLim[0];
						changedcnt = 0;
					}
				}
				break;
			case F_BUTOK:
				if (configLim[1] == 0) {
					ConfigWrite(configLim[2] + DConf_Shift * 2, configLim[3]);
				} else {
					ConfigWrite(configLim[2] + DConf_Shift * 2, configLim[3]);
				}

				changedcnt = 0;
				d = 1;
				display_clear();
				text_set_font(FONT_BOLD);
				text_print(2, 5, saved/*PSTR("Сохранено!")*/, 0); //пишем*
				text_set_font(FONT_NORMAL);
				ts_tmp = timer2_getCounter();
				PT_WAIT_UNTIL(&config_pt,(time_interval(ts_tmp)>=1000));
				workType = 0;
				break;
			case F_BUTCANCEL:
				changedcnt = 0;
				d = 1;
				workType = 0;
				break;
			}
			ConfigDispStatic();
		} else {
			// button released
			changedcnt = 0;
			d = 1;
		}
	} // while
	fileReturn();
	PT_END(&config_pt);
}

PT_THREAD(fileSelect(struct pt *pt)) { //вычисляем новый номер файла. Для этого смотрим, мы в обычной папке. или динамической?
	PT_BEGIN(pt)
	;
	if (!fileData[0]) //обычная папка. В ней все больно просто
	{
		fileNumb = fileData[1] + brCrumbs[level][1]; //берем номер стартового файла и прибавляем сколько мы вниз нащелкали.
	} else //динамическая папка тут все еще проще
	{
		fileNumb = fileData[1]; //ЖЖОШ!
	}
	level++; //на уровень глубже
	brCrumbs[level][0] = fileNumb; //вошли внутрь? супер, запишем инфу о нашем местоположении.
	brCrumbs[level][1] = 0;
	brCrumbs[level][2] = 0;
	fileGet(fileNumb); //считываем информацию
	uint8_t fileType = fileData[0]; //Читаем 1 байт файла
	switch (fileType) //сюда добавляем в зависимости от типа файла.
	{
	case T_FOLDER:
	case T_DFOLDER:
	case T_SFOLDER:
		pageList();
		break;

	case T_CONF:
		PT_WAIT_THREAD(pt, configSelect(fileData[2], 0));
		break;
	case T_SCONF: //обновление конфига
		ConfigWrite(fileData[1] /*+ brCrumbs[level - 1][0]*/,
				brCrumbs[level - 1][1]);
		display_clear();
		text_set_font(FONT_BOLD);
		text_print(2, 5, saved/*PSTR("Сохранено!")*/, 0); //пишем*
		text_set_font(FONT_NORMAL);
		ts_tmp = timer2_getCounter();
		PT_WAIT_UNTIL(pt,(time_interval(ts_tmp)>=1000));
		//vTaskDelay(1000);
		fileReturn();
		break;
	case T_DCONF:
		//configSelect(fileData[2], 1);
		PT_WAIT_THREAD(pt, configSelect(fileData[2], 1));
		break;

	case T_APP:
		PT_WAIT_THREAD(pt,programSelect());
		break;

	}PT_END(pt);
}

static void fileReturn(void) {
	//во первых, удалим текущую информацию о местоположении:
	brCrumbs[level][0] = 0;
	brCrumbs[level][1] = 0;
	brCrumbs[level][2] = 0;
	//вернемся на уровень назад, только если он еще больше нуля. А то уйдем нахер в минуса.
	if (level > 0)
		level--; //находясь на уровне выше, уже можно прочесть инфу о родителе:
	fileNumb = brCrumbs[level][0]; //при этом положение курсора не сбрасывается! мы вернемся туда, откуда пришли.
	fileGet(fileNumb); //Читаем инфу о файле.
	pageList();
}

static void menu_init(void) {
	display_clear();
	fileNumb = 0;
	fileGet(fileNumb);
	dispHead(header);
	pageList();
}
#if 0
static void ConfigUpButton(void) {
	if (configLim[1] == 0) { //значит что без ограничений
		configLim[3]++;
	} else if (configLim[1] == 1) { //значит что вырианты только да или нет
		configLim[3] = !configLim[3];
	} else { //изменяющееся:
		if (configLim[3] < configLim[1])
		configLim[3]++;
		//else configLim[3]=configLim[0];
	}
}

static void ConfigDownButton(void) {
	if (configLim[1] == 0) { //значит что без ограничений
		if (configLim[3] > 0)
		configLim[3]--;
		else
		configLim[3] = 0;
	} else if (configLim[1] == 1) { //значит что вырианты только да или нет
		configLim[3] = !configLim[3];
	} else { //изменяющееся:
		if (configLim[3] > configLim[0])
		configLim[3]--;
		//else configLim[3]=configLim[1];
	}
}
#endif
#if 1
static void ConfigDispSetup(uint8_t conftype) {
	dispStaticDraw(1);
	dispHead(PSTR("Настройка"));
	if (conftype) {
		//DispCursor(11,ROWSHIFT);//СЃС‚Р°РІРёРј РєСѓСЂСЃРѕСЂ
		//DispNumber(brCrumbs[level-1][1]);
		//text_unum();
	}
	text_print(1, 11, header, 0);
	ConfigDispStatic();
}

#endif
//prog_char cur[] PROGMEM = "РўРµРєСѓС‰РµРµ:       ";
static void ConfigDispStatic(void) {
	uint8_t x;
	PGM_P tek = PSTR("Тек. :");
	switch (configLim[1]) {

	case 0:		//значит что без ограничений
		x = text_print(2, 0, tek/*PSTR("Тек. :")*/, 0);
		text_clear_from(2, x);
		text_unum(2, 30, configLim[3], 0, 0);
		break;
	case 1:		//значит что вырианты только да или нет
		if (configLim[3])
			text_print(2, 0, PSTR("Тек. : Да   "), 0);
		else
			text_print(2, 0, PSTR("Тек. : Нет     "), 0);
		break;

	default:
		x = text_print(2, 0, tek/*PSTR("Тек. :")*/, 0);
		text_clear_from(2, x);
		text_unum(2, 30, configLim[3], 0, 0);

		x = text_print(3, 0, PSTR("Мин. :"), 0);
		text_clear_from(3, x);
		text_unum(3, 30, configLim[0], 0, 0);

		x = text_print(4, 0, PSTR("Макс.:"), 0);
		text_clear_from(4, x);
		text_unum(4, 30, configLim[1], 0, 0);
		break;
	}

}

/**************************************/
/**/
//обработчики кнопок меню
PT_THREAD(menuButtons(uint8_t but))		//Обработчик кнопок в меню
{
	PT_BEGIN(&menubuttons_pt)
	;
	switch (but) {
	case F_BUTCANCEL:			//<
		fileReturn();
		break;
	case F_BUTUP:		//- и все что нам надо - изменить положение курсора :)
		if (brCrumbs[level][1] > 0)
			brCrumbs[level][1]--;
		else
			brCrumbs[level][1] = fileData[2];//дошли до начала, начали с конца.
		pageList();			//обрисовываем экран только когда это требуется
		break;
	case F_BUTDOWN:			//+ тут то же самое
		if (brCrumbs[level][1] < fileData[2])
			brCrumbs[level][1]++;
		else
			brCrumbs[level][1] = 0;	//не дошли еще до конца? тогда стремимся к нему. Дошли? класс, начинаем сначала
		pageList();
		break;
	case F_BUTOK:			//>
		PT_WAIT_UNTIL(&menubuttons_pt, fileSelect(&fileselect_pt));
		break;
	}PT_END(&menubuttons_pt);
}

void gui_init(void) {
	menu_init();
	PT_INIT(&gui_pt);
	PT_INIT(&config_pt);
	PT_INIT(&fileselect_pt);
	PT_INIT(&menubuttons_pt);
	PT_INIT(&programselect);
	PT_INIT(&resetsettings_pt);
	PT_INIT(&auto_pt);
	PT_INIT(&calib_pt);
	PT_INIT(&moveto_pt);
	PT_SEM_INIT(&semaphore,1);

}

PT_THREAD(gui_task(struct pt *pt)) {
	static uint8_t b;

	PT_BEGIN(pt)
	;
	while (1) {
		PT_WAIT_UNTIL(pt, ((b = get_buttons()) > 0));
		if (b > 0) {
			PT_WAIT_THREAD(pt, menuButtons(b));
		}
	}PT_END(pt);
}
