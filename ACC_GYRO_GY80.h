/***************************************************************************
  Andrea Toscano 2015
  Comelicottero - Quadcopter
  Universita' degli Studi di Milano
  Informatica Magistrale
  This is a library for the ADXL345 and L3G4200D (IMU-GY80)  
  These displays use I2C to communicate, 2 pins are required to interface.
 ***************************************************************************/

#ifndef ACC_GYRO_GY80_H
#define ACC_GYRO_GY80_H

#include "Arduino.h"
#define N_AXIS 3

// L3G4200D
#define L3G4_ADDRESS 0b01101001 //final 1 or 0?

#define CTRL_REG1        0x20
#define CTRL_REG2        0x21
#define CTRL_REG3        0x22
#define CTRL_REG4        0x23
#define CTRL_REG5        0x24
#define L3G4_HPF1        0x04
#define L3G4_LPF2        0x00
#define L3G4_INTERRUPT   0x00
#define L3G4_BW_100_25   0x1F
#define L3G4_BW_200_50   0x6F
#define L3G4_BW_400_25   0x9F
#define L3G4_BW_400_50   0xAF

#define MODE_250        0x00
#define MODE_500        0x10 
#define MODE_2000       0x30
#define SCALE_250       0.00875
#define SCALE_500       0.0175
#define SCALE_2000      0.070

#define READALLSIX      0x28 | (1 << 7)
#define OUT_X_L         0x28
#define OUT_X_H         0x29
#define OUT_Y_L         0x2A
#define OUT_Y_H         0x2B
#define OUT_Z_L         0x2C
#define OUT_Z_H         0x2D

#define N_SAMPLES_L3G4  20.0

// ADXL345
#define ADXL_ADDRESS 0b01010011  // 0x53
#define REG_DATA_FORMAT 0x31
#define REG_PWR_CTL 0x2D
#define REG_BW_RATE 0x2C

#define RANGE_2      0x08 //  +-2g
#define RANGE_4      0x09 //  +-4g
#define RANGE_8      0x0A //  +-8g
#define RANGE_16     0x0B // +-16g
#define MODE_400_200 0x0C // 400Hz Output Data Rate, 200Hz Bandwidth
#define MODE_200_100 0x0B // 200Hz Output Data Rate, 100Hz Bandwidth
#define MODE_100_50  0x0A // 100Hz Output Data Rate, 50Hz Bandwidth
#define MODE_50_25   0x09 //  50Hz Output Data Rate, 25Hz Bandwidth
#define MEASURE_MODE 0x08

#define DATAX0  0x32  //LSB
#define DATAX1  0x33  //MSB
#define DATAY0  0x34  //LSB
#define DATAY1  0x35  //MSB
#define DATAZ0  0x36  //LSB
#define DATAZ1  0x37  //MSB

// Scale factor: mg/LSB -> g
//#define ADXL_SCALE_FACTOR 0.0039
// Num of samples for Offset computation
#define N_SAMPLES_ADXL 200.0
//#define GRAVITY 9.80665


class ACC_GYRO_GY80 {

    public:
        ACC_GYRO_GY80();
        bool init();
        // L3G4
        void readGyroData(float buff[]);
        float L3G4_Offset[N_AXIS] = {0.0, 0.0, 0.0};
        // ADXL
        void readAccData(float buff[]);
        float ADXL_Offset[N_AXIS] = {0.0, 0.0, 0.0};
     //   float ADXL_Dynamic_Offset[N_AXIS] = {0.0, 0.0, 0.0};
        float ADXL_Gain[N_AXIS] = {0.0, 0.0, 0.0};

    private:
        // L3G4
        void sumGyroSamples();
        void computeGyroOffset();
        float gyroSamplesArray[N_AXIS] = {0.0, 0.0, 0.0};
        int gyroSamplesCounter = 0;
        // ADXL
      //  void sumAccSamples();
     //   void computeAccDynamicOffset();
        float accSamplesArray[N_AXIS] = {0.0, 0.0, 0.0};
     //   int accSamplesCounter = 0;
        // I2C communication using Wire Library
        void readCmd(byte addr, byte reg, byte num, byte buff[]);
        void writeCmd(byte addr, byte reg, byte val);
};
#endif
