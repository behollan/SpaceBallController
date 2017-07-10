void servoInit(){
  servo1.attach(servoPin1);
  servo2.attach(servoPin2); 
}

void servoSpin(){
  for(int angle = 0; angle < 720; angle++)  //play around with 720 to get the rotation we need
  {                                  
    servo1.write(angle);
    servo2.write(angle);                                   
  } 
}


