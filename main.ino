/*
 * Main File - Project Kuroli
 * main.ino
 */

#include <Wire.h>
#include "src/lcd/LiquidCrystal_I2C.h"
#include "src/ina219/Adafruit_INA219.h"
#include "src/hcsr04/HCSR04.h"
#include <SPI.h>
#include <SD.h>

//Class   konstruktor(addr, en,rw,rs,d4,d5,d6,d7,bl,blpol)  //untuk lib proteus
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const byte CS = 4;  //pin chipSelect modul sd card
const byte TRIG = 5;  //pin hcsr04
const byte ECHO = 6;


LiquidCrystal_I2C lcd(0x27,16,2);
Adafruit_INA219 ina219; //address 0x40
HCSR04 hc(TRIG,ECHO); //pin (trig,echo)

struct structKuroli {
  float pwmGas; //besar sinyal pwm gas tangan
  //tambah yg lain di sini
};

struct structIna219 {
  float shuntvoltage; //mV
  float busvoltage;   //V
  float current_mA;   //mA
  float loadvoltage;  //V
};

void sdInit(const byte chipSelect);
void sdWrite(structIna219 _ina, HCSR04 _hc, structKuroli _kuroli);
void serialLog(structIna219 _ina, HCSR04 _hc, structKuroli _kuroli);
void displayInit(byte kolom, byte baris);
void readIna219(structIna219 _ina);
void textIna219(structIna219 _ina);

void setup(){
  Serial.begin(9600);

  sdInit(CS); //chipSelect modul sd card
  ina219.begin();
  displayInit(16,2);
}

void loop(){
  structIna219 myIna219 = {0,0,0,0};
  structKuroli myKuroli = {0};
  readIna219(myIna219);
  sdWrite(myIna219, hc, myKuroli);
  textIna219(myIna219);
}

//SD FUNCTION
void sdInit(const byte chipSelect){
  while(!Serial);
  Serial.print("Inisialisasi SD Card... ");
  if (!SD.begin(chipSelect)){
    Serial.println("GAGAL");
    while(true);  //jika gagal, infinite loop di sini
  }
  Serial.println("SUKSES");

  File dataFile = SD.open("log.txt", FILE_WRITE);
  if (dataFile){  //jika file sudah terinisialisai
    //delimiter dengan tab --> excel bisa
    dataFile.println("Vb\t I\t DIST\t PWM");  //baris pertama
    dataFile.println("(V)\t (mA)\t (cm)\t (V)");  //satuan
    Serial.println("Vb\t I\t DIST\t PWM");
    Serial.println("(V)\t (mA)\t (cm)\t (V)");
    dataFile.close();
  }
}

void sdWrite(structIna219 _ina, HCSR04 _hc, structKuroli _kuroli){
  File dataFile = SD.open("log.txt", FILE_WRITE);
  if (dataFile){  //jika file sudah terinisialisai
    dataFile.print(_ina.busvoltage);
    dataFile.print("\t"); //delimiter --> pindah kolom

    dataFile.print(_ina.current_mA);
    dataFile.print("\t");

    dataFile.print( _hc.dist() );
    dataFile.print("\t");

    dataFile.print(_kuroli.pwmGas);
    dataFile.print("\t");

    dataFile.println(); //pindah baris
    dataFile.close();
  }
  else Serial.println("GAGAL WRITE LOG");
}
//(END) SD FUNCTION

//SERIAL LOGGER FUNCTION
void serialLog(structIna219 _ina, HCSR04 _hc, structKuroli _kuroli){
  Serial.print(_ina.busvoltage);
  Serial.print("\t");

  Serial.print(_ina.current_mA);
  Serial.print("\t");

  Serial.print(_hc.dist());
  Serial.print("\t");

  Serial.print(_kuroli.pwmGas);
  Serial.print("\t");

  Serial.println(); //akhir baris
}
//(END) SERIAL LOGGER FUNCTION


//DISPLAY FUNCTION
void displayInit(const byte kolom, const byte baris){   //inisialisasi di setup
  lcd.begin(kolom, baris);

  for(int i=0; i<3; i++){  //blinking 3 kali
    lcd.backlight();
    delay(250);
    lcd.noBacklight();
    delay(250);
  }
  lcd.backlight();
  
  lcd.setCursor(2,0);
  lcd.print("SISTEM NYALA");
  Serial.println("SISTEM NYALA");
  delay(1000);
  lcd.clear();
}
//(END) DISPLAY FUNCTION


//INA219 FUNCTION
void readIna219(structIna219 _ina){
  _ina.shuntvoltage = ina219.getShuntVoltage_mV();
  _ina.busvoltage = ina219.getBusVoltage_V();
  _ina.current_mA = ina219.getCurrent_mA();
  _ina.loadvoltage = _ina.busvoltage + (_ina.shuntvoltage / 1000);
}

void textIna219(structIna219 _ina){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("mVs Vbs ImA Vo");  //15 karakter
  lcd.setCursor(0,1);
  lcd.print( String(_ina.shuntvoltage,1) + String(" ") );
  lcd.print( String(_ina.busvoltage,1) + String(" ") );
  lcd.print( String(_ina.current_mA,1) + String(" ") );
  lcd.print( String(_ina.loadvoltage,1) + String(" ") );
}
//(END) INA219 FUNCTION