
#include "RF433.h"

const int dataSize = 125;  //Arduino memory is limited (max=1700)
byte storedData[dataSize];  //Create an array to store the data
const unsigned int threshold = 100;  //signal threshold value
int maxSignalLength = 255;   //Set the maximum length of the signal
int dataCounter = 0;    //Variable to measure the length of the signal
int timeDelay = 105;    //Used to slow down the signal transmission (can be from 75 - 135)

void sendSignal(int rfTransmitPin){
   sendSignal(rfTransmitPin, storedData); 
}

void sendSignal(int rfTransmitPin, byte storedData[]){
   pinMode(rfTransmitPin, OUTPUT);
   //digitalWrite(ledPin, HIGH);
   for(int i=0; i<dataSize; i=i+2){
       //Send HIGH signal
       digitalWrite(rfTransmitPin, HIGH);
       delayMicroseconds(storedData[i]*timeDelay);
       //Send LOW signal
       digitalWrite(rfTransmitPin, LOW);
       delayMicroseconds(storedData[i+1]*timeDelay);
   }
   //digitalWrite(ledPin, LOW);
   //delay(1000);
}

void listenForSignal(int rfReceivePin){
   //digitalWrite(ledPin, HIGH);
   //delay(1000);
   //digitalWrite(ledPin,LOW);
   while(analogRead(rfReceivePin)<threshold){
     //Wait here until an RF signal is received
   }
   //digitalWrite(ledPin, HIGH);

   //Read and store the rest of the signal into the storedData array
   for(int i=0; i<dataSize; i=i+2){

      //Identify the length of the HIGH signal---------------HIGH
      dataCounter=0; //reset the counter
      while(analogRead(rfReceivePin)>threshold && dataCounter<maxSignalLength){
        dataCounter++;
      }
      storedData[i]=dataCounter;    //Store the length of the HIGH signal


      //Identify the length of the LOW signal---------------LOW
      dataCounter=0;//reset the counter
      while(analogRead(rfReceivePin)<threshold && dataCounter<maxSignalLength){
        dataCounter++;
      }
      storedData[i+1]=dataCounter;  //Store the length of the LOW signal
   }

     storedData[0]++;  //Account for the first AnalogRead>threshold = lost while listening for signal
     //digitalWrite(ledPin, LOW);
}

void initData(){
  for(int i=0; i<dataSize; i++){
    storedData[i]=0;
  }
}


void printData() {
	Serial.println("HIGH,LOW...");
	     for(int i=0; i<dataSize; i++){

	       Serial.print(storedData[i]);
	       Serial.print(",");
	    }
	    Serial.println();
}
