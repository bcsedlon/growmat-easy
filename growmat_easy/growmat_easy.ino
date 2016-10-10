
#define VERSION 0.1

#define LCD_I2CADDR 0x20
const byte LCD_ROWS = 2;
const byte LCD_COLS = 16;

#define KPD_I2CADDR 0x20
const byte KPD_ROWS = 4;
const byte KPD_COLS = 4;

#define LEDPIN 13

#define LIGHTPIN A0
#define DHTPIN 4
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define DS3231_I2C_ADDRESS 0x68

//const byte LIGHTMODE_ADDR 4
#define LIGHTMODE_ADDR 4
#define LIGHTONHOUR_ADDR 8
#define LIGHTONMIN_ADDR 12
#define LIGHTOFFHOUR_ADDR 16
#define LIGHTOFFMIN_ADDR 20

#define HEATERMODE_ADDR 24
#define HEATERONTEMP_ADDR 28
#define HEATEROFFTEMP_ADDR 32

#define VENTMODE_ADDR 36
#define VENTONTEMP_ADDR 40
#define VENTOFFTEMP_ADDR 44

#define CYCLERMODE_ADDR 48
#define CYCLERONMIN_ADDR 52
#define CYCLERONSEC_ADDR 56
#define CYCLEROFFMIN_ADDR 60
#define CYCLEROFFSEC_ADDR 64

#define TEMPHIGHTEMPALARM_ADDR 68
#define TEMPLOWTEMPALARM_ADDR 72
#define LIGHTVALUEALARM_ADDR 76

#define GSMNUMBER_ADDR 80  //16 bytes
#define GSMCODE_ADDR 96
#define GSMMODE_ADDR 98

#define MESSAGESOFFSET_ADDR 100
#define MESSAGES_ADDR 104
#define MESSAGESCOUNT 32
#define MESSAGELENGTH 16

//                              "0123456789ABCDEF"
#define MESSAGE_ALARM_POWERON   "dd/mm hh:mm ON x"
#define MESSAGE_ALARM_TEMPHIGH  "dd/mm hh:mm T+ x"
#define MESSAGE_ALARM_TEMPLOW   "dd/mm hh:mm T- x"
#define MESSAGE_ALARM_LIGHTHIGH "dd/mm hh:mm L+ x"
#define MESSAGE_ALARM_LIGHTLOW  "dd/mm hh:mm L- x"
#define MESSAGE_ALARM_ON  '1'
#define MESSAGE_ALARM_OFF '0'

#define UISTATE_ALARMLIST 1
#define UISTATE_SETCLOCK 2


//////////////////////////////////
// variables
//////////////////////////////////
DateTime nowSetClock;
DateTime now;
long secondstime;
bool secToggle = false;

char gsmNumber[16];
uint16_t gsmCode = 9999;
byte gsmMode = 0;

char pressed = 0;
int uiState, uiPage;

//unsigned long gsmTime = 0;
//unsigned long uiTime = 0; // ui counter
//int uiState = 0;

//unsigned long cTime = 0;

// control
byte lightMode, heaterMode, ventMode, cyclerMode;
bool lightControl, heaterControl, ventControl, cyclerControl;
bool lightAuto, heaterAuto, ventAuto, cyclerAuto;
unsigned long cyclerDuration = 0; // cycler counter
//byte alarm, tempHighAlarm, tempLowAlarm, lightHighAlarm, lightLowAlarm;

// inputs
float temperature, humidity, heatIndex;
float light;

// parameters
unsigned int lightOnHour, lightOnMin, lightOffHour, lightOffMin;
float heaterOnTemp, heaterOffTemp;
float ventOnTemp, ventOffTemp;
unsigned int cyclerOnMin, cyclerOnSec, cyclerOffMin, cyclerOffSec;
float tempHighTempAlarm, tempLowTempAlarm, lightValueAlarm;

float tempHysteresis = 0.5;
float lightHysteresis = 10;





/*
class Instrument {
	//not used yet !!!
public:
	bool control = false;
	byte mode = 0;
	char name;

	Instrument(char ch) {
		name = ch;
	}

	String getString() {
		char b[4];
		b[0] = name;
		b[1] = control ? '0' : '1';
		b[2] = mode ? 'M' : 'A';
		b[3] = 0;
		return String(b);
	}
} light2('L'), heater2('H'), vent2('V'), cycler2('C');
*/

class Alarm {
	//unsigned long timeActive;
	//unsigned long timeDeactive;

	long timeActive;
	long timeDeactive;

 public:
	// TODO: fix when millis overrun!!!
	int delay = 5;
	bool active;
	bool unAck;

	bool activate(bool state) {
		if(state) {
			if(!active) {
				if(!timeActive)
					//timeActive = millis();
					timeActive = secondstime;
				//if((timeActive + delay) < millis()) {
				if((timeActive + delay) < secondstime) {
					active = true;
					unAck = true;
					timeActive = 0;
					return true;
				}
			}
		}
		else {
			timeActive = 0;
		}
		return false;
	};

	bool deactivate(bool state) {
		if(state){
			if(active) {
				if(!timeDeactive)
					//timeDeactive = millis();
					timeDeactive = secondstime;
				//if((timeDeactive + delay) < millis()) {
				if((timeDeactive + delay) < secondstime) {
					active = false;
					timeDeactive = 0;
					return true;
				}
			}
		}
		else
			timeDeactive = 0;
		return false;
	}

	void ack() {
		unAck = false;
	}
};

Alarm tempHighAlarm2, tempLowAlarm2, lightHighAlarm2, lightLowAlarm2;


#include <Wire.h>

// SIM800L
#include "libraries/Sim800l/Sim800l.h"
#include <SoftwareSerial.h> //is necessary for the library!!
Sim800l sim800l;  //to declare the library


void infoSerial(HardwareSerial *ser);

class GsmManager {
public:
	Sim800l* sim;
	//uint8_t callStatus;
	//00420123456789

	char* gsmNumber;
	//String code;
	int* gsmCode;
	//String response = String("");

	//unsigned long callDuration;
	long secondsCall;

	GsmManager(Sim800l* sim_c, int* gsmCode_c, char* gsmNumber_c) {
		sim = sim_c;
		gsmCode = gsmCode_c;
		gsmNumber = gsmNumber_c;
	}

	bool update() {
		//sim->readSerial();
		return false;
	}

	void call(){
		//if(!callDuration) {
		if(!secondsCall && gsmMode > 1) {
			Serial.println("CALLING ...");
			sim->callNumber(gsmNumber);
			//callDuration= millis();
			secondsCall = secondstime;
		}

	}
	void updateCall() {
		//return;
		//callStatus = sim->getCallStatus();
		// TODO: solve millis overflow
		//if(callDuration && (callDuration + 30000 < millis())){
		if(secondsCall && (secondsCall + 30 < secondstime)){
			Serial.println("HANG UP");
			sim->hangoffCall();
			//callDuration = 0;
			secondsCall = 0;
		}
	}

