void servoInit(){
  servo1.attach(servoPin1);
  servo2.attach(servoPin2); 
}

void servoSpin(){
                             
    servo1.write(45); //play around with 45 to get the rotation we need
    servo2.write(45);   
    delay(3000); //Set length of rotation
    servo1.detach();
    servo2.detach();
}


