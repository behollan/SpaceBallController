void servoSpin(){
    servo1.attach(servoPin1); //Servo intitialization                         
    servo1.write(servoSpeed); //Turn servo on at predefined speed
    delay(rotTime);           //Wait until cooler is lowered
    servo1.write(90);         //Turn servo off
    servo1.detach();          //detach servo
}


