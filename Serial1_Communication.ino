void readValuesFromSerial1() {
 while(Serial1.available())
    {
        char c = Serial1.read();

        // Reading char
        if(Receive(c) > 0)
        {
             timeout = 500;
             GetStart();
             UIyaw = GetByte();
             UIthr = GetByte()+1;
             UIroll = GetByte();
             UIpitch = GetByte()+1;
             on = GetNibble();
             off = GetNibble();
             positionHoldingOn = GetNibble();
             positionHoldingOff = GetNibble();
            
             //Serial.println(UIthr);Serial.println(" ");
     
             if(on == 1 && off == 0){
               globalReset();
               startStop = true; 
               positionHoldingStartStop = false; 
             }
             else if(on == 0 && off == 1){
               startStop = false;
               positionHoldingStartStop = false;
               writeToAllMotors(STOP_SPEED);
             } 
             else if(on == 0 && off == 0 && positionHoldingOn == 1 && positionHoldingOff == 0){
               saveSetpoint();
               positionHoldingStartStop = true;
             }
             else if(on == 0 && off == 0 && positionHoldingOn == 0 && positionHoldingOff == 1){
               resetSetpoint();
               positionHoldingStartStop = false;
             }  
             
             writeValuesToSerial1();   
               
        }//end IF
    }//end WHILE
}// end readValuesFromSerial1

// Writing to serial1 all the output values
void writeValuesToSerial1() {
   // Risposta
   PutStart();
   PutInt((int)(pitchFusion * 100.0 + 10000.0));
   PutInt((int)(rollFusion * 100.0 + 10000.0));
   PutInt((int)(motorValue[0]));
   PutInt((int)(motorValue[1]));
   PutInt((int)(motorValue[2]));
   PutInt((int)(motorValue[3]));
   PutInt((int)(unionGpsData.gpsData[0]*100.0)); //Altitude
   PutLong((unsigned long)(unionGpsData.gpsData[1]*1000000.0)); //Latitude
   PutLong((unsigned long)(unionGpsData.gpsData[2]*1000000.0)); //Longitude
   PutByte((byte)((int)unionGpsData.gpsData[3])); //N sat
   PutLong((unsigned long)(gpsSetpoint[1]*1000000.0)); //Latitude
   PutLong((unsigned long)(gpsSetpoint[2]*1000000.0)); //Longitude
   PutInt((int)((oldHeading)*100.0));
   PutCheck();
   PutEnd();   
   Serial1.println(outBuffer);
   

}
