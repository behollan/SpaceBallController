//Library Includes
#include <IridiumSBD.h> //Download from https://github.com/mikalhart/IridiumSBD
#include <Wire.h> //I2C library, reserves A4 and A5 for I2C
#include <SoftwareSerial.h>
#include <stdio.h>
#include <Servo.h> 
#include <SparkFunMPL3115A2.h>



////////////////////////////////////////////////////////////////////////////
/*  USER DEFINED VARIABLES HERE. CHANGE THESE TO CHANGE PAYLOAD FUNCTION  */
////////////////////////////////////////////////////////////////////////////

#define tweetTime 60000       //Length between tweets (in ms)
#define highTemp  15          //High temperature (when to turn heaters on)
#define lowTemp  10           //Low temperature (when to turn heaters off)
#define altitudeCutoff 80000  //Max altitude (ft) for the cooler to be allowed open
#define rotTime 3000          //Length of time for servo rotation
#define servoSpeed 45         //Servo speed, 0 being full-speed in one direction, 180 being full speed in the other, and a value near 90 being no movement

//Variable initialization
bool state = 0;  //Used for heater control
float prevtemp;  //Used for heater control
int cycle = 0;   //ensures the servos only initiate once 
int prevTime;    //Used for tweeting
#define TRUE 1   //Handy to have
#define FALSE 0  //Handy to have
float batteryTemp, canTemp, goProTemp, extTemp, altitude; //data variables

///////////////////
/* PIN LOCATIONS */
///////////////////

#define FETpin 13 //Heater FET Control Pin
#define servoPin1  9 //Servo PWM pin

// Analog Temp Sensor Pins
#define temp1  A0 //Battery
#define temp2  A1 //Can
#define temp3  A2 //GoPro
#define temp4  A3 //External
#define temp5  A4 //reserved

//////////////////////////////
/* Object instance creation */
//////////////////////////////

SoftwareSerial OpenLog(3, 2); // Pin pin2 is openlog TX and pin3 is openlog RX
SoftwareSerial ssIridium(4, 5); // RockBLOCK serial port on 4/5 (RX/TX)
IridiumSBD isbd(ssIridium, 6);   // RockBLOCK SLEEP pin on 6
Servo servo1;
MPL3115A2 pres;

/////////////////
/* Setup Tasks */
/////////////////

void setup() {
  Wire.begin();        // Join i2c bus
  Serial.begin(9600); //Serial monitor
  OpenLog.begin(9600); //Data logging
  
  servo1.attach(servoPin1); //Servo intitialization
  
  pinMode(FETpin,OUTPUT);   // FET pin 
  
  pres.begin();             //Bring Altimeter online
  pres.setModeAltimeter();  //Set altimeter mode
  pres.setOversampleRate(7);//Oversample to recommended 128
  pres.enableEventFlags();  //Pressure and temp event flags
  
  isbd.setPowerProfile(1);  // RockBlock low current mode
  isbd.begin();             //RockBlock wake up, prepare for comm
  
  prevTime=millis();  //Used for tweet timer
  
  Serial.println("Init Complete.");
  OpenLog.println("Init Complete.");
}

///////////////
/* Main loop */
///////////////

void loop() {    
      mainPayload(); //Controls all payload data, logging, heater control, and cooler control

      char message[15]; //Tweet variable
      sprintf(message,"%f",canTemp); //Format the float to a string for sending
      
      if(millis()-prevTime>tweetTime){
          Serial.print("Sending to twitterBot: "); Serial.println(message);
          OpenLog.print("Sending to twitterBot: "); OpenLog.println(message);
          isbd.sendSBDText(message); //Send current can temperature to the TwitterBot
      }
}
