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

    void init() {}

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

class ID_Gelesen : public State
{
public:
    ID_Gelesen(StateMaschine &stm) : State(stm)
    {
        state_id = StateIdentifier::ID_GELESEN;
    }

    void init();

    void update()
    {
        if (stateMaschine.hardware.keypadManager.is_pressed())
        {
            // Option Karten_ID anzeigen
            if (stateMaschine.hardware.keypadManager.get_key() == '*')
            {
                set_text_and_pause("Abbruch", 0.5f);
                stateMaschine.switch_state(StateIdentifier::AUSGABE);
            }

            // Option Geld aufladen
            if (stateMaschine.hardware.keypadManager.get_key() == 'A')
            {
                stateMaschine.hardware.displayManager.set_new_text("Betrag: ");
                stateMaschine.switch_state(StateIdentifier::AUFLADEN);
            }

            // Option 1.00€ abbuchen
            if (stateMaschine.hardware.keypadManager.get_key() == 'B')
            {
                stateMaschine.hardware.dataManager.pay(-1.f, stateMaschine.hardware.cardReader.get_id());
                set_text_and_pause(stateMaschine.hardware.dataManager.person_to_string(stateMaschine.hardware.cardReader.get_id()), 2.f);
                stateMaschine.switch_state(StateIdentifier::AUSGABE);
            }

            // Option 0.50€ abbuchen
            if (stateMaschine.hardware.keypadManager.get_key() == 'C')
            {
                stateMaschine.hardware.dataManager.pay(-0.5f, stateMaschine.hardware.cardReader.get_id());
                set_text_and_pause(stateMaschine.hardware.dataManager.person_to_string(stateMaschine.hardware.cardReader.get_id()), 2.f);
                stateMaschine.switch_state(StateIdentifier::AUSGABE);
            }

            // Option Karten_ID anzeigen
            if (stateMaschine.hardware.keypadManager.get_key() == 'D')
            {
                set_text_and_pause(stateMaschine.hardware.cardReader.get_id(), 10.f);
                stateMaschine.switch_state(StateIdentifier::AUSGABE);
            }
        }
    };

private:
    void set_text_and_pause(String text, float pause_time)
    {
        stateMaschine.hardware.displayManager.set_new_text(text);
        stateMaschine.set_pause(pause_time);
    };
};

class Ausgabe : public State
{
public:
    Ausgabe(StateMaschine &stm) : State(stm)
    {
        state_id = StateIdentifier::AUSGABE;
    }

    void init();

    void update()
    {
        if (!stateMaschine.still_paused())
        {
            stateMaschine.hardware.ct.reset();
            stateMaschine.switch_state(StateIdentifier::CHIP_AUFLEGEN);
        }
    }
};

/*
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
*/
