// GLOBAL VARS
// argh!
#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_

/**** TIMERS ****/
unsigned long routineComputeTimer = 0;
unsigned long slowRoutineCounter = 0;
unsigned long naviCounter = 0;
unsigned long connectionTimer = 0;

// Fast routine
unsigned int dt = 0;

/**** COMMANDS ****/
int throttle;
float throttleOffset;
float rollOut, pitchOut, yawOut;
int  UIyaw, UIpitch, UIroll;
int UIthr;
unsigned int timeout = 0;
bool startStop = false;
bool positionHoldingStartStop = false;
bool on = 0, off = 0, positionHoldingOn = 0, positionHoldingOff = 0;

/**** MOTORS ****/
uint8_t motors[N_MOTORS] = {motor0, motor1, motor2, motor3};
// Motors' values
float motorValue[N_MOTORS] = {MIN_SPEED,MIN_SPEED,MIN_SPEED,MIN_SPEED };
float motorIIR[N_MOTORS];
float motorIIROld[N_MOTORS];
float motorIIROldOld[N_MOTORS];


/*Sensor variable */
// Gyro average values
float gyroXAverage = 0, gyroYAverage = 0, gyroZAverage = 0;
// Output values from sensor fusion
float pitchFusion = 0, rollFusion = 0;
float accXDegree = 0, accYDegree = 0;

// Magnetometer
float oldHeading = 0;

// GPS
static const uint8_t SLAVE_ADDRESS = 1;
const uint8_t dataCount = 4;
float gpsSetpoint[3] = {0,0,0};

/* Data structure to save gps data coming from Arduino Micro*/
union {
 // Altitude - Latitude - Longitude - N_Sat
 float gpsData[dataCount] = {0,0,0};
 uint8_t rawData[dataCount*sizeof(float)];
}unionGpsData;


#endif
