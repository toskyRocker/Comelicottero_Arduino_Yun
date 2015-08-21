#include "ACC_GYRO_GY80.h"
#include "Arduino.h"
#include <Wire.h>
/***************************************************************************
  Andrea Toscano 2015
  Comelicottero - Quadcopter
  Universita' degli Studi di Milano
  Informatica Magistrale
  This is a library for the ADXL345 and L3G4200D (IMU-GY80)  
  These displays use I2C to communicate, 2 pins are required to interface.
 ***************************************************************************/
ACC_GYRO_GY80::ACC_GYRO_GY80() {
 
  ADXL_Gain[0] = 0.9582;
  ADXL_Offset[0] = -3.0710;
  ADXL_Gain[1] = 0.9658;
  ADXL_Offset[1] = -1.7385;
  ADXL_Gain[2] = 1.0308;
  ADXL_Offset[2] = 12.6018;
}

bool ACC_GYRO_GY80::init() {  
//  Wire.begin();
  // L3G4200D
  writeCmd(L3G4_ADDRESS, CTRL_REG1, L3G4_BW_200_50);
  writeCmd(L3G4_ADDRESS, CTRL_REG2, L3G4_HPF1);
  writeCmd(L3G4_ADDRESS, CTRL_REG3, L3G4_INTERRUPT);
  writeCmd(L3G4_ADDRESS, CTRL_REG4, MODE_2000);
  writeCmd(L3G4_ADDRESS, CTRL_REG5, L3G4_LPF2); 
  // ADXL345
  writeCmd(ADXL_ADDRESS, REG_DATA_FORMAT, RANGE_8);
  writeCmd(ADXL_ADDRESS, REG_PWR_CTL, MEASURE_MODE);
  writeCmd(ADXL_ADDRESS, REG_BW_RATE, MODE_200_100);
  computeGyroOffset();  
  //computeAccDynamicOffset();
  return true;
}

void ACC_GYRO_GY80::readGyroData(float buff[]) {
  byte buffer[6];
  readCmd(L3G4_ADDRESS, READALLSIX, 6, buffer);
  buff[0] = (float)( (((buffer[1] << 8) | buffer[0]) - L3G4_Offset[0]) * SCALE_2000);
  buff[1] = (float)( (((buffer[3] << 8) | buffer[2]) - L3G4_Offset[1]) * SCALE_2000);
  buff[2] = (float)( (((buffer[5] << 8) | buffer[4]) - L3G4_Offset[2]) * SCALE_2000);
}


// Computes the Gyroscope Offset based on N_SAMPLES
void ACC_GYRO_GY80::computeGyroOffset() {
  float temp[N_AXIS] = {0.0, 0.0, 0.0};
  // Summing N_SAMPLES samples
  for (int samples = 0; samples < N_SAMPLES_L3G4; samples++) {
    sumGyroSamples();
  }
  // Dividing samples / N_SAMPLES and applying the right SCALE FACTOR
  for (uint8_t axis = 0; axis < N_AXIS; axis++) {
    temp[axis] = ((float)(gyroSamplesArray[axis]) / N_SAMPLES_L3G4);
    gyroSamplesArray[axis] = 0;
  }
  gyroSamplesCounter = 0;
  for (uint8_t i = 0; i < N_AXIS; i++ ) {
    L3G4_Offset[i] = temp[i];
  }
}


void ACC_GYRO_GY80::sumGyroSamples() {
  byte buffer[6];
  readCmd(L3G4_ADDRESS, READALLSIX, 6, buffer);
  gyroSamplesArray[0] += (float)((buffer[1] << 8) | buffer[0]);
  gyroSamplesArray[1] += (float)((buffer[3] << 8) | buffer[2]);
  gyroSamplesArray[2] += (float)((buffer[5] << 8) | buffer[4]);
  gyroSamplesCounter++;
}


void ACC_GYRO_GY80::readAccData(float buff[]) {
  byte buffer[6];
  readCmd(ADXL_ADDRESS, DATAX0, 6, buffer);
  buff[0] = (((buffer[1] << 8) | buffer[0]) * ADXL_Gain[0]  + ADXL_Offset[0]);
  buff[1] = (((buffer[3] << 8) | buffer[2]) * ADXL_Gain[1]  + ADXL_Offset[1]);
  buff[2] = (((buffer[5] << 8) | buffer[4]) * ADXL_Gain[2]  + ADXL_Offset[2]);
}


// Read with I2C
void ACC_GYRO_GY80::readCmd(byte addr, byte reg, byte num, byte buff[]) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, num);
  while (Wire.available() < num);
  for (byte i = 0; i < num; i++) {
    buff[i] = Wire.read();
  }
}

// Write with I2C
void ACC_GYRO_GY80::writeCmd(byte addr, byte reg, byte val) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}
