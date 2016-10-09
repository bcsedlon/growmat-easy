
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


int uiState, uiPage;

class Alarm {
  bool active;
  bool unAck;
  unsigned long timeActive;
  unsigned long timeDeactive;

 public:
  // TODO: fix when millis overrun!!!
  int delay = 5000;
  bool activate(bool state) {
    if(state) {
      if(!active) {
        if(!timeActive) {
          timeActive = millis();
        }
        if((timeActive + delay) < millis()) {
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
        if((timeDeactive + delay) < millis()) {
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
  bool isUnAck(){
    return unAck;
  }
  bool isActive(){
    return active;
  }
};

Alarm tempHighAlarm2, tempLowAlarm2, lightHighAlarm2, lightLowAlarm2;


#include <Wire.h>

// SIM800L
#include "libraries/Sim800l/Sim800l.h"
#include <SoftwareSerial.h> //is necessary for the library!!
Sim800l sim800l;  //to declare the library

char gsmNumber[16];
int gsmCode = 9999;

class GsmManager {
public:
	Sim800l* sim;
	uint8_t callStatus;
	//00420123456789

	char* gsmNumber;
	//String code;
	int* gsmCode;
	//String response = String("");

	unsigned long callDuration;

	GsmManager(Sim800l* sim_c, int* gsmCode_c, char* gsmNumber_c) {
		sim = sim_c;
		gsmCode = gsmCode_c;
		gsmNumber = gsmNumber_c;
		//???OMEEPROM::read(0, (uint8_t &)&number, 16 );
		//int c;
		//OMEEPROM::read(GSMCODE_ADDR, c);
		//code = String(c, DEC);
	}

	bool update() {
		//sim->readSerial();
		return false;
	}

	void call(){
		if(!callDuration) {
			Serial.println("CALLING ...");
			sim->callNumber(gsmNumber);
			callDuration= millis();
		}

	}
	void updateCall() {
		//return;
		//callStatus = sim->getCallStatus();
		// TODO: solve millis overflow
		if(callDuration && (callDuration + 30000 < millis())){
			Serial.println("HANG OFF");
			sim->hangoffCall();
			callDuration = 0;
		}
	}

	bool sendSMS(char* text) {
		// TODO: check if sim is busy
		return sim->sendSms(gsmNumber, text);
	}



	bool proceedSMS(byte  *lightMode) {

		if(callDuration)
			return false;
		//Serial.println("start proceedSMS");

		/*
		Serial.println("1");
		Serial.println(sim->response1);
		sim->writeSerial("AT+CMGR=21\r\n");
		Serial.println("2");
		Serial.println(sim->response1);
		*/



		String text = sim->readSms(21);
		//String text = sim->response1;

		//Serial.println(text);

		//Serial.println( text.indexOf("\n"));
		for(int i = 0; i < 2; i++)
			text = text.substring(text.indexOf("\n")+1);
		text = text.substring(0, text.indexOf("\n")-1);
		//Serial.println(text);


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
		int pos;
		char ch;
		if(text != "") {
			if(text.substring(0, 4) == String(*gsmCode, DEC)) {
				Serial.println("PASSWORD OK");
				pos = text.indexOf("@00");
				if(pos > -1) {
					text.substring(pos+1).toCharArray(gsmNumber, 16, 0);
					//OMEEPROM::write(GSMNUMBER_ADDR, text, text.length);
					//OMEEPROM::write(GSMNUMBER_ADDR, text, (unsigned char)16);
					//OMEEPROM::write(GSMNUMBER_ADDR, number);
					 for(int i=0; i < 16; i++) {
					      OMEEPROM::write(GSMNUMBER_ADDR + i, gsmNumber[i]);
					  }
				}

				pos = text.indexOf("@L");
				if(pos > -1) {
					Serial.println("LIGHT");
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
			}
			sim->delAllSms();
		}
		//Serial.println("finish proceedSMS");
		return false;
	};

} gsmMgr(&sim800l, &gsmCode, gsmNumber);

// RTC
#include "libraries/RTClib/RTClib.h"
RTC_DS3231 rtc;

// DHT
#include "libraries/DHT-sensor-library-master/DHT.h"
DHT dht(DHTPIN, DHTTYPE);

// Variables
float temperature, humidity, heatIndex;
float light;

bool lightControl, heaterControl, ventControl, cyclerControl;
bool lightAuto, heaterAuto, ventAuto, cyclerAuto;
//byte alarm, tempHighAlarm, tempLowAlarm, lightHighAlarm, lightLowAlarm;

unsigned long uiTime = 0; // ui counter
//int uiState = 0;

unsigned long cyclerDuration = 0; // cycler counter
unsigned long cTime = 0;

// Parameters
byte lightMode, heaterMode, ventMode, cyclerMode;
unsigned int lightOnHour, lightOnMin, lightOffHour, lightOffMin;
float heaterOnTemp, heaterOffTemp;
float ventOnTemp, ventOffTemp;
unsigned int cyclerOnMin, cyclerOnSec, cyclerOffMin, cyclerOffSec;
float tempHighTempAlarm, tempLowTempAlarm, lightValueAlarm;

float tempHysteresis = 0.5;
float lightHysteresis = 10;

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



      // !!! Dirty trick !!!
      if(bitMap[3] == 8) {
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
        if((kTime + 500) > millis()){
          if((kTime + 250) < millis()) {
            //Serial.println("PAUSE");
            return NO_KEY;

        }
       }
        /*
        if(((kTime + 125) > millis()) && ((kTime + 500) < millis())){
          return NO_KEY;
        }
        */
      }
      else
        kTime = 0;

      if(bitMap[3] == 1) return '*';
      if(bitMap[0] == 8) return 'A';
      if(bitMap[0] == 4) return 'B';
      if(bitMap[0] == 1) return 'D';
      if(bitMap[1] == 1) return '#';

      if(bitMap[2] == 1) return '0';
      return NO_KEY;
    }
    // Returns a single key only. Retained for backwards compatibility.
    /*char Keypad_I2C2::getKey() {
      getKeys();
      for(int i = 0; i < 4; i++)
      Serial.println(bitMap[i]);
      return NO_KEY;
      //Serial.println(getKeys());
      //Serial.println(key[0].kstate);
      //Serial.println(key[0].kchar);
      single_key = true;
      //if (getKeys() && key[0].stateChanged && (key[0].kstate==PRESSED))
      if (getKeys()){
        Serial.println(key[0].kchar);
        return key[0].kchar;}
      single_key = false;
      Serial.println('x');
      return NO_KEY;
    };

    bool Keypad_I2C2::isPressed(char keyChar) {
      //Serial.println('x');
      //Serial.println(keyChar);
      for (byte i=0; i<LIST_MAX; i++) {
        Serial.println(key[i].kchar);
         if ( key[i].kchar == keyChar ) {
            Serial.println('XXXXXXXXXXXXXXXx');
            if  (key[i].kstate == PRESSED)
              return true;
         };
      };
      return false; // Not pressed.
    };
 */
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
/*
char k;
// Taking care of some special events.
void keypadEvent(KeypadEvent key){
    Serial.println(key);
    switch (kpd.getState()){
    case PRESSED:
        k = key;
        break;

    case RELEASED:
        k = 0;
        break;

    case HOLD:
        break;
    }
}
*/

// LCD i2c
#include "libraries/NewliquidCrystal/LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);







// Menu
#include "libraries/OMEEPROM/OMEEPROM.h"
#include "libraries/OMMenuMgr/OMMenuMgr.h"

//static unsigned long kTime;
static char k;
class OMMenuMgr2 : public OMMenuMgr {
  public:
    OMMenuMgr2(const OMMenuItem* c_first, uint8_t c_type, Keypad_I2C2* c_kpd) :OMMenuMgr( c_first, c_type) {
      kpd = c_kpd;
    };


    int OMMenuMgr2::_checkDigital() {
      /*
      if(kpd->isPressed('A')) return BUTTON_INCREASE;
      if(kpd->isPressed('B')) return BUTTON_DECREASE;
      if(kpd->isPressed('C')) return BUTTON_FORWARD;
      if(kpd->isPressed('D')) return BUTTON_BACK;
      if(kpd->isPressed('*')) return BUTTON_SELECT;
      return BUTTON_NONE;
      */
      /*
      char k0 = kpd->getKey();
      if(kTime + 125 < millis()) {
         k = 0;
      }
      if (k0){
        kTime = millis();
        //Serial.println(key);
        k = k0;
      }
  */
      //return kpd->getKey2();
      char k = kpd->getKey2();
      //char k = kpd->getState();
      //Serial.println(k);
      if(k == 'A') return BUTTON_INCREASE;
      if(k == 'B') return BUTTON_DECREASE;
      if(k == 'D') return BUTTON_FORWARD;
      if(k == '#') return BUTTON_BACK;
      if(k == '*') return BUTTON_SELECT;
      return BUTTON_NONE;
    }
    //void OMMenuMgr2::setDigitalButtonPins() {
    //};
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

//MENU_SELECT lightAlarm_select = { &lightAlarm,           MENU_SELECT_SIZE(state_listOffOn),   MENU_TARGET(&state_listOffOn) };
//MENU_VALUE lightAlarm_value =   { TYPE_SELECT,     1,     0,     MENU_TARGET(&lightAlarm_select) };
//MENU_ITEM lightAlarm_item    =  { {"LIGHT ALARM"}, ITEM_VALUE,  0,        MENU_TARGET(&lightAlarm_value) };

//                                "123456789ABCDEF"
MENU_VALUE lightValueAlarm_value={ TYPE_FLOAT_10, 102,    0,    MENU_TARGET(&lightValueAlarm), LIGHTVALUEALARM_ADDR };
MENU_ITEM lightValueAlarm_item   ={ {"LIGH ALARM  "},    ITEM_VALUE,  0,        MENU_TARGET(&lightValueAlarm_value) };

MENU_LIST const submenu_list5[] = { &tempHighTempAlarm_item, &tempLowTempAlarm_item, &lightValueAlarm_item};
MENU_ITEM menu_submenu5 = { {"ALARM SET->"},  ITEM_MENU,  MENU_SIZE(submenu_list5),  MENU_TARGET(&submenu_list5) };


/*
  // values to use
//////////////////////////////
byte foo = 0;
byte sel = 0;
unsigned int bar = 1;
long baz  = 0;
float bak = 0.0;


                             // TARGET VAR   LENGTH                          TARGET SELECT LIST
MENU_SELECT state_select = { &sel,           MENU_SELECT_SIZE(state_list),   MENU_TARGET(&state_list) };
                    //    TYPE            MAX    MIN    TARGET
MENU_VALUE foo_value = { TYPE_BYTE,       100,   0,     MENU_TARGET(&foo) };
MENU_VALUE bar_value = { TYPE_UINT,       10000, 100,   MENU_TARGET(&bar) };
MENU_VALUE baz_value = { TYPE_LONG,       10000, 1,     MENU_TARGET(&baz) };
MENU_VALUE bak_value = { TYPE_FLOAT_1000, 0,     0,     MENU_TARGET(&bak) };
MENU_VALUE sel_value = { TYPE_SELECT,     0,     0,     MENU_TARGET(&state_select) };

                                       //        LABEL           TYPE         LENGTH    TARGET
MENU_ITEM item_checkme  = { {"Byte Edit"},    ITEM_VALUE,  0,        MENU_TARGET(&foo_value) };
MENU_ITEM item_barme    = { {"UInt Edit"},    ITEM_VALUE,  0,        MENU_TARGET(&bar_value) };
MENU_ITEM item_bazme    = { {"Long Edit"},    ITEM_VALUE,  0,        MENU_TARGET(&baz_value) };
MENU_ITEM item_bakme    = { {"Float Edit"},   ITEM_VALUE,  0,        MENU_TARGET(&bak_value) };
MENU_ITEM item_state    = { {"Select Input"}, ITEM_VALUE,  0,        MENU_TARGET(&sel_value) };
//MENU_ITEM item_testme   = { {"Test Action"},  ITEM_ACTION, 0,        MENU_TARGET(&uiQwkScreen) };
 */

MENU_VALUE gsmCode_value= { TYPE_UINT, 9999, 1000,    MENU_TARGET(&gsmCode), GSMCODE_ADDR };
MENU_ITEM gsmCode_item   ={ {"GSM PASSWORD"},    ITEM_VALUE,  0,        MENU_TARGET(&gsmCode_value) };

MENU_ITEM item_testme   = { {"CALL!"},  ITEM_ACTION, 0,        MENU_TARGET(&callAction) };
MENU_ITEM item_info   = { {"INFO"},  ITEM_ACTION, 0,        MENU_TARGET(&uiInfo) };
MENU_ITEM item_alarmList   = { {"ALARM LIST->"},  ITEM_ACTION, 0,        MENU_TARGET(&uiAlarmList) };

                   //        List of items in menu level
//MENU_LIST const root_list[]   = { &item_checkme, &item_barme,  &item_state, &item_testme, &menu_submenu1, &menu_submenu2, &menu_submenu3, &menu_submenu4, &menu_submenu5 };//&item_bazme, &item_bakme,
MENU_LIST const root_list[]   = { &menu_submenu1 , &menu_submenu2, &menu_submenu3, &menu_submenu4, &menu_submenu5,&item_alarmList, &item_testme, &gsmCode_item, &item_info };//&item_bazme, &item_bakme,

                  // Root item is always created last, so we can add all other items to it
MENU_ITEM menu_root     = { {"Root"},        ITEM_MENU,   MENU_SIZE(root_list),    MENU_TARGET(&root_list) };

OMMenuMgr2 Menu(&menu_root, MENU_DIGITAL, &kpd);


int saveMessage(char msg[], char status) {
	if((gsmNumber[2] != '0') && status == MESSAGE_ALARM_ON) {
		//Serial.println(gsmNumber);
		gsmMgr.call();
	}
//return 0;
   //using namespace OMEEPROM;
/*
    for(int i=0; i < MESSAGELENGTH; i++) {
      write(MESSAGESOFFSET_ADDR + i, msg[i]);
    }
    return;
*/

    //char buffer[MESSAGELEGTH];
    DateTime now = rtc.now();
    //sprintf(msg, " %d2/%d2 %d2:%d2 ", now.month(), now.day(), now.hour(), now.minute());
    //Serial.println(msg);

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
  /*    *//*
    int p = 0;
    if(now.day() < 10)
      msg[p++] = '0';
    msg[p] = itoa(now.day());
    p=2;

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


    itoa(now.hour(),msg,10);
    msg[2]=':';
    itoa(now.minute(),msg+3,10);
    String(" POWER UP").toCharArray(msg + 5, MESSAGELENGTH - 5);
//         "0123456789ABCDEF"
 */

    int offset;
    OMEEPROM::read(MESSAGESOFFSET_ADDR, offset);
    if(offset >= MESSAGESCOUNT)
      offset = 0;
    //offset = 0;
    for(int i=0; i < MESSAGELENGTH; i++) {
      OMEEPROM::write(MESSAGES_ADDR + offset * MESSAGELENGTH + i, msg[i]);
    }
    offset = (offset + 1) %  MESSAGESCOUNT;
    OMEEPROM::write(MESSAGESOFFSET_ADDR, offset);

    //Serial.println("write");
    //Serial.println(msg);

    return offset;
}

void readMessage(int index, byte* msg) {
 //return;
//    using namespace OMEEPROM;
/*
    for(int i=0; i < MESSAGELENGTH; i++) {
      read(MESSAGESOFFSET_ADDR + i, *(msg + i));
    }
    return;
*/
    int offset;
    OMEEPROM::read(MESSAGESOFFSET_ADDR, offset);
    offset--;
    if(offset >= MESSAGESCOUNT)
      offset = 0;
    offset = (offset + (MESSAGESCOUNT -index )) % MESSAGESCOUNT;
    //offset = 0;
    for(int i=0; i < MESSAGELENGTH; i++) {
      OMEEPROM::read(MESSAGES_ADDR + offset * MESSAGELENGTH + i, *(msg+i));
    }
    *(msg+MESSAGELENGTH) = 0;
    //Serial.println("read");
    //Serial.println(*msg);
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

    read(GSMCODE_ADDR, gsmCode);

    for(int i=0; i < 16; i++) {
         OMEEPROM::read(GSMNUMBER_ADDR + i, *(gsmNumber+i));
    }
}

void saveEEPROM() {
    using namespace OMEEPROM;

    lightMode = 0;
    lightOnHour= 6;
    lightOnMin = 0;
    lightOffHour= 22;
    lightOffMin = 0;
    write(LIGHTMODE_ADDR, lightMode);
    write(LIGHTONHOUR_ADDR, lightOnHour);
    write(LIGHTONMIN_ADDR, lightOnMin);
    write(LIGHTOFFHOUR_ADDR, lightOffHour);
    write(LIGHTOFFMIN_ADDR, lightOffMin);

    heaterMode = 0;
    heaterOnTemp=20.0;
    heaterOffTemp=21.0;
    write(HEATERMODE_ADDR, heaterMode);
    write(HEATERONTEMP_ADDR, heaterOnTemp);
    write(HEATEROFFTEMP_ADDR, heaterOffTemp);

    ventMode = 0;
    ventOnTemp=22.0;
    ventOffTemp=23.0;
    write(VENTMODE_ADDR, ventMode);
    write(VENTONTEMP_ADDR, ventOnTemp);
    write(VENTOFFTEMP_ADDR, ventOffTemp);

    cyclerMode = 0;
    cyclerOnMin= 0;
    cyclerOnSec = 5;
    cyclerOffMin= 0;
    cyclerOffSec = 5;
    write(CYCLERMODE_ADDR, cyclerMode);
    write(CYCLERONMIN_ADDR, cyclerOnMin);
    write(CYCLERONSEC_ADDR, cyclerOnSec);
    write(CYCLEROFFMIN_ADDR, cyclerOffMin);
    write(CYCLEROFFSEC_ADDR, cyclerOffSec);

    tempHighTempAlarm = 21.0;
    tempLowTempAlarm = 19.0;
    lightValueAlarm = 70.0;
    write(TEMPHIGHTEMPALARM_ADDR, tempHighTempAlarm);
    write(TEMPLOWTEMPALARM_ADDR, tempLowTempAlarm);
    write(LIGHTVALUEALARM_ADDR, lightValueAlarm);
}

void setup() {


  pinMode(LEDPIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial);

  Serial.println("GROWMAT EASY");
  Serial.println(VERSION);





  if( OMEEPROM::saved() )
    loadEEPROM();
  //else
  //  saveEEPROM();


  sim800l.begin(); // initializate the library.

  //kpd.addEventListener(keypadEvent);
  Wire.begin( );

  rtc.begin();
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  char msg[MESSAGELENGTH + 1];
/*
  DateTime now = rtc.now();

  //String("        POWER UP").toCharArray(msg, MESSAGELENGTH);
  itoa(now.hour(),msg,10);
  msg[2]=':';
  itoa(now.minute(),msg+3,10);
  String(" POWER UP").toCharArray(msg + 5, MESSAGELENGTH - 5);
//       "0123456789ABCDEF"
  saveMessage(msg);
*/
  saveMessage(MESSAGE_ALARM_POWERON, MESSAGE_ALARM_ON);



  //char msg[MESSAGELENGTH + 1];
  for(int i = 0; i< MESSAGESCOUNT; i++) {
    readMessage(i, (byte*)msg);
    Serial.println(msg);
  }


  kpd.begin( makeKeymap(keys) );

  lcd.begin(LCD_COLS, LCD_ROWS);

  dht.begin();

  uiClear();

  Menu.setDrawHandler(uiDraw);
  Menu.setExitHandler(uiClear);

  Menu.enable(true);

}


bool getControl(bool a, byte mode) {
  if(mode == 0) return a;
  if(mode == 1) return false;
  if(mode == 2) return true;
  return false;
}


char pressed = 0;
unsigned long gsmTime = 0;
void loop() {

 char key = kpd.getKey2();
 if(key == '#') {
    Menu.enable(true);
    uiState = 0;
    uiPage = 0;
 }
 else if(key == '0') {
    uiAlarmList();
    //Menu.enable(false);
    //uiState = UISTATE_ALARMLIST;
    //uiPage = 0;
    //uiScreen();
 }
 else if(!Menu.enable()) {
  //Serial.println(key);
  //Serial.println(pressed);

   // if(kpd.getKey2()=='A')  {
  if((key == 'A') && !pressed){
        uiPage--;
        pressed = 'A';
  }

  //if(kpd.getKey2()=='B')  {
  //if(kpd.isPressed('A'))
  if((key == 'B') && !pressed) {
    uiPage++;
    pressed = 'B';
  }
  if(key == NO_KEY)
    pressed = 0;
  //if(uiState == UISTATE_ALARMS) {
   uiScreen();
  //}
 }

 if(millis() > (cTime + 1000)) {
    cTime = millis();
    cyclerDuration++;
    //Serial.println(cyclerDuration);
    //Serial.println(cyclerAuto);
 }

 gsmMgr.update();
 gsmMgr.updateCall();
 if(millis() > (gsmTime + 30000)) {
	gsmTime = millis();


    gsmMgr.proceedSMS(&lightMode);
 }


  if(cyclerAuto && (cyclerDuration >= (cyclerOnSec + cyclerOnMin * 60))) {
    cyclerDuration = 0;
    cyclerAuto = false;
  };
  if(!cyclerAuto && (cyclerDuration >= (cyclerOffSec + cyclerOffMin * 60))) {
    cyclerDuration = 0;
    cyclerAuto = true;
  };

  cyclerControl = getControl(cyclerAuto, cyclerMode);




  Menu.checkInput();

  float t0, h0;
  t0 = dht.readTemperature();
  h0 = dht.readHumidity();
  if(!isnan(t0)) {
    temperature = t0;
  }
  if(!isnan(h0)) {
    humidity = h0;
  }
  for(int i = 0; i < 100; i++)
    light += analogRead((unsigned char)LIGHTPIN) / 10;
  light /= 100;
  // ------------------------




  DateTime now = rtc.now();
  // ???
  /*
  if (now.hour() > lightOnHour && now.minute() > lightOnMin) {
    lightAuto = true;
  } else {
    lightAuto = false;
  }
  */
  unsigned int l = now.hour() * 60 + now.minute();
  unsigned int lOn = lightOnHour * 60 + lightOnMin;
  unsigned int lOff =lightOffHour * 60 + lightOffMin;

  //???
  if(lOn < lOff) {
    lightAuto = ((lOn < l) && (l < lOff)) ? true : false;
    //lightAuto = l > lOn;
    //lightAuto = l < lOff;
  } else {
    lightAuto = ((lOn > l) && (l  > lOff)) ? false : true;
    //lightAuto = l < lOn;
    //lightAuto = l > lOff;
  }

  lightControl = getControl(lightAuto, lightMode);

  if(temperature < heaterOnTemp)
    heaterAuto = true;
  if(temperature > heaterOffTemp)
    heaterAuto = false;
  heaterControl = getControl(heaterAuto, heaterMode);

   if(temperature > ventOnTemp)
    ventAuto = true;
  if(temperature < ventOffTemp)
    ventAuto = false;
  ventControl = getControl(ventAuto, ventMode);




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
  //Serial.println(lightHighAlarm2.isActive());
  //Serial.println(lightHighAlarm2.timeActive);
  //Serial.println(lightHighAlarm2.timeDeactive);

/*
  //-----------------------------

  if (temperature > tempHighTempAlarm) {
    if(!(tempHighAlarm & 1)) {
      saveMessage(MESSAGE_ALARM_TEMPHIGH, '1');
      tempHighAlarm |=  3;
    }
  }
  else {
    if (temperature < (tempHighTempAlarm - tempHysteresis)) {
      if(tempHighAlarm & 1) {
        saveMessage(MESSAGE_ALARM_TEMPHIGH, '0');
        tempHighAlarm &=  2;
      }
    }
  }

   if (temperature > tempLowTempAlarm) {
    if(!(tempLowAlarm & 1)) {
      saveMessage(MESSAGE_ALARM_TEMPLOW, '1');
      tempLowAlarm |=  3;
    }
  }
  else {
    if (temperature < tempHighTempAlarm + tempHysteresis) {
      if(tempLowAlarm & 1) {
        saveMessage(MESSAGE_ALARM_TEMPLOW, '0');
        tempLowAlarm &=  2;
      }
    }
  }


 if(lightControl) {
  if(light > lightValueAlarm) {
    if(!(lightLowAlarm & 1)) {
      lightLowAlarm |=  3;
      saveMessage(MESSAGE_ALARM_LIGHTLOW, '1');
    }
  }
  else {
    if(light < lightValueAlarm - lightHysteresis) {
      if(lightLowAlarm & 1) {
        lightLowAlarm &=  2;
        saveMessage(MESSAGE_ALARM_LIGHTLOW, '0');
      }
    }
  }
 }
 else if (lightLowAlarm & 1) {
  lightLowAlarm &=  2;
  saveMessage(MESSAGE_ALARM_LIGHTLOW, '0');
 }

 if(!lightControl) {
  if(light < lightValueAlarm) {
    if(!(lightHighAlarm & 1)) {
      lightHighAlarm |=  3;
      saveMessage(MESSAGE_ALARM_LIGHTHIGH, '1');
    }
  }
  else {
    if(light > lightValueAlarm + lightHysteresis) {
      if(lightHighAlarm & 1) {
        lightHighAlarm &=  2;
        saveMessage(MESSAGE_ALARM_LIGHTHIGH, '0');
      }
    }
  }
 }
 else if (lightHighAlarm & 1) {
  lightHighAlarm &=  2;
  saveMessage(MESSAGE_ALARM_LIGHTHIGH, '0');
 }
*/





  //alarm = tempHighAlarm | tempLowAlarm | lightHighAlarm | lightLowAlarm;
  // TODO: ack
  if(kpd.getKeys()) {
    /*
    tempHighAlarm &= 1;
    tempLowAlarm &= 1;
    lightHighAlarm &= 1;
    lightLowAlarm &= 1;
*/
    tempHighAlarm2.ack();
    tempLowAlarm2.ack();
    lightHighAlarm2.ack();
    lightLowAlarm2.ack();
    //Serial.print(alarm);
  }
  //Serial.print(alarm);
  digitalWrite(LEDPIN, cyclerControl);

  if (!Menu.shown()) {
    if(uiTime + 1000 < millis()) {
      uiTime = millis();
      uiClear();
    }
  }

  if (Serial.available() > 0) {
    // read the incoming byte:
    //incomingByte = Serial.read();
    if (Serial.readString().indexOf("?")!=-1 ) {


      Serial.print("T = ");
      Serial.println(temperature);
      Serial.print("H = ");
      Serial.println(humidity);
      Serial.print("L = ");
      Serial.println(light);

      Serial.println();
      Serial.print("GSM NUMBER = ");
      Serial.println(gsmMgr.gsmNumber);
      Serial.print("GSM PASSWORD = ");
      Serial.println(gsmCode);
      Serial.println();

      char msg[MESSAGELENGTH + 1];
      for(int i = 0; i< MESSAGESCOUNT; i++) {
        readMessage(i, (byte*)msg);
        Serial.println(msg);
      }



    }



  }


 //-------------------------------------------
 /*
  char key = kpd.getKey();
  if(time + 125 < millis()) {
    Menu.k = 0;
  }
  if (key){
    time = millis();
    //Serial.println(key);
    Menu.k = key;
 }
 Serial.println(Menu.k);
 */
 //temperature++;





/*
 //Serial.println(temperature);
 if(kTime + 30000 < millis()) {
  //kTime = millis();
  //uiClear();
  //}
  //else
  if(uTime + 1000 < millis()) {
    uTime = millis();
    uiClear();
  }
}*/

}

void callAction() {
  saveEEPROM();

  char* text="Testing Sms";  //text for the message.
  char* number="+420000000000"; //change to a valid number.

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" CALLING ...");
  lcd.setCursor(1, 1);
  lcd.print(number);

  sim800l.callNumber(number);
  /*
  while(Sim800l.getCallStatus() > 0) {
    lcd.setCursor(0, 1);
    lcd.print(Sim800l.getCallStatus(), DEC)    ;
  }
  */
  //delay(2000);
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
/*
void uiClear2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GROWMAT");

  DateTime now = rtc.now();
  lcd.setCursor(0, 1);
  lcd.print(now.hour(), DEC);
  lcd.setCursor(2, 1);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.setCursor(5, 1);
  lcd.print(":");
  lcd.print(now.second(), DEC);

  lcd.setCursor(11, 0);
  lcd.print("t");
  lcd.setCursor(12, 0);
  lcd.print(temperature);
  //lcd.setCursor(15, 0);
  //lcd.print("C");
  lcd.setCursor(11, 1);
  lcd.print("h");
  lcd.setCursor(12, 1);
  lcd.print(humidity);
  //lcd.setCursor(15, 1);
  //lcd.print(" ");



}
*/
void uiInstrument(bool instrument, byte mode) {
  lcd.print(instrument);
  mode ? lcd.print("M") : lcd.print("A");
}

void uiAlarmList() {
  Menu.enable(false);
  uiState = UISTATE_ALARMLIST;
}
void uiScreen() {
  //Menu.enable(false);
  lcd.backlight();

  if(uiState == UISTATE_ALARMLIST) {
     char msg[MESSAGELENGTH + 1];
     uiPage = min(uiPage, MESSAGESCOUNT -2);
     uiPage = max(uiPage, 0);

     //Serial.println(uiPage);
     //lcd.clear();
     lcd.setCursor(0, 0);
     readMessage(uiPage, (byte*)msg);
     //if(uiPage==0)
     // msg[0] = '-';
     lcd.print(msg);
     //Serial.println(msg);
     lcd.setCursor(0, 1);
     readMessage(uiPage + 1, (byte*)msg);
     //if(uiPage==MESSAGESCOUNT - 2)
     //   msg[0] = '-';
     lcd.print(msg);
     //Serial.println(msg);


  }
}

void uiInfo() {/*
  char msg[MESSAGELENGTH + 1];
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  readMessage(0, msg);
  lcd.print(msg);
  lcd.setCursor(0, 1);

  readMessage(1, msg);
  lcd.print(msg);
*/
//          "123456789ABCDEF"
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BCSEDLON@");
  lcd.setCursor(0, 1);
  lcd.print("GMAIL.COM");
  Menu.enable(false);
  /*while( Menu.checkInput() != BUTTON_SELECT ) {
    ; // wait!
  }  */
}

bool secToggle = false;

void uiClear() {
  if(uiState)
    return;

  secToggle ? secToggle = false : secToggle = true;

  if(tempHighAlarm2.isUnAck() || tempLowAlarm2.isUnAck() || lightHighAlarm2.isUnAck() || lightLowAlarm2.isUnAck()) {
    secToggle ? lcd.backlight() : lcd.noBacklight();
  }  else {
    lcd.backlight();
  }

  lcd.setCursor(0, 0);
  if(tempHighAlarm2.isActive())
    secToggle ? lcd.print("+") : lcd.print(" ");
  else if(tempLowAlarm2.isActive())
    secToggle ? lcd.print("-") : lcd.print(" ");
  else //if(!((tempHighAlarm || tempLowAlarm) & 1))
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

  DateTime now = rtc.now();
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
  if(lightHighAlarm2.isActive())
    secToggle ? lcd.print("+") : lcd.print(" ");
  else if(lightLowAlarm2.isActive())
    secToggle ? lcd.print("-") : lcd.print(" ");
  else if(light < lightValueAlarm)
    lcd.print("*");
  else
    lcd.print(" ");

  lcd.print("L");
  uiInstrument(lightControl, lightMode);
  lcd.print(" H");
  uiInstrument(heaterControl, heaterMode);
  lcd.print(" V");
  uiInstrument(ventControl, ventMode);
  lcd.print(" C");
  uiInstrument(cyclerControl, cyclerMode);
/*

  lcd.setCursor(0, 0);
  if(Sim800l.getCallStatus() == 3) {
    lcd.print("R");
  }*/
}


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

