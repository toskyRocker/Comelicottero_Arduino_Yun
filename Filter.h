/***************************************************************************
  Andrea Toscano 2015
  Comelicottero - Quadcopter
  Universita' degli Studi di Milano
  Informatica Magistrale
  This is a library to handle filters
  
 ***************************************************************************/

#ifndef FILTER_H
#define FILTER_H

#include "Arduino.h"

class Filter {
  
  public:
    Filter(float tau, float k);
    void complementary(float *output, float *angleAcc, float *gyroRate, unsigned int *dtime);
    void complementary2(float *output, float *angleAcc, float *gyroRate, unsigned int *dtime);
  
  private:
    float tau, k, alpha;
    float lastOutput;
    float x1, x2, y1;
};
#endif
