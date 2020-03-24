#include <PubSubClient.h>
#include <ESP8266WiFi.h>

// Update these with values suitable for your network.
const char* ssid = "tono.io";
const char* password = "********";

// Config MQTT Server
#define mqtt_server "m15.cloudmqtt.com"
#define mqtt_port 15443
#define mqtt_user "gcixpcpd"
#define mqtt_password "********"

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  delay(10);

  Serial.println();

    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  
  

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("espClient", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      return;
    }
  } else {

    //client.subscribe("/", String(ESP.getChipId()).c_str());
    //client.subscribe("/U********");
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
