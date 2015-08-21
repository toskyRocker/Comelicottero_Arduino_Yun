#include "Arduino.h"
#include "PID.h"

// PID's output bounds
PID::PID(int maxV, int minV) {
  kpAngle = 0.0;
  kpRate = 0.0;
  ki = 0.0;
  kd = 0.0;
  I = 0.0;
  previousError = 0.0;
  D = 0.0;
  MaxValue = maxV;
  MinValue = minV;
}

// Settings PID's coefficients 
void PID::setParameters(float setKpAngle, float setKpRate, float setki, float setkd) {

  kpAngle = setKpAngle;
  kpRate = setKpRate;
  ki = setki;
  kd = setkd;
}

// PID main routine
float PID::compute(float setPoint, float angle, float gyro, unsigned int *dTime) {
  
  float dt = ((float)*dTime / 1000.0); // calculating new dt time based on current dTime
  float D_filter = dt / (dt + RC); // Low pass filter coefficient
  float error = (kpAngle*(setPoint - angle) - gyro); // Calculating error value
  // Computing proportional 
  float P = kpRate * error;
   // Computing integrative 
  float newI = (ki*dt*0.5) * (error + previousError); 
  // Computing derivative
  //D = kf * D + kd * (error - previousError);
  float newD = (error - previousError)/dt;
  // Low pass filter on D
  D = D + D_filter * (newD - D);
  D *= kd;
  //  PID output 
  float output = P + I + newI + D ;
  // Check output direction
  if ((error * output > 0) && ((output > MaxValue) || (output < MinValue))) {	
  	I += ki * errorOldI;  // Special case
    errorOldI = 0;
  }
  else {  // Normal case
  	I += newI;
  	errorOldI = error;
  }
  // Updating previous error
  previousError = error;
  // Output
  output = P + I + D;
  return constrain(output, MinValue, MaxValue); 
}

// Resetting I 
void PID::resetPIDVariables() {
  // All these values must be 0 on reset, otherwise performance is compromised
  I = 0.0; 
  errorOldI = 0.0;
  D = 0.0;
}

