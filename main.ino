/*
 * Main File - Project Kuroli
 * main.ino
 */

#include "kuroli.h"

const byte CS = 10;  //pin chipSelect modul sd card, ke pin SS arduino
//const byte TRIG = 5;  //pin hcsr04
//const byte ECHO = 6;  //sudah diatur di header file
const byte PWM_GAS = A6;  //gas tangan. A6/7 analog only

Kuroli kuroli(PWM_GAS);

void setup(){
  Serial.begin(9600);

  kuroli.sdInit(CS); //chipSelect modul sd card
  kuroli.inaInit();
  kuroli.displayInit(16,2);
}

void loop(){
  structIna219 myIna219 = {0,0,0,0,0};

  float pwmGasVoltage = analogRead(PWM_GAS)*5.0/1024.0;
  structKuroli myKuroli = {pwmGasVoltage};
  
  kuroli.readIna219(&myIna219); //baca shuntVoltage, busVoltage, current, loadVoltage, power
  kuroli.sdWrite(&myIna219, &myKuroli);  //logging loadVoltage, current, power, distance, pwmGas
  kuroli.serialLog(&myIna219, &myKuroli);
  kuroli.textIna219(&myIna219); //lcd shuntvoltage, busvoltage, current_mA, loadvoltage

  delay(500);
}