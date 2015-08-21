byte accCounter = 0;
float sumAccX = 0, sumAccY = 0, sumAccZ = 0;
// Acc average values
float accXAverage = 0, accYAverage = 0, accZAverage = 0;


void readIMU() {  
    readGyro();
    readAcc();
    if (accCounter % ACC_COMPUTE_COUNTER == 0) {
        accXAverage = sumAccX / ACC_COMPUTE_COUNTER;
        accYAverage = sumAccY / ACC_COMPUTE_COUNTER;
        accZAverage = sumAccZ / ACC_COMPUTE_COUNTER;
        accYDegree = atan2(accYAverage, accZAverage) * RAD_TO_DEG; // Roll from Accelerometer
        accXDegree = atan2(accXAverage, sqrt(accYAverage*accYAverage + accZAverage*accZAverage)) * RAD_TO_DEG; // Pitch from Accelerometer
        accCounter = 0;
        sumAccX = 0, sumAccY = 0, sumAccZ = 0;
    }  
    
    // FIRST ORDER COMPLEMENTARY FILTER
    pitchFilter.complementary(&pitchFusion, &accXDegree, &gyroYAverage, &dt);
    rollFilter.complementary(&rollFusion,  &accYDegree, &gyroXAverage, &dt);
   
    
#ifdef DEBUG
  Serial.print("[sensors] accXDegree: ");
  Serial.print(accXDegree);
  Serial.print(", accYDegree: ");
  Serial.println(accYDegree);
  Serial.print("[sensors] pitchFusion: ");
  Serial.print(pitchFusion);
  Serial.print(", rollFusion: ");
  Serial.println(rollFusion);
  #endif
 // }
}

/*
Reading raw data (bits) from gyroscope
*/
void readGyro() {

  // Reading Gyro values
  float buffer[3];
  float tempX, tempY, tempZ;
  gy80.readGyroData(buffer);

  //Choose the signs of each axis
  //It depends on your IMU position
  tempX = -1 *  buffer[XAXIS];
  tempY = -1 * buffer[YAXIS];
  tempZ = buffer[ZAXIS];


  // Gyro filter
  
  gyroXAverage = (ALPHA_IIR * gyroXAverage + (1.0 - ALPHA_IIR) * tempX);
  gyroYAverage = (ALPHA_IIR * gyroYAverage + (1.0 - ALPHA_IIR) * tempY);
  gyroZAverage = (ALPHA_IIR * gyroZAverage + (1.0 - ALPHA_IIR) * tempZ);
  

#ifdef DEBUG
  Serial.print("[sensors] gyroX: ");
  Serial.print(tempX);
  Serial.print(", gyroY: ");
  Serial.print(tempY);
  Serial.print(", gyroZ: ");
  Serial.println(tempZ);
#endif

}

void readAcc() {
  // Declaring buffer for acc data
  float buffer[3];
  float tempX, tempY, tempZ;
  // Reading values from ADXL345
  gy80.readAccData(buffer);

  tempX = -1 * buffer[XAXIS];
  tempY = buffer[YAXIS];
  tempZ = -1 * buffer[ZAXIS];

  sumAccX += tempX;
  sumAccY += tempY;
  sumAccZ += tempZ;
  accCounter++;

#ifdef DEBUG
  Serial.print("[sensors] accX: ");
  Serial.print(tempX);
  Serial.print(", accY: ");
  Serial.print(tempY);
  Serial.print(", accZ: ");
  Serial.println(tempZ);
  Serial.println("");
 #endif

}


