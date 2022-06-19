#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// GPIO 5 D1
#define LED 2

// WiFi
const char *ssid = "Malware-1.0";     // Enter your WiFi name
const char *password = "Karan@#2001"; // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "192.168.1.60";
const char *topic = "esp8266/led";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1888;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  String message;
  for (unsigned int i = 0; i < length; i++)
  {
    message = message + (char)payload[i]; // convert *byte to string
  }
  Serial.print(message);
  if (message == "1")
  {
    Serial.println("\nTurning LED ON");
    digitalWrite(LED, LOW);
  } // LED on
  if (message == "0")
  {
    Serial.println("\nTurning LED OFF");
    digitalWrite(LED, HIGH);
  } // LED off
  Serial.println();
  Serial.println("-----------------------");
}

void setup()
{
  pinMode(LED, OUTPUT);
  // Set software serial baud to 115200;
  Serial.begin(115200);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  // connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected())
  {
    const char *client_id = "esp8266-client-123";
    // client_id += String(WiFi.macAddress());
    Serial.println("Connecting to public emqx mqtt broker.....");
    if (client.connect(client_id))
    {
      Serial.println("Public emqx mqtt broker connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  // publish and subscribe
  client.publish(topic, "hello emqx");
  client.subscribe(topic);
  client.subscribe("test");
}

void loop()
{
  client.loop();
}