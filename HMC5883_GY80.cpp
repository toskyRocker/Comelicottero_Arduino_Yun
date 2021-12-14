#include "HMC5883_GY80.h"
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

static float _hmc5883_Gauss_LSB_XY = 1100.0F;  // Varies with gain
static float _hmc5883_Gauss_LSB_Z  = 980.0F;   // Varies with gain

HMC5883::HMC5883(){
  
}

bool HMC5883::init() {
    
   // Wire.begin();
    // Enable the magnetometer
    write8(HMC5883_ADDRESS, HMC5883_REGISTER_MAG_MR_REG_M, 0x00);
    // Specifies magnetometer's gain
    setMagGain(HMC5883_MAGGAIN_1_3);
    return true;
}

// Write on I2C
void HMC5883::write8(byte addr, byte reg, byte val)
{
  Wire.beginTransmission(addr);
  #if ARDUINO >= 100
    Wire.write((uint8_t)reg);
    Wire.write((uint8_t)val);
  #else
    Wire.send(reg);
    Wire.send(val);
  #endif
  Wire.endTransmission();
}

// Read on I2C
byte HMC5883::read8(byte address, byte reg)
{
  byte value;

  Wire.beginTransmission(address);
  #if ARDUINO >= 100
    Wire.write((uint8_t)reg);
  #else
    Wire.send(reg);
  #endif
  Wire.endTransmission();
  Wire.requestFrom(address, (byte)1);
  #if ARDUINO >= 100
    value = Wire.read();
  #else
    value = Wire.receive();
  #endif  
  return value;
}

// Reading X, Y, Z Values from Magnetometer
void HMC5883::calcMagData()
{
  Wire.beginTransmission((byte)HMC5883_ADDRESS);
  #if ARDUINO >= 100
    Wire.write(HMC5883_REGISTER_MAG_OUT_X_H_M);
  #else
    Wire.send(HMC5883_REGISTER_MAG_OUT_X_H_M);
  #endif
  Wire.endTransmission();
  Wire.requestFrom((byte)HMC5883_ADDRESS, (byte)6);

  // Note high before low (different than accel)  
  #if ARDUINO >= 100
    uint8_t xhi = Wire.read();
    uint8_t xlo = Wire.read();
    uint8_t zhi = Wire.read();
    uint8_t zlo = Wire.read();
    uint8_t yhi = Wire.read();
    uint8_t ylo = Wire.read();
  #else
    uint8_t xhi = Wire.receive();
    uint8_t xlo = Wire.receive();
    uint8_t zhi = Wire.receive();
    uint8_t zlo = Wire.receive();
    uint8_t yhi = Wire.receive();
    uint8_t ylo = Wire.receive();
  #endif
  
  // Shift values to low bit first
  _magData.x = ((int16_t)(xlo | ((int16_t)xhi << 8))) / _hmc5883_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
  _magData.y = ((int16_t)(ylo | ((int16_t)yhi << 8))) / _hmc5883_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
  _magData.z = ((int16_t)(zlo | ((int16_t)zhi << 8))) / _hmc5883_Gauss_LSB_Z * SENSORS_GAUSS_TO_MICROTESLA;
  }

void HMC5883::setMagGain(hmc5883MagGain gain)
{
  write8(HMC5883_ADDRESS, HMC5883_REGISTER_MAG_CRB_REG_M, (byte)gain);
  _magGain = gain;
 
  switch(gain)
  {
    case HMC5883_MAGGAIN_0_88:
      _hmc5883_Gauss_LSB_XY = 1370;
      _hmc5883_Gauss_LSB_Z  = 1100;
      break; 
    case HMC5883_MAGGAIN_1_3:
      _hmc5883_Gauss_LSB_XY = 1100;
      _hmc5883_Gauss_LSB_Z  = 980;
      break;
    case HMC5883_MAGGAIN_1_9:
      _hmc5883_Gauss_LSB_XY = 855;
      _hmc5883_Gauss_LSB_Z  = 760;
      break;
    case HMC5883_MAGGAIN_2_5:
      _hmc5883_Gauss_LSB_XY = 670;
      _hmc5883_Gauss_LSB_Z  = 600;
      break;
    case HMC5883_MAGGAIN_4_0:
      _hmc5883_Gauss_LSB_XY = 450;
      _hmc5883_Gauss_LSB_Z  = 400;
      break;
    case HMC5883_MAGGAIN_4_7:
      _hmc5883_Gauss_LSB_XY = 400;
      _hmc5883_Gauss_LSB_Z  = 255;
      break;
    case HMC5883_MAGGAIN_5_6:
      _hmc5883_Gauss_LSB_XY = 330;
      _hmc5883_Gauss_LSB_Z  = 295;
      break;
    case HMC5883_MAGGAIN_8_1:
      _hmc5883_Gauss_LSB_XY = 230;
      _hmc5883_Gauss_LSB_Z  = 205;
      break;
  } 
}
