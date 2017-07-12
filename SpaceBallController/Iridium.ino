bool  ISBDCallback(){ //Place things here to run during the transmit attempt, prepare data
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
 
} 


void tweetMessage(char message[15]){
    int sendSBDText(message[15]);
}

