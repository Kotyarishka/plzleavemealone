#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "AndroidAP2CEB";
const char* password = "animelife";
const char* host = "192.168.155.171";

int ledPin = 5;
Servo myservo;
int pos = 0;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  connectToWiFi();
  myservo.attach(9);
}

void loop() {
  if (client.connect(host, 3000)) {
    sendHttpRequest();
    handleResponse();
    client.stop();
  }
  delay(1000);
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Connected!");
}

void sendHttpRequest() {
  client.print(String("GET / HTTP/1.1\r\n") +
               "Host: " + String(host) + "\r\n" +
               "Connection: close\r\n" +
               "\r\n");
}

void handleResponse() {
  bool bodyStarted = false;
  int contentLength = 0;

  while (client.connected() || client.available()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      if (!bodyStarted) {
        if (line == "\r") {
          bodyStarted = true;
        } else if (line.startsWith("Content-Length: ")) {
          contentLength = line.substring(16).toInt();
        }
      } else {
        if (contentLength > 0) {
          if (line == "1") {
            digitalWrite(ledPin, HIGH);
            for (pos = 0; pos <= 180; pos += 1) {
                delay(15);  
                }
          } else {
            digitalWrite(ledPin, LOW);
            for (pos = 180; pos <= 0; pos += 1) {
                delay(15);  
                }
          }
          Serial.println(line);
        }
      }
    }
  }
}