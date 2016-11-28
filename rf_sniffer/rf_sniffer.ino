#include "RF433.h"

#include "SerialCommand.h"
/* Adapted from Steven Cogswell library,
 * see https://github.com/scogswell/ArduinoSerialCommand
 * Branched from main, 2013 */

//#include <EEPROM.h>
//#include <avr/wdt.h>

#define LED 13 // Debug, or activity purpose
#define RX A0
#define TX 4

// MCE07
byte storedData1off[] = {11,30,3,4,3,3,3,11,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,10,4,3,3,11,3,11,3,3,4,3,3,4,3,10,3,11,3,4,3,11,3,10,4,10,3,11,3,11,3,11,3,3,3,11,3,4,3,11,3,10,3,11,3,11,3,3,4,3,3,3,4,188,3,3,3,3,30,29,4,3,3,3,4,10,3,11,3,11,3,3,4,3,3,11,3,3,4,3,3,3,4,3,3,3,4,3,3,11,3,4,3,10,4};
byte storedData1on[] = {5,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,10,4,3,3,3,4,3,3,3,4,3,3,4,3,10,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,4,3,10,3,11,3,11,3,11,3,11,3,10,3,4,3,11,3,11,3,3,3,4,3,3,3,4,3,10,4,10,3,11,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,4,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3};
byte storedData2off[] = {16,29,3,4,3,3,3,11,3,11,3,11,3,3,3,4,3,11,3,3,3,4,3,3,3,4,3,3,3,4,3,11,3,3,3,11,3,11,3,4,2,4,3,3,3,11,3,11,3,4,3,11,3,10,3,11,3,11,3,11,3,11,3,3,3,11,3,11,3,3,3,11,3,11,3,3,3,11,3,4,3,3,3,188,4,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,10,4,3,3,3,4,3,3,3,3,4,3,4,3,10,3,4,3,11,3};
byte storedData2on[] = {4,30,3,3,3,4,3,11,3,10,4,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,10,4,3,3,3,3,4,3,11,3,10,4,3,3,11,3,11,3,10,4,10,3,11,3,11,3,3,3,11,3,4,3,10,4,3,3,4,3,10,3,4,3,10,4,10,3,90,4,2,4,3,29,30,3,3,3,4,3,11,3,10,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3};
byte storedData3off[] = {1,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,3,10,3,11,3,4,3,11,3,3,3,4,3,10,3,11,3,11,3,11,3,3,3,4,3,90,3,3,3,3,30,30,3,3,3,4,3,10,3,11,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,10,3,4,3,3,3,4,3,11,3,11,2,4,3,11,3,11,3};
byte storedData3on[] = {5,2,4,3,29,30,3,3,3,4,3,11,3,10,3,11,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,3,10,3,11,3,4,3,11,3,11,3,10,3,4,3,3,3,4,3,3,3,11,3,11,3,90,3,3,4,3,29,30,3,3,3,4,3,11,3,10,4,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3};
byte storedData4off[] = {3,3,3,11,3,10,4,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,10,4,3,3,3,3,4,3,11,3,10,4,3,3,11,3,11,3,11,3,10,3,11,3,11,3,3,3,11,3,4,3,3,3,11,3,11,3,11,3,10,4,3,3,3,4,90,3,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3};
byte storedData4on[] = {4,4,3,10,4,89,4,3,3,3,29,30,3,4,3,3,3,11,3,11,3,10,3,4,3,3,3,11,3,4,3,3,3,4,3,3,3,4,3,3,3,11,3,4,3,11,3,11,3,3,3,4,3,3,3,11,3,11,3,3,3,11,3,11,3,11,3,11,2,11,3,11,3,3,4,11,3,3,3,4,3,10,3,4,3,11,3,10,3,4,3,11,3,188,3,3,3,3,30,29,3,4,3,3,3,11,3,11,3,10,4,3,3,3,4,11,3,3,3,4,3,3,3,4,3};


