#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <string.h>


// Define RFID pins
#define SS_PIN 10     // SDA pin for RFID
#define RST_PIN 9     // RST pin for RFID

String answer="N/A";
// LCD and Bluetooth initialization

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial BTSerial(A0, A1); // RX, TX for Bluetooth module

// RFID initialization
MFRC522 rfid(SS_PIN, RST_PIN);

// Authorized RFID UID
const String authorizedUID = "140,251,53,2"; 

void setup() {
  // Initialize serial communications
  Serial.begin(9600);
  BTSerial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();  // Initialize RFID module
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("Scan your RFID");
}

void loop() {
  String rfidUID = rfid_read();
  if (rfidUID != "") {
    sendData(rfidUID);
    //continue;
  }

  delay(1000);  // Check every second
}

String rfid_read() {
  String rfidUID = "";

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < rfid.uid.size; i++) {
      rfidUID += String(rfid.uid.uidByte[i]);
      if (i < rfid.uid.size - 1) {
        rfidUID += ",";
      }
    }

    rfid.PICC_HaltA();  // Stop reading
    rfid.PCD_StopCrypto1();

    //Serial.print("RFID UID: ");
   // Serial.println(rfidUID);

    lcd.clear();
    if (rfidUID == authorizedUID) {
      /*lcd.setCursor(0, 0);
      lcd.print("Krishna");
      BTSerial.println("Krishna");*/
      lcd.setCursor(0,0);
      lcd.println("Card Detected"); 
      BTSerial.println("Card Detected to transact");
      BTSerial.println("Would you like to transfer your money ? y/n :");
      char ans= (char)BTSerial.read();
      answer+=ans;

      BTSerial.println(answer);

      if(answer=="y"){
        lcd.println("Money successfully transacted");
        Serial.println("Money successfully transacted");
        BTSerial.println("Money successfully transacted");
      }
      else if(answer=="n"){
        lcd.println("Authorization Denied");
        Serial.println("Authorization Denied");
        BTSerial.println("Authorization Denied");
      }


    } else {
      lcd.setCursor(0, 0);
      lcd.print("Please Register");
      //lcd.print(rfidUID);
      lcd.setCursor(0, 1);
      lcd.print("Your Card");
      BTSerial.println("Please Register Your Card");
      delay(2000);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scan your RFID");
    BTSerial.println("Scan your RFID");
  }

  return rfidUID;
}

void sendData(String rfidUID) {
  BTSerial.print("RFID UID: ");
  BTSerial.println(rfidUID);
}