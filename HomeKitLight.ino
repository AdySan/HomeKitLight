#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =	"*******";		// cannot be longer than 32 characters!
const char *pass =	"*******";		//

// Update these with values suitable for your network.
IPAddress server(192, 168, 1, 155);

const int relay = D3;

#define BUFFER_SIZE 100

void callback(const MQTT::Publish& pub) {
  // handle message arrived
  Serial.print(pub.topic());
  Serial.print(" => ");

    Serial.println(pub.payload_string());

    if(pub.payload_string() == "on")
    {
      digitalWrite(relay, HIGH);
    }
    else
    {
      digitalWrite(relay, LOW);
    }

}

WiFiClient wclient;
PubSubClient client(wclient, server);

void setup() {

  pinMode(relay, OUTPUT);

  // Setup console
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();

  client.set_callback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("ESP8266: AdyLight")) {
	client.publish("outTopic","hello world");
	client.subscribe("AdyLight");
      }
    }

    if (client.connected())
      client.loop();
  }
}

