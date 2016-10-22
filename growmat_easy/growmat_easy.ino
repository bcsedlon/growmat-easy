#define TEXT_GROWMATEASY "GROWMAT EASY"
#define VERSION 1

#define LCD_I2CADDR 0x20
const byte LCD_ROWS = 2;
const byte LCD_COLS = 16;

#define KPD_I2CADDR 0x20
const byte KPD_ROWS = 4;
const byte KPD_COLS = 4;

#define LEDPIN 13

#define LIGHTCONTROLPIN 9
#define HEATERCONTROLPIN 10
#define FANCONTROLPIN 11
#define CYCLERCONTROLPIN 12

#define EXTPIN 8

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

#define FANMODE_ADDR 36
#define FANONTEMP_ADDR 40
#define FANOFFTEMP_ADDR 44

#define CYCLERMODE_ADDR 48
#define CYCLERONMIN_ADDR 52
#define CYCLERONSEC_ADDR 56
#define CYCLEROFFMIN_ADDR 60
#define CYCLEROFFSEC_ADDR 64

#define TEMPHIGHTEMPALARM_ADDR 68
#define TEMPLOWTEMPALARM_ADDR 72
#define LIGHTHIGHVALUEALARM_ADDR 76
#define LIGHTLOWVALUEALARM_ADDR 80

#define EXTERNALMODEALARM_ADDR 84

#define GSMNUMBER_ADDR 100  //16 bytes
#define GSMCODE_ADDR 116
#define GSMMODE_ADDR 118

#define MESSAGESOFFSET_ADDR 120
#define MESSAGES_ADDR 124
#define MESSAGESCOUNT 8
#define MESSAGELENGTH 16

//                              "0123456789ABCDEF"
#define MESSAGE_ALARM_POWERON   "dd/mm hh:mm ON x"
#define MESSAGE_ALARM_TEMPHIGH  "dd/mm hh:mm T+ x"
#define MESSAGE_ALARM_TEMPLOW   "dd/mm hh:mm T- x"
#define MESSAGE_ALARM_LIGHTHIGH "dd/mm hh:mm L+ x"
#define MESSAGE_ALARM_LIGHTLOW  "dd/mm hh:mm L- x"
#define MESSAGE_ALARM_EXTERNAL  "dd/mm hh:mm EX x"
#define MESSAGE_ALARM_ON  '1'
#define MESSAGE_ALARM_OFF '0'

#define MESSAGE_TEMPHIGH "T+!\n"
#define MESSAGE_TEMPLOW "T-!\n"
#define MESSAGE_LIGHTHIGH "L+!\n"
#define MESSAGE_LIGHTLOW "L-!\n"
#define MESSAGE_EXTERNAL "EX!\n"

#define MESSAGE_TEMP "\nT="
#define MESSAGE_HUMI "\nH="
#define MESSAGE_LIGHT "\nL="
#define MESSAGE_EXT "\nE="
#define MESSAGE_GSM  "\nGSM="

#define GSMCHECKSMSINTERVAL  60000
#define GSMCALLDURATION  30000

#define UISTATE_ALARMLIST 1
#define UISTATE_SETCLOCK 2
#define UISTATE_MEAS 3

#define KPD_UP 'A'
#define KPD_DOWN 'B'
#define KPD_LEFT '#'
#define KPD_RIGHT 'D'
#define KPD_ENTER '*'
#define KPD_ESC '0'


//////////////////////////////////
// variables
//////////////////////////////////
DateTime nowSetClock;
DateTime now;
unsigned long milliseconds, secondsCounter;
//long secondstime;
bool secToggle = false;

char gsmNumber[16];
uint16_t gsmCode = 9999;
byte gsmMode = 0;

char uiKeyPressed = 0;
int uiState, uiPage;
unsigned long uiKeyTime;

//unsigned long gsmTime = 0;
//unsigned long uiTime = 0; // ui counter
//int uiState = 0;

//unsigned long cTime = 0;

// control
byte lightMode, heaterMode, fanMode, cyclerMode;
bool lightControl, heaterControl, fanControl, cyclerControl;
bool lightAuto, heaterAuto, fanAuto, cyclerAuto;
unsigned long cyclerDuration = 0; // cycler counter
//byte alarm, tempHighAlarm, tempLowAlarm, lightHighAlarm, lightLowAlarm;

// inputs
float temperature, humidity, heatIndex;
float light;

// parameters
unsigned int lightOnHour, lightOnMin, lightOffHour, lightOffMin;
float heaterOnTemp, heaterOffTemp;
float fanOnTemp, fanOffTemp;
unsigned int cyclerOnMin, cyclerOnSec, cyclerOffMin, cyclerOffSec;
float tempHighTempAlarm, tempLowTempAlarm, lightHighValueAlarm, lightLowValueAlarm;

float tempHysteresis = 0.5;
float lightHysteresis = 10;

byte externalModeAlarm;



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
} light2('L'), heater2('H'), fan2('V'), cycler2('C');
*/

class Alarm {

	//long timeActive;
	//long timeDeactive;
	unsigned long timeActive;
	unsigned long timeDeactive;

 public:


	// TODO: fix when millis overrun!!!
	//int delay = 5;
	int alarmActiveDelay = 5000;
	int alarmDeactiveDelay = 5000;
	bool active;
	bool unAck;