// Serial command handler
SerialCommand SCmd;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  pinMode(LED, OUTPUT);

  //SCmd.addCommand("h", scmdHelp);
  SCmd.addCommand("r", scmdCodeReceive);
  SCmd.addCommand("s", scmdCodeSend);
  SCmd.addCommand("1+", scmdCodeSend1On);
  SCmd.addCommand("1-", scmdCodeSend1Off);
  SCmd.addCommand("2+", scmdCodeSend2On);
  SCmd.addCommand("2-", scmdCodeSend2Off);
  SCmd.addCommand("3+", scmdCodeSend3On);
  SCmd.addCommand("3-", scmdCodeSend3Off);
  SCmd.addCommand("4+", scmdCodeSend4On);
  SCmd.addCommand("4-", scmdCodeSend4Off);  
  SCmd.addDefaultHandler(scmdUnrecognized);


  //eeReadConfig();
  //Serial.println();
  //Serial.println();
  //Serial.println();
  scmdBanner();
  //Serial.println(F("Use h(elp) command at prompt if you need some hints."));
  scmdHelp();
  Serial.println();
  //wdt_enable(WDTO_4S);
  //SCmd.printPrompt();
}

void scmdCodeReceive() {
  initData();
  listenForSignal(RX);
  printData();
  //Serial.println();
  Serial.println("Done.");
}

void scmdCodeSend1On() {
  sendSignal(TX, storedData1on);
  Serial.println("Send 1 ON");
}
void scmdCodeSend1Off() {
  sendSignal(TX, storedData1off);
  Serial.println("Send 1 OFF");
}
void scmdCodeSend2On() {
  sendSignal(TX, storedData2on);
  Serial.println("Send 2 ON");
}
void scmdCodeSend2Off() {
  sendSignal(TX, storedData2off);
  Serial.println("Send 2 OFF");
}
void scmdCodeSend3On() {
  sendSignal(TX, storedData3on);
  Serial.println("Send 3 ON");
}
void scmdCodeSend3Off() {
  sendSignal(TX, storedData3off);
  Serial.println("Send 3 OFF");
}
void scmdCodeSend4On() {
  sendSignal(TX, storedData4on);
  Serial.println("Send 4 ON");
}
void scmdCodeSend4Off() {
  sendSignal(TX, storedData4off);
  Serial.println("Send 4 OFF");
}

void scmdCodeSend() {
  sendSignal(TX);
  Serial.println("Send last recieved code.");
}

void loop() {
  /*
  if (rxActive) { // Sniffing
    rfReceive();
  }
  
  if (txActive) { // Sending
    rfSend();
  }
  */
 
  SCmd.readSerial();
  
  /*
  // Handle millis roll over for uptime
  // TODO eats CPU? To be called by an interrupt? No noticable impact...
  if(!millisHigh && millis() >= 3000000000) {
    millisHigh = true;
  } else {
    if(millisHigh && millis() <= 1000000000){
      millisRollOvers++;
      millisHigh = false;
    }
  }
  */
  //wdt_reset();
}


void scmdUnrecognized() {
  Serial.println(F("Unknown command or too long command line. Try h or -h ?"));
  //SCmd.printPrompt();
}

void scmdHelp() {
  char *arg;
  
  arg = SCmd.next();
  if (arg == NULL) {
    scmdBanner();
  
    //Serial.println(uptime());
    //Serial.println(rxActive? F("RF433 receiver active.") : F("RF433 receiver inactive."));
    Serial.println();
    
    Serial.print(F("Available commands are :\r\n"\
               "  h\t This screen, some help and status.\n\r"\
               "  r\t Sniff code (press original remote control button before).\n\r"\
               "  s\t Sends last sniffed code.\r\n"\
               "  1+\t 1 ON\r\n"\
               "  1-\t 1 OFF\r\n"\
               "  2+\t 2 ON\r\n"\
               "  2-\t 2 OFF\r\n"\
               "  3+\t 3 ON\r\n"\
               "  3-\t 3 OFF\r\n"\
               "  4+\t 4 ON\r\n"\
               "  4-\t 4 OFF\r\n"\
               "Use -h for commands help and their options, when they have.\r\n\r\n"));
  } else {
    if(strcmp (arg, "-h") == 0) {
      Serial.println(F("Help has no help, sorry."));
    } else {
      scmdInvalidOption(arg);
    }
  }
               
  //SCmd.printPrompt();
}

void scmdBanner() {
  String banner = F("\r\nMaclean Energy MCE07 remote control codes sniffer\r\n"\
        "Version ");
  Serial.println(banner);
}

void scmdInvalidOption (char *option) {
  if (strlen(option) > 0) {
    String dummy = F("Invalid option or argument \"");
    dummy += option;
    dummy += F("\". Try -h ?");
    Serial.println(dummy);
  } else {
    Serial.println(F("Missing or invalid command argument. Try -h ?"));
  }
}

