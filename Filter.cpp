#include "Filter.h"

Filter::Filter(float tauConst, float kConst) {
  lastOutput = 0.0;
  tau = tauConst;
  k = kConst;
  alpha = 0.0, x1 = 0.0, x2 = 0.0, y1 = 0.0;
}

void Filter::complementary(float *output, float *angleAcc, float *gyroRate, unsigned int *dTime) {
  float dt = ((float)*dTime / 1000.0);
  if (dt > 0) {
    alpha = tau / (tau + dt);
    lastOutput = alpha * ( lastOutput + *gyroRate * dt ) + ( 1 - alpha) * *angleAcc;
    *output = lastOutput;
  }
}

/* TODO
void Filter::complementary2(float *output, float *angleAcc, float *gyroRate, unsigned int *dTime) {
  float dt = ((float)*dTime / 1000.0);
  if (dt > 0) {
    x1 = (*angleAcc - lastOutput)*k*k;
    y1 = dt*x1 + y1;
    x2 = y1 + (*angleAcc - lastOutput)*2*k + *gyroRate;
    lastOutput = dt*x2 + lastOutput;
    *output = lastOutput;
  }
}
*/



