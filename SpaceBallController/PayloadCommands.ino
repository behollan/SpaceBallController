float * tempMeasure(){ //Measures temperature from 4 temp sensors and I2C sensor, returns array of temperatures
  byte _status;
  unsigned int H_dat, T_dat;
  float RH, T_C;

  _status = fetch_humidity_temperature(&H_dat, &T_dat);

  T_C = (float) T_dat * 1.007e-2 - 40.0;
  
  static float data[5]; //Initialize a data array for temp storage
  data[0] = analogRead(temp1);
  data[1] = analogRead(temp2);
  data[2] = analogRead(temp3);
  data[3] = analogRead(temp4);
  data[4] = T_C;
  return(data);
}
float humidity(){ //Reads and returns HIH6130 relative humidity
  byte _status;
  unsigned int H_dat, T_dat;
  float RH, T_C;

  _status = fetch_humidity_temperature(&H_dat, &T_dat);

  RH = (float) H_dat * 6.10e-3;

  return(RH);
}

