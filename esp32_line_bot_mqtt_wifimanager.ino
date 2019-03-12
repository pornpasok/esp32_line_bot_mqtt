#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <PubSubClient.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WiFiManager.h>

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

// Config MQTT Server
#define mqtt_server "m15.cloudmqtt.com"
#define mqtt_port 15443
#define mqtt_user "mqttuser"
#define mqtt_password "mqttpass"

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Set LED_BUILTIN 
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  delay(10);

  Serial.println();

  WiFiManager wifiManager;
  wifiManager.autoConnect();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      return;
    }
  } else {
    // MQTT Topic /ESP32
    client.subscribe("/ESP32");
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg = "";
  int i = 0;
  while (i < length) msg += (char)payload[i++];
  Serial.println(msg);
  digitalWrite(LED_BUILTIN, (msg == "on" ? LOW : HIGH));
}