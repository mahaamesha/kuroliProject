/*
 * Main File - Project Kuroli
 * main.ino
 */

#include "kuroli.h"

const byte CS = 10;  //pin chipSelect modul sd card, ke pin SS arduino
//const byte TRIG = 5;  //pin hcsr04
//const byte ECHO = 6;  //sudah diatur di header file
const byte PWM_GAS = A6;  //gas tangan. A6/7 analog only
byte chooseIna = 226; //choose 219 or 226

Kuroli kuroli(PWM_GAS);

void setup(){
  Serial.begin(9600);

  kuroli.sdInit(CS); //chipSelect modul sd card
  if(chooseIna = 219) kuroli.initIna219();
  else if(chooseIna = 226) kuroli.initIna226();
  kuroli.displayInit(16,2);
}

void loop(){
  structIna myIna = {0,0,0,0,0};

  float pwmGasVoltage = analogRead(PWM_GAS)*5.0/1024.0;
  structKuroli myKuroli = {pwmGasVoltage};
  
  if(chooseIna = 219) kuroli.readIna219(&myIna); //baca shuntVoltage, busVoltage, current, loadVoltage, power
  else if(chooseIna = 226) kuroli.readIna226(&myIna); //baca shuntVoltage, busVoltage, current, loadVoltage, power
  kuroli.sdWrite(&myIna, &myKuroli);  //logging loadVoltage, current, power, distance, pwmGas
  kuroli.serialLog(&myIna, &myKuroli);
  kuroli.textIna(&myIna); //lcd shuntvoltage, busvoltage, current_mA, loadvoltage

  delay(500);
}