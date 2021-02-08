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

#include "Timer.h"
#include "Countdown.h"
#include "Person.h"
#include "DataManager.h"
#include "CardReader.h"
#include "DisplayManager.h"
#include "KeypadManager.h"

#define relay_pin A3
#define led_pin 7
#define chip_select_rfid 5
#define reset_rfid 4
#define chip_select_sd 6




class Hardware {
public:
    Hardware(int countdown) : dataManager(chip_select_sd), keypadManager(10),
                              cardReader(chip_select_rfid, reset_rfid, 4), displayManager(0x27, 20, 4, 5),
                              ct(countdown) {};
    DataManager dataManager;
    KeypadManager keypadManager;
    CardReader cardReader;
    DisplayManager displayManager;
    Countdown ct;
};

class StateMaschine {
public:


    enum class States {
        Warten,ID_gelesen
    };

    StateMaschine(int x) : hardware(x) {};


    void update() {
        hardware.cardReader.update();
        hardware.keypadManager.update();
        hardware.displayManager.update();
    };

    void run(){
        update();
        switch (current_state) {
            case States::Warten:
                break;
            case States::ID_gelesen:
                break;
        }
    };

private:
    States current_state=States::Warten;
    Hardware hardware;
};

void setup() {
    Serial.begin(9600);
    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, LOW);
    StateMaschine stm(20);
    stm.run();
}

void loop() {
    digitalWrite(relay_pin, HIGH); // Selbsthaltung ausschalten
}