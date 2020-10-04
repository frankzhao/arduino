/** 
 * Example to perform an analog read from a potentiometer or varible resistor
 * connected to analog input pin 0
 */

int analogPin = 0;
int potValue = 0;

char debugMsg[128];

void setup() {
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(analogPin); // range is 0-1024
  snprintf(debugMsg, 128, "Value: %d", potValue);
  Serial.println(debugMsg);
  delay(1000);
}