	bool sendSMS(char* text) {
		// TODO: check if sim is busy
		return sim->sendSms(gsmNumber, text);
	}


	bool proceedSMS() { //byte  *lightMode, byte *heaterMode,byte *ventMode,byte *cyclerMode) {

		//if(callDuration)
		//if(!gsmMode)
		//	return false;

		String text = sim->readSms(21);

		//TODO TEST
		text = "#CODE1040 #?";

		if(text != "") {
			sim->delAllSms();

			if(gsmMode) {
				int pos;
				char ch;


				Serial.println(text);

				pos = text.indexOf("#CODE");
				if(text.substring(pos+5, pos+9) == String(*gsmCode, DEC)) {
					Serial.println("CODE OK");

					pos = text.indexOf("#00");
					if(pos > -1) {
						// save new gsm number
						text.substring(pos+1).toCharArray(gsmNumber, 16, 0);
						for(int i=0; i < 16; i++) {
							OMEEPROM::write(GSMNUMBER_ADDR + i, gsmNumber[i]);
						}
					}


					pos = text.indexOf("#?");
					if(pos > -1) {// && gsmMode > 1) {
						//TODO
						//char msg[256];// = "GROWMAT INFO";
						//bool r = sim->sendSms(gsmNumber, msg);
						Serial.println("SEND SMS");
					}

					pos = text.indexOf("#L");
					if(pos > -1) {
						//Serial.println("LIGHT");
						ch = text.charAt(pos+2);
						if(ch=='A') lightMode = 0;
						else if(ch=='0') lightMode = 1;
						else if(ch=='1') lightMode = 2;
					}
					pos = text.indexOf("#H");
					if(pos > -1) {
						ch = text.charAt(pos+2);
						if(ch=='A') heaterMode = 0;
						else if(ch=='0') heaterMode = 1;
						else if(ch=='1') heaterMode = 2;
					}
					pos = text.indexOf("#V");
					if(pos > -1) {
						ch = text.charAt(pos+2);
						if(ch=='A') ventMode = 0;
						else if(ch=='0') ventMode = 1;
						else if(ch=='1') ventMode = 2;
					}
					pos = text.indexOf("#C");
					if(pos > -1) {
						ch = text.charAt(pos+2);
						if(ch=='A') cyclerMode = 0;
						else if(ch=='0') cyclerMode = 1;
						else if(ch=='1') cyclerMode = 2;
					}
				}
			}
		}
		return false;
	}



	/*

	return false;
		//Serial.println("start proceedSMS");

		/*
		Serial.println("1");
		Serial.println(sim->response1);
		sim->writeSerial("AT+CMGR=21\r\n");
		Serial.println("2");
		Serial.println(sim->response1);
		*/




		//String text = sim->response1;

		//Serial.print("SMS: ");
		//Serial.println(text);

		//Serial.println( text.indexOf("\n"));

/*
		for(int i = 0; i < 2; i++)
			text = text.substring(text.indexOf("\n")+1);
		text = text.substring(0, text.indexOf("\n")-1);
		Serial.println(text);
*/

		//return false;
		//String text;
		//Serial.println(response);
		//Serial.println("!");
/*
		String text=String("");
		if(response.indexOf("CMGR:")!=-1) {
			text = response.substring(response.indexOf("CMGR:")+5, response.lastIndexOf("\n")-1);
		};
		Serial.println(text);*/

/*
		int pos;
		char ch;
		Serial.println(text);
		if(text != "") {
			pos = text.indexOf("#CODE");
			if(text.substring(pos+5, pos+9) == String(*gsmCode, DEC)) {
				Serial.println("PASSWORD OK");
				pos = text.indexOf("#00");
				if(pos > -1) {
					// save new gsm number
					text.substring(pos+1).toCharArray(gsmNumber, 16, 0);
					for(int i=0; i < 16; i++) {
						OMEEPROM::write(GSMNUMBER_ADDR + i, gsmNumber[i]);
					 }
				}

				pos = text.indexOf("#?");
				if(pos > -1) {
					//TODO
					char* msg = "GROWMAT INFO";
					bool r = sim->sendSms(gsmNumber, msg);

					Serial.print("SMS INFO: ");
					//delay(250);
					//Serial.println(r);
				}

				pos = text.indexOf("#L");
				if(pos > -1) {
					//Serial.println("LIGHT");
					ch = text.charAt(pos+2);
					if(ch=='A')
						*lightMode = 0;
					else if(ch=='0') {
						*lightMode = 1;
						//lightControl = 0;
					}
					else if(ch=='1') {
						*lightMode = 2;
						//lightControl = 1;
					}
				}
				pos = text.indexOf("#H");
				if(pos > -1) {
					ch = text.charAt(pos+2);
					if(ch=='A')
						*heaterMode = 0;
					else if(ch=='0')
						*heaterMode = 1;
					else if(ch=='1')
						*heaterMode = 2;
				}
				pos = text.indexOf("#V");
				if(pos > -1) {
					ch = text.charAt(pos+2);
					if(ch=='A')
						*ventMode = 0;
					else if(ch=='0')
						*ventMode = 1;
					else if(ch=='1')
						*ventMode = 2;
				}
				pos = text.indexOf("#C");
				if(pos > -1) {
					ch = text.charAt(pos+2);
					if(ch=='A')
						*cyclerMode = 0;
					else if(ch=='0')
						*cyclerMode = 1;
					else if(ch=='1')
						*cyclerMode = 2;
				}
			}
			sim->delAllSms();
		}
		//Serial.println("finish proceedSMS");
		return false;
	};
*/

} gsmMgr(&sim800l, &gsmCode, gsmNumber);

// RTC
#include "libraries/RTClib/RTClib.h"
RTC_DS3231 rtc;

// DHT
#include "libraries/DHT-sensor-library-master/DHT.h"
DHT dht(DHTPIN, DHTTYPE);





// Keypad 4x4 i2c
#include "libraries/Keypad_I2C/Keypad_I2C.h"
#include "libraries/Keypad/Keypad.h"

class Keypad_I2C2 : public Keypad_I2C {
	unsigned long kTime;

public:

    Keypad_I2C2(char *userKeymap, byte *row, byte *col, byte numRows, byte numCols, byte address, byte width = 1) : Keypad_I2C(userKeymap, row, col, numRows, numCols, address, width) {
    };

