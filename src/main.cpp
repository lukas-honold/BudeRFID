//internal arduino libs
#include <Arduino.h>
#include "SPI.h"

//external libs
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

//class files
#include "Countdown.h"
#include "Hardware.h"
#include "Person.h"
#include "Statemaschine.h"
#include "States.h"
#include "Timer.h"



void setup() {
    Serial.begin(9600);
    SPI.begin();
    Hardware hrdw(30);
    StateMaschine stm(hrdw);
    State *states[5] = {
        new ChipAuflegen(stm),
        new Warten(stm),
        new ID_Gelesen(stm),
        new Ausgabe(stm),
        new Aufladen(stm)};

    auto nbr_states = sizeof(states) / sizeof(State *);
    stm.run(states, nbr_states, StateIdentifier::CHIP_AUFLEGEN);
}

void loop() {
}