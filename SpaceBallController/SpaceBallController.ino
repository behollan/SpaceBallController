#include <IridiumSBD.h>
#include <Wire.h> //I2C library, reserves A4 and A5 for I2C

// Analog Temp Sensor Pins
int temp1 = A0;
int temp2 = A1;
int temp3 = A2;
int temp4 = A3;


float * tempMeasure();
byte fetch_humidity_temperature(unsigned int *p_Humidity, unsigned int *p_Temperature);
void print_float(float f, int num_digits);

#define TRUE 1
#define FALSE 0

#include <SoftwareSerial.h>

SoftwareSerial OpenLog(2, 3); // RX, TX
SoftwareSerial ssIridium(4, 5); // RockBLOCK serial port on 18/19

IridiumSBD isbd(ssIridium, 6);   // RockBLOCK SLEEP pin on 10

void setup() {
  Serial.begin(9600);
  OpenLog.begin(9600);
}

void loop() {
  ISBDCallback(){ //Place things here to run during the transmit attempt
    //In this case, keep logging data during the transmission
    Serial.print(* tempMeasure() );
    Serial.print(" ");
    Serial.println( humidity() );
  
    OpenLog.print(* tempMeasure());
    OpenLog.print(" ");
    OpenLog.println( humidity() );
  }

  
}
