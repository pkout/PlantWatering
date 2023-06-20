#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

const char *SSID = "pela";
const char *PASSWORD = "rockbed50";
const char *URL = "http://jarvis.petrkout.com:4000/plants";
const int UNITS_COUNT = 4;
const int LOOP_DELAY_MS = 1000;
const int STATUS_CHECK_PERIOD_MS = 5000;
WiFiClient client;
HTTPClient http;
int runningTimeMs;
int unitOnDuration[UNITS_COUNT];
String responseBody;
String onOff, duration;
int durationMs;

void setupSerial() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("Connecting to ");
  Serial.println(SSID);
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  bool ledOnOff = false;
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    if (ledOnOff) {
      digitalWrite(LED_BUILTIN, LOW);
    }
    else {
      digitalWrite(LED_BUILTIN, HIGH);
    }

    ledOnOff = !ledOnOff;
    
    delay(1000);
    Serial.print(".");
    // Serial.println(WiFi.status());
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

int makeHTTPRequest(String &responseBody) {
  http.begin(client, URL);

  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    String payload = http.getString();
    responseBody = payload;
  }

  http.end();

  return httpResponseCode;
}

int parsePayloadSegment(const char *payload, String &segment, const uint8 index) {
  char str[strlen(payload)];
  char *p;
  String unit;
  uint8 i = 0;

  strcpy(str, payload);
  p = strtok(str, "|");

  while (p != NULL) {
    if (i == index) {
      segment = String(p);

      return 0;
    }
    
    p = strtok(NULL, "|");
    i++;
  }

  return -1;
}

int getOnOff(const char *payload, String &onOff, uint8 unitIndex) {
  return parsePayloadSegment(payload, onOff, unitIndex);
}

int getDuration(const char *payload, String &duration, uint8 unitIndex) {
  return parsePayloadSegment(payload, duration, unitIndex + 1);
}

void setup() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // LED off

  setupSerial();
  setupWifi();
  delay(1000);
}

int gpioByIndex(int index) {
  switch (index) {
    case 0:
      return D1;
    case 1:
      return D2;
    case 2:
      return D3;
    case 3:
      return D4;
  }

  return -1;
}

void loop() {
  uint8 unitIndex = 0;
  uint8 segmentsPerUnit = 2;
  int status = 0;
  
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);

    if (runningTimeMs % STATUS_CHECK_PERIOD_MS == 0) {
      status = makeHTTPRequest(responseBody);
      Serial.print("Response status code: ");
      Serial.println(status);

      if (status == -1) {
        Serial.println("Restarting");
        ESP.restart();
      }
      
      Serial.print("Response body: ");
      Serial.println(responseBody);
    }
    
    for (uint8 unitIndex = 0; unitIndex < UNITS_COUNT; unitIndex++) {
      status = getOnOff(responseBody.c_str(), onOff, unitIndex * segmentsPerUnit);
      status = getDuration(responseBody.c_str(), duration, unitIndex * segmentsPerUnit);
      durationMs = duration.toInt();
      Serial.print("Unit: ");
      Serial.println(unitIndex + 1);
      Serial.print("OnOff: ");
      Serial.println(onOff);
      Serial.print("Duration: ");
      Serial.println(duration);
      
      if (unitOnDuration[unitIndex] == 0) {
        Serial.print("Turning on GPIO ");
        Serial.println(unitIndex);
        digitalWrite(gpioByIndex(unitIndex), HIGH);
      }

      if (unitOnDuration[unitIndex] < durationMs) {
        unitOnDuration[unitIndex] = unitOnDuration[unitIndex] + LOOP_DELAY_MS;
      }
      else {
        Serial.print("Turning off GPIO ");
        Serial.println(unitIndex);
        digitalWrite(gpioByIndex(unitIndex), LOW);
        unitOnDuration[unitIndex] = 0;
      }
    }
  }
  else {
    Serial.println("Disconnected");
  }

  runningTimeMs = runningTimeMs + LOOP_DELAY_MS;
  
  delay(LOOP_DELAY_MS);
  
/*  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  
  int httpCode = https.GET();
      if (https.begin(*client, "https://jigsaw.w3.org/HTTP/connection.html")) {  // HTTPS

      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
*/
 
  /*digitalWrite(D1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(7000);                       // wait for a second
  digitalWrite(D1, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);                       // wait for a second
  digitalWrite(D2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(7000);                       // wait for a second
  digitalWrite(D2, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);*/                       // wait for a second
}