	bool activate(bool state) {
		if(state) {
			if(!active) {
				if(!timeActive)
					timeActive = milliseconds;
					//timeActive = secondstime;
				if((timeActive + alarmActiveDelay) < milliseconds) {
				//if((timeActive + delay) < secondstime) {
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
					timeDeactive = milliseconds;
					//timeDeactive = secondstime;
				if((timeDeactive + alarmDeactiveDelay) < milliseconds) {
				//if((timeDeactive + delay) < secondstime) {
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

Alarm tempHighAlarm2, tempLowAlarm2, lightHighAlarm2, lightLowAlarm2, externalAlarm2 ;


#include <Wire.h>

// SIM800L
#include "libraries/Sim800l/Sim800l.h"
#include <SoftwareSerial.h> //is necessary for the library!!
//Sim800l sim800l;  //to declare the library

class Sim800l2 : public Sim800l {

public:
	/*
	Sim800l2() : Sim800l() {
		//TODO TEST !!!
		na = true;
    };
    */

	bool Sim800l2::sendSmsBegin(char* number){

		printSerial("AT+CMGF=1\r"); //set sms to text mode
	    _buffer=_readSerial();
	    printSerial("AT+CMGS=\"");  // command to send sms
	    printSerial(number);
	    printSerial("\"\r");
	    _buffer=_readSerial();
	    return true;
	}
	/*
	template <typename TYPE> void sendSmsText(TYPE value)
	{
		//Serial.print(value);
		printSerial(value);
	}
	*/
	void Sim800l2::sendSmsText(char* s) {
		printSerial(s);
	}
	void Sim800l2::sendSmsText(char ch) {
		printSerial(ch);
	}
	void Sim800l2::sendSmsText(float f) {
		printSerial(f);
	}
	void Sim800l2::sendSmsText(int i) {
		printSerial(i);
	}

	bool Sim800l2::sendSmsEnd(){
		//printSerial("\r");
		delay(100);
		printSerial((char)26);
		_buffer=_readSerial();
		//expect CMGS:xxx   , where xxx is a number,for the sending sms.
		if (((_buffer.indexOf("CMGS") ) != -1 ) )
			return true;
		else
			return false;
	}
};

Sim800l sim800l;  //to declare the library


class GsmManager {
public:
	Sim800l2* sim;
	//uint8_t callStatus;
	//00420123456789

	char* gsmNumber;
	//String code;
	int* gsmCode;
	//String response = String("");

	//unsigned long callDuration;
	unsigned long millisecondsCall;

	GsmManager(Sim800l2* sim_c, int* gsmCode_c, char* gsmNumber_c) {
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
		if(!millisecondsCall && gsmMode > 1) {
			Serial.println(F("CALLING"));
			sim->callNumber(gsmNumber);
			//callDuration= millis();
			//secondsCall = secondstime;
			millisecondsCall= milliseconds;
		}

	}
	void updateCall() {
		//return;
		//callStatus = sim->getCallStatus();
		// TODO: solve millis overflow
		//if(callDuration && (callDuration + 30000 < millis())){
		//if(secondsCall && (secondsCall + 30 < secondstime)){
		if(millisecondsCall && (millisecondsCall + GSMCALLDURATION < milliseconds)){
			Serial.println(F("HANG UP"));
			sim->hangoffCall();
			//callDuration = 0;
			millisecondsCall = 0;
		}
	}

	bool sendSMS(char* text) {
		// TODO: check if sim is busy
		return sim->sendSms(gsmNumber, text);
	}


	bool proceedSMS() { //byte  *lightMode, byte *heaterMode,byte *fanMode,byte *cyclerMode) {

		//if(callDuration)
		//if(!gsmMode)
		//	return false;

		String text = sim->readSms(21);
		//TODO TEST
		//text = "#CODE9999 #00420724095917";

		if(text != "") {
			sim->delAllSms();

			if(gsmMode) {
				int pos;
				char ch;

				Serial.println(text);

				pos = text.indexOf("#P");
				if(text.substring(pos+2, pos+6) == String(*gsmCode, DEC)) {
					//Serial.println("CODE OK");

					pos = text.indexOf("#00");
					if(pos > -1) {
						// save new gsm number
						text.substring(pos+1).toCharArray(gsmNumber, 16, 0);
						for(int i=0; i < 16; i++) {
							OMEEPROM::write(GSMNUMBER_ADDR + i, gsmNumber[i]);
						}
					}

					pos = text.indexOf("#MODE");
					if(pos > -1) {
						// save new gsm number
						gsmMode = text.charAt(pos + 5) - 48;
						OMEEPROM::write(GSMMODE_ADDR, gsmMode);
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
					pos = text.indexOf("#F");
					if(pos > -1) {
						ch = text.charAt(pos+2);
						if(ch=='A') fanMode = 0;
						else if(ch=='0') fanMode = 1;
						else if(ch=='1') fanMode = 2;
					}
					pos = text.indexOf("#C");
					if(pos > -1) {
						ch = text.charAt(pos+2);
						if(ch=='A') cyclerMode = 0;
						else if(ch=='0') cyclerMode = 1;
						else if(ch=='1') cyclerMode = 2;
					}

					pos = text.indexOf("#?");
					if(pos > -1) {// && gsmMode > 1) {

						//TODO
						//char msg[256];// = "GROWMAT INFO";
						//bool r = sim->sendSms(gsmNumber, msg);
						Serial.println(F("SENDING SMS"));

						//sim->sendSms(gsmNumber, "TEST");

						sim->sendSmsBegin(gsmNumber);
						sim->sendSmsText("\n");
						//sim->sendSmsText("TEST");
						//sim->sendSmsEnd();


						if(tempHighAlarm2.active) sim->sendSmsText(MESSAGE_TEMPHIGH);
						if(tempLowAlarm2.active) sim->sendSmsText(MESSAGE_TEMPLOW);
						if(lightHighAlarm2.active) sim->sendSmsText(MESSAGE_LIGHTHIGH);
						if(lightLowAlarm2.active) sim->sendSmsText(MESSAGE_LIGHTLOW);
						if(externalAlarm2.active) sim->sendSmsText(MESSAGE_EXTERNAL);

						//sim->sendSmsTextT(10);
						//sim->sendSmsTextT(0.3);
						//sim->sendSmsTextT("XD");
						//sim->sendSmsTextT('!');
						lightControl = getInstrumentControl(lightAuto, lightMode);
						heaterControl = getInstrumentControl(heaterAuto, heaterMode);
						fanControl = getInstrumentControl(fanAuto, fanMode);
						cyclerControl = getInstrumentControl(cyclerAuto, cyclerMode);

						sim->sendSmsText("L");
						lightControl ? sim->sendSmsText('1'):sim->sendSmsText('0');
						lightMode ? sim->sendSmsText('M'):sim->sendSmsText('A');
						sim->sendSmsText(" H");
						heaterControl ? sim->sendSmsText('1'):sim->sendSmsText('0');
						heaterMode ? sim->sendSmsText('M'):sim->sendSmsText('A');
						sim->sendSmsText(" F");
						fanControl ? sim->sendSmsText('1'):sim->sendSmsText('0');
						fanMode ? sim->sendSmsText('M'):sim->sendSmsText('A');
						sim->sendSmsText(" C");
						cyclerControl ? sim->sendSmsText('1'):sim->sendSmsText('0');
						cyclerMode ? sim->sendSmsText('M'):sim->sendSmsText('A');
						sim->sendSmsText("\n");

						sim->sendSmsText(MESSAGE_TEMP);
						sim->sendSmsText(temperature);
						sim->sendSmsText(MESSAGE_HUMI);
						sim->sendSmsText(humidity);
						sim->sendSmsText(MESSAGE_LIGHT);
						sim->sendSmsText(light);
						sim->sendSmsText(MESSAGE_EXT);
						sim->sendSmsText(digitalRead(EXTPIN));


						sim->sendSmsText(MESSAGE_GSM);
						sim->sendSmsText(gsmMode);
						sim->sendSmsText("\n");
						//sim->sendSmsText("\nGSM MODE=");
						//sim->sendSmsText(gsmMode);
						//sim->sendSmsText("\nGSM NUMBER=");
						//sim->sendSmsText(gsmNumber);



						//sim->sendSmsText("\n\n");
						char msg[MESSAGELENGTH + 1];
						for(int i = 0; i< MESSAGESCOUNT; i++) {
						//for(int i = 0; i< 3; i++) {
							sim->sendSmsText("\n");
							readMessage(i, (byte*)msg);
							sim->sendSmsText(msg);

						 }


						sim->sendSmsEnd();
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
						*fanMode = 0;
					else if(ch=='0')
						*fanMode = 1;
					else if(ch=='1')
						*fanMode = 2;
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

    char Keypad_I2C2::getRawKey() {

    	getKeys();

    	if(bitMap[3] == 1) return '*';
    	if(bitMap[3] == 2) return '7';
    	if(bitMap[3] == 4) return '4';
    	if(bitMap[3] == 8) return '1';

    	if(bitMap[2] == 1) return '0';
    	if(bitMap[2] == 2) return '8';
    	if(bitMap[2] == 4) return '5';
    	if(bitMap[2] == 8) return '2';

    	if(bitMap[1] == 1) return '#';
    	if(bitMap[1] == 2) return '9';
    	if(bitMap[1] == 4) return '6';
    	if(bitMap[1] == 8) return '3';

    	if(bitMap[0] == 1) return 'D';
    	if(bitMap[0] == 2) return 'C';
    	if(bitMap[0] == 4) return 'B';
    	if(bitMap[0] == 8) return 'A';

    	return NO_KEY;

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

    		if(bitMap[2] == 2) fanMode=1;
    		if(bitMap[1] == 2) fanMode=2;
    		if(bitMap[0] == 2) fanMode=0;

    		if(bitMap[2] == 1) cyclerMode=1;
    		if(bitMap[1] == 1) cyclerMode=2;
    		if(bitMap[0] == 1) cyclerMode=0;

    		return NO_KEY;
    	}

    	//Serial.println(kTime);
    	// For menu system, makes delay between first and next
    	if(bitMap[0] || bitMap[1] || bitMap[2] || bitMap[3]) {
    		if(!kTime) {
    			kTime = millis();
    		}
    		if((kTime + 500) > millis()){
    			if((kTime + 200) < millis()) {
    				return NO_KEY;
    			}
    		}
    	}
        else
        	kTime = 0;

    	return getRawKey();
    	/*
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
    	*/
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
MENU_ITEM lightOnHour_item   = { {"LIGHT ON    [H]"},   ITEM_VALUE,  0,        MENU_TARGET(&lightOnHour_value) };
MENU_ITEM lightOnMin_item   =  { {"LIGHT ON    [M]"},    ITEM_VALUE,  0,        MENU_TARGET(&lightOnMin_value) };
MENU_ITEM lightOffHour_item  = { {"LIGHT OFF   [H]"},  ITEM_VALUE,  0,        MENU_TARGET(&lightOffHour_value) };
MENU_ITEM lightOffMin_item   = { {"LIGHT OFF   [M]"},   ITEM_VALUE,  0,        MENU_TARGET(&lightOffMin_value) };

MENU_SELECT heaterMode_select ={ &heaterMode,           MENU_SELECT_SIZE(state_list),   MENU_TARGET(&state_list) };
MENU_VALUE heaterMode_value =  { TYPE_SELECT,     0,     0,     MENU_TARGET(&heaterMode_select) };
MENU_ITEM heaterMode_item    = { {"HEATER CONTROL"}, ITEM_VALUE,  0,        MENU_TARGET(&heaterMode_value) };
MENU_VALUE heaterOnTemp_value ={ TYPE_FLOAT_10, 99,     0,    MENU_TARGET(&heaterOnTemp), HEATERONTEMP_ADDR  };
MENU_VALUE heaterOffTemp_value={ TYPE_FLOAT_10, 99,     0,    MENU_TARGET(&heaterOffTemp), HEATEROFFTEMP_ADDR };
//                                "123456789ABCDEF"
MENU_ITEM heaterOnTemp_item   ={ {"HEATER ON   [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&heaterOnTemp_value)};
MENU_ITEM heaterOffTemp_item  ={ {"HEATER OFF  [C]"},   ITEM_VALUE,  0,        MENU_TARGET(&heaterOffTemp_value) };

MENU_SELECT fanMode_select ={ &fanMode,           MENU_SELECT_SIZE(state_list),   MENU_TARGET(&state_list) };
MENU_VALUE fanMode_value =  { TYPE_SELECT,     0,     0,     MENU_TARGET(&fanMode_select) };
MENU_ITEM fanMode_item    = { {"FAN CONTROL"}, ITEM_VALUE,  0,        MENU_TARGET(&fanMode_value) };
MENU_VALUE fanOnTemp_value ={ TYPE_FLOAT_10, 99,   -99,    MENU_TARGET(&fanOnTemp) , FANONTEMP_ADDR};
MENU_VALUE fanOffTemp_value={ TYPE_FLOAT_10, 99,   -99,    MENU_TARGET(&fanOffTemp) , FANOFFTEMP_ADDR};
//                                "123456789ABCDEF"
MENU_ITEM fanOnTemp_item     ={ {"FAN ON     [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&fanOnTemp_value) };
MENU_ITEM fanOffTemp_item    ={ {"FAN OFF    [C]"},   ITEM_VALUE,  0,        MENU_TARGET(&fanOffTemp_value) };

MENU_SELECT cyclerMode_select = { &cyclerMode,           MENU_SELECT_SIZE(state_list),   MENU_TARGET(&state_list) };
MENU_VALUE cyclerMode_value =   { TYPE_SELECT,     0,     0,     MENU_TARGET(&cyclerMode_select) };
MENU_ITEM cyclerMode_item    = { {"CYCLER CONTROL"}, ITEM_VALUE,  0,        MENU_TARGET(&cyclerMode_value) };
MENU_VALUE cyclerOnMin_value = { TYPE_UINT,      0,     0,     MENU_TARGET(&cyclerOnMin) , CYCLERONMIN_ADDR};
MENU_VALUE cyclerOnSec_value = { TYPE_UINT,      0,     0,     MENU_TARGET(&cyclerOnSec), CYCLERONSEC_ADDR };
MENU_VALUE cyclerOffMin_value= { TYPE_UINT,      0,     0,     MENU_TARGET(&cyclerOffMin), CYCLEROFFMIN_ADDR };
MENU_VALUE cyclerOffSec_value= { TYPE_UINT,      0,     0,     MENU_TARGET(&cyclerOffSec), CYCLEROFFSEC_ADDR };
//                                "123456789ABCDEF"
MENU_ITEM cyclerOnMin_item   = { {"CYCLER ON   [M]"},   ITEM_VALUE,  0,        MENU_TARGET(&cyclerOnMin_value) };
MENU_ITEM cyclerOnSec_item   = { {"CYCLER ON   [S]"},    ITEM_VALUE,  0,        MENU_TARGET(&cyclerOnSec_value) };
MENU_ITEM cyclerOffMin_item  = { {"CYCLER OFF  [M]"},  ITEM_VALUE,  0,        MENU_TARGET(&cyclerOffMin_value) };
MENU_ITEM cyclerOffSec_item  = { {"CYCLER OFF  [S]"},   ITEM_VALUE,  0,        MENU_TARGET(&cyclerOffSec_value) };

MENU_LIST const submenu_list1[] = { &lightMode_item, &lightOnHour_item, &lightOnMin_item, &lightOffHour_item,  &lightOffMin_item};
MENU_ITEM menu_submenu1 = { {"LIGHT->"},  ITEM_MENU,  MENU_SIZE(submenu_list1),  MENU_TARGET(&submenu_list1) };

MENU_LIST const submenu_list2[] = { &heaterMode_item, &heaterOnTemp_item, &heaterOffTemp_item };
MENU_ITEM menu_submenu2 = { {"HEATER->"},  ITEM_MENU,  MENU_SIZE(submenu_list2),  MENU_TARGET(&submenu_list2) };

MENU_LIST const submenu_list3[] = { &fanMode_item, &fanOnTemp_item, &fanOffTemp_item };
MENU_ITEM menu_submenu3 = { {"FAN->"},  ITEM_MENU,  MENU_SIZE(submenu_list3),  MENU_TARGET(&submenu_list3) };

MENU_LIST const submenu_list4[] = { &cyclerMode_item, &cyclerOnMin_item, &cyclerOnSec_item, &cyclerOffMin_item,  &cyclerOffSec_item};
MENU_ITEM menu_submenu4 = { {"CYCLER->"},  ITEM_MENU,  MENU_SIZE(submenu_list4),  MENU_TARGET(&submenu_list4) };

// Alarms
MENU_VALUE tempHighTempAlarm_value={ TYPE_FLOAT_10, 99,    -99,    MENU_TARGET(&tempHighTempAlarm), TEMPHIGHTEMPALARM_ADDR };
MENU_ITEM tempHighTempAlarm_item   ={ {"TEMP HIGH   [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&tempHighTempAlarm_value) };

MENU_VALUE tempLowTempAlarm_value={ TYPE_FLOAT_10, 99,    -99,    MENU_TARGET(&tempLowTempAlarm), TEMPLOWTEMPALARM_ADDR };
MENU_ITEM tempLowTempAlarm_item   ={ {"TEMP LOW    [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&tempLowTempAlarm_value) };

//TODO !!!

//                                "123456789ABCDEF"
MENU_VALUE lightHighValueAlarm_value={ TYPE_FLOAT_10, 102,    0,    MENU_TARGET(&lightHighValueAlarm), LIGHTHIGHVALUEALARM_ADDR };
MENU_ITEM lightHighValueAlarm_item   ={ {"LIGH HIGH"},    ITEM_VALUE,  0,        MENU_TARGET(&lightHighValueAlarm_value) };
MENU_VALUE lightLowValueAlarm_value={ TYPE_FLOAT_10, 102,    0,    MENU_TARGET(&lightLowValueAlarm), LIGHTLOWVALUEALARM_ADDR };
MENU_ITEM lightLowValueAlarm_item   ={ {"LIGH LOW"},    ITEM_VALUE,  0,        MENU_TARGET(&lightLowValueAlarm_value) };

MENU_VALUE externalModeAlarm_value={ TYPE_BYTE, 1,    -1,    MENU_TARGET(&externalModeAlarm), EXTERNALMODEALARM_ADDR };
MENU_ITEM externalModeAlarm_item   ={ {"EXTERNAL"},    ITEM_VALUE,  0,        MENU_TARGET(&externalModeAlarm_value) };

MENU_LIST const submenu_list5[] = { &tempHighTempAlarm_item, &tempLowTempAlarm_item, &lightHighValueAlarm_item, &lightLowValueAlarm_item, &externalModeAlarm_item};
MENU_ITEM menu_submenu5 = { {"ALARM SET->"},  ITEM_MENU,  MENU_SIZE(submenu_list5),  MENU_TARGET(&submenu_list5) };

MENU_VALUE gsmMode_value= { TYPE_BYTE, 3, 0,    MENU_TARGET(&gsmMode), GSMMODE_ADDR };
MENU_ITEM gsmMode_item   ={ {"GSM MODE->"},    ITEM_VALUE,  0,        MENU_TARGET(&gsmMode_value) };
MENU_VALUE gsmCode_value= { TYPE_UINT, 9999, 1000,    MENU_TARGET(&gsmCode), GSMCODE_ADDR };
MENU_ITEM gsmCode_item   ={ {"GSM CODE->"},    ITEM_VALUE,  0,        MENU_TARGET(&gsmCode_value) };
MENU_LIST const submenu_list6[] = { &gsmMode_item, &gsmCode_item};
MENU_ITEM menu_submenu6 = { {"GSM->"},  ITEM_MENU,  MENU_SIZE(submenu_list6),  MENU_TARGET(&submenu_list6) };


MENU_ITEM item_setClock   = { {"SET CLOCK->"},  ITEM_ACTION, 0,        MENU_TARGET(&uiSetClock) };
MENU_ITEM item_alarmList   = { {"ALARM LIST->"},  ITEM_ACTION, 0,        MENU_TARGET(&uiAlarmList) };


//MENU_ITEM item_testme   = { {"RESET!"},  ITEM_ACTION, 0,        MENU_TARGET(&uiResetAction) };
//MENU_ITEM item_info   = { {"INFO->"},  ITEM_ACTION, 0,        MENU_TARGET(&uiInfo) };


//        List of items in menu level
MENU_LIST const root_list[]   = { &menu_submenu1 , &menu_submenu2, &menu_submenu3, &menu_submenu4, &menu_submenu5, &item_alarmList,&item_setClock, &menu_submenu6 };// &item_testme, , &item_info//&item_bazme, &item_bakme,

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
/*
    String m("");
    m.reserve(16);
    //String m = String();//"0123456789ABCDEF");
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
*/
	char* p = msg;
	if(now.day() < 10)
	   itoa(0, p++, 10);
	itoa(now.day(), p, 10);
	msg[2]='/';

		p = msg + 3;
	if(now.month() < 10)
	   itoa(0, p++, 10);
	itoa(now.month(), p, 10);
	msg[5]=' ';
	p = msg + 6;
	if(now.hour() < 10)
	   itoa(0, p++, 10);
	itoa(now.hour(), p, 10);
	msg[8]=':';
	p = msg + 9;
	if(now.minute() < 10)
	   itoa(0, p++, 10);
	itoa(now.minute(), p, 10);

//	msg[11]=' ';
/*
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
	*/
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

    read(FANMODE_ADDR, fanMode);
    read(FANONTEMP_ADDR, fanOnTemp);
    read(FANOFFTEMP_ADDR, fanOffTemp);

    read(CYCLERMODE_ADDR, cyclerMode);
    read(CYCLERONMIN_ADDR, cyclerOnMin);
    read(CYCLERONSEC_ADDR, cyclerOnSec);
    read(CYCLEROFFMIN_ADDR, cyclerOffMin);
    read(CYCLEROFFSEC_ADDR, cyclerOffSec);

    read(TEMPHIGHTEMPALARM_ADDR, tempHighTempAlarm);
    read(TEMPLOWTEMPALARM_ADDR, tempLowTempAlarm);
    read(LIGHTHIGHVALUEALARM_ADDR, lightHighValueAlarm);
    read(LIGHTLOWVALUEALARM_ADDR, lightLowValueAlarm);

    read(GSMMODE_ADDR, gsmMode);
    read(GSMCODE_ADDR, gsmCode);

    read(EXTERNALMODEALARM_ADDR, externalModeAlarm);

    for(int i=0; i < 16; i++) {
         OMEEPROM::read(GSMNUMBER_ADDR + i, *(gsmNumber+i));
    }
}

void saveDefaultEEPROM() {
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
    heaterOnTemp=0.0;
    heaterOffTemp=21.0;
    OMEEPROM::write(HEATERMODE_ADDR, heaterMode);
    OMEEPROM::write(HEATERONTEMP_ADDR, heaterOnTemp);
    OMEEPROM::write(HEATEROFFTEMP_ADDR, heaterOffTemp);

    fanMode = 0;
    fanOnTemp=23.0;
    fanOffTemp=23.0;
    OMEEPROM::write(FANMODE_ADDR, fanMode);
    OMEEPROM::write(FANONTEMP_ADDR, fanOnTemp);
    OMEEPROM::write(FANOFFTEMP_ADDR, fanOffTemp);

    cyclerMode = 0;
    cyclerOnMin= 0;
    cyclerOnSec = 30;
    cyclerOffMin= 0;
    cyclerOffSec = 30;
    OMEEPROM::write(CYCLERMODE_ADDR, cyclerMode);
    OMEEPROM::write(CYCLERONMIN_ADDR, cyclerOnMin);
    OMEEPROM::write(CYCLERONSEC_ADDR, cyclerOnSec);
    OMEEPROM::write(CYCLEROFFMIN_ADDR, cyclerOffMin);
    OMEEPROM::write(CYCLEROFFSEC_ADDR, cyclerOffSec);

    tempHighTempAlarm = 21.0;
    tempLowTempAlarm = 19.0;
    lightHighValueAlarm = 40.0;
    lightLowValueAlarm = 60.0;
    OMEEPROM::write(TEMPHIGHTEMPALARM_ADDR, tempHighTempAlarm);
    OMEEPROM::write(TEMPLOWTEMPALARM_ADDR, tempLowTempAlarm);
    OMEEPROM::write(LIGHTHIGHVALUEALARM_ADDR, lightHighValueAlarm);
    OMEEPROM::write(LIGHTLOWVALUEALARM_ADDR, lightLowValueAlarm);

    int iZero = 0;
    OMEEPROM::write(MESSAGESOFFSET_ADDR, iZero);

    char chZero = '0';
    for(int i=0 ; i < 15; i++)
    	OMEEPROM::write(GSMNUMBER_ADDR + i, chZero);
    char bZero = 0;
    OMEEPROM::write(GSMNUMBER_ADDR + 15, bZero);
    //gsm??

    gsmMode = 0;
    gsmCode = 9999;
    OMEEPROM::write(GSMMODE_ADDR, gsmMode);
    OMEEPROM::write(GSMCODE_ADDR, gsmCode);

    externalModeAlarm = 0;
    OMEEPROM::write(EXTERNALMODEALARM_ADDR, externalModeAlarm);

}

//////////////////////////////////
// setup
//////////////////////////////////
void setup() {



	pinMode(EXTPIN, INPUT);
	digitalWrite(EXTPIN, HIGH);
	externalAlarm2.alarmActiveDelay = 0;

	pinMode(LEDPIN, OUTPUT);

	pinMode(LIGHTCONTROLPIN, OUTPUT);
	pinMode(HEATERCONTROLPIN, OUTPUT);
	pinMode(FANCONTROLPIN, OUTPUT);
	pinMode(CYCLERCONTROLPIN, OUTPUT);

	Serial.begin(9600);
	while(!Serial);

	Serial.println();
	Serial.print(TEXT_GROWMATEASY);
	Serial.print(' ');
	Serial.println(VERSION);
	//Serial.println(F("##############"));

	if( OMEEPROM::saved() )
		loadEEPROM();
	else
		saveDefaultEEPROM();

	//kpd.addEfanListener(keypadEfan);
	Wire.begin( );
	sim800l.begin();
	kpd.begin( makeKeymap(keys) );
	lcd.begin(LCD_COLS, LCD_ROWS);
	dht.begin();
	rtc.begin();
	if (rtc.lostPower()) {
		Serial.println(F("RTC LOST POWER!"));
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// This line sets the RTC with an explicit date & time, for example to set
		// January 21, 2014 at 3am you would call:
		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}
	now = rtc.now();
	//secondstime = now.secondstime();
	milliseconds = millis();

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
  	ser->print(" FAN");
  	fanControl ? ser->print('1'):ser->print('0');
  	fanMode ? ser->print('M'):ser->print('A');
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
unsigned long millisecondsGsm;
void loop() {
	//gsmMode = 0;

	now = rtc.now();
	milliseconds = millis();
	// second counter
	//if(now.secondstime() > secondstime) {
	if(secondsCounter + 1000 < milliseconds || milliseconds < secondsCounter) {

		//cyclerDuration += now.secondstime() - secondstime ;
		//TOD add real time diference
		cyclerDuration++;

		secToggle ? secToggle = false : secToggle = true;
		//if (!Menu.shown()) {
		//	if(!uiState)
		//		uiMain();
		//}

		//secondstime = now.secondstime();
		secondsCounter = millis();
		//Serial.println(secondstime);
		//Serial.println(milliseconds);
		Serial.print('.');

		// how often check sms, time expensive
		//if(!(secondstime % 60) && gsmMode > 0) {
		if((millisecondsGsm + GSMCHECKSMSINTERVAL < milliseconds) && gsmMode > 0) {
			if(!gsmMgr.millisecondsCall) {
				//Serial.println("SMS CHECK");
				millisecondsGsm = milliseconds;
				gsmMgr.proceedSMS();//&lightMode, &heaterMode, &fanMode, &cyclerMode);
			}
		}
	}

	if (!Menu.shown()) {
		if(!uiState)
			uiMain();
	}

	char key = kpd.getKey2();
	if(key == '#') {
		//uiMain();
		uiState = 0;
		uiPage = 0;

		Menu.enable(true);
	}
	else if(key == '7') {
		uiAlarmList();
	}
	else if(key == '8') {
		uiMeas();
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
		gsmMgr.proceedSMS();//&lightMode, &heaterMode, &fanMode, &cyclerMode);
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
  	light = 100 - light;
  	light = max(0, light);
	light = min(99, light);


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

    if(temperature > fanOnTemp)
    	fanAuto = true;
    if(temperature < fanOffTemp)
    	fanAuto = false;
    fanControl = getInstrumentControl(fanAuto, fanMode);

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

	if(lightLowAlarm2.activate(lightControl && (light < lightLowValueAlarm)))
		saveMessage(MESSAGE_ALARM_LIGHTLOW, MESSAGE_ALARM_ON);
	if(lightLowAlarm2.deactivate(!lightControl || (light > lightLowValueAlarm + lightHysteresis)))
		saveMessage(MESSAGE_ALARM_LIGHTLOW, MESSAGE_ALARM_OFF);

	if(lightHighAlarm2.activate(!lightControl && (light > lightHighValueAlarm)))
		saveMessage(MESSAGE_ALARM_LIGHTHIGH, MESSAGE_ALARM_ON);
	if(lightHighAlarm2.deactivate(lightControl || (light < lightHighValueAlarm - lightHysteresis)))
		saveMessage(MESSAGE_ALARM_LIGHTHIGH, MESSAGE_ALARM_OFF);

	byte externalAlarm = digitalRead(EXTPIN);
	if(externalModeAlarm == 0) {
		// alarm in 0
		externalAlarm = !externalAlarm;
	}
	else if (externalModeAlarm == 1) {
		// alarm in 1
		//externalAlarm = externalAlarm;
	}
	else {
		// no alarm
		externalAlarm = 0;
	}

	if(externalAlarm2.activate(externalAlarm))
		saveMessage(MESSAGE_ALARM_EXTERNAL, MESSAGE_ALARM_ON);
	if(externalAlarm2.deactivate(!externalAlarm))
		saveMessage(MESSAGE_ALARM_EXTERNAL, MESSAGE_ALARM_OFF);

	if(kpd.getKeys()) {
		tempHighAlarm2.ack();
		tempLowAlarm2.ack();
		lightHighAlarm2.ack();
		lightLowAlarm2.ack();
		externalAlarm2.ack();
    }

	//TEST
	//digitalWrite(LEDPIN, cyclerControl);

	digitalWrite(LIGHTCONTROLPIN, !lightControl);
	digitalWrite(HEATERCONTROLPIN, !heaterControl);
	digitalWrite(FANCONTROLPIN, !fanControl);
	digitalWrite(CYCLERCONTROLPIN, !cyclerControl);
	//////////////////////////////////
	// communication
	//////////////////////////////////
  	if (Serial.available() > 0) {
  		// read the incoming byte:
  		//incomingByte = Serial.read();
  		if (Serial.readString().indexOf("?")!=-1 ) {

  			Serial.println(F("OK"));

  			Serial.print(MESSAGE_TEMP);
  			Serial.print(temperature);
  			Serial.print(MESSAGE_HUMI);
  			Serial.print(humidity);
  			Serial.print(MESSAGE_LIGHT);
  			Serial.print(light);
  			Serial.print(MESSAGE_EXT);
  			Serial.print(digitalRead(EXTPIN));
  			Serial.println();

  			//infoSerial((Stream*)&Serial);

  			/*
  			//TODO NO MEMEORY !!!
  			if(tempHighAlarm2.active) Serial.print(MESSAGE_TEMPHIGH);
  			if(tempLowAlarm2.active) Serial.print(MESSAGE_TEMPLOW);
  			if(lightHighAlarm2.active) Serial.print(MESSAGE_LIGHTHIGH);
  			if(lightLowAlarm2.active) Serial.print(MESSAGE_LIGHTLOW);
  			Serial.println();

  			Serial.print("L");
  			lightControl ? Serial.print('1'):Serial.print('0');
  			lightMode ? Serial.print('M'):Serial.print('A');
  			Serial.print(" H");
  			heaterControl ? Serial.print('1'):Serial.print('0');
  			heaterMode ? Serial.print('M'):Serial.print('A');
  			Serial.print(" F");
  			fanControl ? Serial.print('1'):Serial.print('0');
  			fanMode ? Serial.print('M'):Serial.print('A');
  			Serial.print(" C");
  			cyclerControl ? Serial.print('1'):Serial.print('0');
  			cyclerMode ? Serial.print('M'):Serial.print('A');
  			Serial.println();


  			*/

  			Serial.print(MESSAGE_GSM);
  			Serial.print(gsmMode);
  			Serial.print(' ');
  			Serial.print(gsmNumber);
  			Serial.print(' ');
  			Serial.print(gsmCode);
  			Serial.println();

  			/*
  			//TODO NO MEMEORY !!!
  			Serial.println();

  			char msg[MESSAGELENGTH + 1];
  			for(int i = 0; i< MESSAGESCOUNT; i++) {
  				readMessage(i, (byte*)msg);
  				Serial.println(msg);
  			}

  			*/
   		}
  	}
}

void uiResetAction() {

	//TEST save default values
	saveDefaultEEPROM();

	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("RESET OK"));
	/*
	lcd.setCursor(1, 1);
	lcd.print(gsmNumber);

	sim800l.callNumber(gsmNumber);
	*/
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
	mode ? lcd.print('M') : lcd.print('A');
}


// User interface menu functions
void uiAlarmList() {
	Menu.enable(false);
	uiState = UISTATE_ALARMLIST;
	uiPage=0;
	uiKeyTime =0;
	uiKeyPressed = 0;
	lcd.clear();
}

void uiMeas() {
	Menu.enable(false);
	uiState = UISTATE_MEAS;
	uiPage=0;
	uiKeyTime =0;
	uiKeyPressed = 0;
	lcd.clear();
}

void uiSetClock() {
	Menu.enable(false);
	uiState = UISTATE_SETCLOCK;
	nowSetClock = rtc.now();
	uiPage=0;
	uiKeyTime = 0;
	uiKeyPressed = 0;
	lcd.clear();
}


void uiScreen() {
	//Menu.enable(false);
	lcd.backlight();

	char key = kpd.getRawKey();
	// First key stroke after delay, then pause and then continuously
	if(key) {
		 if(!uiKeyTime) {
			 uiKeyTime = milliseconds;
		 }

		 if((uiKeyTime + 120) > milliseconds) {
			 key = 0;
		 }
		 else {
			 if(uiKeyPressed) {
				 key = 0;
			 }
			 else {
				 uiKeyPressed = key;
			 }
		 }

		 if((uiKeyTime + 600) < milliseconds){
			 uiKeyPressed = 0;
		 }
	}
	else {

		uiKeyTime = 0;
		uiKeyPressed = 0;
	}


	/*
	if((k == KPD_LEFT) && !pressed){
		uiPage--;
		pressed = k;
	}
	if((k == KPD_RIGHT) && !pressed) {
		uiPage++;
		pressed = k;
	}
	if((k == '*') && !pressed) {
			pressed = k;
	}
	if(k == NO_KEY)
		pressed = 0;
	 */
	if(uiState == UISTATE_MEAS) {
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print(light);

		lcd.setCursor(0, 1);
		lcd.print(digitalRead(EXTPIN));
		/*
		if(abs(temperature) < 10)
			lcd.print('0');
		lcd.print(temperature);

		lcd.setCursor(5, 0);
		lcd.print(' ');
		if(abs(temperature) < 10)
			lcd.print('0');
		lcd.print(humidity);

		lcd.setCursor(7, 0);
		lcd.print(' ');
		if(abs(light) < 10)
			lcd.print('0');
		lcd.print(light);
		lcd.print(' ');
		 */

	}

	if(uiState == UISTATE_ALARMLIST) {

		if(key == KPD_UP)
			uiPage--;
		if(key == KPD_DOWN)
			uiPage++;

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

		//if(key == KPD_LEFT)
		//	uiPage--;
		if(key == KPD_RIGHT)
			uiPage++;

		lcd.setCursor(0,0);
				  //"0123456789ABCDEF"
		lcd.print(F("SET CLOCK"));

		uiPage = max(0, uiPage);
		uiPage = min(4, uiPage);


		//k = kpd.getk2();




		uint8_t hh = nowSetClock.hour();
		uint8_t mm = nowSetClock.minute();
		uint8_t d = nowSetClock.day();
		uint8_t m = nowSetClock.month();
		uint16_t y = nowSetClock.year();

		lcd.setCursor(11,0);
		if(uiPage==0) {
			if(key==KPD_UP)hh++;
			if(key==KPD_DOWN)hh--;
					  //"0123456789ABCDEF"
			lcd.print(F(" HOUR"));
		}

		if(uiPage==1) {
			if(key==KPD_UP)mm++;
			if(key==KPD_DOWN)mm--;
			lcd.print(F("  MIN"));
		}

		if(uiPage==2) {
			if(key==KPD_UP)d++;
			if(key==KPD_DOWN)d--;
			lcd.print(F("  DAY"));
		}

		if(uiPage==3) {
			if(key==KPD_UP)m++;
			if(key==KPD_DOWN)m--;
			lcd.print(F("MONTH"));
		}

		if(uiPage==4) {
			if(key==KPD_UP)y++;
			if(key==KPD_DOWN)y--;
			lcd.print(F(" YEAR"));
		}

		hh = min(23, hh);
		hh = max(0, hh);
		mm = min(59, mm);
		mm = max(0, mm);
		d = min(31, d);
		d = max(1, d);
		m = min(12, m);
		m = max(1, m);
		y = min(9999, y);
		y = max(2000, y);

		nowSetClock = DateTime(y, m, d, hh, mm, 0);

		lcd.setCursor(0,1);
		if(hh<10)
			lcd.print('0');
		lcd.print(hh);
		lcd.print(':');
		if(mm<10)
			lcd.print('0');
		lcd.print(mm);
		lcd.print(' ');

		if(d<10)
			lcd.print('0');
		lcd.print(d);
		lcd.print('/');
		if(m<10)
			lcd.print('0');
		lcd.print(m);
		lcd.print('/');
		//if(y<10)
		//	lcd.print('0');
		lcd.print(y);


		//!!!TODO
		if(key == KPD_ENTER) {

			rtc.adjust(nowSetClock);
			//secondstime = nowSetClock.secondstime();
			Menu.enable(true);
			//uiState=0;
		}
		/*
		if(key==KPD_ESC) {
			Menu.enable(true);
			uiState=0;
		}
		*/
	}
}

void uiInfo() {
	//"123456789ABCDEF"
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("BCSEDLON@"));
	lcd.setCursor(0, 1);
	lcd.print(F("GMAIL.COM"));
	Menu.enable(false);
}



void uiMain() {

	if(tempHighAlarm2.unAck || tempLowAlarm2.unAck || lightHighAlarm2.unAck || lightLowAlarm2.unAck || externalAlarm2.unAck) {
		secToggle ? lcd.backlight() : lcd.noBacklight();
	}
	else {
		lcd.backlight();
	}

	lcd.setCursor(0, 0);
	if(tempHighAlarm2.active)
		secToggle ? lcd.print('+') : lcd.print(' ');
	else if(tempLowAlarm2.active)
		secToggle ? lcd.print('-') : lcd.print(' ');
	else
		lcd.print(' ');

	lcd.print('t');
	if(abs(temperature) < 10)
		lcd.print('0');
	lcd.print(temperature);

	lcd.setCursor(6, 0);
	lcd.print(" h");
	if(abs(temperature) < 10)
		lcd.print('0');
	lcd.print(humidity);

	//DateTime now = rtc.now();
	lcd.setCursor(10, 0);
	lcd.print(' ');
	if (now.hour() < 10)
		lcd.print('0');
	lcd.print(now.hour(), DEC);
	if(secToggle) {
		externalAlarm2.active ? lcd.print('!') : lcd.print(':');
	}
	else
		lcd.print(' ');
	if (now.minute() < 10)
		lcd.print('0');
	lcd.print(now.minute(), DEC);


	lcd.setCursor(0, 1);
	if(lightHighAlarm2.active)
		secToggle ? lcd.print('+') : lcd.print(' ');
	else if(lightLowAlarm2.active)
		secToggle ? lcd.print('-') : lcd.print(' ');
	else if(light > lightLowValueAlarm)
		lcd.print('*');
	else
		lcd.print(' ');

	/*
	lcd.print(light2.getString());
	lcd.print(" ");
	lcd.print(heater2.getString());
	lcd.print(" ");
	lcd.print(fan2.getString());
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
	lcd.print(" F");
	uiInstrument(fanControl, fanMode);
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
