// Setting PIDs coefficients
void initPID() {
        PIDroll.setParameters( ROLL_PID_KP_ANGLE,ROLL_PID_KP_RATE, ROLL_PID_KI, ROLL_PID_KD);
	PIDpitch.setParameters(PITCH_PID_KP_ANGLE,PITCH_PID_KP_RATE, PITCH_PID_KI, PITCH_PID_KD);
	PIDyaw.setParameters(YAW_PID_KP_ANGLE, YAW_PID_KP_RATE, YAW_PID_KI, YAW_PID_KD); 
}

void resetI() {
      PIDroll.resetPIDVariables(); 
      PIDpitch.resetPIDVariables();
      PIDyaw.resetPIDVariables();    
}
