// GLOBAL DEFINE VARS
#ifndef CONFIG_H_
#define CONFIG_H_


/*UNCOMMENT FOR DEBUG MODE ON */
//#define DEBUG 1

/* BAUD SERIAL MONITOR */
#define SERIAL_BAUD 115200


/* ESC */
#define STOP_SPEED 2200
#define MIN_SPEED 2800
#define MAX_SPEED 3800
#define NO_CONNECTION_SPEED 3000

/* MOTORS */
#define N_MOTORS 4
#define motor0 OCR3A // 0 pin 5
#define motor1 OCR1C  //1 pin 11
#define motor2 OCR1B // 2 pin 10 
#define motor3 OCR1A // 3 pin 9

/* NAVIGATION PID */
#define NAVIGATION_KP 0.6
#define NAVIGATION_MAXV 5.0
#define NAVIGATION_MINV -5.0


/* PID */
#define ROLL_PID_KP_ANGLE 3
#define ROLL_PID_KP_RATE 0.12
#define ROLL_PID_KI 1.35
#define ROLL_PID_KD 0.004
#define ROLL_PID_MAXV 300
#define ROLL_PID_MINV -300

#define PITCH_PID_KP_ANGLE 3
#define PITCH_PID_KP_RATE 0.12
#define PITCH_PID_KI 1.35
#define PITCH_PID_KD 0.004
#define PITCH_PID_MAXV 300
#define PITCH_PID_MINV -300

#define YAW_PID_KP_ANGLE 1
#define YAW_PID_KP_RATE 1
#define YAW_PID_KI 3
#define YAW_PID_KD 0.003
#define YAW_PID_MAXV 300
#define YAW_PID_MINV -300

// IMU & PID computation SampleTime in ms
#define FAST_ROUTINE_COMPUTE_TIMER 8 // e.g.  5ms = 200Hz  8ms = 125Hz

#define F_NUM  2  // Hz
#define F_DEN  10 // Hz
#define T (FAST_ROUTINE_COMPUTE_TIMER / 1000.0)
#define A (1- (T * F_NUM))
#define B (1- (T * F_DEN))



/* SENSORS */
#define XAXIS 0
#define YAXIS 1
#define ZAXIS 2
#define RAD_TO_DEG 180.0/PI
#define DEG_TO_RAD PI/180.0

// Hipass Filter Coeff for Gyro Data
#define ALPHA_IIR 0.82
// Complementary Filter Parameter
#define TAU_CF 0.49
// Complementary Filter 2 Parameter
#define K_CF2 1.1
// Scale factor for Accelerometer Computation : Accelerometer_Freq = Routine_Freq/ACC_COMPUTE_TIMER
#define ACC_COMPUTE_COUNTER 3

/* GPS */
#define EARTH_RADIUS 6372795
// Scale factor for Navigation routine
#define NAVI_COMPUTE_COUNTER 100 //  1/100 speed of fast routines's frequency


/* MAGNETOMETER */
#define SLOW_ROUTINE_COMPUTE_COUNTER 15
// Choose your declination in rad here:
// http://www.magnetic-declination.com/
#define DECLINATION_ANGLE 0.03729 // MILAN declination in rad
#define ALPHA_MAG 0.85

/* COMMANDS */
#define YAW_MIN 0.0
#define YAW_MAX 256.0
#define YAW_MIN_ANGLE -30.0
#define YAW_MAX_ANGLE 30.0

#define PITCH_MIN 0.0
#define PITCH_MAX 256.0
#define PITCH_MIN_ANGLE -10.0
#define PITCH_MAX_ANGLE 10.0

#define ROLL_MIN 0.0
#define ROLL_MAX 256.0
#define ROLL_MIN_ANGLE -10.0
#define ROLL_MAX_ANGLE 10.0

#define THR_IN_MIN  0.0
#define THR_IN_MAX  256.0
#define THR_OUT_MIN -1.2
#define THR_OUT_MAX 1.2
#define THR_RANGE (MAX_SPEED - MIN_SPEED)

/* SERIAL1 CONNECTION*/
#define CONNECTION_TIMER 10

#endif
