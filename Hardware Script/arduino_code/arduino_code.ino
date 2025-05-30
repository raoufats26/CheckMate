#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// ----- RFID -----
#define RFID_SS_PIN 10
#define RFID_RST_PIN 9
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);

// ----- Keypad -----
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, A0};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ----- Communication with ESP8266 -----
SoftwareSerial espSerial(A1, A2); // RX, TX

// ----- Servo and LEDs -----
Servo doorServo;
#define SERVO_PIN A3
#define GREEN_LED A4
#define RED_LED A5

#define DOOR_OPEN_ANGLE 90
#define DOOR_CLOSED_ANGLE 160

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  // Don't attach servo here — only when used
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  Serial.println("System ready. Scan your RFID.");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String rfidUID = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      if (rfid.uid.uidByte[i] < 0x10) rfidUID += "0";
      rfidUID += String(rfid.uid.uidByte[i], HEX);
    }
    rfidUID.toUpperCase();

    Serial.print("Card UID: ");
    Serial.println(rfidUID);
    Serial.println("Enter 4-digit PIN:");

    String enteredPin = "";
    unsigned long startTime = millis();

    while (enteredPin.length() < 4 && (millis() - startTime < 15000)) {
      char key = keypad.getKey();
      if (key >= '0' && key <= '9') {
        enteredPin += key;
        Serial.print("*");
      }
    }
    Serial.println();

    if (enteredPin.length() < 4) {
      Serial.println("PIN entry timeout or incomplete!");
      digitalWrite(RED_LED, HIGH);
      delay(2000);
      digitalWrite(RED_LED, LOW);
      return;
    }

    // Send data
    String msg = rfidUID + "," + enteredPin;
    espSerial.println(msg);
    Serial.println("Sent to ESP: " + msg);

    // Wait for response
    String response = "";
    unsigned long respStart = millis();
    while ((millis() - respStart) < 10000) {
      while (espSerial.available()) {
        char c = espSerial.read();
        response += c;
      }
      if (response.indexOf("true") != -1 || response.indexOf("false") != -1) {
        break;
      }
      delay(50);
    }

    if (response.indexOf("true") != -1) {
      Serial.println("ACCESS_GRANTED");
      digitalWrite(GREEN_LED, HIGH);

      doorServo.attach(SERVO_PIN);
      doorServo.write(DOOR_OPEN_ANGLE);
      delay(4000);
      doorServo.write(DOOR_CLOSED_ANGLE);
      delay(4000);
      doorServo.detach();

      digitalWrite(GREEN_LED, LOW);
    } else if (response.indexOf("false") != -1) {
      Serial.println("ACCESS_DENIED");
      digitalWrite(RED_LED, HIGH);
      delay(3000);
      digitalWrite(RED_LED, LOW);
    } else {
      Serial.println("No response from ESP!");
      digitalWrite(RED_LED, HIGH);
      delay(6000);
      digitalWrite(RED_LED, LOW);
    }

    // Wait for card removal
    while (rfid.PICC_IsNewCardPresent()) delay(100);
    rfid.PICC_HaltA();
  }
}

