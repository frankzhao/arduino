#include <SoftwareSerial.h>

#define rxGPS 2
#define txGPS 4

SoftwareSerial gps = SoftwareSerial(rxGPS, txGPS);
String gpsBuffer = "";


void setup() {
    pinMode(rxGPS, INPUT);
    pinMode(txGPS, OUTPUT);

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
    gpsBuffer = readGPS();
    if (gpsBuffer != "") {
      Serial.println(gpsBuffer);
      parseMessage(gpsBuffer);
    }
}

String readGPS() {
    if (gps.available()) {
        String line = gps.readStringUntil('\n');
        return line;
    }
    return "";
}

// http://aprs.gids.nl/nmea/
void parseMessage(String msg) {
  if (msg.substring(0, 6) == "$GPGGA") {
    String pos = msg.substring(18, 42);
    Serial.println("--- Fix data: " + pos);
  }
}