    char Keypad_I2C2::getKey2() {
    	getKeys();

    	//TODO !!! Dirty trick !!!
    	if(bitMap[3] == 4) {
    		if(bitMap[2] == 8) lightMode=1;
    		if(bitMap[1] == 8) lightMode=2;
    		if(bitMap[0] == 8) lightMode=0;

    		if(bitMap[2] == 4) heaterMode=1;
    		if(bitMap[1] == 4) heaterMode=2;
    		if(bitMap[0] == 4) heaterMode=0;

    		if(bitMap[2] == 2) ventMode=1;
    		if(bitMap[1] == 2) ventMode=2;
    		if(bitMap[0] == 2) ventMode=0;

    		if(bitMap[2] == 1) cyclerMode=1;
    		if(bitMap[1] == 1) cyclerMode=2;
    		if(bitMap[0] == 1) cyclerMode=0;

    		return NO_KEY;
    	}

    	//Serial.println(kTime);
    	if(bitMap[0] || bitMap[1] || bitMap[2] || bitMap[3]) {
    		if(!kTime) {
    			kTime = millis();
    		}
    		if((kTime + 600) > millis()){
    			if((kTime + 200) < millis()) {
    				return NO_KEY;
    			}
    		}
    	}
        else
        	kTime = 0;

    	if(bitMap[3] == 1) return '*';
    	if(bitMap[0] == 8) return 'A';
    	if(bitMap[0] == 4) return 'B';
    	if(bitMap[0] == 1) return 'D';

    	if(bitMap[3] == 8) return '1';

    	if(bitMap[2] == 8) return '2';

    	if(bitMap[1] == 8) return '3';
    	if(bitMap[1] == 1) return '#';

    	if(bitMap[2] == 1) return '0';
    	return NO_KEY;
    }
};

