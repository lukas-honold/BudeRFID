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


class Hardware{
public:
    Hardware(int countdown): dataManager(chip_select_sd), keypadManager(10),
                             cardReader(chip_select_rfid, reset_rfid, 4), displayManager(0x27, 20, 4, 5),
                             ct(countdown){};
    DataManager dataManager;
    KeypadManager keypadManager;
    CardReader cardReader;
    DisplayManager displayManager;
    Countdown ct;
};


class State;
class StateMaschine{
public:
    void set_next_state(State*);
};
class TestState;
class Test2State;


class State {
public:
    State(StateMaschine *stm,Hardware& hrdw):m_hardware(hrdw),m_stm(stm) {

    }

    virtual void update() = 0;

    StateMaschine* m_stm;
    Hardware& m_hardware;
};


class TestState : private State{
public:
    TestState(StateMaschine*stm,Hardware& hrdw):State(stm,hrdw){

    };

    void update(){
        m_stm->set_next_state(m_stm->test2);
    }

};



class StateMaschine {
public:
    StateMaschine(int countdown) : hardware(countdown) {
        hardware.dataManager.import_data();
    };

    void update() {
        hardware.keypadManager.update();
        hardware.displayManager.update();
        hardware.cardReader.update();
    }

    void run() {
        while (hardware.ct.alive())
        {
            update();
            running_state->update();
        }
    }



    void set_next_state(State* next_state){
        running_state = next_state;
    }


private:
    Hardware hardware;
    State* running_state;
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