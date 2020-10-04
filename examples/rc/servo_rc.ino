/**
 * Example servo control using 3 channel receiver
 */

#include <Servo.h> 

#define SERVO_PIN 9
Servo servo;
int pos = 90;  // 9g servo is 0-180, set center
int currPos;

int throttle; // CLOSED 1088 - 1883 OPEN
int aileron;  // LEFT 1887 - CENTRE 1489 - 1088 RIGHT
int aux;      // Button D: 0=1882, 1=1481, 2=1081

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
  servo.write(pos); // reset to middle

  // receiver pins
  pinMode(3, INPUT); // aileron
  pinMode(5, INPUT); // elevator
  pinMode(6, INPUT); // aux
}

void loop() {
  throttle = pulseIn(3, HIGH, 25000);
  aileron = pulseIn(5, HIGH, 25000);
  aux = pulseIn(6, HIGH, 25000);

  // aileron
  pos = map(aileron, 1800, 1000, 0, 180);
  servo.write(pos);

  // aux 0-1-2
//  aux = map(aux, 1800, 1000, 0, 180);
//  servo.write(aux);

  // throttle
//  throttle = map(throttle, 1000, 1800, 0, 180);
//  servo.write(throttle);
  
//  Serial.print("Throttle: ");
//  Serial.println(throttle);
//
//  Serial.print("Aileron: ");
//  Serial.println(aileron);
//
//  Serial.print("Aux: ");
//  Serial.println(aux);
//

}
