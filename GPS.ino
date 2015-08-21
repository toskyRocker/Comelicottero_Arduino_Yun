
/* 
 * It receives GPS position from Arduino Micro
 */
void getGPSPosition(byte writeCommand) {
  
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write((byte)1);
  if (Wire.requestFrom (SLAVE_ADDRESS, sizeof unionGpsData) == sizeof unionGpsData){
    for(byte i = 0; i < 4*dataCount; i++){
       unionGpsData.rawData[i] =  Wire.read();
     }
   }
  
  Wire.endTransmission();
/*  for (int i = 0; i < dataCount; i++)  {
       Serial.print ("Float n. ");
       Serial.print (i);
       Serial.print (" Value: ");
       Serial.println (unionGpsData.gpsData[i],6);  
       } */
} 

/*
 * Given  (lat, long) coordinates of 2 points it computes the distance between them.
 */
float distance(float lat1, float long1, float lat2, float long2)
{
  // returns distance in meters between two positions, both specified
  // as signed decimal-degrees latitude and longitude. Uses great-circle
  // distance computation for hypothetical sphere of radius 6372795 meters.
  // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
  float delta = radians(long1-long2);
  float sdlong = sin(delta);
  float cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = sq(delta);
  delta += sq(clat2 * sdlong);
  delta = sqrt(delta);
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * EARTH_RADIUS;
}

float course_to (float lat1, float long1, float lat2, float long2)
{
  // returns course in degrees (North=0, West=270) from position 1 to position 2,
  // both specified as signed decimal-degrees latitude and longitude.
  // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
  // Courtesy of Maarten Lamers
  float dlon = radians(long2-long1);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float a1 = sin(dlon) * cos(lat2);
  float a2 = sin(lat1) * cos(lat2) * cos(dlon);
  a2 = cos(lat1) * sin(lat2) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0)
  {
    a2 += TWO_PI;
  }
  return degrees(a2);
}


/* 
 * It saees the coordinates of the setpoint
 */
void saveSetpoint() {
  
  gpsSetpoint[1] = unionGpsData.gpsData[1]; // Latitude setpoint
  gpsSetpoint[2] = unionGpsData.gpsData[2]; // Longitude setpoin
}

/*
 * Reset Setpoint coordinates
 */
void resetSetpoint() {
  gpsSetpoint[1] = 0;
  gpsSetpoint[2] = 0;
}


