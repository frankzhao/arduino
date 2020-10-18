#include <WiFiNINA.h>

void writeResponse(WiFiClient* client, char* body) {
  char httpHeader[] =
    "HTTP/1.1 200 OK\n"
    "Content-type:text/html\n\n";

  client->print(httpHeader);
  client->print(body);
  client->println();
}

void disconnectClient(WiFiClient* client) {
  client->stop();
  Serial.println("client disconnected");
}

