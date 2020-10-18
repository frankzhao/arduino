#include <WiFiNINA.h>
#include "credentials.h"
#include "wifi.h"
#include "http.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

WiFiServer server(80);            //server socket

WiFiClient client = server.available();

int ledPin = 2;
char htmlTemplate[] =
  "Click <a href=\"/H\">here</a> turn the LED on<br>\n"
  "Click <a href=\"/L\">here</a> turn the LED off<br><br>\n"
  "Random reading from analog pin: %d\n";
char page[1024]; // rendered page

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  while (!Serial);
  
  enable_WiFi();
  connect_WiFi(ssid, pass);

  server.begin();
  printWifiStatus();

}

void loop() {
  client = server.available();

  if (client) {
    printWEB();
  }
}

void printWEB() {

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            sprintf(page, htmlTemplate, analogRead(A1));
            writeResponse(&client, page);
            
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        if (currentLine.endsWith("GET /H")) {
          digitalWrite(ledPin, HIGH);
          writeResponse(&client, (char*) "Set LED ON");
          break;
        }
          if (currentLine.endsWith("GET /L")) {
          digitalWrite(ledPin, LOW);       
        }

      }
    }
    disconnectClient(&client);
  }
}
