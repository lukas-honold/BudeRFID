//internal arduino libs
#include <Arduino.h>

//external libs
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

//class files
#include "Timer.h"
#include "Countdown.h"
#include "Person.h"
#include "Hardware.h"
#include "Statemaschine.h"
#include "States.h"

#define led_pin 7


void setup() {
    Serial.begin(9600);
    StateMaschine stm;

    State *states[] = {
        new ChipAuflegen(stm),
        new Warten(stm),
        new ID_Gelesen(stm),
        new Ausgabe(stm),
        new Aufladen(stm)
    };

    auto nbr_states = sizeof(states)/sizeof(State*);
    stm.run(states, nbr_states, StateIdentifier::CHIP_AUFLEGEN);
}

void loop() {
}