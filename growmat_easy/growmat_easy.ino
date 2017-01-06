#define TEXT_GROWMATEASY "GROWMAT EASY"
#define VERSION 1

#include "libraries/RF433/RF433.h"
#define RFRX_PIN A4
#define RFTX_PIN 4
// MCE07
byte rf1off[] = {11,30,3,4,3,3,3,11,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,10,4,3,3,11,3,11,3,3,4,3,3,4,3,10,3,11,3,4,3,11,3,10,4,10,3,11,3,11,3,11,3,3,3,11,3,4,3,11,3,10,3,11,3,11,3,3,4,3,3,3,4,188,3,3,3,3,30,29,4,3,3,3,4,10,3,11,3,11,3,3,4,3,3,11,3,3,4,3,3,3,4,3,3,3,4,3,3,11,3,4,3,10,4};
byte rf1on[] = {5,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,10,4,3,3,3,4,3,3,3,4,3,3,4,3,10,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,4,3,10,3,11,3,11,3,11,3,11,3,10,3,4,3,11,3,11,3,3,3,4,3,3,3,4,3,10,4,10,3,11,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,4,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3};
byte rf2off[] = {16,29,3,4,3,3,3,11,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,11,3,3,3,11,3,11,3,4,2,4,3,3,3,11,3,11,3,4,3,11,3,10,3,11,3,11,3,11,3,11,3,3,3,11,3,11,3,3,3,11,3,11,3,3,3,11,3,4,3,3,3,188,4,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,10,4,3,3,3,4,3,3,3,3,4,3,4,3,10,3,4,3,11,3};
byte rf2on[] = {4,11,3,3,3,11,3,4,3,3,3,11,3,3,3,11,3,11,3,188,3,3,4,2,30,30,3,3,3,4,3,10,3,11,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,10,3,4,3,3,3,4,3,11,3,10,3,4,3,11,3,11,3,11,3,10,3,11,3,11,3,4,3,11,3,3,3,11,3,3,3,4,3,11,3,3,3,11,3,11,3,90,3,3,3,3,30,30,3,3,3,4,3,10,3,11,3,11,3};
byte rf3off[] = {5,3,3,3,29,30,3,3,4,3,3,11,3,11,3,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,10,3,11,3,11,3,3,4,11,3,3,3,4,3,10,3,11,3,11,3,11,3,3,3,4,3,188,3,3,4,2,30,29,4,3,3,3,4,10,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,11,3};
byte rf3on[] = {5,2,4,3,29,30,3,3,3,4,3,11,3,10,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,3,10,3,11,3,4,3,11,3,11,3,10,3,4,3,3,3,4,3,3,3,11,3,11,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,4,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3};
byte rf4off[] = {4,11,3,4,3,10,3,11,3,11,3,11,3,11,3,10,3,4,3,11,3,11,3,10,3,4,3,11,3,3,3,4,3,11,3,3,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,10,3,4,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,2,11,3,11,3,3,4,11,3,10,3,11,3,4,3,10,3,4,3,3,3,11,3,4,3};
byte rf4on[] = {4,4,3,10,4,89,4,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,2,11,3,11,3,3,4,11,3,3,3,4,3,10,3,4,3,11,3,10,3,4,3,11,3,188,3,3,3,3,30,29,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,11,3,3,3,4,3,3,3,4,3};


#define LCD_I2CADDR 0x20
const byte LCD_ROWS = 2;
const byte LCD_COLS = 16;

#define KPD_I2CADDR 0x20
const byte KPD_ROWS = 4;
const byte KPD_COLS = 4;

#define LEDPIN 13

//#define LIGHTCONTROL_PIN 9
//#define HEATERCONTROL_PIN 13 //10
//#define FANCONTROL_PIN 14 //11
//#define CYCLERCONTROL_PIN 12

#define EXT_PIN 8
#define EXTANA_PIN A2

#define POWERANA_PIN A3

#define ONEWIREBUS_PIN 5

#define LIGHT_PIN A0
#define DHT_PIN 6
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define ECINPUT_PIN 2
#define ECENABLE_PIN 3

#define PHANA_PIN A1
//#define PHTAPPIN A4
//#define PHTDPIN

#define SR04TX_PIN 7
#define SR04RX_PIN 12

#define SAMPLES 10


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

#define TEMPHIGHALARM_ADDR 68
#define TEMPLOWALARM_ADDR 72
#define LIGHTHIGHALARM_ADDR 76
#define LIGHTLOWALARM_ADDR 80
#define PHHIGHALARM_ADDR 84
#define PHLOWALARM_ADDR 88
#define PHLOWX_ADDR 92
#define PHHIGHX_ADDR 96
#define PHLOWY_ADDR 100
#define PHHIGHY_ADDR 104
#define ECHIGHALARM_ADDR 108
#define ECLOWALARM_ADDR 112
#define ECLOWX_ADDR 116
#define ECHIGHX_ADDR 120
#define ECLOWY_ADDR 124
#define ECHIGHY_ADDR 128
#define HEATERONTEMPNIGHT_ADDR 132
#define HEATEROFFTEMPNIGHT_ADDR 136
#define FANONTEMPNIGHT_ADDR 140
#define FANOFFTEMPNIGHT_ADDR 144
#define TEMPHIGHNIGHTALARM_ADDR 148
#define TEMPLOWNIGHTALARM_ADDR 152
#define HUMIHIGHALARM_ADDR 156
#define HUMILOWALARM_ADDR 160
#define LEVELHIGHALARM_ADDR 164
#define LEVELLOWALARM_ADDR 168

#define EXTERNALMODEALARM_ADDR 196

#define GSMNUMBER_ADDR 200  //16 bytes
#define GSMCODE_ADDR 216
#define GSMMODE_ADDR 220

#define MESSAGESOFFSET_ADDR 224
#define MESSAGES_ADDR 228
#define MESSAGESCOUNT 8
#define MESSAGELENGTH 16

// TODO: save memory!!!
#define MESSAGE_LIGHT_CONTROL "L:"
#define MESSAGE_HEATER_CONTROL "H:"
#define MESSAGE_FAN_CONTROL "F:"
#define MESSAGE_CYCLER_CONTROL "C:"

//                              "0123456789ABCDEF"
#define MESSAGE_ALARM_POWERON   "dd/mm hh:mm ON x"
#define MESSAGE_ALARM_TEMPHIGH  "dd/mm hh:mm T+ x"
#define MESSAGE_ALARM_TEMPLOW   "dd/mm hh:mm T- x"
#define MESSAGE_ALARM_LIGHTHIGH "dd/mm hh:mm L+ x"
#define MESSAGE_ALARM_LIGHTLOW  "dd/mm hh:mm L- x"
#define MESSAGE_ALARM_EXTERNAL  "dd/mm hh:mm EX x"
#define MESSAGE_ALARM_PHHIGH	"dd/mm hh:mm PH+x"
#define MESSAGE_ALARM_PHLOW     "dd/mm hh:mm PH-x"
#define MESSAGE_ALARM_ECHIGH	"dd/mm hh:mm EC+x"
#define MESSAGE_ALARM_ECLOW     "dd/mm hh:mm EC-x"
#define MESSAGE_ALARM_HUMIHIGH	"dd/mm hh:mm H +x"
#define MESSAGE_ALARM_HUMILOW   "dd/mm hh:mm H -x"
#define MESSAGE_ALARM_LEVELHIGH	"dd/mm hh:mm D +x"
#define MESSAGE_ALARM_LEVELLOW  "dd/mm hh:mm D -x"
#define MESSAGE_ALARM_POWER     "dd/mm hh:mm PW x"
#define MESSAGE_ALARM_ON  '1'
#define MESSAGE_ALARM_OFF '0'

//const char MESSAGE_TEMPHIGH[] PROGMEM  = {"T+!"};
#define MESSAGE_TEMPHIGH "T+!"
#define MESSAGE_TEMPLOW "T-!"
#define MESSAGE_LIGHTHIGH "L+!"
#define MESSAGE_LIGHTLOW "L-!"
#define MESSAGE_EXTERNAL "EX!"
#define MESSAGE_HUMIHIGH "T+!"
#define MESSAGE_HUMILOW "T-!"
//const char MESSAGE_POWERLOW[] PROGMEM  = {"PW!"};
#define MESSAGE_POWERLOW "PW!"
#define MESSAGE_LEVELHIGH "D+!"
#define MESSAGE_LEVELLOW "D-!"

//const char MESSAGE_TEMP[] PROGMEM = {"TA="};
#define MESSAGE_TEMP "TA="
#define MESSAGE_HUMI "HA="
#define MESSAGE_LIGHT "LA="
#define MESSAGE_EXT "EX="
#define MESSAGE_GSM  "GM="
#define MESSAGE_TEMP2  "TM="
#define MESSAGE_PH  "PH="
#define MESSAGE_EC  "EC="
#define MESSAGE_POWER "PW="
#define MESSAGE_LEVEL "LV="

#define MESSAGE_CMD_LIGHT  "#L"
#define MESSAGE_CMD_HEATER  "#H"
#define MESSAGE_CMD_FAN  "#F"
#define MESSAGE_CMD_CYCLER  "#C"
#define MESSAGE_CMD_CYCLER  "#C"
//const char MESSAGE_CMD_REQUEST[] PROGMEM  = {"#?"};
#define MESSAGE_CMD_REQUEST  "#?"
#define MESSAGE_CMD_GSMMODE  "#M"

