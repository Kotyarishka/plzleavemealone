#include <WiFi.h>
#include <HTTPClient.h>

#define LED_INDICATOR 5
#define SOLENOID 4

#define ssid "Wokwi-GUEST"
#define password ""
#define serverAddress "http://fbc7-91-210-248-232.ngrok.io/api/v1"

#define lockId "lock1"

WiFiClient wifiClient;
HTTPClient httpInit;

enum State {
  connecting,
  connected,
  error
} ledState;

bool ledStatus = false;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password, 6);

  ledState = connecting;

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  httpInit.begin(wifiClient, serverAddress "/lock/create");
  
  httpInit.addHeader("Content-Type", "application/json");
  int httpResponseCode = httpInit.POST("{\"lockId\": \"" lockId "\"}");

  if (httpResponseCode == 200) {
    ledState = connected;
  } else if (httpResponseCode == 302) {
    ledState = connected;
  } else {
    String serverResponse = httpInit.getString();

    ledState = error;
  };

  pinMode(LED_INDICATOR, OUTPUT);
  pinMode(SOLENOID, OUTPUT);

  digitalWrite(LED_INDICATOR, LOW);
  digitalWrite(4, HIGH);

  httpInit.end();
}

int requestTimer = millis();
void makeRequest() {
  if (requestTimer >= millis()) {
    return;
  };

  HTTPClient http;

  http.begin(wifiClient, serverAddress "/lock/" lockId);
  http.addHeader("Content-Type", "text/plain");
  
  int httpCode = http.GET();
  String response = http.getString();

  if (httpCode == HTTP_CODE_OK) {
    if (response == "locked") {
      digitalWrite(LED_INDICATOR, HIGH);
      digitalWrite(SOLENOID, HIGH);
    } else if (response == "unlocked") {
      digitalWrite(LED_INDICATOR, LOW);
      digitalWrite(SOLENOID, LOW);
    }

  } else {
    ledState = error;
  }

  http.end();

  requestTimer = millis() + 100;
};

int updateLedTimer = millis();
void updateLedState() {
  
  if (updateLedTimer >= millis()) {
    return;
  }

  if (ledState == error) {
    if (ledStatus) {
      digitalWrite(LED_INDICATOR, LOW);
      updateLedTimer = millis() + 200;

      ledStatus = false;
    } else {
      digitalWrite(LED_INDICATOR, HIGH);
      updateLedTimer = millis() + 200;

      ledStatus = true;
    }
  }
}

void loop() {
  // Make request to server
  makeRequest();

  // update led state
  updateLedState();
}