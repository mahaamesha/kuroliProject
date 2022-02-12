/*
 * Source File
 * kuroli.cpp
*/

#include "kuroli.h"

// Konstruktor
// [class]::[konstruktor](param)
Kuroli::Kuroli(byte GAS)
{
  _lcd = new LiquidCrystal_I2C(0x27,16,2);
  _ina = new Adafruit_INA219;
  _ina226 = new INA226_WE;
  _hc = new HCSR04(TRIG,ECHO);    //set default pin

  pinMode(GAS, INPUT);
  
  Kuroli::GAS = GAS;
}

//SD FUNCTION
void Kuroli::sdInit(const byte chipSelect){
  while(!Serial);
  Serial.print("Inisialisasi SD Card... ");
  if (!SD.begin(chipSelect)){
    Serial.println("GAGAL.\n");
    while(true);  //jika gagal, infinite loop di sini
  }
  Serial.println("SUKSES.\n");

  File dataFile = SD.open("log.txt", FILE_WRITE);
  if (dataFile){  //jika file sudah terinisialisai
    //delimiter dengan tab --> excel bisa
    dataFile.println("V_LOAD \tCURRENT\tPOWER  \tDIST  \tPWMGAS \tV_SHUNT");  //baris pertama
    dataFile.println("(V)    \t(mA)   \t(mW)   \t(cm)  \t(V)    \t(mV)");  //satuan
    Serial.println("V_LOAD \tCURRENT\tPOWER  \tDIST  \tPWMGAS \tV_SHUNT");
    Serial.println("(V)    \t(mA)   \t(mW)   \t(cm)  \t(V)    \t(mV)");
    dataFile.close();
  }
}

void Kuroli::sdWrite(structIna *_structIna219, structKuroli *_structKuroli){
  File dataFile = SD.open("log.txt", FILE_WRITE);
  if (dataFile){  //jika file sudah terinisialisai
    dataFile.print((*_structIna219).loadVoltage);
    dataFile.print("\t"); //delimiter --> pindah kolom

    dataFile.print((*_structIna219).current);
    dataFile.print("\t");

    dataFile.print((*_structIna219).power);
    dataFile.print("\t");

    dataFile.print( _hc->dist() );
    dataFile.print("\t");

    dataFile.print((*_structKuroli).pwmGas);
    dataFile.print("\t");

    dataFile.print((*_structIna219).shuntVoltage);
    dataFile.print("\t");

    dataFile.println(); //pindah baris
    dataFile.close();
  }
  else Serial.println("Crushed on sdWrite(_param)");
}
//(END) SD FUNCTION

//SERIAL LOGGER FUNCTION
void Kuroli::serialLog(structIna *_structIna, structKuroli *_structKuroli){
  Serial.print((*_structIna).loadVoltage);
  Serial.print("\t");

  Serial.print((*_structIna).current);
  Serial.print("\t");

  Serial.print((*_structIna).power);
  Serial.print("\t");

  Serial.print(_hc->dist());
  Serial.print("\t");

  Serial.print((*_structKuroli).pwmGas);
  Serial.print("\t");

  Serial.print((*_structIna).shuntVoltage);
  Serial.print("\t");

  Serial.println(); //akhir baris
}
//(END) SERIAL LOGGER FUNCTION


//DISPLAY FUNCTION
void Kuroli::displayInit(const byte kolom, const byte baris){   //inisialisasi di setup
  _lcd->begin(kolom, baris);

  for(int i=0; i<3; i++){  //blinking 3 kali
    _lcd->backlight();
    delay(250);
    _lcd->noBacklight();
    delay(250);
  }
  _lcd->backlight();
  
  _lcd->setCursor(2,0);
  _lcd->print("SISTEM NYALA");
  Serial.println("SISTEM NYALA");
  delay(250);
  _lcd->clear();
}
//(END) DISPLAY FUNCTION


//INA219 FUNCTION
void Kuroli::initIna219(){
  _ina->begin();
  //_ina->setCalibration_32V_19A(); //uncomment this if not set in default
}

void Kuroli::readIna219(structIna *_structIna219){
  (*_structIna219).shuntVoltage = _ina->getShuntVoltage_mV();
  (*_structIna219).busVoltage = _ina->getBusVoltage_V();
  (*_structIna219).current = _ina->getCurrent_mA() * 10 / 2;  //Multiplied by 10 -> RSHUNT=0.01
  (*_structIna219).loadVoltage = (*_structIna219).busVoltage + ((*_structIna219).shuntVoltage / 1000);
  (*_structIna219).power = (*_structIna219).current * (*_structIna219).loadVoltage;
}
//(END) INA219 FUNCTION


//INA226 FUNCTION
void Kuroli::initIna226(){
  Wire.begin();
  _ina226->init();

  /* Set Resistor and Current Range
     resistor is 5.0 mOhm
     current range is up to 10.0 A
     default was 100 mOhm and about 1.3 A
  */
  //_ina226->setResistorRange(0.005, 10); //uncomment this if not set in default
  
  /* If the current values delivered by the INA226 differ by a constant factor
     from values obtained with calibrated equipment you can define a correction factor.
     Correction factor = current delivered from calibrated equipment / current delivered by INA226
  */
  // ina226.setCorrectionFactor(0.95);

  _ina226->waitUntilConversionCompleted();  //comment this, the first data might be zero
}

void Kuroli::readIna226(structIna *_structIna226){
  _ina226->readAndClearFlags();
  (*_structIna226).shuntVoltage = _ina226->getShuntVoltage_mV();
  (*_structIna226).busVoltage = _ina226->getBusVoltage_V();
  (*_structIna226).current = _ina226->getCurrent_mA();
  (*_structIna226).loadVoltage = (*_structIna226).busVoltage + ((*_structIna226).shuntVoltage / 1000);
  (*_structIna226).power = _ina226->getBusPower();

  //Overflow Check
  if(!_ina226->overflow) Serial.println("INA226: Values OK - no overflow");
  else Serial.println("INA226: Overflow! Choose higher current range");
}
//(END) INA226 FUNCTION


//TEXT INA FUNCTION
void Kuroli::textIna(structIna *_structIna){
  _lcd->clear();
  _lcd->setCursor(0,0);
  _lcd->print("mVs Vbs ImA Vo");  //15 karakter
  _lcd->setCursor(0,1);
  _lcd->print( String((*_structIna).shuntVoltage,1) + String(" ") );
  _lcd->print( String((*_structIna).busVoltage,1) + String(" ") );
  _lcd->print( String((*_structIna).current,1) + String(" ") );
  _lcd->print( String((*_structIna).loadVoltage,1) + String(" ") );
}
//(END) TEXT INA FUNCTION