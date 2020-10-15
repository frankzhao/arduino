#include <SoftwareSerial.h>
#include <MicroNMEA.h>

#define rxGPS 4
#define txGPS 7
#define ppsGPS 2 // Uno supports pins 2, 3 for interrupt

SoftwareSerial gps = SoftwareSerial(rxGPS, txGPS);
String gpsBuffer = "";
long prevUs = 0;
long currUs = 0;
long elapsedUs = 0;

char buffer[85];
MicroNMEA nmea(buffer, sizeof(buffer));


void setup() {
    pinMode(rxGPS, INPUT);
    pinMode(txGPS, OUTPUT);
    pinMode(ppsGPS, INPUT);

    attachInterrupt(0, handlePPS, RISING);

    Serial.begin(9600);
    gps.begin(9600);
    digitalWrite(txGPS, HIGH);
    while(gps.available()) {
      if (gps.read() == '\r') {
        break;
      }
    }
}

void loop() {
    while (gps.available()) {
    char fixInfo[128] = "Latest fix: ";
    char timestamp[128] = "Timestamp: ";
    char c = gps.read();
    if (nmea.process(c)) {
        char posStr[64];
        sprintf(posStr,"%ld, %ld", nmea.getLatitude(), nmea.getLongitude());
        strcat(fixInfo, posStr);
        Serial.println(fixInfo);

        long nowUs = micros();
        // elapsed time since last pps interrupt
        int ms = (nowUs - prevUs) / 1000;

        char tStr[64];
        sprintf(tStr, "%02d:%02d:%02d.%03d",
            nmea.getHour(),
            nmea.getMinute(),
            nmea.getSecond(),
            ms
        );
        strcat(timestamp, tStr);
        Serial.println(timestamp);
    }
}
}

void handlePPS() {
    long us = micros();
    if (prevUs == 0) {
        prevUs = us;
    } else {
        prevUs = us;
    }
}

// http://aprs.gids.nl/nmea/
void parseMessage(String msg) {
  if (msg.substring(0, 6) == "$GPGGA") {
    Serial.println(msg);
    String pos = msg.substring(18, 42);
    Serial.println("--- Fix data: " + pos);
  }
}

