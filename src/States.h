#pragma once
#include "Statemaschine.h"
#include <Arduino.h>

class ChipAuflegen : public State
{
public:
    ChipAuflegen(StateMaschine &stm) : State(stm)
    {
        state_id = StateIdentifier::CHIP_AUFLEGEN;
    };

    void init(){}

    void update()
    {
        stateMaschine.hardware.displayManager.set_new_text("Chip auflegen");
        stateMaschine.switch_state(StateIdentifier::WARTEN);
    }
};

class Warten : public State
{
public:
    Warten(StateMaschine &stm) : State(stm)
    {
        state_id = StateIdentifier::WARTEN;
    };

    void init(){};

    void update()
    {
        if (stateMaschine.hardware.cardReader.is_card_present())
        {
            stateMaschine.hardware.ledManager.blink(1, 3.f);
            stateMaschine.hardware.displayManager.set_new_text(stateMaschine.hardware.dataManager.person_to_string(stateMaschine.hardware.cardReader.get_id()));
            stateMaschine.switch_state(StateIdentifier::ID_GELESEN);
        }
    }
};

class Test1State : public State
{
public:
    Test1State(StateMaschine &stm) : State(stm)
    {
        state_id = StateIdentifier::State1;
    };

    void init(){};

    void update()
    {
        //hier sachen machen
        Serial.println("State1");
        stateMaschine.switch_state(StateIdentifier::State2);
    }
};

class Test2State : public State
{
public:
    Test2State(StateMaschine &stm) : State(stm)
    {
        state_id = StateIdentifier::State2;
    };
    void update()
    {
        Serial.println("State2");
        stateMaschine.hardware.ledManager.toggle_permanent(true);
        stateMaschine.switch_state(StateIdentifier::State1);
    }

    void init()
    {
        //wird einmal aufgerufen
    }

private:
};
