
#include "UnoJoy.h"
#include<math.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 


LiquidCrystal_I2C lcd(0x27,16,2); //creacion de un objeto con los parametros indicados

//definicion de nuevos caracteres
byte A[8] = {
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B11111,
};
byte B[8] = {
B00000,
B00000,
B00000,
B00000,
B00000,
B00000,
B11111,
B11111,
};
byte C[8] = {
B00000,
B00000,
B00000,
B00000,
B00000,
B11111,
B11111,
B11111,
};
byte D[8] = {
B00000,
B00000,
B00000,
B00000,
B11111,
B11111,
B11111,
B11111,
};
byte E[8] = {
B00000,
B00000,
B00000,
B11111,
B11111,
B11111,
B11111,
B11111,
};
byte F[8] = {
B00000,
B00000,
B11111,
B11111,
B11111,
B11111,
B11111,
B11111, 
};

byte G[8] = {
B00000,
B11111,
B11111,
B11111,
B11111,
B11111,
B11111,
B11111, 
};

byte H[8] = {
B11111,
B11111,
B11111,
B11111,
B11111,
B11111,
B11111,
B11111, 
};

int pinA1 = A1;    // potenciómetro para acelerador
int pinA2 = A2;    // potenciómetro para freno

//Inicialización variales para acelerador y freno display
int dataAcelerador = 0; 
int dataFreno = 0;    

volatile float temp, counter = 499; //punto 0 volante

//funcion para crear caracteres
void crearCaracteres(){
  lcd.createChar (8,A);
 lcd.createChar (9,B);
 lcd.createChar (10,C);
 lcd.createChar (11,D);
 lcd.createChar (12,E);
 lcd.createChar (13,F);
 lcd.createChar (14,G);
 lcd.createChar (15,H); 
}

//Funcion que generar los datos a mostrar para mostrar tanto del acelerador como el frenado
void acelAndBrakes(int data, int fila){
  
  if (data <= 3){
    lcd.setCursor(8,fila);
    lcd.print(" ");
  }
  else if (data <=  125  && data >=4 ){
    lcd.setCursor(8,fila);
    lcd.write(byte (8)); 
    lcd.setCursor(9,fila);
    lcd.print(" ");
    
  }
  else if(data >=126 && data <=251){
    lcd.setCursor(9,fila);
    lcd.write(byte (9)); 
    lcd.setCursor(10,fila);
    lcd.print(" ");
  }
  else if(data >=252 && data <=377){
    lcd.setCursor(10,fila);
    lcd.write(byte (10));
    lcd.setCursor(11,fila);
    lcd.print(" ");
  }
  else if(data >=378 && data <=503){
    lcd.setCursor(11,fila);
    lcd.write(byte (11)); 
    lcd.setCursor(12,fila);
    lcd.print(" ");
  }
  else if(data >=504 && data <=629){
    lcd.setCursor(12,fila);
    lcd.write(byte (12)); 
    lcd.setCursor(13,fila);
    lcd.print(" ");
  }
  else if(data >=630 && data <=755){
    lcd.setCursor(13,fila);
    lcd.write(byte (13));
    lcd.setCursor(14,fila);
    lcd.print(" ");
  }
  else if(data >=756 && data <=881){
    lcd.setCursor(14,fila);
    lcd.write(byte (14)); 
    lcd.setCursor(15,fila);
    lcd.print(" ");
  }
  else if(data >=882){
    lcd.setCursor(15,fila);
    lcd.write(byte (15)); 
  }
}


void setup(){

  
  
  lcd.init();

 //Encender la luz de fondo.
  lcd.backlight(); 
  
  for (int i=0;i<=1;i++){
    lcd.display();
    lcd.setCursor(4,0);
    lcd.print("Cargando");
    delay(700);    
    lcd.noDisplay(); 
    delay(700);  
  }
  //creamos los nuevos caracteres
  crearCaracteres();
  
  
  //setUp pines para volante y freno e inicializacion unoJoy
  setupPins(); 
  setupUnoJoy();

  pinMode(2, INPUT_PULLUP); 
  
  pinMode(3, INPUT_PULLUP); 
  
  attachInterrupt(0, ai0, RISING);
   
  attachInterrupt(1, ai1, RISING);

  lcd.display();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Acelera");
  lcd.setCursor(0,1);
  lcd.print("Freno");
  
}

void loop(){

  dataForController_t controllerData = getControllerData();
  setControllerData(controllerData);

  dataAcelerador = analogRead(pinA1); 
  acelAndBrakes(dataAcelerador, 0);
  
  dataFreno = analogRead(pinA2); 
  acelAndBrakes(dataFreno, 1);
}

void ai0() {

  if(digitalRead(3)==LOW) {
  counter = counter + 0.8;
  }else{
  counter= counter - 0.8;
  }
  }
   
  void ai1() {

  if(digitalRead(2)==LOW) {
  counter= counter - 0.8;
  }else{
  counter= counter + 0.8;
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

  


  if(counter >= 1000){
  counter=1000;
  }
  if(counter <= 0){
  counter= 0;
  }
  
  controllerData.leftStickX = round(counter) >> 2;
  controllerData.leftStickY = analogRead(A1) >> 2;
  controllerData.rightStickX = analogRead(A2) >> 2;
  controllerData.rightStickY = analogRead(A3) >> 2;
  // And return the data!
  return controllerData;
}
