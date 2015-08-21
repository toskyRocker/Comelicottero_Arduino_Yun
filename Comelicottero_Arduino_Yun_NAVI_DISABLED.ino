/*
University: Universita' degli Studi di Milano - Master in Computer Science
Address: Via Comelico 39, Milano
Thesis: Comelicottero Quadcopter
Built and programmed by
Andrea Toscano: andrea.toscano1@studenti.unimi.it    

Quad-X Configuration
       
pin 5 FRONT LEFT          pin 11  FRONT RIGHT
              \         / 
                \ --- /
                 |   |
                / --- \
              /         \ 
pin 9 REAR LEFT          pin 10  REAR RIGHT

                          ----------  
----    VCC -> 5V        | Arduino  |
GPS |   Rx  -> Tx        |          |
----    Tx  -> Rx        |  Micro   |
        GND -> GND        ----------
                                     
-----   V_IN ->  5V       ----------       
IMU  |  GND  ->  GND     | Arduino  | 
-----   SCL  ->  SCL     |          | 
        SDA  ->  SDA     |   Yun    |
                          ----------

+++copy the line below on Linino's console to execute tcp-serial redirect+++
>>
python tcp_serial_redirect.py -p /dev/ttyATH0 -b 115200 -P 8888 

*/

// Standard Arduino Libraries
#include <Wire.h>

// Comelicottero Libraries
#include "config.h"
#include "globalVariables.h"
#include "PID.h"
#include "ACC_GYRO_GY80.h"
#include "HMC5883_GY80.h"
#include "Filter.h"
#include "Protocol.h"


// Three instances of PIDController
PID PIDpitch(PITCH_PID_MAXV, PITCH_PID_MINV);
PID PIDroll(ROLL_PID_MAXV, ROLL_PID_MINV);
PID PIDyaw(YAW_PID_MAXV, YAW_PID_MINV);

// IMU instance
ACC_GYRO_GY80 gy80;

// UNCOMMENT TO ENABLE NAV
// Magnetometer instance  
//HMC5883 mag;

// Filter instances
Filter pitchFilter(TAU_CF, K_CF2);
Filter rollFilter(TAU_CF, K_CF2);

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(SERIAL_BAUD);
  Wire.begin();
  initPID(); // Init PIDs' parameters
  
  while(!gy80.init());  // Init GY80's sensors
// UNCOMMENT TO ENABLE NAV
//  while(!mag.init()); // Init magnetometer
    globalReset();
    initMotors(); // Init motors with min values
    writeToAllMotors(STOP_SPEED);
    // Start timer, useful for updating IMU values
    routineComputeTimer = millis(); // IMU and control system
    connectionTimer = millis(); // no connection timer
}

void loop() {
  if(millis() - connectionTimer >= CONNECTION_TIMER) {
    connectionTimer = millis();
    if(timeout > 0) {
          timeout--;
       }
      // when it runs over CONNECTION_TIMER
      // Safe procedure
      else{
          UIroll = 0.0;
          UIpitch = 0.0;
          UIyaw = 0.0;
          throttle = NO_CONNECTION_SPEED;
      }
  }// End connectionTimer
  
   readValuesFromSerial1(); // read commands from user

   dt = millis() - routineComputeTimer; 
   if(dt >= FAST_ROUTINE_COMPUTE_TIMER) { 
      routineComputeTimer = millis();
      slowRoutineCounter++;
    
   if( startStop == true) { 
      computeFastRoutine(); // Fast routine IMU+PID
    }
   if(slowRoutineCounter % SLOW_ROUTINE_COMPUTE_COUNTER == 0) {
      slowRoutineCounter = 0; 
      // UNCOMMENT TO ENABLE NAV
     // computeSlowRoutine();  // Slow routine: GPS and Compass
   }// END SLOW_ROUTINE
 }// END FAST_ROUTINE
} // END LOOP


/*void computeSlowRoutine() {
  calcHeading(); // Heading calculation with magnetometer
  getGPSPosition(1); // Receiving from Micro GPS info.
}
*/
void computeFastRoutine() {
  readIMU(); // read sensor values from IMU
  flightControl(); // PID routine  
}


void globalReset() {
      resetI();
      resetOutput();
      resetSetpoint();
}
