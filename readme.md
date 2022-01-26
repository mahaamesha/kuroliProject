PROJECT KUROLI
@programmer mahaamesha

Program untuk monitoring konsumsi daya, jarak, dan waktu penggunaan kursi roda lipat.\
\
Format Target:\
time | load voltage | current_mA | power consumption | distance\




[`1-Des-2021`]\
Folder src: hcsr04, ina219, lcd.\
Sketch main:\
  1. inisialisasi lcd, hcsr, sd, ina219;
  2. baca ina219;
  3. tampilkan ke LCD (voltageshunt, busvoltage, current_mA, loadvoltage);
  4. simpan ke sd card (busvoltage, current_mA, distance, pwmGas);
  5. tampilkan log ke serial.\
File PinConnection: SD, LCD, HCSR

[`26-Jan-2022`]\
Add custom calibration for INA219.\
setCalibration_32V_19A()\
For the calculation, check INA219_Calibration.xlsx