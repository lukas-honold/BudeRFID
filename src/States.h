#pragma once
#include "Statemaschine.h"
#include <Arduino.h>

class Test1State : public State {
public:
    Test1State(StateMaschine &stm):State(stm) {
        state_id = StateIdentifier::State1;
    };

    void update(){
        //hier sachen machen
        Serial.println("State1");
        stateMaschine.switch_state(StateIdentifier::State2);
    }
};

class Test2State : public State {
public:
    Test2State(StateMaschine &stm):State(stm) {
        state_id = StateIdentifier::State2;
    };
    void update(){
        Serial.println("State2");
        stateMaschine.hardware.ledManager.toggle_permanent(true);
        stateMaschine.switch_state(StateIdentifier::State1);
    }

private:

};
