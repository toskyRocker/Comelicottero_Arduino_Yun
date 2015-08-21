/***************************************************************************
  Andrea Toscano 2015
  Comelicottero - Quadcopter
  Universita' degli Studi di Milano
  Informatica Magistrale
  This is a library for PID control system
  It requires to specify RC  and coefficients' values
 ***************************************************************************/

#ifndef PID_H_
#define PID_H_

#include "Arduino.h"
#include "config.h"

class PID
{
  public:
  
    PID(int maxV, int minV);

    void setParameters(float setKpAngle, float setKpRate, float setki, float setkd);
    float compute(float setPoint, float angle, float gyro, unsigned int *dTime);
    void resetPIDVariables();
    
    
    int MaxValue;
    int MinValue;

  private:
    
    const float RC = 1/(2*PI*20); // Everything above 20Hz is noise.
    float kpRate; // kp on Angle
    float kpAngle; // kp on Angular Speed
    float ki;
    float kd;
    float I;
    float D;
    float previousError; 
    float newI; // 
    float errorOldI;

};

#endif

