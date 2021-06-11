#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "config-env.h"

#define LEDPIN 2
#define DEBUG

void setup_wifi();
void reconnect();
void callback(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(MQTT_SERVER, 1883, callback, espClient);

void setup() {
  pinMode(LEDPIN, OUTPUT);

  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  setup_wifi();

  #ifdef DEBUG
    Serial.println("Setup completed...");
  #endif

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(100);
}


void setup_wifi() {
  delay(10);

  #ifdef DEBUG
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);
  #endif

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG
      Serial.print(".");
    #endif
  }

  #ifdef DEBUG
    Serial.println("");
    Serial.println("WiFi connection established ");
    Serial.print("=> IP address: ");
    Serial.println(WiFi.localIP());
  #endif
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Data received");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println((char) payload[0]);

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LEDPIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(LEDPIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }
}


void reconnect() {
  //Boucle jusqu'à obtenur une reconnexion
  while (!client.connected()) {
    #ifdef DEBUG
      Serial.print("Connecting to MQTT Server...");
    #endif
    if (client.connect(MQTT_CLIENT_NAME)) {
      #ifdef DEBUG
        Serial.println("OK");
      #endif
      client.subscribe("test/led");
    } else {
      #ifdef DEBUG
        Serial.print("KO, error : ");
        Serial.print(client.state());
      #endif
      delay(5000);
    }
  }
}