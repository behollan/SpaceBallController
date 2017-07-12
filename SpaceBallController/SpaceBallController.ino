//Library Includes
#include <IridiumSBD.h> //Download from https://github.com/mikalhart/IridiumSBD
#include <Wire.h> //I2C library, reserves A4 and A5 for I2C
#include <SoftwareSerial.h>
#include <stdio.h>
#include <Servo.h> 
#include <SparkFunMPL3115A2.h>
 
//Function declarations
float * tempMeasure();
byte fetch_humidity_temperature(unsigned int *p_Humidity, unsigned int *p_Temperature);
void print_float(float f, int num_digits);
void servoInit();
void servoSpin();

//Variables defines
#define TRUE 1
#define FALSE 0

bool state = 0;
float prevtemp;
int cycle = 0; //ensures the servos only initiate once 
int prevTime;
int tweetTime =60000;

#define highTemp  15
#define lowTemp  10

#define FETpin 13 //Heater FET Control Pin
#define servoPin1  9 //change X to whatever pwm pin
#define servoPin2  10 //change Y to whatever pwm pin

// Analog Temp Sensor Pins
#define temp1  A0 //Battery
#define temp2  A1 //Can
#define temp3  A2 //GoPro
#define temp4  A3 //External
#define temp5  A4 //reserved

//Object instance creation
SoftwareSerial OpenLog(2, 3); // RX, TX
SoftwareSerial ssIridium(4, 5); // RockBLOCK serial port on 4/5 (RX, TX)
IridiumSBD isbd(ssIridium, 6);   // RockBLOCK SLEEP pin on 6
Servo servo1;
Servo servo2;  
MPL3115A2 pres;

void setup() {
  Wire.begin();        // Join i2c bus
  Serial.begin(9600); //Serial monitor
  OpenLog.begin(9600); //Data logging
  servoInit(); //Servo intitialization
  pinMode(FETpin,OUTPUT); // FET pin 
  pres.begin(); //Bring Altimeter online
  pres.setModeAltimeter(); //Set altimeter mode
  pres.setOversampleRate(7); //Oversample to recommended 128
  pres.enableEventFlags(); //Pressure and temp event flags
  
  isbd.setPowerProfile(1); // low current
  isbd.begin(); // wake up, prepare for comm
  prevTime=millis();
  Serial.println("Init Complete.");
}


void loop() {    
    float *tempPoint;
    tempPoint = tempMeasure(); //Grab the tempMeasure pointer
    
    //Build array of temperatureData
    float tempData [5] = {*(tempPoint + 0),*(tempPoint + 1),*(tempPoint + 2),*(tempPoint + 3),*(tempPoint + 4)}; 
    int arrayLength = sizeof(tempData)/sizeof(tempData[0]);

    float batteryTemp = tempData[0]; //Store battery temp in a more friendly variable name
    float canTemp = tempData[1]; //Store can temperature in a more friendly variable name
    float goProTemp = tempData[2]; //Store goPro temperature in a more friendly variable name
    float extTemp = tempData[3]; //Store external temperature in a more friendly variable name

    float altitude = pres.readAltitudeFt();

    tempControl(batteryTemp);        
    
    for (int i=0; i < arrayLength; i++){ //For each element of the array, print and log it
      Serial.print(tempData[i]);
      Serial.print(", ");
      
      OpenLog.print(tempData[i]);
      OpenLog.print(",");
    }

    Serial.print(altitude);
    OpenLog.print(altitude);
    
    Serial.println();
    OpenLog.println();

    if ((altitude > 75000 || canTemp <= 0) && cycle == 0)  //need to figure out a value for temperature
      {
        servoSpin();
        Serial.println("Cooler Lowered");
        OpenLog.println("Cooler Lowered");
        cycle++;
      }

      char message[15];
      sprintf(message,"%f",canTemp);
    
    if(millis()-prevTime>tweetTime){
        tweetMessage(message);
      }
 }
