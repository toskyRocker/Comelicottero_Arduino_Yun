// mapped values of roll, pitch, yaw
float mappedRoll = 0, mappedPitch = 0, mappedYaw = 0, errorPitch, errorRoll, errorYaw;

void flightControl() {
  controlsMapping();
  if(positionHoldingStartStop == true) {
    //Serial.println("I'm in navigation");
    naviCounter++;
    if(naviCounter % NAVI_COMPUTE_COUNTER == 0) {
      //Serial.println("I'm in NAVI control");
      naviCounter = 0;
       float setpointLat = gpsSetpoint[1];
       float setpointLong = gpsSetpoint[2];
       float currentLat = unionGpsData.gpsData[1];
       float currentLong = unionGpsData.gpsData[2]; 
       float currentHeading = oldHeading;
       float distanceXY = distance(            
                              setpointLat,          // Setpoint Latitude
                              setpointLong,          // Setpoint Longitude
                              currentLat, // Current Latitude 
                              currentLong); // Current Longitude
                             
      //Serial.print("DistanzaXY: ");Serial.println(distanceXY,6);
      if(distanceXY > 2.5) {
                              
         float LongitudeDistance = distance(
                           setpointLat,
                           setpointLong,
                           setpointLat,
                           currentLong
                                 );
         if((currentLong < setpointLong))
           LongitudeDistance *= -1;
             
         float LatitudeDistance = distance(
                           setpointLat,
                           setpointLong,
                           currentLat,
                           setpointLong
                                 ) ;
         if((currentLat < setpointLat) )
           LatitudeDistance *= -1;
                                 
        //Serial.print("LongitudeDistance: ");Serial.print(LongitudeDistance, 6);Serial.print(" LatitudeDistance: ");Serial.println(LatitudeDistance, 6);
        LongitudeDistance = constrain(LongitudeDistance*NAVIGATION_KP,NAVIGATION_MINV, NAVIGATION_MAXV); 
        LatitudeDistance  = constrain(LatitudeDistance *NAVIGATION_KP,NAVIGATION_MINV, NAVIGATION_MAXV); 
        //Serial.print("LongitudeDistance AFTER: ");Serial.print(LongitudeDistance, 6);Serial.print(" LatitudeDistance AFTER: ");Serial.println(LatitudeDistance, 6);
        
        mappedRoll = -(LongitudeDistance * sin(currentHeading * DEG_TO_RAD) - LatitudeDistance * cos(currentHeading * DEG_TO_RAD));
        mappedRoll = -(LongitudeDistance * cos(currentHeading * DEG_TO_RAD) + LatitudeDistance * sin(currentHeading * DEG_TO_RAD));  
  
        //Serial.print("UIr: ");Serial.print(UIr);
        //Serial.print(" UIp ");Serial.println(UIp);Serial.println("");
}
    else {
       mappedPitch = 0;
       mappedRoll = 0;
    }
  }// END naviCounter
 }// END NavigationRoutine
  
  
  // Timing PIDs computation.
 // dTime = millis() - pidComputeTimer;
//  if (dTime >= PID_COMPUTE_TIMER) {
//    pidComputeTimer = millis();    
    // avoiding integral for values less then MIN_SPEED
    if(throttle > MIN_SPEED) {
      rollOut = PIDroll.compute(mappedRoll, rollFusion, gyroXAverage, &dt); // ROLL
      pitchOut = PIDpitch.compute(mappedPitch, pitchFusion, gyroYAverage, &dt); // PITCH
      yawOut = PIDyaw.compute(mappedYaw, 0, gyroZAverage, &dt);    // YAW   
  
    }
    else {
      resetI();
    }
    
   // Serial.print(pitchOut);Serial.print(F(" "));Serial.print(PIDpitch.I);Serial.print(F(" "));Serial.println(throttle);

    /**** X CONFIGURATION ****/
    motorValue[0] = throttle + (rollOut + pitchOut - yawOut); // Front Left
    motorValue[1] = throttle + (-rollOut + pitchOut + yawOut); // Front Right
    motorValue[2] = throttle + (-rollOut - pitchOut - yawOut); // Rear Right
    motorValue[3] = throttle + (rollOut - pitchOut + yawOut); // Rear Left


    /**** MOTOR COMPENSATION FILTER ****/
    /**** Writing output to ESCs   ****/
    for (uint8_t nMotor = 0; nMotor < N_MOTORS; nMotor++  ) {
       motorIIROldOld[nMotor] = motorIIROld[nMotor];
       motorIIROld[nMotor] = motorIIR[nMotor]; 
       motorIIR[nMotor] = (1-B)*motorValue[nMotor] + B*motorIIROld[nMotor];
       motorValue[nMotor] = (1/(1-A))*(motorIIR[nMotor] - A*motorIIROldOld[nMotor]);
       motorValue[nMotor] = constrain(motorValue[nMotor], MIN_SPEED, MAX_SPEED);
       writeToMotor((int)motorValue[nMotor], nMotor);
    }
    /*Serial.print(motorValue[0]);Serial.print(", ");Serial.println(motorValue[1]);
    Serial.print(motorValue[3]);Serial.print(", ");Serial.println(motorValue[2]);
    Serial.println("");
    */
 
}

void resetOutput() {
// UIpitch = 0, UIroll = 0, UIyaw = 0, UIthr = THR_IN_MIN; 
 pitchOut = 0, rollOut = 0, yawOut = 0;
 throttle = MIN_SPEED, throttleOffset = 0;
 for(uint8_t nMotor = 0; nMotor < N_MOTORS; nMotor++) {
    motorIIR[nMotor] = MIN_SPEED;
    motorIIROld[nMotor] = MIN_SPEED;
    motorIIROldOld[nMotor] =  MIN_SPEED;
  }
}

  /* 
  MAPPING: From GUI values to flightcontrol values
  roll =>  (-10deg, +10deg)
  pitch => (-10deg, +10deg)
  yaw => (-30deg/s, +30deg/s)
  */
void controlsMapping() {
  if(positionHoldingStartStop == false) { 
    //pitch =>  (+15, -15)  Negative angle when quadcopter flights ahead
    mappedPitch = -mapFloat( (float)UIpitch, PITCH_MIN, PITCH_MAX, PITCH_MIN_ANGLE, PITCH_MAX_ANGLE );
    mappedRoll = mapFloat( (float)UIroll, ROLL_MIN, ROLL_MAX, ROLL_MIN_ANGLE, ROLL_MAX_ANGLE );
  }
  mappedYaw = mapFloat( (float)UIyaw, YAW_MIN, YAW_MAX, YAW_MIN_ANGLE, YAW_MAX_ANGLE );  
 
  float tempThrottleOffset = mapFloat((float)UIthr, THR_IN_MIN, THR_IN_MAX, THR_OUT_MIN, THR_OUT_MAX);
  throttleOffset += pow(tempThrottleOffset, 3);
  throttleOffset = constrain(throttleOffset, 0, THR_RANGE);
 // Serial.print(throttleOffset);Serial.print(F(" "));
  throttle = constrain((MIN_SPEED + throttleOffset), MIN_SPEED, MAX_SPEED);
 // Serial.println(throttle);
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) { 
    x = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
   return constrain(x, out_min, out_max);
}