#define MESSAGE_CMD_PARREADINT "#PRI"
#define MESSAGE_CMD_PARREADFLOAT "#PRF"
#define MESSAGE_CMD_PARWRITEINT "#PWI"
#define MESSAGE_CMD_PARWRITEFLOAT "#PWF"
#define MESSAGE_CMD_PARRELOAD "#PLD"

//#define MESSAGE_CMD_SAVEFLOAT  "#SF"
//#define MESSAGE_CMD_SAVEINT  "#SI"

//#define GSMCHECKSMSINTERVAL  60000
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

#include <avr/wdt.h>

//////////////////////////////////
// variables
//////////////////////////////////
DateTime nowSetClock;
DateTime now;
unsigned long milliseconds, millisecondsPrev;
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
float temperature, humidity, heatIndex, ph, ec, temperature2, level;
float light;
float extana, powerana;

// parameters
unsigned int lightOnHour, lightOnMin, lightOffHour, lightOffMin;
unsigned int cyclerOnMin, cyclerOnSec, cyclerOffMin, cyclerOffSec;

float heaterOnTemp, heaterOffTemp, heaterOnTempNight, heaterOffTempNight;
float fanOnTemp, fanOffTemp, fanOnTempNight, fanOffTempNight;

float tempHighAlarm, tempLowAlarm, lightHighAlarm, lightLowAlarm, tempHighNightAlarm, tempLowNightAlarm;
float phHighPhAlarm, phLowAlarm, ecHighAlarm, ecLowAlarm;//, temp2HighTempAlarm, temp2LowTempAlarm, levelHighAlarm, levelLowAlarm;
float humiHighAlarm, humiLowAlarm;
float levelHighAlarm, levelLowAlarm;

float tempHysteresis = 0.5;
float lightHysteresis = 10;
float phHysteresis = 0.2;
float ecHysteresis = 0.2;
float humiHysteresis = 0.5;
float levelHysteresis = 0.5;

unsigned long lightOnOffDelay = 1800; //seconds, 0.5h
//float tempDayNightDelay = 60.0;
//float levelHysteresis = 0.2;

int phLowX, phHighX;
float phLowY, phHighY;

int ecLowX, ecHighX;
float ecLowY, ecHighY;

byte externalModeAlarm;


