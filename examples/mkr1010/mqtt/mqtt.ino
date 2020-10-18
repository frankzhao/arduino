#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <credentials.h>
#include <wifi.h>

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int ledPin = 2;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

char broker[] = "broker.shiftr.io";
int port = 1883;
char topic[] = "boiler80/outsideTemp";

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  while (!Serial);
  
  enable_WiFi();
  connect_WiFi(ssid, pass);

  printWifiStatus();

  // You can provide a unique client ID, if not set the library uses Arduino-millis()
  // Each client must have a unique client ID
  // mqttClient.setId("clientId");

  // You can provide a username and password for authentication
  mqttClient.setUsernamePassword("try", "try");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

   connectMqtt(broker, port);

  Serial.println("You're connected to the MQTT broker!");
  Serial.print("Subscribing to topic: ");
  Serial.println(topic);

  // subscribe to a topic
  mqttClient.subscribe(topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(topic);
}

void connectMqtt(char* broker, int port) {
  while (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
  }
}

void loop() {
  int messageSize = mqttClient.parseMessage();
  if (messageSize) {
    digitalWrite(ledPin, HIGH);
    // we received a message, print out the topic and contents
    Serial.print("Received a message with topic '");
    Serial.print(mqttClient.messageTopic());
    Serial.print("', length ");
    Serial.print(messageSize);
    Serial.println(" bytes:");
    
    while (mqttClient.available()) {
      Serial.print((char)mqttClient.read());
    }
    Serial.println();
    delay(100); // keep the led on for a bit
    digitalWrite(ledPin, LOW);
  }
}
