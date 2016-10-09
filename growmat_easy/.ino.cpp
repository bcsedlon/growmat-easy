#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2016-10-09 23:47:05

#include "Arduino.h"
#include <Wire.h>
#include "libraries/Sim800l/Sim800l.h"
#include <SoftwareSerial.h>
#include "libraries/RTClib/RTClib.h"
#include "libraries/DHT-sensor-library-master/DHT.h"
#include "libraries/Keypad_I2C/Keypad_I2C.h"
#include "libraries/Keypad/Keypad.h"
#include "libraries/NewliquidCrystal/LiquidCrystal_I2C.h"
#include "libraries/OMEEPROM/OMEEPROM.h"
#include "libraries/OMMenuMgr/OMMenuMgr.h"
int saveMessage(char msg[], char status) ;
void readMessage(int index, byte* msg) ;
void loadEEPROM() ;
void saveEEPROM() ;
void setup() ;
bool getControl(bool a, byte mode) ;
void loop() ;
void callAction() ;
void uiDraw(char* p_text, int p_row, int p_col, int len) ;
void uiInstrument(bool instrument, byte mode) ;
void uiAlarmList() ;
void uiSetClock() ;
void uiScreen() ;
void uiInfo() ;
void uiClear() ;
void uiQwkScreen() ;

#include "growmat_easy.ino"


#endif
