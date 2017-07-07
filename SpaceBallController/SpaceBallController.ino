/*TODO::
- Setup Iridium transmission in main loop
- Heater feedback loop
  - Must buy 2 FET's
- Add servo control for cooler
*/
//Library Includes
#include <IridiumSBD.h> //Download from https://github.com/mikalhart/IridiumSBD
#include <Wire.h> //I2C library, reserves A4 and A5 for I2C
#include <SoftwareSerial.h>
#include <stdio.h>

//Function declarations
float * tempMeasure();
byte fetch_humidity_temperature(unsigned int *p_Humidity, unsigned int *p_Temperature);
void print_float(float f, int num_digits);

//Variables defines
#define TRUE 1
#define FALSE 0

// Analog Temp Sensor Pins
int temp1 = A0;
int temp2 = A1;
int temp3 = A2;
int temp4 = A3;
int temp5 = A4;


//Object creation
//SoftwareSerial OpenLog(2, 3); // RX, TX
//SoftwareSerial ssIridium(4, 5); // RockBLOCK serial port on 4/5 (RX, TX)
//IridiumSBD isbd(ssIridium, 6);   // RockBLOCK SLEEP pin on 10

void setup() {
  Serial.begin(9600); //Serial monitor
  //OpenLog.begin(9600); //Data logging
  Serial.println("Init Complete. Printing temp data.");
}

void loop() {    
    float *tempPoint;
    tempPoint = tempMeasure(); //Grab the tempMeasure pointer
    
    //Build array of temperatureData
    float tempData [5] = {*(tempPoint + 0),*(tempPoint + 1),*(tempPoint + 2),*(tempPoint + 3),*(tempPoint + 4)}; 
    int arrayLength = sizeof(tempData)/sizeof(tempData[0]);

    //Serial.println(*(tempPoint + 0));
    
    //Serial.println(tempPoint);
    
    for (int i=0; i < arrayLength; i++){ //For each element of the array, print and log it
      Serial.print(tempData[i]);
      Serial.print(", ");
      
      //OpenLog.print(tempData[i]);
      //OpenLog.print(",");
    }
    //float hum = humidity(); //Get humidity data
    //OpenLog.println( hum ); //Log humidity 
    //Serial.println( hum ); //Print humidity
    Serial.println("");
    delay(100);
}

/*
bool  ISBDCallback(){ //Place things here to run during the transmit attempt
    //In this case, keep logging data during the transmission
    float *tempPoint;
    tempPoint = tempMeasure(); //Grab the tempMeasure pointer
    
    //Build array of temperatureData
    float tempData [5] = {*(tempPoint + 0),*(tempPoint + 1),*(tempPoint + 2),*(tempPoint + 3),*(tempPoint + 4)}; 
    int arrayLength = sizeof(tempData)/sizeof(tempData[0]);
    
    for (int i=0; i < arrayLength; i++){ //For each element of the array, print and log it
      Serial.print(tempData[i]);
      Serial.print(", ");
      
      OpenLog.print(tempData[i]);
      OpenLog.print(",");
    }
    float hum = humidity(); //Get humidity data
    OpenLog.println( hum ); //Log humidity 
    Serial.println( hum ); //Print humidity
  }
  */
