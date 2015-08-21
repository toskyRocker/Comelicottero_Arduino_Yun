// init motors at startup
void initMotors() {
  DDRB |= (1 << 7) | (1 << 6) | (1 << 5);
//  ICR1 = 0x1FFF;  // 250Hz
  ICR1 = 0x1387;  // 400Hz

  TCCR1A = 0b10101010;
  TCCR1B = 0b00011010;
  motor0 = 0x0000;
  motor1 = 0x0000;
  motor2 = 0x0000;
  DDRC |= (1 << PC6); 
 // ICR3 = 0x1FFF;  
  ICR3 = 0x1387; // 400Hz
  TCCR3A = 0b10101010;
  TCCR3B = 0b00011010;
  motor3 = 0x0000;
}

// motors' start procedure
void writeToAllMotors(int motorSpeed) {
       
        motor0 = motorSpeed;
        motor1 = motorSpeed; 
        motor2 = motorSpeed;
        motor3 = motorSpeed;
}

// Use this method to write into motors' registers
void writeToMotor(int motorValue, int index) {
switch (index) {
  case 0:
    motor0 = motorValue;
    break;
  case 1:
    motor1 = motorValue;
    break;
  case 2:
   motor2 = motorValue;
    break;
  case 3:
    motor3 = motorValue;
    break;
}
}

