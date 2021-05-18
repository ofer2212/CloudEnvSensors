// libreries
#include <Adafruit_Sensor.h>
#include "SCAsyncTimer.h"
#include "Streaming.h"
#include <DHT.h>
#include <DHT_U.h>
#include <analogWrite.h>
#define DHTTYPE    DHT22
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// cloud andwifi config
#define WLAN_SSID       "ofme400"
#define WLAN_PASS       "18041986"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "ofer221"
#define AIO_KEY         "aio_ZdZB29PSRvm0WzRixDs0IH8POxAB"


// pins config
#define red_led_pin  32
#define green_led_pin  33
#define photoresistor_pin  35
#define dht_pin  25

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish  temp_level = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish light_level = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/light");
Adafruit_MQTT_Publish humidity_level = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");

int photoresistor_value = 0; // LIGHT SENSOR VALUE
float tempValue = 0; // TEMPERATURE SENSORE VALUE
DHT_Unified dht(dht_pin, DHTTYPE); // DHT22

//TIMER DEFINITION
SCAsyncTimer sensorsDelay(10, UNIT_SECONDS);



void MQTT_connect();
void toggleRed(bool state);
void toggleGreen(bool state);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting..."));
  delay(1000);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial << "Connecting to " << WLAN_SSID << endl;
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial << "WiFi connected" << endl << "IP address: " << WiFi.localIP() << endl << endl;
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
  dht.begin();
  sensorsDelay.start();
}


void loop() {
    MQTT_connect();
    if (sensorsDelay.isTriggered()) {
      yield();
      Serial<<"\nSending value ..."<<endl;
      if (! light_level.publish( getLightLevel ())) {
        Serial << "Failed" << endl;
      } else {
        Serial << "Sent to cloud!" << endl;
      }
  
      if (! temp_level.publish(getTemp())) {
        Serial << "Failed" << endl;
      } else {
        Serial << "Sent to cloud!" << endl;
      }
    }
}

void toggleRed(bool state) {
  digitalWrite(red_led_pin, state);
  digitalWrite(green_led_pin, !state);
  Serial << "Red led is on" << endl;
}

void toggleGreen(bool state) {
  digitalWrite(green_led_pin, state);
  digitalWrite(red_led_pin, !state);
  Serial << "Green led is on" << endl;
}
float getTemp() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    Serial << endl << "last temp: " << tempValue << endl;
    Serial  << "current temp: " << (float)event.temperature << endl;
    if ((float)event.temperature > tempValue) {
      toggleRed(true);
    } else {
      toggleGreen(true);
    }
    tempValue = (float)event.temperature;
    return (float)event.temperature;
  }
  else return -1;
}
int getLightLevel () {
  photoresistor_value = analogRead(photoresistor_pin);
  int lightValue = map(photoresistor_value, 0, 5000, 0, 100);
  Serial << endl << "light: " << lightValue << endl;
  return lightValue;
}


void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial << mqtt.connectErrorString(ret) << endl;
    Serial << "Retrying MQTT connection in 5 seconds..." << endl << endl;
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial << "MQTT Connected!" << endl;
}
