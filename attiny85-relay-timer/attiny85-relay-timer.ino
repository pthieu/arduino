int relayPin = 4;
// 'offPeriod', note relay is set to inverse logic, turning humidifier on
// requires a LOW
unsigned long offPeriod = 15 * 60;
unsigned long onPeriod = 5 * 60;
 
void setup()
{
  pinMode(relayPin, OUTPUT);
}

void loop()
{
  digitalWrite(relayPin, LOW);
  delaySeconds(onPeriod);
  digitalWrite(relayPin, HIGH);
  delaySeconds(offPeriod);
}

void delaySeconds(int s) {
  for (int i = 0; i < s; i++) {
    delay(1000);
  }
}