bool parseCmd(String &text, const char* cmd, byte &mode, const int address=0) {
	int pos = text.indexOf(cmd);
	if(pos > -1) {
		char ch = text.charAt(pos + strlen(cmd));
		if(ch=='A') mode = 0;
		else if(ch=='0') mode = 1;
		else if(ch=='1') mode = 2;
		if(address)
			OMEEPROM::write(address, mode);
		return true;
	}
	return false;
}
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

	unsigned long timeActive;
	unsigned long timeDeactive;

 public:
	// TODO: fix when millis overrun!!!
	int alarmActiveDelay = 5000;
	int alarmDeactiveDelay = 5000;
	bool active;
	bool unAck;

	bool activate(bool state) {
		if(state) {
			if(!active) {
				if(!timeActive)
					timeActive = millis();
				if((timeActive + alarmActiveDelay) < millis()) {
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
					timeDeactive = millis();
				if((timeDeactive + alarmDeactiveDelay) < millis()) {
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

Alarm tempHighAlarm2, tempLowAlarm2, lightHighAlarm2, lightLowAlarm2, externalAlarm2, phHighAlarm2, phLowAlarm2,
	ecHighAlarm2, ecLowAlarm2, humiHighAlarm2, humiLowAlarm2, powerAlarm2, levelHighAlarm2, levelLowAlarm2;


#include "libraries/OneWire/OneWire.h"
#include "libraries/DallasTemperature/DallasTemperature.h"
//#define ONEWIREBUS_PIN 2

OneWire oneWire(ONEWIREBUS_PIN);
DallasTemperature oneWireSensors(&oneWire);

#include <Wire.h>

// SIM800L
#include "libraries/Sim800l/Sim800l.h"
#include <SoftwareSerial.h> //is necessary for the library!!
//Sim800l sim800l;  //to declare the library

class Sim800l2 : public Sim800l {

public:

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

	void Sim800l2::sendSmsTextLn(char* s) {
		printSerial(s);
		printSerial('\n');
	}
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
		printSerial("\r");
		delay(100);
		printSerial((char)26);
		_buffer=_readSerial();
		//expect CMGS:xxx   , where xxx is a number,for the g sms.
		if (((_buffer.indexOf("CMGS") ) != -1 ) )
			return true;
		else
			return false;
	}
};

// TODO: ???
//Sim800l sim800l;  //to declare the library
Sim800l2 sim800l;  //to declare the library

class GsmManager {

public:
	Sim800l2* sim;
	//uint8_t callStatus;
	//00420123456789

	char* gsmNumber;
	int* gsmCode;

	//unsigned long callDuration;
	unsigned long millisecondsCall;

	GsmManager(Sim800l2* sim_c, int* gsmCode_c, char* gsmNumber_c) {
		sim = sim_c;
		gsmCode = gsmCode_c;
		gsmNumber = gsmNumber_c;
	}
	/*
	bool update() {
		//sim->readSerial();
		return false;
	}
	*/

	void call(){
		if(!millisecondsCall && gsmMode > 1) {
			//Serial.println(F("CALL"));
			Serial.println('C');
			sim->callNumber(gsmNumber);
			millisecondsCall= milliseconds;
		}
	}

	void updateCall() {
		// TODO: solve millis overflow
		if(millisecondsCall && (millisecondsCall + GSMCALLDURATION < milliseconds)){
			//Serial.println(F("HANG"));
			Serial.println('H');
			sim->hangoffCall();
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
		//Serial.println('R');
		String text = sim->readSms(21);
		//TODO TEST
		//text = "#CODE9999 #00420724095917";
		//Serial.print("SMS");
		//Serial.print(sim->sendSms(gsmNumber, "TEST"));

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
						//text.substring(pos+1).toCharArray(gsmNumber, text.indexOf("", pos), 0);
						text.substring(pos+1).toCharArray(gsmNumber, 16, 0);
						gsmNumber[14] = 0;

						for(int i=0; i < 16; i++) {
							OMEEPROM::write(GSMNUMBER_ADDR + i, gsmNumber[i]);
						}
					}

					pos = text.indexOf("#M");
					if(pos > -1) {
						// save new gsm number
						gsmMode = text.charAt(pos + 2) - 48;
						OMEEPROM::write(GSMMODE_ADDR, gsmMode);
					}

			  		parseCmd(text, MESSAGE_CMD_LIGHT, lightMode);
			  		parseCmd(text, MESSAGE_CMD_HEATER, heaterMode);
			  		parseCmd(text, MESSAGE_CMD_FAN, fanMode);
			  		parseCmd(text, MESSAGE_CMD_CYCLER, cyclerMode);

			  		/*
					pos = text.indexOf("#L");
					if(pos > -1) {
						//Serial.println("LIGHT");
						ch = text.charAt(pos+2);
						if(ch=='A') lightMode = 0;
						else if(ch=='0') lightMode = 1;
						else if(ch=='1') lightMode = 2;
						//OMEEPROM::write(LIGHTMODE_ADDR, lightMode);
					}
					pos = text.indexOf("#H");
					if(pos > -1) {
						ch = text.charAt(pos+2);
						if(ch=='A') heaterMode = 0;
						else if(ch=='0') heaterMode = 1;
						else if(ch=='1') heaterMode = 2;
						//OMEEPROM::write(HEATERMODE_ADDR, heaterMode);
					}
					pos = text.indexOf("#F");
					if(pos > -1) {
						ch = text.charAt(pos+2);
						if(ch=='A') fanMode = 0;
						else if(ch=='0') fanMode = 1;
						else if(ch=='1') fanMode = 2;
						//OMEEPROM::write(FANMODE_ADDR, fanMode);
					}
					pos = text.indexOf("#C");
					if(pos > -1) {
						ch = text.charAt(pos+2);
						if(ch=='A') cyclerMode = 0;
						else if(ch=='0') cyclerMode = 1;
						else if(ch=='1') cyclerMode = 2;
						//OMEEPROM::write(CYCLERMODE_ADDR, cyclerMode);
					}
					*/

					pos = text.indexOf(MESSAGE_CMD_REQUEST);
					//pos = text.indexOf("#?");
					if(pos > -1) {// && gsmMode > 1) {

						// TODO:
						//char msg[256];// = "GROWMAT INFO";
						//bool r = sim->sendSms(gsmNumber, msg);
						Serial.println(F("SMS"));

						//Serial.print(sim->sendSms(gsmNumber, "TEST"));

						sim->sendSmsBegin(gsmNumber);
						sim->sendSmsText('\n');
						//sim->sendSmsText("TEST");
						//sim->sendSmsEnd();

						if(tempHighAlarm2.active) sim->sendSmsTextLn(MESSAGE_TEMPHIGH);
						if(tempLowAlarm2.active) sim->sendSmsTextLn(MESSAGE_TEMPLOW);
						if(lightHighAlarm2.active) sim->sendSmsTextLn(MESSAGE_LIGHTHIGH);
						if(lightLowAlarm2.active) sim->sendSmsTextLn(MESSAGE_LIGHTLOW);
						if(externalAlarm2.active) sim->sendSmsTextLn(MESSAGE_EXTERNAL);
						if(humiHighAlarm2.active) sim->sendSmsTextLn(MESSAGE_HUMIHIGH);
						if(humiLowAlarm2.active) sim->sendSmsTextLn(MESSAGE_HUMILOW);
						if(levelHighAlarm2.active) sim->sendSmsTextLn(MESSAGE_LEVELHIGH);
						if(levelLowAlarm2.active) sim->sendSmsTextLn(MESSAGE_LEVELLOW);
						if(powerAlarm2.active) sim->sendSmsTextLn(MESSAGE_POWERLOW);

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
						sim->sendSmsText('\n');

						sim->sendSmsText(MESSAGE_TEMP);
						sim->sendSmsText(temperature);
						sim->sendSmsText('\n');
						sim->sendSmsText(MESSAGE_HUMI);
						sim->sendSmsText(humidity);
						sim->sendSmsText('\n');
						sim->sendSmsText(MESSAGE_LIGHT);
						sim->sendSmsText(light);
						sim->sendSmsText('\n');
						sim->sendSmsText(MESSAGE_EXT);
						sim->sendSmsText(extana);
						sim->sendSmsText('\n');
						sim->sendSmsText(MESSAGE_POWER);
						sim->sendSmsText(powerana);
						sim->sendSmsText('\n');
						sim->sendSmsText(MESSAGE_TEMP2);
						sim->sendSmsText(temperature2);
						sim->sendSmsText('\n');
						sim->sendSmsText(MESSAGE_PH);
						sim->sendSmsText(ph);
						sim->sendSmsText('\n');
						sim->sendSmsText(MESSAGE_EC);
						sim->sendSmsText(ec);
						sim->sendSmsText('\n');
						sim->sendSmsText(MESSAGE_LEVEL);
						sim->sendSmsText(level);
						sim->sendSmsText('\n');

						sim->sendSmsText(MESSAGE_GSM);
						sim->sendSmsText(gsmMode);
						sim->sendSmsText("\n");
						//sim->sendSmsText("\nGSM NUMBER=");
						//sim->sendSmsText(gsmNumber);

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
} gsmMgr(&sim800l, &gsmCode, gsmNumber);

// RTC
#include "libraries/RTClib/RTClib.h"
RTC_DS3231 rtc;

// DHT
#include "libraries/DHT-sensor-library-master/DHT.h"
DHT dht(DHT_PIN, DHTTYPE);

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
MENU_SELECT_ITEM  sel_rc_disable={ 3, {"DISABLE RADIO"} };
//MENU_SELECT_ITEM  sel_rcon= { 3, {"RC ON"} };

MENU_SELECT_LIST  const state_list[] = { &sel_auto, &sel_off, &sel_on, &sel_rc_disable };
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
MENU_VALUE heaterOnTempNight_value ={ TYPE_FLOAT_10, 99,     0,    MENU_TARGET(&heaterOnTempNight), HEATERONTEMPNIGHT_ADDR  };
MENU_VALUE heaterOffTempNight_value={ TYPE_FLOAT_10, 99,     0,    MENU_TARGET(&heaterOffTempNight), HEATEROFFTEMPNIGHT_ADDR };
//                                "123456789ABCDEF"
MENU_ITEM heaterOnTemp_item   ={ {"HEATER ON   [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&heaterOnTemp_value)};
MENU_ITEM heaterOffTemp_item  ={ {"HEATER OFF  [C]"},   ITEM_VALUE,  0,        MENU_TARGET(&heaterOffTemp_value) };
MENU_ITEM heaterOnTempNight_item   ={ {"HEATER ON N [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&heaterOnTempNight_value)};
MENU_ITEM heaterOffTempNight_item  ={ {"HEATER OF N [C]"},   ITEM_VALUE,  0,        MENU_TARGET(&heaterOffTempNight_value) };

MENU_SELECT fanMode_select ={ &fanMode,           MENU_SELECT_SIZE(state_list),   MENU_TARGET(&state_list) };
MENU_VALUE fanMode_value =  { TYPE_SELECT,     0,     0,     MENU_TARGET(&fanMode_select) };
MENU_ITEM fanMode_item    = { {"FAN CONTROL"}, ITEM_VALUE,  0,        MENU_TARGET(&fanMode_value) };
MENU_VALUE fanOnTemp_value ={ TYPE_FLOAT_10, 99,   -99,    MENU_TARGET(&fanOnTemp) , FANONTEMP_ADDR};
MENU_VALUE fanOffTemp_value={ TYPE_FLOAT_10, 99,   -99,    MENU_TARGET(&fanOffTemp) , FANOFFTEMP_ADDR};
MENU_VALUE fanOnTempNight_value ={ TYPE_FLOAT_10, 99,   -99,    MENU_TARGET(&fanOnTempNight) , FANONTEMPNIGHT_ADDR};
MENU_VALUE fanOffTempNight_value={ TYPE_FLOAT_10, 99,   -99,    MENU_TARGET(&fanOffTempNight) , FANOFFTEMPNIGHT_ADDR};
//                                "123456789ABCDEF"
MENU_ITEM fanOnTemp_item     ={ {"FAN ON     [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&fanOnTemp_value) };
MENU_ITEM fanOffTemp_item    ={ {"FAN OFF    [C]"},   ITEM_VALUE,  0,        MENU_TARGET(&fanOffTemp_value) };
MENU_ITEM fanOnTempNight_item     ={ {"FAN ON  N  [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&fanOnTempNight_value) };
MENU_ITEM fanOffTempNight_item    ={ {"FAN OFF N  [C]"},   ITEM_VALUE,  0,        MENU_TARGET(&fanOffTempNight_value) };

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

MENU_LIST const submenu_list2[] = { &heaterMode_item, &heaterOnTemp_item, &heaterOffTemp_item, &heaterOnTempNight_item, &heaterOffTempNight_item };
MENU_ITEM menu_submenu2 = { {"HEATER->"},  ITEM_MENU,  MENU_SIZE(submenu_list2),  MENU_TARGET(&submenu_list2) };

MENU_LIST const submenu_list3[] = { &fanMode_item, &fanOnTemp_item, &fanOffTemp_item, &fanOnTempNight_item, &fanOffTempNight_item };
MENU_ITEM menu_submenu3 = { {"FAN->"},  ITEM_MENU,  MENU_SIZE(submenu_list3),  MENU_TARGET(&submenu_list3) };

MENU_LIST const submenu_list4[] = { &cyclerMode_item, &cyclerOnMin_item, &cyclerOnSec_item, &cyclerOffMin_item,  &cyclerOffSec_item};
MENU_ITEM menu_submenu4 = { {"CYCLER->"},  ITEM_MENU,  MENU_SIZE(submenu_list4),  MENU_TARGET(&submenu_list4) };

// Alarms
MENU_VALUE tempHighAlarm_value={ TYPE_FLOAT_10, 99,    -99,    MENU_TARGET(&tempHighNightAlarm), TEMPHIGHALARM_ADDR };
MENU_ITEM tempHighAlarm_item   ={ {"TEMP HIGH   [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&tempHighAlarm_value) };
MENU_VALUE tempLowAlarm_value={ TYPE_FLOAT_10, 99,    -99,    MENU_TARGET(&tempLowNightAlarm), TEMPLOWALARM_ADDR };
MENU_ITEM tempLowAlarm_item   ={ {"TEMP LOW    [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&tempLowAlarm_value) };

MENU_VALUE tempHighNightAlarm_value={ TYPE_FLOAT_10, 99,    -99,    MENU_TARGET(&tempHighAlarm), TEMPHIGHNIGHTALARM_ADDR };
MENU_ITEM tempHighNightAlarm_item   ={ {"TEMP HIGH N [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&tempHighNightAlarm_value) };
MENU_VALUE tempLowNightAlarm_value={ TYPE_FLOAT_10, 99,    -99,    MENU_TARGET(&tempLowAlarm), TEMPLOWNIGHTALARM_ADDR };
MENU_ITEM tempLowNightAlarm_item   ={ {"TEMP LOW  N [C]"},    ITEM_VALUE,  0,        MENU_TARGET(&tempLowNightAlarm_value) };

//                               TYPE             MAX    MIN    TARGET
MENU_VALUE phLowX_value = 	   { TYPE_UINT,       1023,  0,     MENU_TARGET(&phLowX), PHLOWX_ADDR };
MENU_VALUE phHighX_value = 	   { TYPE_UINT,       1023,  0,     MENU_TARGET(&phHighX), PHHIGHX_ADDR };
MENU_VALUE phLowY_value =	   { TYPE_FLOAT,      14,    0,     MENU_TARGET(&phLowY), PHLOWY_ADDR };
MENU_VALUE phHighY_value =	   { TYPE_FLOAT,      14,    0,     MENU_TARGET(&phHighY), PHHIGHY_ADDR };

//                                "123456789ABCDEF"
MENU_ITEM phLowX_item   =   { {"PH LOW  X   [-]"},   	ITEM_VALUE,  0,        MENU_TARGET(&phLowX_value) };
MENU_ITEM phHighX_item   =  { {"PH HIGH X   [-]"},    ITEM_VALUE,  0,        MENU_TARGET(&phHighX_value) };
MENU_ITEM phLowY_item  =    { {"PH LOW  Y  [PH]"},  ITEM_VALUE,  0,        MENU_TARGET(&phLowY_value) };
MENU_ITEM phHighY_item   =  { {"PH HIGH Y  [PH]"},   ITEM_VALUE,  0,        MENU_TARGET(&phHighY_value) };

MENU_LIST const submenu_list_ph[] = {&phLowX_item, &phLowY_item,  &phHighX_item, &phHighY_item};
MENU_ITEM menu_submenu_ph = { {"PH->"},  ITEM_MENU,  MENU_SIZE(submenu_list_ph),  MENU_TARGET(&submenu_list_ph) };

//                               TYPE             MAX    MIN    TARGET
MENU_VALUE ecLowX_value = 	   { TYPE_UINT,       0,  0,     MENU_TARGET(&ecLowX), ECLOWX_ADDR };
MENU_VALUE ecHighX_value = 	   { TYPE_UINT,       0,  0,     MENU_TARGET(&ecHighX), ECHIGHX_ADDR };
MENU_VALUE ecLowY_value =	   { TYPE_FLOAT,      0,    0,     MENU_TARGET(&ecLowY), ECLOWY_ADDR };
MENU_VALUE ecHighY_value =	   { TYPE_FLOAT,      0,    0,     MENU_TARGET(&ecHighY), ECHIGHY_ADDR };

//                                "123456789ABCDEF"
MENU_ITEM ecLowX_item   =   { {"EC LOW  X   [-]"},   	ITEM_VALUE,  0,        MENU_TARGET(&ecLowX_value) };
MENU_ITEM ecHighX_item   =  { {"EC HIGH X   [-]"},    ITEM_VALUE,  0,        MENU_TARGET(&ecHighX_value) };
MENU_ITEM ecLowY_item  =    { {"EC LOW  Y [S/M]"},  ITEM_VALUE,  0,        MENU_TARGET(&ecLowY_value) };
MENU_ITEM ecHighY_item   =  { {"EC HIGH Y [S/M]"},   ITEM_VALUE,  0,        MENU_TARGET(&ecHighY_value) };

MENU_LIST const submenu_list_ec[] = {&ecLowX_item, &ecLowY_item,  &ecHighX_item, &ecHighY_item};
MENU_ITEM menu_submenu_ec = { {"EC->"},  ITEM_MENU,  MENU_SIZE(submenu_list_ec),  MENU_TARGET(&submenu_list_ec) };

MENU_VALUE phLowAlarm_value =	   { TYPE_FLOAT,      14,    0,     MENU_TARGET(&phLowAlarm), PHLOWALARM_ADDR };
MENU_VALUE phHighPhAlarm_value =	   { TYPE_FLOAT,      14,    0,     MENU_TARGET(&phHighPhAlarm), PHHIGHALARM_ADDR };
MENU_ITEM phLowAlarm_item  =    { {"PH LOW     [PH]"},  ITEM_VALUE,  0,        MENU_TARGET(&phLowAlarm_value) };
MENU_ITEM phHighPhAlarm_item   =  { {"PH HIGH    [PH]"},   ITEM_VALUE,  0,        MENU_TARGET(&phHighPhAlarm_value) };

MENU_VALUE ecLowAlarm_value =	   { TYPE_FLOAT,      0,    0,     MENU_TARGET(&ecLowAlarm), ECLOWALARM_ADDR };
MENU_VALUE ecHighAlarm_value =	   { TYPE_FLOAT,      0,    0,     MENU_TARGET(&ecHighAlarm), ECHIGHALARM_ADDR };
MENU_ITEM ecLowAlarm_item  =    { {"EC LOW    [S/M]"},  ITEM_VALUE,  0,        MENU_TARGET(&ecLowAlarm_value) };
MENU_ITEM ecHighAlarm_item   =  { {"EC HIGH   [S/M]"},   ITEM_VALUE,  0,        MENU_TARGET(&ecHighAlarm_value) };

MENU_VALUE humiLowAlarm_value =	   { TYPE_FLOAT,      0,    0,     MENU_TARGET(&humiLowAlarm), HUMILOWALARM_ADDR };
MENU_VALUE humiHighAlarm_value =	   { TYPE_FLOAT,      0,    0,     MENU_TARGET(&humiHighAlarm), HUMIHIGHALARM_ADDR };
MENU_ITEM humiLowAlarm_item  =    { {"HUMI LOW    [%]"},  ITEM_VALUE,  0,        MENU_TARGET(&humiLowAlarm_value) };
MENU_ITEM humiHighAlarm_item   =  { {"HUMI HIGH   [%]"},   ITEM_VALUE,  0,        MENU_TARGET(&humiHighAlarm_value) };

MENU_VALUE levelLowAlarm_value =	   { TYPE_FLOAT,      0,    -200,     MENU_TARGET(&levelLowAlarm), LEVELLOWALARM_ADDR };
MENU_VALUE levelHighAlarm_value =	   { TYPE_FLOAT,      0,    -200,     MENU_TARGET(&levelHighAlarm), LEVELHIGHALARM_ADDR };
MENU_ITEM levelLowAlarm_item  =    { {"LEVEL LOW  [CM]"},  ITEM_VALUE,  0,        MENU_TARGET(&levelLowAlarm_value) };
MENU_ITEM levelHighAlarm_item   =  { {"LEVEL HIGH [CM]"},   ITEM_VALUE,  0,        MENU_TARGET(&levelHighAlarm_value) };

//                                "123456789ABCDEF"
MENU_VALUE lightHighAlarm_value={ TYPE_FLOAT_10, 102,    0,    MENU_TARGET(&lightHighAlarm), LIGHTHIGHALARM_ADDR };
MENU_ITEM lightHighAlarm_item   ={ {"LIGH HIGH"},    ITEM_VALUE,  0,        MENU_TARGET(&lightHighAlarm_value) };
MENU_VALUE lightLowAlarm_value={ TYPE_FLOAT_10, 102,    0,    MENU_TARGET(&lightLowAlarm), LIGHTLOWALARM_ADDR };
MENU_ITEM lightLowAlarm_item   ={ {"LIGH LOW"},    ITEM_VALUE,  0,        MENU_TARGET(&lightLowAlarm_value) };

MENU_VALUE externalModeAlarm_value={ TYPE_BYTE, 2,    0,    MENU_TARGET(&externalModeAlarm), EXTERNALMODEALARM_ADDR };
MENU_ITEM externalModeAlarm_item   ={ {"EXTERNAL"},    ITEM_VALUE,  0,        MENU_TARGET(&externalModeAlarm_value) };

MENU_LIST const submenu_list5[] = { &tempHighAlarm_item, &tempLowAlarm_item, &humiHighAlarm_item, &humiLowAlarm_item,  &tempHighNightAlarm_item, &tempLowNightAlarm_item, &lightHighAlarm_item, &lightLowAlarm_item,
			&phLowAlarm_item, &phHighPhAlarm_item, &ecLowAlarm_item, &ecHighAlarm_item,  &levelLowAlarm_item, &levelHighAlarm_item, &externalModeAlarm_item};
MENU_ITEM menu_submenu5 = { {"ALARM SET->"},  ITEM_MENU,  MENU_SIZE(submenu_list5),  MENU_TARGET(&submenu_list5) };

MENU_VALUE gsmMode_value= { TYPE_BYTE, 3, 0,    MENU_TARGET(&gsmMode), GSMMODE_ADDR };
MENU_ITEM gsmMode_item   ={ {"GSM MODE->"},    ITEM_VALUE,  0,        MENU_TARGET(&gsmMode_value) };
MENU_VALUE gsmCode_value= { TYPE_UINT, 9999, 1000,    MENU_TARGET(&gsmCode), GSMCODE_ADDR };
MENU_ITEM gsmCode_item   ={ {"GSM CODE->"},    ITEM_VALUE,  0,        MENU_TARGET(&gsmCode_value) };
MENU_LIST const submenu_list6[] = { &gsmMode_item, &gsmCode_item};
MENU_ITEM menu_submenu6 = { {"GSM->"},  ITEM_MENU,  MENU_SIZE(submenu_list6),  MENU_TARGET(&submenu_list6) };

MENU_ITEM item_setClock   = { {"SET CLOCK->"},  ITEM_ACTION, 0,        MENU_TARGET(&uiSetClock) };
//MENU_ITEM item_alarmList   = { {"ALARM LIST->"},  ITEM_ACTION, 0,        MENU_TARGET(&uiAlarmList) };

MENU_ITEM item_reset   = { {"RESET!"},  ITEM_ACTION, 0,        MENU_TARGET(&uiResetAction) };
//MENU_ITEM item_info   = { {"INFO->"},  ITEM_ACTION, 0,        MENU_TARGET(&uiInfo) };

//        List of items in menu level
MENU_LIST const root_list[]   = { &menu_submenu1 , &menu_submenu2, &menu_submenu3, &menu_submenu4, &menu_submenu5, &item_setClock, &menu_submenu6, &menu_submenu_ph, &menu_submenu_ec, &item_reset };//&item_alarmList, &item_testme, , &item_info//&item_bazme, &item_bakme,

// Root item is always created last, so we can add all other items to it
MENU_ITEM menu_root     = { {"Root"},        ITEM_MENU,   MENU_SIZE(root_list),    MENU_TARGET(&root_list) };

OMMenuMgr2 Menu(&menu_root, MENU_DIGITAL, &kpd);



int saveMessage(char msg[], char status) {
	if((gsmNumber[2] != '0') && status == MESSAGE_ALARM_ON) {
		gsmMgr.call();
	}

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

    msg[11]= ' ';
    msg[15] = status;

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

void serialPrintParInt(int address)
{
	int val;
	OMEEPROM::read(address, val);
	Serial.print(val);
	Serial.println();
	Serial.println();
}
void serialPrintParFloat(int address)
{
	float val;
	OMEEPROM::read(address, val);
	Serial.println(val);
	Serial.println();
	Serial.println();
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
    read(HEATERONTEMPNIGHT_ADDR, heaterOnTempNight);
    read(HEATEROFFTEMPNIGHT_ADDR, heaterOffTempNight);
    read(FANMODE_ADDR, fanMode);
    read(FANONTEMP_ADDR, fanOnTemp);
    read(FANOFFTEMP_ADDR, fanOffTemp);
    read(FANONTEMPNIGHT_ADDR, fanOnTempNight);
    read(FANOFFTEMPNIGHT_ADDR, fanOffTempNight);
    read(CYCLERMODE_ADDR, cyclerMode);
    read(CYCLERONMIN_ADDR, cyclerOnMin);
    read(CYCLERONSEC_ADDR, cyclerOnSec);
    read(CYCLEROFFMIN_ADDR, cyclerOffMin);
    read(CYCLEROFFSEC_ADDR, cyclerOffSec);
    read(TEMPHIGHALARM_ADDR, tempHighAlarm);
    read(TEMPLOWALARM_ADDR, tempLowAlarm);
    read(LIGHTHIGHALARM_ADDR, lightHighAlarm);
    read(LIGHTLOWALARM_ADDR, lightLowAlarm);
    read(TEMPHIGHNIGHTALARM_ADDR, tempHighNightAlarm);
    read(TEMPLOWNIGHTALARM_ADDR, tempLowNightAlarm);
    read(PHLOWALARM_ADDR, phLowAlarm);
    read(PHHIGHALARM_ADDR, phHighPhAlarm);
    read(PHLOWX_ADDR, phLowX);
    read(PHHIGHX_ADDR, phHighX);
    read(PHLOWY_ADDR, phLowY);
    read(PHHIGHY_ADDR, phHighY);
    read(ECLOWALARM_ADDR, ecLowAlarm);
    read(ECHIGHALARM_ADDR, ecHighAlarm);
    read(ECLOWX_ADDR, ecLowX);
    read(ECHIGHX_ADDR, ecHighX);
    read(ECLOWY_ADDR, ecLowY);
    read(ECHIGHY_ADDR, ecHighY);
    read(GSMMODE_ADDR, gsmMode);
    read(GSMCODE_ADDR, gsmCode);
    read(EXTERNALMODEALARM_ADDR, externalModeAlarm);
    read(HUMIHIGHALARM_ADDR, humiHighAlarm);
    read(HUMILOWALARM_ADDR, humiLowAlarm);
    read(LEVELHIGHALARM_ADDR, levelHighAlarm);
    read(LEVELLOWALARM_ADDR, levelLowAlarm);

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
    heaterOnTemp=15.0;
    heaterOffTemp=16.0;
    heaterOnTempNight=10.0;
    heaterOffTempNight=11.0;
    OMEEPROM::write(HEATERMODE_ADDR, heaterMode);
    OMEEPROM::write(HEATERONTEMP_ADDR, heaterOnTemp);
    OMEEPROM::write(HEATEROFFTEMP_ADDR, heaterOffTemp);
    OMEEPROM::write(HEATERONTEMPNIGHT_ADDR, heaterOnTempNight);
    OMEEPROM::write(HEATEROFFTEMPNIGHT_ADDR, heaterOffTempNight);

    fanMode = 0;
    fanOnTemp=20.0;
    fanOffTemp=19.0;
    fanOnTempNight=15.0;
    fanOffTempNight=14.0;
    OMEEPROM::write(FANMODE_ADDR, fanMode);
    OMEEPROM::write(FANONTEMP_ADDR, fanOnTemp);
    OMEEPROM::write(FANOFFTEMP_ADDR, fanOffTemp);
    OMEEPROM::write(FANONTEMPNIGHT_ADDR, fanOnTempNight);
    OMEEPROM::write(FANOFFTEMPNIGHT_ADDR, fanOffTempNight);

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

    tempHighAlarm = 21.0;
    tempLowAlarm = 19.0;
    lightHighAlarm = 40.0;
    lightLowAlarm = 60.0;
    tempHighNightAlarm = 15.0;
    tempLowNightAlarm = 10.0;
    OMEEPROM::write(TEMPHIGHALARM_ADDR, tempHighAlarm);
    OMEEPROM::write(TEMPLOWALARM_ADDR, tempLowAlarm);
    OMEEPROM::write(LIGHTHIGHALARM_ADDR, lightHighAlarm);
    OMEEPROM::write(LIGHTLOWALARM_ADDR, lightLowAlarm);
    OMEEPROM::write(TEMPHIGHNIGHTALARM_ADDR, tempHighNightAlarm);
    OMEEPROM::write(TEMPLOWNIGHTALARM_ADDR, tempLowNightAlarm);

    phLowX = 655;
    phHighX = 528;
    phLowY = 4.01;
    phHighY = 7.00;
    phLowAlarm = 0;
    phHighPhAlarm = 14;
    OMEEPROM::write(PHLOWALARM_ADDR, phLowAlarm);
    OMEEPROM::write(PHHIGHALARM_ADDR, phHighPhAlarm);
    OMEEPROM::write(PHLOWX_ADDR, phLowX);
    OMEEPROM::write(PHHIGHX_ADDR, phHighX);
    OMEEPROM::write(PHLOWY_ADDR, phLowY);
    OMEEPROM::write(PHHIGHY_ADDR, phHighY);

    ecLowX = 242;
    ecHighX = 125;
    ecLowY = 1.278;
    ecHighY = 4.523;
    ecLowAlarm = 0;
    ecHighAlarm = 1000;
    OMEEPROM::write(ECLOWALARM_ADDR, ecLowAlarm);
    OMEEPROM::write(ECHIGHALARM_ADDR, ecHighAlarm);
    OMEEPROM::write(ECLOWX_ADDR, ecLowX);
    OMEEPROM::write(ECHIGHX_ADDR, ecHighX);
    OMEEPROM::write(ECLOWY_ADDR, ecLowY);
    OMEEPROM::write(ECHIGHY_ADDR, ecHighY);

    humiLowAlarm = 0.0;
    humiHighAlarm = 100.0;
    OMEEPROM::write(HUMIHIGHALARM_ADDR, humiHighAlarm);
    OMEEPROM::write(HUMILOWALARM_ADDR, humiLowAlarm);

    levelLowAlarm = -200.0;
    levelHighAlarm = 0.0;
    OMEEPROM::write(LEVELHIGHALARM_ADDR, levelHighAlarm);
    OMEEPROM::write(LEVELLOWALARM_ADDR, levelLowAlarm);
    /*
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
	*/

    externalModeAlarm = 0;
    OMEEPROM::write(EXTERNALMODEALARM_ADDR, externalModeAlarm);
}

//////////////////////////////////
// setup
//////////////////////////////////
void setup() {

	oneWireSensors.begin();

	pinMode(SR04RX_PIN, INPUT);
	pinMode(SR04TX_PIN, OUTPUT);

	pinMode(ECINPUT_PIN ,INPUT_PULLUP);
	pinMode(ECENABLE_PIN ,OUTPUT);

	pinMode(EXT_PIN, INPUT);
	digitalWrite(EXT_PIN, HIGH);
	//externalAlarm2.alarmActiveDelay = 0;

	pinMode(LEDPIN, OUTPUT);
	/*
	pinMode(LIGHTCONTROL_PIN, OUTPUT);
	pinMode(HEATERCONTROL_PIN, OUTPUT);
	pinMode(FANCONTROL_PIN, OUTPUT);
	pinMode(CYCLERCONTROL_PIN, OUTPUT);
	*/

	Serial.begin(9600);
	while(!Serial);
	/*
	Serial.println();
	Serial.print(TEXT_GROWMATEASY);
	Serial.print(' ');
	Serial.println(VERSION);
	//Serial.println(F("##############"));
	*/
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

	wdt_enable(WDTO_8S);
}

/*
void proccedCmd(String text, bool checkPassword) {
	int pos;
	char ch;

	if(checkPassword) {
		pos = text.indexOf("#P");
		if(text.substring(pos+2, pos+6) != String(*gsmCode, DEC)) {
		//Serial.println("CODE NOK");
			return;
		}
	}

	pos = text.indexOf("#00");
	if(pos > -1) {
		// save new gsm number
		text.substring(pos+1).toCharArray(gsmNumber, 16, 0);
		gsmNumber[14] = 0;
		for(int i=0; i < 16; i++) {
			OMEEPROM::write(GSMNUMBER_ADDR + i, gsmNumber[i]);
		}
	}
	pos = text.indexOf(MESSAGE_CMD_GSMMODE);
	if(pos > -1) {
		gsmMode = text.charAt(pos + strlen(MESSAGE_CMD_GSMMODE)) - 48;
		OMEEPROM::write(GSMMODE_ADDR, gsmMode);
	}

	pos = text.indexOf(MESSAGE_CMD_LIGHT);
	if(pos > -1) {
		//Serial.println("LIGHT");
		ch = text.charAt(pos + strlen(MESSAGE_CMD_HEATER));
		if(ch=='A') lightMode = 0;
		else if(ch=='0') lightMode = 1;
		else if(ch=='1') lightMode = 2;
		//OMEEPROM::write(LIGHTMODE_ADDR, lightMode);
	}
	pos = text.indexOf(MESSAGE_CMD_HEATER);
	if(pos > -1) {
		ch = text.charAt(pos + strlen(MESSAGE_CMD_HEATER));
		if(ch=='A') heaterMode = 0;
		else if(ch=='0') heaterMode = 1;
		else if(ch=='1') heaterMode = 2;
		//OMEEPROM::write(HEATERMODE_ADDR, heaterMode);
	}
	pos = text.indexOf(MESSAGE_CMD_FAN);
	if(pos > -1) {
		ch = text.charAt(pos + strlen(MESSAGE_CMD_FAN));
		if(ch=='A') fanMode = 0;
		else if(ch=='0') fanMode = 1;
		else if(ch=='1') fanMode = 2;
		//OMEEPROM::write(FANMODE_ADDR, fanMode);
	}
	pos = text.indexOf(MESSAGE_CMD_CYCLER);
	if(pos > -1) {
		ch = text.charAt(pos + strlen(MESSAGE_CMD_CYCLER));
		if(ch=='A') cyclerMode = 0;
		else if(ch=='0') cyclerMode = 1;
		else if(ch=='1') cyclerMode = 2;
		//OMEEPROM::write(CYCLERMODE_ADDR, cyclerMode);
	}
	pos = text.indexOf(MESSAGE_CMD_REQUEST);
	if(pos > -1) {

	}
}
*/

bool getInstrumentControl(bool a, byte mode) {
	if(mode == 0) return a;
	if(mode == 1) return false;
	if(mode == 2) return true;
	return false;
}

double calcPH(double ph) {
  double phratio = (double)(phLowX - phHighX) / (double)(phHighY - phLowY); //move to setup
  //ph = getRawValue(PH_INPUTPIN, PH_SAMPLES)
  ph = ((double)phHighX - ph) / phratio + (double)phHighY;    // Calculate PH
  return ph;
}

double calcPHTemp(double temp) {
  //temp = getRawValue(PHTEMP_INPUTPIN, PH_SAMPLES)
  return temp / 3.4  * (5 / 10.24);   // LM35 connect to CA3140 for amplify 3 time
}

double analogRead(int pin, int samples){
  int result = 0;
  for(int i=0; i<samples; i++){
    result += analogRead(pin);
  }
  return (double)(result / samples);
}

double calcEC(long lowPulseTime, long highPulseTime) {
  double ec_a, ec_b, ec, pulseTime;

  ec_a =  (ecHighY - ecLowY) / (1/ (double) ecHighX - 1/ (double)ecLowX);
  ec_b = ecLowY - ec_a / (float) ecLowX;
  pulseTime = (double)((lowPulseTime + highPulseTime) / 2 + 2);

  ec = (ec_a / pulseTime + ec_b);
  return ec;
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
//unsigned long millisecondsGsm;
unsigned long lightOnDuration, lightOffDuration; //day / night
bool lightControlLast, heaterControlLast, fanControlLast, cyclerControlLast;
byte secondsCounter;
//bool newSecond;

void loop() {

	wdt_reset();

	//gsmMode = 0;

	now = rtc.now();
	milliseconds = millis();
	// second counter
	//if(now.secondstime() > secondstime) {
	//newSecond = false;
	if(millisecondsPrev + 1000 < milliseconds || milliseconds < millisecondsPrev) {
		//newSecond = true;
		secondsCounter++;



		//TODO:
		//once per 10 seconds, offset 0
		if (secondsCounter % 10 == 0) {
			oneWireSensors.requestTemperatures(); // Send the command to get temperatures
			temperature2 = oneWireSensors.getTempCByIndex(0);
			//temperature2 = getTemp(oneWire);
			//temperature2 = getTemp();
		}
		/*
		//once per 10 seconds, offset 1
		else if(secondsCounter % 10 == 1) {
			//level =
			digitalWrite(SR04TX_PIN, LOW);
			delayMicroseconds(2);
			digitalWrite(SR04TX_PIN, HIGH);
			delayMicroseconds(10);
			digitalWrite(SR04TX_PIN, LOW);
			long duration = pulseIn(SR04RX_PIN, HIGH);
			//Calculate the distance (in cm) based on the speed of sound.
			float distance = duration / 58.2;
			if (distance >= 200 || distance <= 0){
				//out of range
				distance = NAN;
			}
			level = -distance;
		}*/
		else {
			float t0, h0;
			t0 = dht.readTemperature();
			h0 = dht.readHumidity();
			if(!isnan(t0)) temperature = t0;
			if(!isnan(h0)) humidity = h0;
			light = analogRead(LIGHT_PIN, SAMPLES) / 10.23;
			extana = analogRead(EXTANA_PIN, SAMPLES) / 10.23;
			powerana = analogRead(POWERANA_PIN, SAMPLES) / 10.23;
			ph = calcPH(analogRead(PHANA_PIN, SAMPLES));
			ec = calcEC(pulseIn(ECINPUT_PIN, LOW), pulseIn(ECINPUT_PIN, HIGH));

			//level =
			digitalWrite(SR04TX_PIN, LOW);
			delayMicroseconds(2);
			digitalWrite(SR04TX_PIN, HIGH);
			delayMicroseconds(10);
			digitalWrite(SR04TX_PIN, LOW);
			long duration = pulseIn(SR04RX_PIN, HIGH);
			//Calculate the distance (in cm) based on the speed of sound.
			float distance = duration / 58.2;
			if (distance >= 200 || distance <= 0){
				//out of range
				distance = NAN;
			}
			level = -distance;
		}

		if(lightAuto) {
			lightOnDuration++;
			lightOffDuration = 0;
		}
		else {
			lightOffDuration++;
			lightOnDuration = 0;
		}

		//TODO:
		//once per 5 seconds, offset 8
		bool rcUpdate = (secondsCounter % 5 == 8);

		if(lightMode != 3 ) {
			if(rcUpdate || lightControlLast != lightControl) {
				lightControl ? sendSignal(RFTX_PIN, rf1on) : sendSignal(RFTX_PIN, rf1off);
				lightControlLast = lightControl;
				delay(50);
			}
		}
		if(heaterMode != 3) {
			if(rcUpdate || heaterControlLast != heaterControl) {
				heaterControl ? sendSignal(RFTX_PIN, rf2on) : sendSignal(RFTX_PIN, rf2off);
				heaterControlLast = heaterControl;
				delay(50);
			}
		}
		if(fanMode != 3) {
			if(rcUpdate || fanControlLast != fanControl) {
				fanControl ? sendSignal(RFTX_PIN, rf3on) : sendSignal(RFTX_PIN, rf3off);
				fanControlLast = fanControl;
				delay(50);
			}
		}
		if(cyclerMode != 3) {
			if(rcUpdate || cyclerControlLast != cyclerControl) {
				cyclerControl ? sendSignal(RFTX_PIN, rf4on) : sendSignal(RFTX_PIN, rf4off);
				cyclerControlLast = cyclerControl;
				delay(50);
			}
		}

		//cyclerDuration += now.secondstime() - secondstime ;
		//TOD add real time diference
		cyclerDuration++;

		secToggle ? secToggle = false : secToggle = true;
		//if (!Menu.shown()) {
		//	if(!uiState)
		//		uiMain();
		//}

		//secondstime = now.secondstime();
		millisecondsPrev = millis();
		//Serial.println(secondstime);
		//Serial.println(milliseconds);
		//Serial.print('.');

		// how often check sms, time expensive
		//if(!(secondstime % 60) && gsmMode > 0) {
		/*
		if((millisecondsGsm + GSMCHECKSMSINTERVAL < milliseconds) && gsmMode > 0) {
			if(!gsmMgr.millisecondsCall) {
				//Serial.println("SMS CHECK");
				millisecondsGsm = milliseconds;
				gsmMgr.proceedSMS();//&lightMode, &heaterMode, &fanMode, &cyclerMode);
			}
		}
		*/
		//once per 60 seconds, offset 9
		if((secondsCounter % 60 == 9) && gsmMode > 0) {
			if(!gsmMgr.millisecondsCall) {
				//Serial.println("SMS CHECK");
				gsmMgr.proceedSMS();//&lightMode, &heaterMode, &fanMode, &cyclerMode);
			}
		}

		//TODO:
		//if(!Menu.enable()) {
		//	uiScreen();
		//}
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

		//TODO:
		uiScreen();
	}

	Menu.checkInput();

	//////////////////////////////////
	// gsm
	//////////////////////////////////
	//gsmMgr.update();
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
/*
	t0 = dht.readTemperature();
	h0 = dht.readHumidity();
	if(!isnan(t0)) temperature = t0;
	if(!isnan(h0)) humidity = h0;

	light = analogRead(LIGHT_PIN, SAMPLES) / 10.23;
	extana = analogRead(EXTANA_PIN, SAMPLES) / 10.23;
	powerana = analogRead(POWERANA_PIN, SAMPLES) / 10.23;
	ph = calcPH(analogRead(PHANA_PIN, SAMPLES));
	//ph = analogRead(PHANA_PIN, SAMPLES);
	ec = calcEC(pulseIn(ECINPUT_PIN, LOW), pulseIn(ECINPUT_PIN, HIGH));

	//oneWireSensors.requestTemperatures(); // Send the command to get temperatures
	//temperature2 = oneWireSensors.getTempCByIndex(0);
	//temperature2 = getTemp();
*/




	//////////////////////////////////
	// outputs
	//////////////////////////////////
  	unsigned int l = now.hour() * 60 + now.minute();
  	unsigned int lOn = lightOnHour * 60 + lightOnMin;
  	unsigned int lOff =lightOffHour * 60 + lightOffMin;

    if(lOn < lOff) {
    	lightAuto = ((lOn <= l) && (l < lOff)) ? true : false;
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
	float tempHigh, tempLow;

	if(lightAuto) {
		//day - light
		tempHigh = tempHighAlarm;
		tempLow = tempLowAlarm;
	}
	else {
		//night - dark
		tempHigh = tempHighNightAlarm;
		tempLow = tempLowNightAlarm;
	}
	if(lightAuto && (lightOnDuration < lightOnOffDelay)) {
		//change from night to day
		tempLow = tempLowNightAlarm;
	}
	if(!lightAuto && (lightOffDuration < lightOnOffDelay)) {
		//change from night to day
		tempHigh = tempHighAlarm;
	}

	if(tempHighAlarm2.activate(temperature > tempHigh))
		saveMessage(MESSAGE_ALARM_TEMPHIGH, MESSAGE_ALARM_ON);
	if(tempHighAlarm2.deactivate(temperature < (tempHigh - tempHysteresis)))
		saveMessage(MESSAGE_ALARM_TEMPHIGH, MESSAGE_ALARM_OFF);

	if(tempLowAlarm2.activate(temperature < tempLow))
		saveMessage(MESSAGE_ALARM_TEMPLOW, MESSAGE_ALARM_ON);
	if(tempLowAlarm2.deactivate(temperature > (tempLow + tempHysteresis)))
		saveMessage(MESSAGE_ALARM_TEMPLOW, MESSAGE_ALARM_OFF);

	if(lightLowAlarm2.activate(lightControl && (light < lightLowAlarm)))
		saveMessage(MESSAGE_ALARM_LIGHTLOW, MESSAGE_ALARM_ON);
	if(lightLowAlarm2.deactivate(!lightControl || (light > lightLowAlarm + lightHysteresis)))
		saveMessage(MESSAGE_ALARM_LIGHTLOW, MESSAGE_ALARM_OFF);

	if(lightHighAlarm2.activate(!lightControl && (light > lightHighAlarm)))
		saveMessage(MESSAGE_ALARM_LIGHTHIGH, MESSAGE_ALARM_ON);
	if(lightHighAlarm2.deactivate(lightControl || (light < lightHighAlarm - lightHysteresis)))
		saveMessage(MESSAGE_ALARM_LIGHTHIGH, MESSAGE_ALARM_OFF);

	byte externalAlarm = extana > 50; //digitalRead(EXT_PIN);
	if(externalModeAlarm == 0) {
		// alarm in 0
		externalAlarm = !externalAlarm;
	}


	//else if (externalModeAlarm == 1) {
		// alarm in 1
		//externalAlarm = externalAlarm;
	//}
	//else {
		// no alarm
	if(externalModeAlarm == 2)
		externalAlarm = 0;
	//}

	if(externalAlarm2.activate(externalAlarm))
		saveMessage(MESSAGE_ALARM_EXTERNAL, MESSAGE_ALARM_ON);
	if(externalAlarm2.deactivate(!externalAlarm))
		saveMessage(MESSAGE_ALARM_EXTERNAL, MESSAGE_ALARM_OFF);

	byte powerAlarm = powerana < 50; //digitalRead(EXT_PIN);
	if(powerAlarm2.activate(powerAlarm))
		saveMessage(MESSAGE_ALARM_POWER, MESSAGE_ALARM_ON);
	if(powerAlarm2.deactivate(!powerAlarm))
		saveMessage(MESSAGE_ALARM_POWER, MESSAGE_ALARM_OFF);

	if(phHighAlarm2.activate(ph > phHighPhAlarm))
		saveMessage(MESSAGE_ALARM_PHHIGH, MESSAGE_ALARM_ON);
	if(phHighAlarm2.deactivate(ph < (phHighPhAlarm - phHysteresis)))
		saveMessage(MESSAGE_ALARM_PHHIGH, MESSAGE_ALARM_OFF);

	if(phLowAlarm2.activate(ph < phLowAlarm))
		saveMessage(MESSAGE_ALARM_PHLOW, MESSAGE_ALARM_ON);
	if(phLowAlarm2.deactivate(ph > (phLowAlarm + phHysteresis)))
		saveMessage(MESSAGE_ALARM_PHLOW, MESSAGE_ALARM_OFF);

	if(ecHighAlarm2.activate(ec > ecHighAlarm))
		saveMessage(MESSAGE_ALARM_ECHIGH, MESSAGE_ALARM_ON);
	if(ecHighAlarm2.deactivate(ec < (ecHighAlarm - ecHysteresis)))
		saveMessage(MESSAGE_ALARM_ECHIGH, MESSAGE_ALARM_OFF);

	if(ecLowAlarm2.activate(ec < ecLowAlarm))
		saveMessage(MESSAGE_ALARM_ECLOW, MESSAGE_ALARM_ON);
	if(ecLowAlarm2.deactivate(ec > (ecLowAlarm + ecHysteresis)))
		saveMessage(MESSAGE_ALARM_ECLOW, MESSAGE_ALARM_OFF);

	if(humiHighAlarm2.activate(humidity > humiHighAlarm))
		saveMessage(MESSAGE_ALARM_HUMIHIGH, MESSAGE_ALARM_ON);
	if(humiHighAlarm2.deactivate(humidity < (humiHighAlarm - humiHysteresis)))
		saveMessage(MESSAGE_ALARM_HUMIHIGH, MESSAGE_ALARM_OFF);

	if(humiLowAlarm2.activate(humidity < humiLowAlarm))
		saveMessage(MESSAGE_ALARM_HUMILOW, MESSAGE_ALARM_ON);
	if(humiLowAlarm2.deactivate(humidity > (humiLowAlarm + humiHysteresis)))
		saveMessage(MESSAGE_ALARM_HUMILOW, MESSAGE_ALARM_OFF);

	if(levelHighAlarm2.activate(level > levelHighAlarm))
		saveMessage(MESSAGE_ALARM_HUMIHIGH, MESSAGE_ALARM_ON);
	if(levelHighAlarm2.deactivate(level < (levelHighAlarm - levelHysteresis)))
		saveMessage(MESSAGE_ALARM_HUMIHIGH, MESSAGE_ALARM_OFF);

	if(levelLowAlarm2.activate(level < levelLowAlarm))
		saveMessage(MESSAGE_ALARM_HUMILOW, MESSAGE_ALARM_ON);
	if(levelLowAlarm2.deactivate(level > (levelLowAlarm + levelHysteresis)))
		saveMessage(MESSAGE_ALARM_HUMILOW, MESSAGE_ALARM_OFF);

	if(kpd.getRawKey()) {
		tempHighAlarm2.ack();
		tempLowAlarm2.ack();
		lightHighAlarm2.ack();
		lightLowAlarm2.ack();
		externalAlarm2.ack();
		phHighAlarm2.ack();
		phLowAlarm2.ack();
		ecHighAlarm2.ack();
		ecLowAlarm2.ack();
		humiHighAlarm2.ack();
		humiLowAlarm2.ack();
		levelHighAlarm2.ack();
		levelLowAlarm2.ack();
		powerAlarm2.ack();
	}

	//TEST
	//digitalWrite(LEDPIN, cyclerControl);

	/*
	digitalWrite(LIGHTCONTROL_PIN, !lightControl);
	digitalWrite(HEATERCONTROL_PIN, !heaterControl);
	digitalWrite(FANCONTROL_PIN, !fanControl);
	digitalWrite(CYCLERCONTROL_PIN, !cyclerControl);
	*/

	//////////////////////////////////
	// communication
	//////////////////////////////////
  	if (Serial.available() > 0) {
  		// read the incoming byte:
  		//incomingByte = Serial.read();

  		String text = Serial.readString();
  		//parseCmd(text, "#L", lightMode);
  		parseCmd(text, MESSAGE_CMD_LIGHT, lightMode);
  		parseCmd(text, MESSAGE_CMD_HEATER, heaterMode);
  		parseCmd(text, MESSAGE_CMD_FAN, fanMode);
  		parseCmd(text, MESSAGE_CMD_CYCLER, cyclerMode);

  		int pos = text.indexOf(MESSAGE_CMD_PARREADINT);
  		if (pos >= 0) {
  			serialPrintParInt(text.substring(pos + strlen(MESSAGE_CMD_PARREADINT)).toInt());
  		}
  		pos = text.indexOf(MESSAGE_CMD_PARREADFLOAT);
  		if (pos >= 0) {
  			serialPrintParFloat(text.substring(pos + strlen(MESSAGE_CMD_PARREADFLOAT)).toFloat());
  		}
  		pos = text.indexOf(MESSAGE_CMD_PARWRITEINT);
  		if (pos >= 0) {
  			int address = text.substring(pos + strlen(MESSAGE_CMD_PARWRITEINT)).toInt();
  			//#PWI0125:25
  			int value = text.substring(pos + strlen(MESSAGE_CMD_PARWRITEINT) + 5).toInt();
  			OMEEPROM::write(address, value);
  		}
  		pos = text.indexOf(MESSAGE_CMD_PARWRITEFLOAT);
  		if (pos >= 0) {
  			int address = text.substring(pos + strlen(MESSAGE_CMD_PARWRITEINT)).toInt();
  			//#PWI0125:25
  			float value = text.substring(pos + strlen(MESSAGE_CMD_PARWRITEINT) + 5).toFloat();
  			OMEEPROM::write(address, value);
  		}
  		pos = text.indexOf(MESSAGE_CMD_PARRELOAD);
  		if (pos >= 0) {
  			loadEEPROM();
  		}

		if (text.indexOf(MESSAGE_CMD_REQUEST)!=-1 ) {
  		//if (text.indexOf("?")!=-1 ) {
  		//if (Serial.readString().indexOf("?")!=-1 ) {
			Serial.println();


			lightControl = getInstrumentControl(lightAuto, lightMode);
			heaterControl = getInstrumentControl(heaterAuto, heaterMode);
			fanControl = getInstrumentControl(fanAuto, fanMode);
			cyclerControl = getInstrumentControl(cyclerAuto, cyclerMode);

			Serial.print(MESSAGE_LIGHT_CONTROL);
			lightControl ? Serial.print('1') : Serial.print('0');
			lightMode ? Serial.println('M') : Serial.println('A');
			Serial.print(MESSAGE_HEATER_CONTROL);
			heaterControl ? Serial.print('1') : Serial.print('0');
			heaterMode ? Serial.println('M') : Serial.println('A');
			Serial.print(MESSAGE_FAN_CONTROL);
			fanControl ? Serial.print('1') : Serial.print('0');
			fanMode ? Serial.println('M') : Serial.println('A');
			Serial.print(MESSAGE_CYCLER_CONTROL);
			cyclerControl ? Serial.print('1') : Serial.print('0');
			cyclerMode ? Serial.println('M') : Serial.println('A');
			//Serial.println();
			Serial.println();

  			Serial.print(MESSAGE_TEMP);
  			Serial.println(temperature);
  			Serial.print(MESSAGE_HUMI);
  			Serial.println(humidity);
  			Serial.print(MESSAGE_LIGHT);
  			Serial.println(light);
  			Serial.print(MESSAGE_EXT);
  			//Serial.print(digitalRead(EXT_PIN));
  			Serial.println(extana);
  			Serial.print(MESSAGE_POWER);
  			Serial.println(powerana);

  			Serial.print(MESSAGE_PH);
  			Serial.println(ph);
  			Serial.print(MESSAGE_EC);
  			Serial.println(ec);
  			Serial.print(MESSAGE_TEMP2);
  			Serial.println(temperature2);
  			Serial.print(MESSAGE_LEVEL);
  			Serial.println(level);
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
  			Serial.println();
  			//Serial.println();
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

	delay(1000);

	//lcd.setCursor(1, 1);
	//lcd.print(gsmNumber);

	//sim800l.callNumber(gsmNumber);


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
	if(mode == 0)
		lcd.print('A');
	else if(mode < 3)
		lcd.print('M');
	else
		lcd.print('X');
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
		if(key == KPD_UP)
			uiPage--;
		if(key == KPD_DOWN)
			uiPage++;
		uiPage = max(0, uiPage);
		uiPage = min(5, uiPage);

		//TODO:
		//lcd.clear();

		if(uiPage == 0) {
			lcd.setCursor(0, 0);
			uiLcdPrintAlarm(tempHighAlarm2.active, tempLowAlarm2.active);
			lcd.print(F("TEMP[C]:"));
			lcd.print(temperature);
			uiLcdPrintSpaces8();
			lcd.setCursor(0, 1);
			uiLcdPrintAlarm(humiHighAlarm2.active, humiLowAlarm2.active);
			lcd.print(F("HUMI[%]:"));
			lcd.print(humidity);
			uiLcdPrintSpaces8();
		}
		else if(uiPage == 1) {
			lcd.setCursor(0, 0);
			uiLcdPrintAlarm(phHighAlarm2.active, phLowAlarm2.active);
			lcd.print(F("PH [PH]:"));
			lcd.print(ph);
			uiLcdPrintSpaces8();
			lcd.setCursor(0, 1);
			uiLcdPrintAlarm(ecHighAlarm2.active, ecLowAlarm2.active);
			lcd.print(F("EC[S/M]:"));
			lcd.print(ec);
			uiLcdPrintSpaces8();
		}
		else if(uiPage == 2) {
			lcd.setCursor(0, 0);
			uiLcdPrintAlarm(lightHighAlarm2.active, lightLowAlarm2.active);
			lcd.print(F("LIGHT[-]:"));
			lcd.print(int(light));
			uiLcdPrintSpaces8();
			lcd.setCursor(0, 1);
			uiLcdPrintAlarm(false, externalAlarm2.active);
			lcd.print(F("EXT  [%]:"));
			lcd.print(int(extana));
			uiLcdPrintSpaces8();
		}
		else if(uiPage == 3) {
			lcd.setCursor(0, 0);
			lcd.print(F(" PH X[-]:"));
			//lcd.setCursor(8, 0);
			//TODO: use global from loop
			lcd.print(analogRead(PHANA_PIN, SAMPLES));
			uiLcdPrintSpaces8();
			lcd.setCursor(0, 1);
			lcd.print(F(" EC X[-]:"));
			//TODO:
			double pulseTime = (double)((pulseIn(ECINPUT_PIN, LOW) + pulseIn(ECINPUT_PIN, HIGH)) / 2 + 2);
			lcd.print(pulseTime);
			uiLcdPrintSpaces8();

		}
		else if(uiPage == 4) {
			lcd.setCursor(0, 0);
			uiLcdPrintAlarm(false, false);
			lcd.print(F("TEMP2[C]:"));
			lcd.print(temperature2);
			uiLcdPrintSpaces8();
			lcd.setCursor(0, 1);
			uiLcdPrintAlarm(false, powerAlarm2.active);
			lcd.print(F("POWER[%]:"));
			lcd.print(int(powerana));
			uiLcdPrintSpaces8();
		}
		else if(uiPage == 5) {
			lcd.setCursor(0, 0);
			if(lightAuto) {
				lcd.print(F(" DAY *  "));
				uiLcdPrintSpaces8();
			}
			else {
				lcd.print(F(" NIGHT  "));
				uiLcdPrintSpaces8();
			}
			lcd.setCursor(0, 1);
			uiLcdPrintAlarm(levelHighAlarm2.active, levelLowAlarm2.active);
			lcd.print(F("LEVEL[CM]:"));
			lcd.print(level);
			uiLcdPrintSpaces8();
		}
		//delay(200);
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

/*
void uiInfo() {
	//"123456789ABCDEF"
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(F("BCSEDLON@"));
	lcd.setCursor(0, 1);
	lcd.print(F("GMAIL.COM"));
	Menu.enable(false);
}
*/

void uiLcdPrintAlarm(bool alarmHigh, bool alarmLow) {
	if(alarmHigh)
		lcd.print('+');
	else if(alarmLow)
		lcd.print('-');
	else
		lcd.print(' ');
}

void uiLcdPrintSpaces8() {
	lcd.print(F("        "));
}

void uiMain() {

	if(tempHighAlarm2.unAck || tempLowAlarm2.unAck || lightHighAlarm2.unAck || lightLowAlarm2.unAck || externalAlarm2.unAck
			|| phHighAlarm2.unAck || phLowAlarm2.unAck || ecHighAlarm2.unAck || ecLowAlarm2.unAck || humiHighAlarm2.unAck || humiLowAlarm2.unAck
			|| powerAlarm2.unAck || levelHighAlarm2.unAck || levelLowAlarm2.unAck) {
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
	else if(light > lightLowAlarm)
		lcd.print('*');
	else
		lcd.print(' ');

	lcd.print("L");
	uiInstrument(lightControl, lightMode);
	lcd.print(" H");
	uiInstrument(heaterControl, heaterMode);
	lcd.print(" F");
	uiInstrument(fanControl, fanMode);
	lcd.print(" C");
	uiInstrument(cyclerControl, cyclerMode);


//}
	/*
	else if(uiPage == 1) {
		lcd.clear();
		lcd.setCursor(0, 0);
		uiLcdPrintAlarm(lightHighAlarm2.active, lightLowAlarm2.active);
		lcd.print(F("LIGHT[-]:"));
		lcd.print(int(light));


		lcd.setCursor(0, 1);
		uiLcdPrintAlarm(false, externalAlarm2.active);
		lcd.print(F("POWER[%]:"));
		lcd.print(int(extana));
	}
	else if(uiPage == 2) {
			lcd.clear();
			lcd.setCursor(0, 0);
			uiLcdPrintAlarm(tempHighAlarm2.active, tempLowAlarm2.active);
			lcd.print(F("TEMP[C]:"));
			lcd.print(temperature);

			lcd.setCursor(0, 1);
			uiLcdPrintAlarm(false, false);
			lcd.print(F("HUMI[%]:"));
			lcd.print(humidity);

	}
	else if(uiPage == 3) {
		lcd.clear();
		lcd.setCursor(0, 0);
		uiLcdPrintAlarm(2.active, phLowAlarm2.active);
		lcd.print(F("PH[PH]:"));
		lcd.print(ph);

		lcd.setCursor(0, 1);
		uiLcdPrintAlarm(ecHighAlarm2.active, ecLowAlarm2.active);
		lcd.print(F("EC[S/M]:"));
		lcd.print(ec);
	}


	//TODO: ???
	char key = kpd.getKey2();
	if(key == KPD_UP)
		uiPage--;
	if(key == KPD_DOWN)
		uiPage++;
	uiPage = max(0, uiPage);
	uiPage = min(3, uiPage);*/
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

float getTemp(OneWire ds){
	//returns the temperature from one DS18S20 in DEG Celsius
	byte data[12];
	byte addr[8];

	if ( !ds.search(addr)) {
		//no more sensors on chain, reset search
		ds.reset_search();
		return -1000;
	}

	if ( OneWire::crc8( addr, 7) != addr[7]) {
		Serial.println("CRC is not valid!");
		return -1000;
	}

	if ( addr[0] != 0x10 && addr[0] != 0x28) {
		Serial.print("Device is not recognized");
		return -1000;
	}

	ds.reset();
	ds.select(addr);
	ds.write(0x44,1); // start conversion, with parasite power on at the end

	delay(750); // Wait for temperature conversion to complete

	byte present = ds.reset();
	ds.select(addr);
	ds.write(0xBE); // Read Scratchpad


	for (int i = 0; i < 9; i++) { // we need 9 bytes
		data[i] = ds.read();
	}

	ds.reset_search();

	byte MSB = data[1];
	byte LSB = data[0];

	float tempRead = ((MSB << 8) | LSB); //using two's compliment
	float TemperatureSum = tempRead / 16;

	return TemperatureSum;
}
