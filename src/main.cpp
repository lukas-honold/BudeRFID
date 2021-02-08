#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#define arduino_ide

#ifdef arduino_ide
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SD.h>

#else

#include <Keypad/src/Keypad.h>
#include <LiquidCrystal_I2C/LiquidCrystal_I2C.h>
#include <MFRC522/src/MFRC522.h>
#include <SD/src/SD.h>

#endif


#define relay_pin A3
#define led_pin 7
#define chip_select_rfid 5
#define reset_rfid 4
#define chip_select_sd 6

#include "Timer.h"
#include "Countdown.h"
#include "Person.h"
#include "DataManager.h"
#include "CardReader.h"
#include "DisplayManager.h"
#include "KeypadManager.h"


void setup() {
  Serial.begin(9600);
  Serial.println("Hello");
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);
  DataManager dataManager(chip_select_sd);
  dataManager.import_data();
  KeypadManager keypadManager(10);
  CardReader cardReader(chip_select_rfid, reset_rfid, 4);
  DisplayManager displayManager(0x27, 20, 4, 5);
  Countdown ct(20.0);

  while (ct.alive()) {
    keypadManager.update();
    cardReader.update();
    if (keypadManager.get_is_pressed()) {
      ct.reset();
      Serial.println(keypadManager.get_key());
      displayManager.update();
    }
    displayManager.set_new_text("ID:" + String(cardReader.get_id()));
    displayManager.update();

  }

}

void loop() {
  digitalWrite(relay_pin, HIGH); // Selbsthaltung ausschalten
}