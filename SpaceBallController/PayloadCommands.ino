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

void tempControl(float temp){
  
  if(prevtemp < highTemp && temp >= highTemp) state = 0;
  if(prevtemp > lowTemp && temp <= lowTemp) state = 1;

  digitalWrite(FETpin,state);

  Serial.print(state);

  prevtemp = temp;
  
}

