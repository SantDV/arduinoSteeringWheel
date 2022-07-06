
#include "UnoJoy.h"
#include<math.h>

volatile float temp, counter = 499; 

void setup(){
  setupPins();
  setupUnoJoy();

  pinMode(2, INPUT_PULLUP); 
  
  pinMode(3, INPUT_PULLUP); 
  
  attachInterrupt(0, ai0, RISING);
   
  
  attachInterrupt(1, ai1, RISING);
  
}

void loop(){

  dataForController_t controllerData = getControllerData();
  setControllerData(controllerData);    
}

void ai0() {

  if(digitalRead(3)==LOW) {
  counter = counter + 1.32;
  }else{
  counter= counter - 1.32;
  }
  }
   
  void ai1() {

  if(digitalRead(2)==LOW) {
  counter= counter - 1.32;
  }else{
  counter= counter + 1.32;
  }
  }
  

void setupPins(void){

  for (int i = 4; i <= 12; i++){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(A4, INPUT);
  digitalWrite(A4, HIGH);
  pinMode(A5, INPUT);
  digitalWrite(A5, HIGH);
}

dataForController_t getControllerData(void){
  

  dataForController_t controllerData = getBlankDataForController();


  controllerData.squareOn = !digitalRead(4);
  controllerData.crossOn = !digitalRead(5);
  controllerData.dpadUpOn = !digitalRead(6);
  controllerData.dpadDownOn = !digitalRead(7);
  controllerData.dpadLeftOn = !digitalRead(8);
  controllerData.dpadRightOn = !digitalRead(9);
  controllerData.l1On = !digitalRead(10);
  controllerData.r1On = !digitalRead(11);
  controllerData.selectOn = !digitalRead(12);
  controllerData.startOn = !digitalRead(A4);
  controllerData.homeOn = !digitalRead(A5);
  


  if(counter >= 990){
  counter=counter - 1.32;
  }
  if(counter <= 10){
  counter= counter + 1.32;
  }
  
  controllerData.leftStickX = round(counter) >> 2;
  controllerData.leftStickY = analogRead(A1) >> 2;
  controllerData.rightStickX = analogRead(A2) >> 2;
  controllerData.rightStickY = analogRead(A3) >> 2;
  // And return the data!
  return controllerData;
}
