int trigPin = 0;    //Trig 
int echoPin = 2;    //Echo 
 
float duration, cm, inches;
volatile unsigned long LastPulseTime;

void setup() {
  Serial.begin (115200); //Serial Port begin
  pinMode(trigPin, OUTPUT); //Define outputs
  pinMode(echoPin, INPUT); //Define inputs
  attachInterrupt(digitalPinToInterrupt(echoPin), EchoPinISR, CHANGE);  // Pin 2 interrupt on any change
}
 
void loop() {
  // The sensor will going to triggered by a HIGH pulse of 10 or more microseconds.
  // Provide a short LOW pulse beforehand to ensure a clean HIGH pulse:

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor a HIGH pulse
  // The duration time in microseconds from the broadcast
  // of the ping to the reception of its echo off of an obstacle.

  // pinMode(echoPin, INPUT);
  // duration = pulseIn(echoPin, HIGH);

  // converting the time into a distance

  cm = (LastPulseTime/2) / 29.1;
  inches = (LastPulseTime/2) / 74.0;

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  delay(1000);
}

void EchoPinISR() {
  static unsigned long startTime;

  if (digitalRead(echoPin)) { // Gone HIGH
    startTime = micros();
  }
  else { // Gone LOW
    LastPulseTime = micros() - startTime;
  }
}