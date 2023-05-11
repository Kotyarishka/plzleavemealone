#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define LED_INDICATOR 5
#define SOLENOID 26
#define SS_PIN  21  /*Slave Select Pin*/
#define RST_PIN 22  /*Reset Pin for RC522*/
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define ssid "225"
#define password "225225225"
#define serverAddress "http://192.168.0.149:3000/api/v1"

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

  SPI.begin();          /*SPI communication initialized*/
  mfrc522.PCD_Init();   /*RFID sensor initialized*/ 
  Serial.println("Put your card to the reader...");
  Serial.println();


  WiFi.begin(ssid, password, 6);

  ledState = connecting;

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting");
  }
  Serial.println("Connected");
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
  Serial.println(httpCode);
  Serial.println(response);
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

  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  /*Select Card*/
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  /*Show UID for Card/Tag on serial monitor*/
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  Serial.print(content);
  content.toUpperCase();
  if (content.substring(1) == "8B 07 CC 22") /*UID for the Card/Tag we want to give access Replace with your card UID*/
  {
    Serial.println("Authorized access");  /*Print message if UID match with the database*/
    Serial.println();
    delay(500);
    digitalWrite(SOLENOID, HIGH);  /*Led Turn ON*/
    delay(2500);
    digitalWrite(SOLENOID, LOW);
  }
 else   {
    Serial.println(" Access denied"); /*If UID do not match print message*/
  }
}
