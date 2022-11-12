#include <ESP8266WiFi.h>
#include <EasyNeoPixels.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

const char* WIFI_SSID = "CHANGE_ME";
const char* WIFI_PASSWORD = "CHANGE_ME";

const char* mqtt_host = "CHANGE_ME";
const uint mqtt_port = 1883;
const char* mqtt_user = "CHANGE_ME";
const char* mqtt_pwdd = "CHANGE_ME";

const char* tripwire1_feed = "/feeds/tripwire1";

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, mqtt_host, mqtt_port, mqtt_user, mqtt_pwdd);
Adafruit_MQTT_Subscribe tripwire1 = Adafruit_MQTT_Subscribe(&mqtt, tripwire1_feed);

void setup() {
  Serial.begin(115200);
  setupEasyNeoPixels(14, 1);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  mqtt.subscribe(&tripwire1);
}

char* tripWire1_lastState = "";
char* tripWire2_lastState = "";
char* doorSensor1_lastState = "";

void loop() {
  if (tripwire1_laststate == "") {
    writeEasyNeoPixel(0, LOW);
  }

  MQTT_connect();

  Adafruit_MQTT_Subscribe* subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &tripwire1) {
      const char* msg = (char*)tripwire1.lastread;
    
      if (strcmp(msg, "tripped") == 0) {
        Serial.println("[tripwire 1] tripped");
        tripwire1_laststate = "tripped";
        writeEasyNeoPixel(0, HIGH);
        writeEasyNeoPixel(0, 0, 255, 0);
      } else if (strcmp(msg, "reset")) {
        tripwire1_laststate = "";
        Serial.println("[tripwire 1] reset");
        writeEasyNeoPixel(0, LOW);
      } else {
        Serial.print(F("[tripwire 1] unknown message: "));
        Serial.println(msg);
      }
    }
  }

  if (!mqtt.ping()) {
    Serial.println("[MQTT] Disconnecting...");
    mqtt.disconnect();
  }
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("[MQTT] Connecting... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {  // connect will return 0 for connected
    Serial.println(ret);
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("[MQTT] Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();

    delay(5000);  // wait 5 seconds

    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1)
        ;
    }
  }
  Serial.println("[MQTT] Connected!");
}