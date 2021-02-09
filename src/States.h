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
            stateMaschine.hardware.displayManager.set_new_text(
                stateMaschine.hardware.dataManager.person_to_string(stateMaschine.hardware.cardReader.get_id()));
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

    void init()
    {
        Serial.print("Hallo jemand zuhause?");
        paused = false;
    };

    void update()
    {
        
        if (stateMaschine.hardware.keypadManager.is_pressed() && !paused)
        {
            char key = stateMaschine.hardware.keypadManager.get_key();
            Serial.println(key);
            switch (key)
            {
            case ('*'):
                // Option Abbruch
                set_text_and_pause("Abbruch", 0.5f);
                next_state = StateIdentifier::AUSGABE;
                break;
            case 'A':
                // Option Geld aufladen
                stateMaschine.hardware.displayManager.set_new_text("Betrag: ");
                next_state = StateIdentifier::AUFLADEN;
                break;
            case 'B':
                // Option 1.00€ abbuchen
                stateMaschine.hardware.dataManager.pay(-1.f, stateMaschine.hardware.cardReader.get_id());
                set_text_and_pause(
                    stateMaschine.hardware.dataManager.person_to_string(
                        stateMaschine.hardware.cardReader.get_id()),
                    2.f);
                next_state = StateIdentifier::AUSGABE;
                break;
            case 'C':
                // Option 0.50€ abbuchen
                stateMaschine.hardware.dataManager.pay(-0.5f, stateMaschine.hardware.cardReader.get_id());
                set_text_and_pause(
                    stateMaschine.hardware.dataManager.person_to_string(
                        stateMaschine.hardware.cardReader.get_id()),
                    2.f);
                next_state = StateIdentifier::AUSGABE;
                break;
            case 'D':
                // Option Karten_ID anzeigen
                set_text_and_pause(stateMaschine.hardware.cardReader.get_id(), 10.f);
                next_state = StateIdentifier::AUSGABE;
                break;
            default:
                break;
            }
            paused = true;
        }

        if (!pause.alive() && paused)
        {
            stateMaschine.switch_state(next_state);
        }
    };

private:
    void set_text_and_pause(String text, float pause_time)
    {
        stateMaschine.hardware.displayManager.set_new_text(text);
        pause = Countdown(pause_time);
    };

    StateIdentifier next_state;
    bool paused;
    Countdown pause;
};

class Ausgabe : public State
{
public:
    Ausgabe(StateMaschine &stm) : State(stm)
    {
        state_id = StateIdentifier::AUSGABE;
    }

    void init(){};

    void update()
    {
        stateMaschine.hardware.ct.reset();
        stateMaschine.switch_state(StateIdentifier::CHIP_AUFLEGEN);
    };
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
