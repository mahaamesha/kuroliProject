PROJECT KUROLI
@programmer mahaamesha

Program untuk monitoring konsumsi daya, jarak, dan waktu penggunaan kursi roda lipat.\
\
Format:\
time | load voltage | current_mA | power consumption | distance\




1 Desember 2021\
Folder src: hcsr04, ina219, lcd.\
Sketch main:\
  1. inisialisasi lcd, hcsr, sd, ina219;
  2. baca ina219;
  3. tampilkan ke LCD (voltageshunt, busvoltage, current_mA, loadvoltage);
  4. simpan ke sd card (busvoltage, current_mA, distance, pwmGas);
  5. tampilkan log ke serial.\
File PinConnection: SD, LCD, HCSR
