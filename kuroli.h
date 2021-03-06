/*
 * Header File
 * kuroli.h
*/

#ifndef KUROLI_H
#define KUROLI_H

#include "Arduino.h"
#include <Wire.h>
#include "src/lcd/LiquidCrystal_I2C.h"
#include "src/ina219/Adafruit_INA219.h"
#include "src/ina226_WE/ina226_WE.h"
#include "src/hcsr04/HCSR04.h"
#include <SPI.h>
#include <SD.h>

struct structKuroli {
  float pwmGas; //besar sinyal pwm gas tangan
  //tambah yg lain di sini
};

struct structIna {
  float shuntVoltage; //mV
  float busVoltage; //V
  float current; //mA
  float loadVoltage; //V
  float power; //mW
};

class Kuroli {
  public:
    Kuroli(byte GAS);

    void sdInit(const byte chipSelect);
    void sdWrite(structIna *_structIna, structKuroli *_structKuroli);
    void serialLog(structIna *_structIna, structKuroli *_structKuroli);
    void displayInit(const byte kolom, const byte baris);
    void initIna219();
    void readIna219(structIna *_structIna219);
    void initIna226();
    void readIna226(structIna *_structIna226);
    void textIna(structIna *_structIna);
  
  private:
    LiquidCrystal_I2C *_lcd;
    Adafruit_INA219 *_ina;  //uncomment if using ina219
    INA226_WE *_ina226; //uncomment if using ina226
    HCSR04 *_hc;

    byte TRIG=5, ECHO=6;
    byte GAS;
};

#endif