char keys[KPD_ROWS][KPD_COLS] = {
  {'D','C','B','A'},
  {'#','9','6','3'},
  {'0','8','5','2'},
  {'*','7','4','1'}
};
byte rowPins[KPD_ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[KPD_COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad
Keypad_I2C2 kpd( makeKeymap(keys), rowPins, colPins, KPD_ROWS, KPD_COLS, KPD_I2CADDR, PCF8574 );

// LCD i2c
#include "libraries/NewliquidCrystal/LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Menu
#include "libraries/OMEEPROM/OMEEPROM.h"
#include "libraries/OMMenuMgr/OMMenuMgr.h"

//static unsigned long kTime;
//static char k;

class OMMenuMgr2 : public OMMenuMgr {
public:

    OMMenuMgr2(const OMMenuItem* c_first, uint8_t c_type, Keypad_I2C2* c_kpd) :OMMenuMgr( c_first, c_type) {
      kpd = c_kpd;
    };

    int OMMenuMgr2::_checkDigital() {
    	char k = kpd->getKey2();

    	if(k == 'A') return BUTTON_INCREASE;
    	if(k == 'B') return BUTTON_DECREASE;
    	if(k == 'D') return BUTTON_FORWARD;
    	if(k == '#') return BUTTON_BACK;
    	if(k == '*') return BUTTON_SELECT;
    	return BUTTON_NONE;
    }

private:
    Keypad_I2C2* kpd;
};

// Create a list of states and values for a select input
MENU_SELECT_ITEM  sel_auto= { 0, {"AUTO!"} };
MENU_SELECT_ITEM  sel_off = { 1, {"OFF!"} };
MENU_SELECT_ITEM  sel_on  = { 2, {"ON!"} };

MENU_SELECT_LIST  const state_list[] = { &sel_auto, &sel_off, &sel_on };
MENU_SELECT_LIST  const state_listOffOn[] = { &sel_off, &sel_on };

MENU_SELECT lightMode_select = { &lightMode,           MENU_SELECT_SIZE(state_list),   MENU_TARGET(&state_list) };
MENU_VALUE lightMode_value =   { TYPE_SELECT,     0,     0,     MENU_TARGET(&lightMode_select), LIGHTMODE_ADDR };
MENU_ITEM lightMode_item    =  { {"LIGHT CONTROL"}, ITEM_VALUE,  0,        MENU_TARGET(&lightMode_value) };
//                               TYPE             MAX    MIN    TARGET
MENU_VALUE lightOnHour_value = { TYPE_UINT,       23,    0,     MENU_TARGET(&lightOnHour), LIGHTONHOUR_ADDR };
MENU_VALUE lightOnMin_value =  { TYPE_UINT,       59,    0,     MENU_TARGET(&lightOnMin), LIGHTONMIN_ADDR };
MENU_VALUE lightOffHour_value ={ TYPE_UINT,       23,    0,     MENU_TARGET(&lightOffHour), LIGHTOFFHOUR_ADDR };
MENU_VALUE lightOffMin_value = { TYPE_UINT,       59,    0,     MENU_TARGET(&lightOffMin), LIGHTOFFMIN_ADDR };
//                                "123456789ABCDEF"
MENU_ITEM lightOnHour_item   = { {"LIGHT ON    [h]"},   ITEM_VALUE,  0,        MENU_TARGET(&lightOnHour_value) };
MENU_ITEM lightOnMin_item   =  { {"LIGHT ON    [m]"},    ITEM_VALUE,  0,        MENU_TARGET(&lightOnMin_value) };
MENU_ITEM lightOffHour_item  = { {"LIGHT OFF   [h]"},  ITEM_VALUE,  0,        MENU_TARGET(&lightOffHour_value) };
MENU_ITEM lightOffMin_item   = { {"LIGHT OFF   [m]"},   ITEM_VALUE,  0,        MENU_TARGET(&lightOffMin_value) };

MENU_SELECT heaterMode_select ={ &heaterMode,           MENU_SELECT_SIZE(state_list),   MENU_TARGET(&state_list) };
MENU_VALUE heaterMode_value =  { TYPE_SELECT,     0,     0,     MENU_TARGET(&heaterMode_select) };
MENU_ITEM heaterMode_item    = { {"HEATER CONTROL"}, ITEM_VALUE,  0,        MENU_TARGET(&heaterMode_value) };
MENU_VALUE heaterOnTemp_value ={ TYPE_FLOAT_10, 99,     0,    MENU_TARGET(&heaterOnTemp), HEATERONTEMP_ADDR  };
MENU_VALUE heaterOffTemp_value={ TYPE_FLOAT_10, 99,     0,    MENU_TARGET(&heaterOffTemp), HEATEROFFTEMP_ADDR };
//                                "123456789ABCDEF"
MENU_ITEM heaterOnTemp_item   ={ {"HEATER ON   [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&heaterOnTemp_value)};
MENU_ITEM heaterOffTemp_item  ={ {"HEATER OFF  [C]"},   ITEM_VALUE,  0,        MENU_TARGET(&heaterOffTemp_value) };

MENU_SELECT ventMode_select ={ &ventMode,           MENU_SELECT_SIZE(state_list),   MENU_TARGET(&state_list) };
MENU_VALUE ventMode_value =  { TYPE_SELECT,     0,     0,     MENU_TARGET(&ventMode_select) };
MENU_ITEM ventMode_item    = { {"VENT CONTROL"}, ITEM_VALUE,  0,        MENU_TARGET(&ventMode_value) };
MENU_VALUE ventOnTemp_value ={ TYPE_FLOAT_10, 99,   -99,    MENU_TARGET(&ventOnTemp) , VENTONTEMP_ADDR};
MENU_VALUE ventOffTemp_value={ TYPE_FLOAT_10, 99,   -99,    MENU_TARGET(&ventOffTemp) , VENTOFFTEMP_ADDR};
//                                "123456789ABCDEF"
MENU_ITEM ventOnTemp_item     ={ {"VENT ON     [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&ventOnTemp_value) };
MENU_ITEM ventOffTemp_item    ={ {"VENT OFF    [C]"},   ITEM_VALUE,  0,        MENU_TARGET(&ventOffTemp_value) };

MENU_SELECT cyclerMode_select = { &cyclerMode,           MENU_SELECT_SIZE(state_list),   MENU_TARGET(&state_list) };
MENU_VALUE cyclerMode_value =   { TYPE_SELECT,     0,     0,     MENU_TARGET(&cyclerMode_select) };
MENU_ITEM cyclerMode_item    = { {"CYCLER CONTROL"}, ITEM_VALUE,  0,        MENU_TARGET(&cyclerMode_value) };
MENU_VALUE cyclerOnMin_value = { TYPE_UINT,      0,     0,     MENU_TARGET(&cyclerOnMin) , CYCLERONMIN_ADDR};
MENU_VALUE cyclerOnSec_value = { TYPE_UINT,      0,     0,     MENU_TARGET(&cyclerOnSec), CYCLERONSEC_ADDR };
MENU_VALUE cyclerOffMin_value= { TYPE_UINT,      0,     0,     MENU_TARGET(&cyclerOffMin), CYCLEROFFMIN_ADDR };
MENU_VALUE cyclerOffSec_value= { TYPE_UINT,      0,     0,     MENU_TARGET(&cyclerOffSec), CYCLEROFFSEC_ADDR };
//                                "123456789ABCDEF"
MENU_ITEM cyclerOnMin_item   = { {"CYCLER ON   [m]"},   ITEM_VALUE,  0,        MENU_TARGET(&cyclerOnMin_value) };
MENU_ITEM cyclerOnSec_item   = { {"CYCLER ON   [s]"},    ITEM_VALUE,  0,        MENU_TARGET(&cyclerOnSec_value) };
MENU_ITEM cyclerOffMin_item  = { {"CYCLER OFF  [m]"},  ITEM_VALUE,  0,        MENU_TARGET(&cyclerOffMin_value) };
MENU_ITEM cyclerOffSec_item  = { {"CYCLER OFF  [s]"},   ITEM_VALUE,  0,        MENU_TARGET(&cyclerOffSec_value) };

MENU_LIST const submenu_list1[] = { &lightMode_item, &lightOnHour_item, &lightOnMin_item, &lightOffHour_item,  &lightOffMin_item};
MENU_ITEM menu_submenu1 = { {"LIGHT->"},  ITEM_MENU,  MENU_SIZE(submenu_list1),  MENU_TARGET(&submenu_list1) };

MENU_LIST const submenu_list2[] = { &heaterMode_item, &heaterOnTemp_item, &heaterOffTemp_item };
MENU_ITEM menu_submenu2 = { {"HEATER->"},  ITEM_MENU,  MENU_SIZE(submenu_list2),  MENU_TARGET(&submenu_list2) };

MENU_LIST const submenu_list3[] = { &ventMode_item, &ventOnTemp_item, &ventOffTemp_item };
MENU_ITEM menu_submenu3 = { {"VENT->"},  ITEM_MENU,  MENU_SIZE(submenu_list3),  MENU_TARGET(&submenu_list3) };

MENU_LIST const submenu_list4[] = { &cyclerMode_item, &cyclerOnMin_item, &cyclerOnSec_item, &cyclerOffMin_item,  &cyclerOffSec_item};
MENU_ITEM menu_submenu4 = { {"CYCLER->"},  ITEM_MENU,  MENU_SIZE(submenu_list4),  MENU_TARGET(&submenu_list4) };

// Alarms
MENU_VALUE tempHighTempAlarm_value={ TYPE_FLOAT_10, 99,    -99,    MENU_TARGET(&tempHighTempAlarm), TEMPHIGHTEMPALARM_ADDR };
MENU_ITEM tempHighTempAlarm_item   ={ {"TEMP HIGH A [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&tempHighTempAlarm_value) };

MENU_VALUE tempLowTempAlarm_value={ TYPE_FLOAT_10, 99,    -99,    MENU_TARGET(&tempLowTempAlarm), TEMPLOWTEMPALARM_ADDR };
MENU_ITEM tempLowTempAlarm_item   ={ {"TEMP LOW AL [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&tempLowTempAlarm_value) };

//                                "123456789ABCDEF"
MENU_VALUE lightValueAlarm_value={ TYPE_FLOAT_10, 102,    0,    MENU_TARGET(&lightValueAlarm), LIGHTVALUEALARM_ADDR };
MENU_ITEM lightValueAlarm_item   ={ {"LIGH ALARM  "},    ITEM_VALUE,  0,        MENU_TARGET(&lightValueAlarm_value) };

MENU_LIST const submenu_list5[] = { &tempHighTempAlarm_item, &tempLowTempAlarm_item, &lightValueAlarm_item};
MENU_ITEM menu_submenu5 = { {"ALARM SET->"},  ITEM_MENU,  MENU_SIZE(submenu_list5),  MENU_TARGET(&submenu_list5) };

MENU_VALUE gsmMode_value= { TYPE_BYTE, 3, 0,    MENU_TARGET(&gsmMode), GSMMODE_ADDR };
MENU_ITEM gsmMode_item   ={ {"GSM MODE->"},    ITEM_VALUE,  0,        MENU_TARGET(&gsmMode_value) };
MENU_VALUE gsmCode_value= { TYPE_UINT, 9999, 1000,    MENU_TARGET(&gsmCode), GSMCODE_ADDR };
MENU_ITEM gsmCode_item   ={ {"GSM CODE->"},    ITEM_VALUE,  0,        MENU_TARGET(&gsmCode_value) };
MENU_LIST const submenu_list6[] = { &gsmMode_item, &gsmCode_item};
MENU_ITEM menu_submenu6 = { {"GSM->"},  ITEM_MENU,  MENU_SIZE(submenu_list6),  MENU_TARGET(&submenu_list6) };

MENU_ITEM item_testme   = { {"CALL!"},  ITEM_ACTION, 0,        MENU_TARGET(&callAction) };
MENU_ITEM item_setClock   = { {"SET CLOCK->"},  ITEM_ACTION, 0,        MENU_TARGET(&uiSetClock) };
MENU_ITEM item_info   = { {"INFO->"},  ITEM_ACTION, 0,        MENU_TARGET(&uiInfo) };
MENU_ITEM item_alarmList   = { {"ALARM LIST->"},  ITEM_ACTION, 0,        MENU_TARGET(&uiAlarmList) };

//        List of items in menu level
MENU_LIST const root_list[]   = { &menu_submenu1 , &menu_submenu2, &menu_submenu3, &menu_submenu4, &menu_submenu5,  &item_alarmList, &item_testme, &item_setClock, &menu_submenu6, &item_info };//&item_bazme, &item_bakme,

// Root item is always created last, so we can add all other items to it
MENU_ITEM menu_root     = { {"Root"},        ITEM_MENU,   MENU_SIZE(root_list),    MENU_TARGET(&root_list) };

OMMenuMgr2 Menu(&menu_root, MENU_DIGITAL, &kpd);







int saveMessage(char msg[], char status) {
	if((gsmNumber[2] != '0') && status == MESSAGE_ALARM_ON) {
		gsmMgr.call();
	}

    //DateTime now = rtc.now();
    //sprintf(msg, " %d2/%d2 %d2:%d2 ", now.month(), now.day(), now.hour(), now.minute());
    //Serial.println(msg);

    //TODO change to sprintf?
    String m = String();
    if(now.day() < 10)
      m += '0';
    m += now.day();
    m += '/';
    if(now.month() < 10)
      m += '0';
    m += now.month();
    m += ' ';
    if(now.hour() < 10)
      m += '0';
    m += now.hour();
    m += ':';
    if(now.minute() < 10)
      m += '0';
    m += now.minute();
    m.toCharArray(msg, 12);
    msg[11]= ' ';
    msg[15] = status;
    //---------------

    int offset;
    OMEEPROM::read(MESSAGESOFFSET_ADDR, offset);
    if(offset >= MESSAGESCOUNT)
      offset = 0;
    for(int i=0; i < MESSAGELENGTH; i++) {
      OMEEPROM::write(MESSAGES_ADDR + offset * MESSAGELENGTH + i, msg[i]);
    }
    offset = (offset + 1) %  MESSAGESCOUNT;
    OMEEPROM::write(MESSAGESOFFSET_ADDR, offset);
    return offset;
}

void readMessage(int index, byte* msg) {
    int offset;
    OMEEPROM::read(MESSAGESOFFSET_ADDR, offset);
    offset--;
    if(offset >= MESSAGESCOUNT)
      offset = 0;
    offset = (offset + (MESSAGESCOUNT -index )) % MESSAGESCOUNT;
    for(int i=0; i < MESSAGELENGTH; i++) {
      OMEEPROM::read(MESSAGES_ADDR + offset * MESSAGELENGTH + i, *(msg+i));
    }
    *(msg+MESSAGELENGTH) = 0;
}

void loadEEPROM() {
    using namespace OMEEPROM;

    read(LIGHTMODE_ADDR, lightMode);
    read(LIGHTONHOUR_ADDR, lightOnHour);
    read(LIGHTONMIN_ADDR, lightOnMin);
    read(LIGHTOFFHOUR_ADDR, lightOffHour);
    read(LIGHTOFFMIN_ADDR, lightOffMin);

    read(HEATERMODE_ADDR, heaterMode);
    read(HEATERONTEMP_ADDR, heaterOnTemp);
    read(HEATEROFFTEMP_ADDR, heaterOffTemp);

    read(VENTMODE_ADDR, ventMode);
    read(VENTONTEMP_ADDR, ventOnTemp);
    read(VENTOFFTEMP_ADDR, ventOffTemp);

    read(CYCLERMODE_ADDR, cyclerMode);
    read(CYCLERONMIN_ADDR, cyclerOnMin);
    read(CYCLERONSEC_ADDR, cyclerOnSec);
    read(CYCLEROFFMIN_ADDR, cyclerOffMin);
    read(CYCLEROFFSEC_ADDR, cyclerOffSec);

    read(TEMPHIGHTEMPALARM_ADDR, tempHighTempAlarm);
    read(TEMPLOWTEMPALARM_ADDR, tempLowTempAlarm);
    read(LIGHTVALUEALARM_ADDR, lightValueAlarm);

    read(GSMMODE_ADDR, gsmMode);
    read(GSMCODE_ADDR, gsmCode);

    for(int i=0; i < 16; i++) {
         OMEEPROM::read(GSMNUMBER_ADDR + i, *(gsmNumber+i));
    }
}

void saveEEPROM() {
	// save defaults

    lightMode = 0;
    lightOnHour= 6;
    lightOnMin = 0;
    lightOffHour= 22;
    lightOffMin = 0;
    OMEEPROM::write(LIGHTMODE_ADDR, lightMode);
    OMEEPROM::write(LIGHTONHOUR_ADDR, lightOnHour);
    OMEEPROM::write(LIGHTONMIN_ADDR, lightOnMin);
    OMEEPROM::write(LIGHTOFFHOUR_ADDR, lightOffHour);
    OMEEPROM::write(LIGHTOFFMIN_ADDR, lightOffMin);

    heaterMode = 0;
    heaterOnTemp=20.0;
    heaterOffTemp=21.0;
    OMEEPROM::write(HEATERMODE_ADDR, heaterMode);
    OMEEPROM::write(HEATERONTEMP_ADDR, heaterOnTemp);
    OMEEPROM::write(HEATEROFFTEMP_ADDR, heaterOffTemp);

    ventMode = 0;
    ventOnTemp=22.0;
    ventOffTemp=23.0;
    OMEEPROM::write(VENTMODE_ADDR, ventMode);
    OMEEPROM::write(VENTONTEMP_ADDR, ventOnTemp);
    OMEEPROM::write(VENTOFFTEMP_ADDR, ventOffTemp);

    cyclerMode = 0;
    cyclerOnMin= 0;
    cyclerOnSec = 5;
    cyclerOffMin= 0;
    cyclerOffSec = 5;
    OMEEPROM::write(CYCLERMODE_ADDR, cyclerMode);
    OMEEPROM::write(CYCLERONMIN_ADDR, cyclerOnMin);
    OMEEPROM::write(CYCLERONSEC_ADDR, cyclerOnSec);
    OMEEPROM::write(CYCLEROFFMIN_ADDR, cyclerOffMin);
    OMEEPROM::write(CYCLEROFFSEC_ADDR, cyclerOffSec);

    tempHighTempAlarm = 21.0;
    tempLowTempAlarm = 19.0;
    lightValueAlarm = 70.0;
    OMEEPROM::write(TEMPHIGHTEMPALARM_ADDR, tempHighTempAlarm);
    OMEEPROM::write(TEMPLOWTEMPALARM_ADDR, tempLowTempAlarm);
    OMEEPROM::write(LIGHTVALUEALARM_ADDR, lightValueAlarm);

    //gsm??
    /*
    gsmMode = 0;
    gsmCode = 9999;
    OMEEPROM::write(GSMMODE_ADDR, gsmMode);
    OMEEPROM::write(GSMCODEE_ADDR, gsmCode);
    */
}

//////////////////////////////////
// setup
//////////////////////////////////
void setup() {
	pinMode(LEDPIN, OUTPUT);

	Serial.begin(9600);
	while(!Serial);

	Serial.print("GROWMAT EASY ");
	Serial.println(VERSION);
	Serial.println();

	if( OMEEPROM::saved() )
		loadEEPROM();

	//kpd.addEventListener(keypadEvent);
	Wire.begin( );
	sim800l.begin();
	kpd.begin( makeKeymap(keys) );
	lcd.begin(LCD_COLS, LCD_ROWS);
	dht.begin();
	rtc.begin();
	if (rtc.lostPower()) {
		Serial.println("RTC lost power, lets set the time!");
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}
	now = rtc.now();
	secondstime = now.secondstime();

	saveMessage(MESSAGE_ALARM_POWERON, MESSAGE_ALARM_ON);

	/*
	//TEST
	char msg[MESSAGELENGTH + 1];
	for(int i = 0; i< MESSAGESCOUNT; i++) {
		readMessage(i, (byte*)msg);
		Serial.println(msg);
	}
	*/

	uiMain();
	Menu.setDrawHandler(uiDraw);
	Menu.setExitHandler(uiMain);
	Menu.enable(true);
}


bool getInstrumentControl(bool a, byte mode) {
	if(mode == 0) return a;
	if(mode == 1) return false;
	if(mode == 2) return true;
	return false;
}

/*
void getInstrumentString(bool a, byte mode, char* b) {
	//TODO is it safe?
	//char b[3];
	*b = getInstrumentControl(a, mode) ? '0' : '1';
	*(b+1) = mode ? 'M' : 'A';
	//*(b+2) = 0;
	//return b;
}
*/
/*
void infoSerial(Stream* ser) {
	if(tempHighAlarm2.active) ser->println("TEMP HIGH!");
  	if(tempLowAlarm2.active) ser->println("TEMP LOW!");
  	if(lightHighAlarm2.active) ser->println("LIGHT HIGH!");
  	if(lightLowAlarm2.active) ser->println("LIGHT LOW!");
  	ser->println();

  	ser->print("LIGHT");
  	lightControl ? ser->print('1'):ser->print('0');
  	lightMode ? ser->print('M'):ser->print('A');
  	ser->print(" HEATER");
  	heaterControl ? ser->print('1'):ser->print('0');
  	heaterMode ? ser->print('M'):ser->print('A');
  	ser->print(" VENT");
  	ventControl ? ser->print('1'):ser->print('0');
  	ventMode ? ser->print('M'):ser->print('A');
  	ser->print(" CYCLER");
  	cyclerControl ? ser->print('1'):ser->print('0');
  	cyclerMode ? ser->print('M'):ser->print('A');
  	ser->println();

  	ser->print("TEMP=");
  	ser->print(temperature);
  	ser->print(" HUMI=");
  	ser->print(humidity);
  	ser->print(" LIGHT=");
  	ser->print(light);
  	ser->println();

  	ser->print("GSM MODE=");
  	ser->print(gsmMode);
  	ser->print(" NUMBER=");
  	ser->print(gsmNumber);
  	ser->print(" CODE=");
  	ser->print(gsmCode);
  	ser->println();

  	ser->println();

  	char msg[MESSAGELENGTH + 1];
  	for(int i = 0; i< MESSAGESCOUNT; i++) {
  		readMessage(i, (byte*)msg);
  		ser->println(msg);
  	}
 }
*/
//////////////////////////////////
// main loop
//////////////////////////////////
void loop() {

	now = rtc.now();

	// second counter
	if(now.secondstime() > secondstime) {

		cyclerDuration += now.secondstime() - secondstime ;

		secToggle ? secToggle = false : secToggle = true;
		if (!Menu.shown()) {
			if(!uiState)
				uiMain();
		}
		secondstime = now.secondstime();
		Serial.println(secondstime);


		// how often check sms, time expensive
		if(!(secondstime % 10) && gsmMode > 0) {
			//Serial.println("SMS CHECK");
			gsmMgr.proceedSMS();//&lightMode, &heaterMode, &ventMode, &cyclerMode);
		}
	}

	char key = kpd.getKey2();
	if(key == '#') {
		Menu.enable(true);
		uiState = 0;
		uiPage = 0;
	}
	else if(key == '0') {
		uiAlarmList();
	}
	else if(!Menu.enable()) {
		/*
		//Serial.println(key);
		//Serial.println(pressed);

		// if(kpd.getKey2()=='A')  {
		if((key == 'A') && !pressed){
			uiPage--;
			pressed = 'A';
		}

		//if(kpd.getKey2()=='B')  {
		if((key == 'B') && !pressed) {
			uiPage++;
			pressed = 'B';
		}

		if(key == NO_KEY)
			pressed = 0;
		*/
		uiScreen();
	}

	Menu.checkInput();

	//////////////////////////////////
	// gsm
	//////////////////////////////////
	gsmMgr.update();
	gsmMgr.updateCall();



	//TODO cycle time from RTC
	/*
	// main screen refresh
	if (!Menu.shown()) {
		if(uiTime + 1000 < millis()) {
			uiTime = millis();
			uiClear();
		}
	}
	if(millis() > (cTime + 1000)) {
		cTime = millis();
		cyclerDuration++;
		//Serial.println(cyclerDuration);
		//Serial.println(cyclerAuto);
		nowSetClock = rtc.now();
	}
	 */




	/*
	if(millis() > (gsmTime + 60000)) {
		gsmTime = millis();
		//TODO remmove parametres, use global?
		gsmMgr.proceedSMS();//&lightMode, &heaterMode, &ventMode, &cyclerMode);
	}
	*/


	//////////////////////////////////
	// inputs
	//////////////////////////////////
	float t0, h0;
	t0 = dht.readTemperature();
	h0 = dht.readHumidity();
	if(!isnan(t0)) temperature = t0;
	if(!isnan(h0)) humidity = h0;
  	for(int i = 0; i < 100; i++)
  		light += analogRead((unsigned char)LIGHTPIN) / 10;
  	light /= 100;


	//////////////////////////////////
	// outputs
	//////////////////////////////////
  	unsigned int l = now.hour() * 60 + now.minute();
  	unsigned int lOn = lightOnHour * 60 + lightOnMin;
  	unsigned int lOff =lightOffHour * 60 + lightOffMin;

    if(lOn < lOff) {
    	lightAuto = ((lOn < l) && (l < lOff)) ? true : false;
    }
    else {
    	lightAuto = ((lOn > l) && (l  > lOff)) ? false : true;
    }
    lightControl = getInstrumentControl(lightAuto, lightMode);

    if(temperature < heaterOnTemp)
    	heaterAuto = true;
    if(temperature > heaterOffTemp)
    	heaterAuto = false;
    heaterControl = getInstrumentControl(heaterAuto, heaterMode);

    if(temperature > ventOnTemp)
    	ventAuto = true;
    if(temperature < ventOffTemp)
    	ventAuto = false;
    ventControl = getInstrumentControl(ventAuto, ventMode);

	if(cyclerAuto && (cyclerDuration >= (cyclerOnSec + cyclerOnMin * 60))) {
		cyclerDuration = 0;
		cyclerAuto = false;
	};
	if(!cyclerAuto && (cyclerDuration >= (cyclerOffSec + cyclerOffMin * 60))) {
		cyclerDuration = 0;
		cyclerAuto = true;
	};
	cyclerControl = getInstrumentControl(cyclerAuto, cyclerMode);


	//////////////////////////////////
	// alarms
	//////////////////////////////////
	if(tempHighAlarm2.activate(temperature > tempHighTempAlarm))
		saveMessage(MESSAGE_ALARM_TEMPHIGH, MESSAGE_ALARM_ON);;
	if(tempHighAlarm2.deactivate(temperature < (tempHighTempAlarm - tempHysteresis)))
		saveMessage(MESSAGE_ALARM_TEMPHIGH, MESSAGE_ALARM_OFF);

	if(tempLowAlarm2.activate(temperature < tempLowTempAlarm))
		saveMessage(MESSAGE_ALARM_TEMPLOW, MESSAGE_ALARM_ON);;
	if(tempLowAlarm2.deactivate(temperature > (tempLowTempAlarm + tempHysteresis)))
		saveMessage(MESSAGE_ALARM_TEMPLOW, MESSAGE_ALARM_OFF);

	if(lightLowAlarm2.activate(lightControl && (light > lightValueAlarm)))
		saveMessage(MESSAGE_ALARM_LIGHTLOW, MESSAGE_ALARM_ON);
	if(lightLowAlarm2.deactivate(!lightControl || (light < lightValueAlarm - lightHysteresis)))
		saveMessage(MESSAGE_ALARM_LIGHTLOW, MESSAGE_ALARM_OFF);

	if(lightHighAlarm2.activate(!lightControl && (light < lightValueAlarm)))
		saveMessage(MESSAGE_ALARM_LIGHTHIGH, MESSAGE_ALARM_ON);
	if(lightHighAlarm2.deactivate(lightControl || (light > lightValueAlarm + lightHysteresis)))
		saveMessage(MESSAGE_ALARM_LIGHTHIGH, MESSAGE_ALARM_OFF);

	if(kpd.getKeys()) {
		tempHighAlarm2.ack();
		tempLowAlarm2.ack();
		lightHighAlarm2.ack();
		lightLowAlarm2.ack();
    }

	//TEST
	digitalWrite(LEDPIN, cyclerControl);

	//////////////////////////////////
	// communication
	//////////////////////////////////
  	if (Serial.available() > 0) {
  		// read the incoming byte:
  		//incomingByte = Serial.read();
  		if (Serial.readString().indexOf("?")!=-1 ) {
 			//Serial.println("TEST");
  			//infoSerial((Stream*)&Serial);


  			if(tempHighAlarm2.active) Serial.println("TEMP HIGH!");
  			if(tempLowAlarm2.active) Serial.println("TEMP LOW!");
  			if(lightHighAlarm2.active) Serial.println("LIGHT HIGH!");
  			if(lightLowAlarm2.active) Serial.println("LIGHT LOW!");
  			Serial.println();

  			Serial.print("L");
  			lightControl ? Serial.print('1'):Serial.print('0');
  			lightMode ? Serial.print('M'):Serial.print('A');
  			Serial.print(" H");
  			heaterControl ? Serial.print('1'):Serial.print('0');
  			heaterMode ? Serial.print('M'):Serial.print('A');
  			Serial.print(" V");
  			ventControl ? Serial.print('1'):Serial.print('0');
  			ventMode ? Serial.print('M'):Serial.print('A');
  			Serial.print(" C");
  			cyclerControl ? Serial.print('1'):Serial.print('0');
  			cyclerMode ? Serial.print('M'):Serial.print('A');
  			Serial.println();

  			Serial.print("TEMP=");
  			Serial.print(temperature);
  			Serial.print(" HUMI=");
  			Serial.print(humidity);
  			Serial.print(" LIGHT=");
  			Serial.print(light);
  			Serial.println();

  			Serial.print("GSM MODE=");
  			Serial.print(gsmMode);
  			Serial.print(" NUMBER=");
  			Serial.print(gsmNumber);
  			Serial.print(" CODE=");
  			Serial.print(gsmCode);
  			Serial.println();

  			Serial.println();

  			char msg[MESSAGELENGTH + 1];
  			for(int i = 0; i< MESSAGESCOUNT; i++) {
  				readMessage(i, (byte*)msg);
  				Serial.println(msg);
  			}


  			////////////////TEST
  			/*
  			char buffer[256];// = "TEST";

  			int p = 0;
  			//buffer[p++] = 'A';
  			//buffer[p++] = 'B';
  			//buffer[p++] = 0;

  			p += sprintf(buffer + p, "TIME=%02d:%02d\n", now.hour(), now.minute());

  			if(tempLowAlarm2.active)
  				p += sprintf(buffer + p, "TEMP LOW!\n");
  			if(tempHighAlarm2.active)
  				p += sprintf(buffer + p, "TEMP HIGH!\n");
  			if(lightLowAlarm2.active)
  				p += sprintf(buffer + p, "LIGHT LOW!\n");
  			if(lightHighAlarm2.active)
  				p += sprintf(buffer + p, "LIGHT HIGH!\n");

  			p += sprintf(buffer + p, "T=%02.01f H=%02.01f L=%02.01f\n ", temperature, humidity, light);
  			p += sprintf(buffer + p, "GSM MODE=%d NUMBER=%s CODE=%04d\n ", gsmMode, gsmNumber, gsmCode);

  			//p += sprintf(buffer + p, "L%s ", getInstrumentString(lightControl, lightMode));
  			//p += sprintf(buffer + p, "H%s ", getInstrumentString(heaterControl, heaterMode));
  			//p += sprintf(buffer + p, "V%s ", getInstrumentString(ventControl, ventMode));
  			//p += sprintf(buffer + p, "C%s\n", getInstrumentString(cyclerControl, cyclerMode));

  			p += sprintf(buffer + p, "L%c%c\n", lightControl ? '0' : '1', lightMode > 0 ? 'M' : 'A');
  			p += sprintf(buffer + p, "H%c%c\n", heaterControl ? '0' : '1', heaterMode > 0 ? 'M' : 'A');
  			p += sprintf(buffer + p, "V%c%c\n", ventControl ? '0' : '1', ventMode > 0 ? 'M' : 'A');
  			p += sprintf(buffer + p, "C%c%c\n", cyclerControl ? '0' : '1', cyclerMode > 0 ? 'M' : 'A');

  			Serial.println(p);
  			Serial.println(buffer);
  			*/

  		}
  	}
}

void callAction() {
	//TEST save default values
	saveEEPROM();

	//char* text="Testing Sms";  //text for the message.
	//char* number="+420000000000"; //change to a valid number.

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(" CALLING ...");
	lcd.setCursor(1, 1);
	lcd.print(gsmNumber);

	sim800l.callNumber(gsmNumber);
	/*
  	while(Sim800l.getCallStatus() > 0) {
    	lcd.setCursor(0, 1);
    	lcd.print(Sim800l.getCallStatus(), DEC)    ;
  	}
	*/
    //error=Sim800l.sendSms(number,text);
}

void uiDraw(char* p_text, int p_row, int p_col, int len) {
	lcd.backlight();
	lcd.setCursor(p_col, p_row);
	for( int i = 0; i < len; i++ ) {
		if( p_text[i] < '!' || p_text[i] > '~' )
			lcd.write(' ');
		else
			lcd.write(p_text[i]);
	}
}

void uiInstrument(bool instrument, byte mode) {
	lcd.print(instrument);
	mode ? lcd.print("M") : lcd.print("A");
}


// user interface menu functions
void uiAlarmList() {
	Menu.enable(false);
	uiState = UISTATE_ALARMLIST;
}

void uiSetClock() {
	Menu.enable(false);
	uiState = UISTATE_SETCLOCK;
	nowSetClock = rtc.now();
}

void uiScreen() {
	//Menu.enable(false);
	lcd.backlight();
	char key = kpd.getKey2();

	if(uiState == UISTATE_ALARMLIST) {
		if((key == 'A') && !pressed){
			uiPage--;
			pressed = 'A';
		}
		if((key == 'B') && !pressed) {
			uiPage++;
			pressed = 'B';
		}
		if(key == NO_KEY)
			pressed = 0;

		char msg[MESSAGELENGTH + 1];
		uiPage = min(uiPage, MESSAGESCOUNT -2);
		uiPage = max(uiPage, 0);
		lcd.setCursor(0, 0);
		readMessage(uiPage, (byte*)msg);
		lcd.print(msg);
		lcd.setCursor(0, 1);
		readMessage(uiPage + 1, (byte*)msg);
		lcd.print(msg);
	}

	if(uiState == UISTATE_SETCLOCK) {

		uint8_t h = nowSetClock.hour();
		uint8_t m = nowSetClock.minute();

		key = kpd.getKey2();

		if(key == '1') h--;
		if(key == '2') h++;
		if(key == '3') m--;
		if(key == 'A') m++;
		//TODO if < time return

		m = min(59, m);
		m = max(0, m);
		h = min(23, h);
		h = max(0, h);

		nowSetClock = DateTime(nowSetClock.year(), nowSetClock.month(), nowSetClock.day(), h, m, nowSetClock.second());

		//Serial.print("uiScreen ");
		//TODO sprintf
		//DateTime now = rtc.now();
		lcd.setCursor(0,0);
		lcd.print("SET CLOCK  ");
		if(nowSetClock.hour()<10)
			lcd.print('0');
		lcd.print(nowSetClock.hour());
		lcd.print(':');
		if(nowSetClock.minute()<10)
			lcd.print('0');
		lcd.print(nowSetClock.minute());
		lcd.setCursor(0,1);
		lcd.print("1- H 2+  3- M A+");

		if(key == 'D') {

			rtc.adjust(nowSetClock);
			secondstime = nowSetClock.secondstime();
			Menu.enable(true);
			//uiState=0;
		}
	}
}

void uiInfo() {
	//"123456789ABCDEF"
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("BCSEDLON@");
	lcd.setCursor(0, 1);
	lcd.print("GMAIL.COM");
	Menu.enable(false);
}



void uiMain() {

	//if(uiState)
	//	return;

	//TODO
	//secToggle ? secToggle = false : secToggle = true;

	if(tempHighAlarm2.unAck || tempLowAlarm2.unAck || lightHighAlarm2.unAck || lightLowAlarm2.unAck) {
		secToggle ? lcd.backlight() : lcd.noBacklight();
	}
	else {
		lcd.backlight();
	}

	lcd.setCursor(0, 0);
	if(tempHighAlarm2.active)
		secToggle ? lcd.print("+") : lcd.print(" ");
	else if(tempLowAlarm2.active)
		secToggle ? lcd.print("-") : lcd.print(" ");
	else
		lcd.print(" ");

	lcd.print("t");
	if(abs(temperature) < 10)
		lcd.print("0");
	lcd.print(temperature);

	lcd.setCursor(6, 0);
	lcd.print(" h");
	if(abs(temperature) < 10)
		lcd.print("0");
	lcd.print(humidity);

	//DateTime now = rtc.now();
	lcd.setCursor(10, 0);
	lcd.print(" ");
	if (now.hour() < 10)
		lcd.print("0");
	lcd.print(now.hour(), DEC);
	secToggle ? lcd.print(":") : lcd.print(" ");
	if (now.minute() < 10)
		lcd.print("0");
	lcd.print(now.minute(), DEC);



	lcd.setCursor(0, 1);
	if(lightHighAlarm2.active)
		secToggle ? lcd.print("+") : lcd.print(" ");
	else if(lightLowAlarm2.active)
		secToggle ? lcd.print("-") : lcd.print(" ");
	else if(light < lightValueAlarm)
		lcd.print("*");
	else
		lcd.print(" ");

	/*
	lcd.print(light2.getString());
	lcd.print(" ");
	lcd.print(heater2.getString());
	lcd.print(" ");
	lcd.print(vent2.getString());
	lcd.print(" ");
	lcd.print(cycler2.getString());
	*/
	/*
	lcd.print("L");
	lcd.print(getInstrumentString(lightControl, lightMode));
	lcd.print(" H");
	lcd.print(getInstrumentString(heaterControl, heaterMode));
	lcd.print(" V");
	lcd.print(getInstrumentString(lightControl, lightMode));
	lcd.print(" C");
	lcd.print(getInstrumentString(cyclerControl, cyclerMode));
	*/

	lcd.print("L");
	uiInstrument(lightControl, lightMode);
	lcd.print(" H");
	uiInstrument(heaterControl, heaterMode);
	lcd.print(" V");
	uiInstrument(ventControl, ventMode);
	lcd.print(" C");
	uiInstrument(cyclerControl, cyclerMode);

}

/*
// example
void uiQwkScreen() {
	lcd.clear();
	Menu.enable(false);

	lcd.print("Action!");
	lcd.setCursor(0, 1);
	lcd.print("Enter 2 return");

	while( Menu.checkInput() != BUTTON_SELECT ) {
		; // wait!
	}

	Menu.enable(true);
	lcd.clear();
}
*/
