#include <Arduino.h>
#include "mkr1000mqtt.h"

int waterSensorPin = 16;
int motorPWMPin = 2;
int sensorValue = 0;
int pumpPower = 255; // max 255
int defaultPumpSeconds = 5; // pump on time
int defaultMoistureReportSeconds = 10 * 60; // moisture reporting interval
int defaultPumpCheckSeconds = 60 * 60; // interval for automatic pumping check 1hr

bool manualMode = false;
char debugMsg[128];
char sensorData[128];

void setup() {
  Serial.begin(9600);
  pinMode(waterSensorPin, INPUT); // water sensor
  pinMode(motorPWMPin, OUTPUT); // motor pwm
  setupCloudIoT();
}

void blinkLED(int durationMs) {
  digitalWrite(LED_BUILTIN, 1);
  delay(durationMs);
  digitalWrite(LED_BUILTIN, 0);
}

int readMoistureSensor() {
  sensorValue = analogRead(waterSensorPin); // range is 0-1024
  snprintf(sensorData, 128, "Moisture: %d", sensorValue);
  publishTelemetry("/moisture", sensorData);
  return sensorValue;
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  blinkLED(500);
  if (topic == "/devices/water-pump/commands") {
    if (payload == "ping") {
      publishTelemetry("/test", "pong");
    }
    
    if (payload == "manual") {
      manualMode = true;
      publishTelemetry("/mode", "Manual mode");
    }

    if (payload == "auto") {
      manualMode = false;
      publishTelemetry("/mode", "Automatic mode");
    }

    if (payload == "wifi") {
      publishTelemetry("/network", getDefaultSensor());
    }

    if (payload == "moisture") {
      readMoistureSensor();
    }
  } else if (topic == "/devices/water-pump/commands/pump") {
    // manual pump duration
    int duration = payload.toInt();
    pumpSeconds(duration);
  } else if (topic == "/devices/water-pump/commands/pump_seconds") {
    // change default pump duration
    int duration = payload.toInt();
    defaultPumpSeconds = duration;
    snprintf(debugMsg, 128, "Set default pump duration to %d seconds", defaultPumpSeconds);
    publishTelemetry("/pump", debugMsg);
  } else if (topic == "/devices/water-pump/commands/moisture_seconds") {
    // how often to report moisture
    int duration = payload.toInt();
    defaultMoistureReportSeconds = duration;
    snprintf(debugMsg, 128, "Set default moisture reporting duration to %d seconds", defaultMoistureReportSeconds);
    publishTelemetry(debugMsg);
  } else if (topic == "/devices/water-pump/commands/pump_check_seconds") {
    // how often to check if we need to water
    int duration = payload.toInt();
    defaultPumpCheckSeconds = duration;
    snprintf(debugMsg, 128, "Set default pump checking duration to %d seconds", defaultPumpCheckSeconds);
    publishTelemetry(debugMsg);
  }
}

unsigned long moistureMillis = 0;
unsigned long checkMillis = 0;
void loop() {
  mqtt->loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!mqttClient->connected()) {
    connect();
  }

  if (!manualMode) {
    // report moisture once every few minutes
    if (millis() - moistureMillis > defaultMoistureReportSeconds*1000) {
      moistureMillis = millis();
      sensorValue = analogRead(waterSensorPin); // range is 0-1024
      snprintf(sensorData, 128, "Moisture: %d", sensorValue);
      publishTelemetry("/moisture", sensorData);
    }

    // check whether we need to pump every few mins
    if (millis() - checkMillis > defaultPumpCheckSeconds*1000) {
      checkMillis = millis();
      if (readMoistureSensor() > 400) {
        pumpSeconds(defaultPumpSeconds);
      }
    }
  } else {
    // wait for manual commands
  }
}

void pumpSeconds(int durationSecs) {
  snprintf(debugMsg, 128, "Pump for %d seconds", durationSecs);
  publishTelemetry("/pump", debugMsg);
  Serial.println(debugMsg);
  pumpControl(0);
  pumpControl(255);
  delay(durationSecs * 1000);
  pumpControl(0);
}

void pumpControl(int power) {
  snprintf(debugMsg, 128, "Pump set to: %d power", power);
  Serial.println(debugMsg);
  if (power == 0) {
    digitalWrite(LED_BUILTIN, 0);
  } else {
    digitalWrite(LED_BUILTIN, 1);
  }
  analogWrite(motorPWMPin, power);
}
