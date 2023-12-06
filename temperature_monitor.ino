#include <DHT.h>
#include "heltec.h"
#include "WiFi.h"
#include <HTTPClient.h>
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;
const int daylightOffset_sec = 0;
// WiFi credentials
const char* ssid = "#ssid";               // change SSID
const char* password = "#password";  // change password
// Google script ID and required credentials
String GOOGLE_SCRIPT_ID = "#google_script_id";  // change Gscript ID


#define DHT_SENSOR_PIN 27  // ESP32 pin GIOP21 connected to DHT22 sensor
#define DHT_SENSOR_TYPE DHT22
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
float currentTempC = 0.0;
float currentTempF = 0.0;
float currentHumidity = 0.0;
// LCD display function
void displayReadingsOnOled() {
  String FtemperatureDisplay = "Temperature: " + (String)currentTempF + "°F";
  String CtemperatureDisplay = "Temperature: " + (String)currentTempC + "°C";
  String humidityDisplay = "Humidity: " + (String)currentHumidity + "%";
  // Clear the OLED screen
  Heltec.display->clear();
  // Prepare to display temperatureF
  Heltec.display->drawString(0, 0, FtemperatureDisplay);
  // Prepare to display temperatureC
  Heltec.display->drawString(0, 12, CtemperatureDisplay);
  // Prepare to display humidity
  Heltec.display->drawString(0, 24, humidityDisplay);
  // Display the readings
  Heltec.display->display();
}
// put your setup code here, to run once:
void setup() {
  // Serial Monitor to start
  Serial.begin(115200);
  // initialize the DHT sensor
  dht_sensor.begin();
  // LED light on
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Start Heltec specific LCD display
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, false /*Serial Enable*/);
  currentTempC = dht_sensor.readTemperature();
  currentTempF = dht_sensor.readTemperature(true);
  currentHumidity = dht_sensor.readHumidity();
  displayReadingsOnOled();
}
// put your main code here, to run repeatedly:
void loop() {
  // read humidity
  float humi = dht_sensor.readHumidity();
  // read temperature in Celsius
  float tempC = dht_sensor.readTemperature();
  // read temperature in Fahrenheit
  float tempF = dht_sensor.readTemperature(true);
  // check whether the reading is successful or not
  // display on the Serial Monitor (Check the specific COM port and baud number)
  if (isnan(tempC) || isnan(tempF) || isnan(humi)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");
    Serial.print(" | ");
    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");
    // Now Update Heltec Specific LCD monitor
    currentTempC = tempC;
    currentTempF = tempF;
    currentHumidity = humi;
    displayReadingsOnOled();
    if (WiFi.status() == WL_CONNECTED) {
      static bool flag = false;
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
      }
      char timeStringBuff[50];  //50 chars should be enough
      strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
      String asString(timeStringBuff);
      asString.replace(" ", "-");
      Serial.print("Time:");
      Serial.println(asString);
      String urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + "value1=" + String(currentHumidity) + "&value2=" + String(currentTempF) + "&value3=ManasaBoreddy16336125";
      Serial.print("POST data to spreadsheet:");
      Serial.println(urlFinal);
      HTTPClient http;
      http.begin(urlFinal.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
      int httpCode = http.GET();
      Serial.print("HTTP Status Code: ");
      Serial.println(httpCode);
      //---------------------------------------------------------------------
      //getting response from google sheet
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: " + payload);
      }
      //---------------------------------------------------------------------
      http.end();
    }
  }
  // wait a 4 seconds between readings
  delay(4000);
}
