  //
  const int TriggerPin = 9;      //Trig pin
  const int EchoPin = 10;         //Echo pin
  long Duration = 1;
  const int led = 2;
  
  long safetyDuration;
  int distance;
  int safetyDistance;
  
  //microphone
  int soundDetectedPin = 8; // Use Pin 8 as our Input
  int soundDetectedVal = HIGH; // This is where we record our Sound Measurement
  boolean bAlarm = false;
  unsigned long lastSoundDetectTime; // Record the time that we measured a sound
  int soundAlarmTime = 500; // Number of milli seconds to keep the sound alarm high
  
  //vib motor
  int motorPin = 3;


void setup() {
  // put your setup code here, to run once:
  pinMode(TriggerPin,OUTPUT);  // Trigger is an output pin
  pinMode(EchoPin,INPUT);      // Echo is an input pin
  pinMode(led, OUTPUT);
  Serial.begin(9600);          // Serial Output

  //microphone  
  pinMode (soundDetectedPin, INPUT) ; // input from the Sound Detection Module


  //motor
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
  while (! Serial);
  Serial.println("Speed 0 to 255");
}


void loop() {

delayMicroseconds(2);

  // put your main code here, to run repeatedly:
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
    if (Serial.available())    //motor turning on cuz of proximity
  {
    int speed = Serial.parseInt();
    if (speed >= 0 && speed <= 255)
    {
      analogWrite(motorPin, speed);
  }
  }
  }
  else{
    digitalWrite(led, LOW);
  }
 
  Serial.print("Distance = ");             // Output to serial
  Serial.print(Distance_cm);
  Serial.println(" cm");
  delay(400);                             // Wait to do next measurement


    //microphone

  soundDetectedVal = digitalRead (soundDetectedPin) ; // read the sound alarm time

  
  if (soundDetectedVal == HIGH) // If we hear a sound
  {
  
    lastSoundDetectTime = millis(); // record the time of the sound alarm
    // The following is so you don't scroll on the output screen
    if (!bAlarm){
      Serial.println("LOUD, LOUD");
      bAlarm = true;
      digitalWrite(led, HIGH);   //light turning on cuz of sound
      if (Serial.available())   //motor turning on cuz of sound
  {
    int speed = Serial.parseInt();
    if (speed >= 0 && speed <= 255)
    {
      analogWrite(motorPin, speed);
  }
  }
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



  
} 


long Distance(long time)
{
  long DistanceCalc;                      // Calculation variable
   
  DistanceCalc = ((time * 0.034) / 2);     // Actual calculation in cm
    //DistanceCalc = time / 74 / 2;         // Actual calculation in inches
  return DistanceCalc;                    // return calculated value
}
