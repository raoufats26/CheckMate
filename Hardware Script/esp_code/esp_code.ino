#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

// Wi-Fi credentials
const char* ssid = "Raouf";
const char* password = "raouf3421";

// Cloud API endpoint
const char* serverUrl = "https://check-mate-gamma.vercel.app/api/hardware_endpoint";
const char* pingUrl = "https://check-mate-gamma.vercel.app/api/hardware_endpoint";  // Use same endpoint to keep warm

// Keep server awake every 5 minutes
unsigned long lastPing = 0;
const unsigned long pingInterval = 5 * 60 * 1000; // 5 minutes

void setup() {
  Serial.begin(9600);
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.print(F("Connecting to WiFi"));
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(F("\n✅ WiFi connected"));
  Serial.print(F("📶 IP Address: "));
  Serial.println(WiFi.localIP());
}

void loop() {
  // Periodic ping to keep server alive
  if (millis() - lastPing > pingInterval) {
    lastPing = millis();
    keepServerAlive();
  }

  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();

    if (msg.length() > 0) {
      int separatorIndex = msg.indexOf(',');
      if (separatorIndex > 0) {
        String rfid = msg.substring(0, separatorIndex);
        String pin = msg.substring(separatorIndex + 1);

        Serial.println(F("\n➡️ Sending to cloud:"));
        Serial.println("RFID: " + rfid);
        Serial.println("PIN: " + pin);

        if (WiFi.status() == WL_CONNECTED) {
          Serial.print(F("📉 Free Heap: "));
          Serial.println(ESP.getFreeHeap());

          std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure());
          client->setInsecure();

          HTTPClient https;
          if (https.begin(*client, serverUrl)) {
            https.setTimeout(10000); // 10s timeout
            https.setReuse(false);
            https.addHeader(F("Content-Type"), F("application/json"));

            String postBody = "{\"rfid_tag\":\"" + rfid + "\",\"pin\":\"" + pin + "\"}";
            Serial.println("POST Body: " + postBody);

            int httpCode = https.POST(postBody);

            // Retry on error -11
            int retries = 3;
            while (retries-- > 0 && httpCode == -11) {
              Serial.println(F("🔁 Retrying..."));
              delay(300);
              httpCode = https.POST(postBody);
            }

            if (httpCode > 0) {
              String response = https.getString();
              Serial.println("✅ Response Code: " + String(httpCode));
              Serial.println("📨 Server Response: " + response);

              if (response.indexOf("\"access\":true") >= 0) {
  Serial.println("true");
} else {
  Serial.println("false");
}

            } else {
              Serial.print(F("❌ POST Error: "));
              Serial.println(httpCode);
              Serial.println(F("ACCESS_DENIED"));
              delay(1000);
            }

            https.end();
          } else {
            Serial.println(F("❌ Unable to connect to server"));
            Serial.println(F("ACCESS_DENIED"));
            delay(1000);
          }
        } else {
          Serial.println(F("⚠️ WiFi not connected."));
          Serial.println(F("ACCESS_DENIED"));
          delay(1000);
        }
      }
    }
  }
}

void keepServerAlive() {
  if (WiFi.status() == WL_CONNECTED) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure());
    client->setInsecure();

    HTTPClient https;
    if (https.begin(*client, pingUrl)) {
      https.setTimeout(5000);
      https.GET();  // Just ping, ignore response
      https.end();
      Serial.println("📡 Pinged server to stay awake.");
    }
  }
}
