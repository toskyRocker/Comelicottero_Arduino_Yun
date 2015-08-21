/*
 // UNCOMMENT TO ENABLE NAV
void calcHeading() {
   mag.calcMagData();
   // Converting acc data in Radian
   float accX = accXDegree*DEG_TO_RAD;
   float accY = accYDegree*DEG_TO_RAD;
   // Calculating new mag heading
   float X = mag._magData.x*cos(accX) + mag._magData.z*sin(accX);
   float Y = mag._magData.x*sin(accY)*sin(accX) + mag._magData.y*cos(accY) - mag._magData.z*sin(accY)*cos(accX);
   float heading = atan2(Y, X);  

   // Declination angle correction
    heading += DECLINATION_ANGLE;
    // Correct for when signs are reversed.
    if(heading < 0) {
      heading += 2*PI;}
    // Check for wrap due to addition of declination.
    else if(heading > 2*PI){
    heading -= 2*PI;}
    
    // Convert radians to degrees for readability.
    float headingDegrees = heading * RAD_TO_DEG; 
    // IIR Lowpass Filter
    oldHeading = (ALPHA_MAG * oldHeading + (1.0 - ALPHA_MAG) * headingDegrees);
}
*/
