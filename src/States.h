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
        pause = Countdown(1.f);
        paused = false;
    };

    void update()
    {
        if (stateMaschine.hardware.keypadManager.is_pressed() && !paused)
        {
            switch (stateMaschine.hardware.keypadManager.get_key())
            {
            case ('*'):
                // Option Abbruch
                set_text_and_pause("Abbruch", 1.f);
                next_state = StateIdentifier::AUSGABE;
                break;
            case 'A':
                // Option Geld aufladen
                stateMaschine.hardware.displayManager.set_new_text("Aufladen:");
                stateMaschine.hardware.displayManager.set_new_text("Betrag:", true);
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
        pause.set_new_time(pause_time);
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
        stateMaschine.hardware.displayManager.set_new_text("");
        stateMaschine.hardware.displayManager.set_new_text("", true);
        stateMaschine.switch_state(StateIdentifier::CHIP_AUFLEGEN);
    }
};

class Aufladen : public State
{
public:
    Aufladen(StateMaschine &stm) : State(stm)
    {
        state_id = StateIdentifier::AUFLADEN;
    }

    void init()
    {
        counter = 0;
        is_comma = false;
        betrag = "";
        beschreibung = "";
        finished = false;
        pause = Countdown(1.f);
    };

    void update()
    {
        if (stateMaschine.hardware.keypadManager.is_pressed() && !finished)
        {
            switch (stateMaschine.hardware.keypadManager.get_key())
            {
            case '*':
                // Option Abbruch
                stateMaschine.hardware.displayManager.set_new_text("Abbruch");
                stateMaschine.hardware.displayManager.set_new_text("", true);
                next_state = StateIdentifier::AUSGABE;
                pause.set_new_time(1.f);
                finished = true;
                break;

            case 'A':
                // Option Bestätigen
                id = stateMaschine.hardware.cardReader.get_id();
                stateMaschine.hardware.dataManager.pay(betrag.toFloat(), id);
                // String person = stateMaschine.hardware.dataManager.person_to_string(id);
                // stateMaschine.hardware.displayManager.set_new_text(person);
                stateMaschine.hardware.displayManager.set_new_text("Neues Guthaben:");
                stateMaschine.hardware.displayManager.set_new_text(String(stateMaschine.hardware.dataManager.person_guthaben(id)), true);
                next_state = StateIdentifier::AUSGABE;
                pause.set_new_time(3.f);
                finished = true;
                break;

            // Werden nicht behandelt --------
            case 'B':
            case 'C':
                break;
                // -------------------------------

            case 'D':
                betrag.remove(betrag.length() - 1);
                if (is_comma)
                {
                    counter--;
                }
                if (counter == -1)
                {
                    is_comma = false;
                    counter = 0;
                }
                stateMaschine.hardware.displayManager.set_new_text(String("Betrag: "+betrag), true);
                break;

            case '#':
                is_comma = true;
                betrag += '.';
                stateMaschine.hardware.displayManager.set_new_text(String("Betrag: "+betrag), true);
                break;

            default:             // Zahlen von 0-9 werden behandelt
                if (counter < 2) // maximal 2 Nachkommastellen zulassen
                {
                    betrag += stateMaschine.hardware.keypadManager.get_key();
                    beschreibung = "Betrag: ";
                    beschreibung += betrag;
                    stateMaschine.hardware.displayManager.set_new_text(beschreibung, true);
                }
                else
                {
                    break;
                }
                if (is_comma)
                {
                    counter++;
                }

                break;
            };
        }

        if (!pause.alive() && finished)
        {
            stateMaschine.switch_state(next_state);
        }
    }

private:
    String beschreibung, betrag, id;
    StateIdentifier next_state;
    bool finished, is_comma;
    int counter;
    Countdown pause;
};
