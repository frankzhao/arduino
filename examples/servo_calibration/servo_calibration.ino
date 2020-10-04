/** 
 * Set servo position from serial port
 */

#include <Servo.h> 

#define SERVO_PIN 5

String cmd;
Servo servo;

void setup() {
  Serial.begin(9600);
  servo.attach(SERVO_PIN);
}

void loop() {

  while (Serial.available()) {
    char c = Serial.read(); // read one byte
    cmd += c; // append to string
    delay(2);  // allow buffer to fill
  }

  if (cmd.length() > 0) {
    int pos = cmd.toInt();
    servo.write(pos);
    Serial.println(pos); // echo
    cmd=""; // reset
  } 
}
