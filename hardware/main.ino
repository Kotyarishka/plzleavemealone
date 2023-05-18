#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define LED_INDICATOR 5
#define SOLENOID 26
#define SS_PIN 21  /*Slave Select Pin*/
#define RST_PIN 22 /*Reset Pin for RC522*/
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define ssid "225"
#define password "225225225"
#define serverAddress "http://192.168.0.150:3000/api/v1"

#define lockId "lock1"

WiFiClient wifiClient;
HTTPClient httpInit;

bool cardOverride = false;

void setup()
{
  Serial.begin(115200);

  SPI.begin();        /*SPI communication initialized*/
  mfrc522.PCD_Init(); /*RFID sensor initialized*/

  WiFi.begin(ssid, password, 6);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting");
  }
  Serial.println("Connected");
  httpInit.begin(wifiClient, serverAddress "/lock/create");

  httpInit.addHeader("Content-Type", "application/json");
  httpInit.POST("{\"lockId\": \"" lockId "\"}");

  // pinMode(LED_INDICATOR, OUTPUT);
  pinMode(SOLENOID, OUTPUT);

  // digitalWrite(LED_INDICATOR, LOW);
  digitalWrite(4, HIGH);

  httpInit.end();
}

int requestTimer = millis();
void makeRequest()

{ 
  Serial.println("make request 1");
  if (requestTimer >= millis())
  {
    return;
  };

  if (cardOverride) {
    return;
  };
  
  Serial.println("make request 2");

  HTTPClient http;

  http.begin(wifiClient, serverAddress "/lock/" lockId);
  http.addHeader("Content-Type", "text/plain");

  int httpCode = http.GET();
  String response = http.getString();
  // Serial.println(httpCode);
  // Serial.println(response);
  if (httpCode == HTTP_CODE_OK)
  {
    if (response == "locked")
    {
      digitalWrite(SOLENOID, HIGH);
    }
    else if (response == "unlocked")
    {
      digitalWrite(SOLENOID, LOW);
    }
  }

  http.end();

  requestTimer = millis() + 100;
};

int cardTimer = millis();
String decoupleId = "";
void checkRFID()
{
  if (cardTimer > millis())
  {
    return;
  }

  if (!mfrc522.PICC_IsNewCardPresent())
  {
    cardTimer = millis() + 100;
    cardOverride = false;

    Serial.println("no new card avaibla");
    decoupleId = "";
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial())
  {
    cardTimer = millis() + 100;
    cardOverride = false;

    decoupleId = "";
    return;
  }

  if (decoupleId == "")
  {
    // generate decoupleId from random string and numbers

    for (int i = 0; i < 10; i++)
    {
      decoupleId += String(random(0, 10));
    }
  }

  String content;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();
  Serial.print(content);
  Serial.println(" rfid");
    if (content.substring(1) == "8B 07 CC 22") /*UID for the Card/Tag we want to give access Replace with your card UID*/
  {
    requestTimer = millis() + 5000;
    cardOverride = true;
    
    digitalWrite(SOLENOID, LOW);

  }
}

void loop()
{


  // Make request to server
  makeRequest();

  // Check RFID
  checkRFID();

  Serial.println(cardOverride);  
}

void loop()
{
  // Make request to server
  makeRequest();

  // Check RFID
  checkRFID();
}
