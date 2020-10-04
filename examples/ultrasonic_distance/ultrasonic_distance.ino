/**
 * Calculate distance using ultrasonic sensor HC-SR04
 */

#define trig 9  // trigger
#define echo 10 // echo

long duration;
float distance;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600); // serial console output
}

void loop() {
  digitalWrite(trig, LOW);  // clear trig

  // pulse for 10us
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // read echo
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034/2.0;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  delay(50);  // pause 50ms
}
