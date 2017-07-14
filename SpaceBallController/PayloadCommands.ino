float * tempMeasure(){ //Measures temperature from 4 temp sensors and I2C sensor, returns array of temperatures
 
  static float data[5]; //Initialize a data array for temp storage
  data[0] = analogRead(temp1);
  data[1] = analogRead(temp2);
  data[2] = analogRead(temp3);
  data[3] = analogRead(temp4);
  data[4] = analogRead(temp5);
  //data[4] = T_C;
  
  int arrayLength = sizeof(data)/sizeof(data[0]);
  
  for (int i=0; i < arrayLength; i++){ //For each element of the array, convert to voltage then to temperature
    data[i]= 100*(data[i]*5/1024-0.5) ;
  }
  return data;
}

void tempControl(float temp){ //Heater feedback loop

  //Hysteresis controller
  if(prevtemp < highTemp && temp >= highTemp){
    state = 0;
    Serial.print("Heater state set to ");Serial.println(state);
    OpenLog.print("Heater state set to ");OpenLog.println(state);
  }
  
  if(prevtemp > lowTemp && temp <= lowTemp){
    state = 1;
    Serial.print("Heater state set to ");Serial.println(state);
    OpenLog.print("Heater state set to ");OpenLog.println(state);
  }

  digitalWrite(FETpin,state); //Turn heater on or off
  
  prevtemp = temp;
  
}

void mainPayload(){
    float *tempPoint;
    tempPoint = tempMeasure(); //Grab the tempMeasure pointer
    
    //Build array of temperatureData
    float tempData [5] = {*(tempPoint + 0),*(tempPoint + 1),*(tempPoint + 2),*(tempPoint + 3),*(tempPoint + 4)}; 
    int arrayLength = sizeof(tempData)/sizeof(tempData[0]);

    batteryTemp = tempData[0]; //Store battery temp in a more friendly variable name
    canTemp = tempData[1]; //Store can temperature in a more friendly variable name
    goProTemp = tempData[2]; //Store goPro temperature in a more friendly variable name
    extTemp = tempData[3]; //Store external temperature in a more friendly variable name

    altitude = pres.readAltitudeFt(); // determine altitude in ft

    tempControl(batteryTemp); //Set heater state based on temperature     
    
    for (int i=0; i < arrayLength; i++){ //For each element of the array, print and log it
      Serial.print(tempData[i]);
      Serial.print(", ");
      
      OpenLog.print(tempData[i]);
      OpenLog.print(",");
    }
    float altTemp = pres.readTemp();

    Serial.print(altTemp); Serial.print(",");
    OpenLog.print(altTemp); OpenLog.print(",");
    
    Serial.println(altitude);
    OpenLog.println(altitude);
    

    if ((altitude > altitudeCutoff || canTemp <= 0) && cycle == 0)  //need to figure out a value for temperature
      {
        servoSpin();
        Serial.println("Cooler Lowered");
        OpenLog.println("Cooler Lowered");
        cycle++;
      }
  }
