/*TODO::
- Setup Iridium transmission in main loop
- Add servo control for cooler
*/
//Library Includes
#include <IridiumSBD.h> //Download from https://github.com/mikalhart/IridiumSBD
#include <Wire.h> //I2C library, reserves A4 and A5 for I2C
#include <SoftwareSerial.h>
#include <stdio.h>
#include <Servo.h> 
 
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

//Object creation
SoftwareSerial OpenLog(2, 3); // RX, TX
//SoftwareSerial ssIridium(4, 5); // RockBLOCK serial port on 4/5 (RX, TX)
//IridiumSBD isbd(ssIridium, 6);   // RockBLOCK SLEEP pin on 10
Servo servo1;
Servo servo2;  

void setup() {
  Serial.begin(9600); //Serial monitor
  OpenLog.begin(9600); //Data logging
  servoInit(); //Servo intitialization
  pinMode(FETpin,OUTPUT); // FET pin 
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

    tempControl(batteryTemp);        
    
    for (int i=0; i < arrayLength; i++){ //For each element of the array, print and log it
      Serial.print(tempData[i]);
      Serial.print(", ");
      
      OpenLog.print(tempData[i]);
      OpenLog.print(",");
    }
    
    Serial.println();
    OpenLog.println();

    if (canTemp <= 0 && cycle == 0)  //need to figure out a value for temperature
      {
        servoSpin();
        cycle++;
      }
 }
