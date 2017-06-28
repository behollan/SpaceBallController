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


//Object creation
SoftwareSerial OpenLog(2, 3); // RX, TX
SoftwareSerial ssIridium(4, 5); // RockBLOCK serial port on 4/5 (RX, TX)
IridiumSBD isbd(ssIridium, 6);   // RockBLOCK SLEEP pin on 10

void setup() {
  Serial.begin(9600); //Serial monitor
  OpenLog.begin(9600); //Data logging
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
