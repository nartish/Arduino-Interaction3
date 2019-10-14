//microphone
  int soundDetectedPin = 8; // Use Pin 10 as our Input
  int soundDetectedVal = HIGH; // This is where we record our Sound Measurement
  boolean bAlarm = false;
  
  unsigned long lastSoundDetectTime; // Record the time that we measured a sound
  
  int soundAlarmTime = 500; // Number of milli seconds to keep the sound alarm high
  
//led
  const int led = 2;
  
//motor
  const int pwm = 4 ; 
  const int in_1 = 8 ;
  const int in_2 = 9 ;
  
//proximity sensor
  int TriggerPin = 9;      //Trig pin
  int EchoPin = 10;        //Echo pin
  long Duration = 1;
  
  long safetyDuration;
  int distance;
  int safetyDistance;


  
void setup ()
{
  Serial.begin(9600);  
  pinMode (soundDetectedPin, INPUT) ; // input from the Sound Detection Module

  //led
  pinMode(led, OUTPUT);

  //motor
   pinMode(pwm,OUTPUT) ; //we have to set PWM pin as output
   pinMode(in_1,OUTPUT) ; //Logic pins are also set as output
   pinMode(in_2,OUTPUT) ;
  //proximity sensor
  pinMode(TriggerPin,OUTPUT);  // Trigger is an output pin
  pinMode(EchoPin,INPUT);      // Echo is an input pin
  
}


void loop ()
{
  //microphone
  soundDetectedVal = digitalRead (soundDetectedPin) ; // read the sound alarm time
 
  
  if (soundDetectedVal == LOW) // If we hear a sound
  {
  
    lastSoundDetectTime = millis(); // record the time of the sound alarm
    // The following is so you don't scroll on the output screen
    if (!bAlarm){
      Serial.println("LOUD, LOUD");
      bAlarm = true;
      digitalWrite(led, HIGH);
      
    //For Clock wise motion , in_1 = High , in_2 = Low
   digitalWrite(in_1,HIGH) ;
   digitalWrite(in_2,LOW) ;
   analogWrite(pwm,255) ;
   /* setting pwm of the motor to 255 we can change the speed of rotation
   by changing pwm input but we are only using arduino so we are using highest
   value to driver the motor */
   //Clockwise for 3 secs
   delay(300) ;
   //For brake
   digitalWrite(in_1,LOW) ;
    
    }
  }
  else
  {
    if( (millis()-lastSoundDetectTime) > soundAlarmTime  &&  bAlarm){
      Serial.println("quiet");
      bAlarm = false;
      digitalWrite(led, LOW);
    }
  }





//  // put your main code here, to run repeatedly:
  digitalWrite(TriggerPin, LOW);                   
  delayMicroseconds(2);
  digitalWrite(TriggerPin, HIGH);          // Trigger pin to HIGH
  delayMicroseconds(10);                   // 10us high 
  digitalWrite(TriggerPin, LOW);           // Trigger pin to HIGH
 
  Duration = pulseIn(EchoPin,HIGH);        // Waits for the echo pin to get high
                                           // returns the Duration in microseconds
  long Distance_cm = Distance(Duration);   // Use function to calculate the distance

  safetyDistance = Distance_cm; 

  if (safetyDistance <= 10){
    digitalWrite(led, HIGH);   //light turning on cuz of proximity
    
  }
  else{
    digitalWrite(led, LOW);
  }
 
  Serial.print("Distance = ");             // Output to serial
  Serial.print(Distance_cm);
  Serial.println(" cm");
  delay(400);                             // Wait to do next measurement
}



long Distance(long time)
{
  long DistanceCalc;                      // Calculation variable
   
  DistanceCalc = ((time * 0.034) / 2);     // Actual calculation in cm
    //DistanceCalc = time / 74 / 2;         // Actual calculation in inches
  return DistanceCalc;                    // return calculated value
}
