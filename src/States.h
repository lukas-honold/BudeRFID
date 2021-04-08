#pragma once

#include <Arduino.h>

#include "Statemaschine.h"

class ChipAuflegen : public State
{
public:
    ChipAuflegen(StateMaschine &stm) : State(stm)
    {
        stateMaschine.hardware.ledManager.blink(1, 2.f);
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

    void init()
    {
        paused = false;
        pause = Countdown(1.f);
    };

    void update()
    {
        new_time_left = int(stateMaschine.hardware.ct.getTimeLeft() / 1000);
        if (!paused)
        {
            if (new_time_left != time_left)
            {
                time_left = new_time_left;
                standby_message = "Standby in: ";
                standby_message += time_left;
                stateMaschine.hardware.displayManager.set_new_text(standby_message, true);
            }

            if (stateMaschine.hardware.dataManager.is_card_present() && !paused)
            {
                stateMaschine.hardware.ledManager.toggle_permanent(true);
                person_text = stateMaschine.hardware.dataManager.person_to_string(stateMaschine.hardware.dataManager.get_id());
                stateMaschine.hardware.displayManager.set_new_text(person_text);

                if (person_text == "Falsche Karte")
                {
                    stateMaschine.hardware.ledManager.blink(5, 0.2f);
                    stateMaschine.hardware.displayManager.set_new_text(String("ID: " + stateMaschine.hardware.dataManager.get_id()), true);
                    next_state = StateIdentifier::AUSGABE;
                    paused = true;
                    pause.set_new_time(10.f);
                }
                else
                {
                    stateMaschine.hardware.displayManager.set_new_text("", true);
                    stateMaschine.switch_state(StateIdentifier::ID_GELESEN);
                }
            }

            // Anzeige des Gesamtguthabens aller Personen bei Tastendruck von D
            if (stateMaschine.hardware.keypadManager.is_pressed())
            {
                switch (stateMaschine.hardware.keypadManager.get_key())
                {
                case 'D':
                    float total = stateMaschine.hardware.dataManager.total_guthaben();
                    stateMaschine.hardware.displayManager.set_new_text("Gesamt:");
                    stateMaschine.hardware.displayManager.set_new_text(String(total), true);
                    paused = true;
                    pause.set_new_time(3.f);
                    next_state = StateIdentifier::AUSGABE;
                    break;

                default:
                    break;
                }
            }
        }

        if (!pause.alive() && paused)
        {
            stateMaschine.switch_state(next_state);
        }
    }

private:
    int time_left, new_time_left;
    bool paused;
    StateIdentifier next_state;
    String standby_message, person_text;
    Countdown pause;
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
        payment_successful = false;
        stateMaschine.hardware.ct.reset();
    };

    void update()
    {
        if (stateMaschine.hardware.keypadManager.is_pressed() && !paused)
        {
            stateMaschine.hardware.ledManager.toggle_permanent(false);
            switch (stateMaschine.hardware.keypadManager.get_key())
            {
            case ('*'):
                // Option Abbruch
                set_text_and_pause("Abbruch", 1.f);
                next_state = StateIdentifier::AUSGABE;
                stateMaschine.hardware.ledManager.blink(3, 0.2f);
                break;

                // Option Geld aufladen oder abbuchen ---------
            case 'A':
            case 'B':
                stateMaschine.hardware.displayManager.set_new_text("Aufladen/Zahlen:");
                stateMaschine.hardware.displayManager.set_new_text("Betrag:", true);
                next_state = StateIdentifier::AUFLADEN;
                break;
                // --------------------------------------------

            case 'C':
                // Option 1.00€ abbuchen
                payment_successful = stateMaschine.hardware.dataManager.pay(-1.f, stateMaschine.hardware.dataManager.get_id());
                if (payment_successful)
                {
                    set_text_and_pause(stateMaschine.hardware.dataManager.person_to_string(stateMaschine.hardware.dataManager.get_id()), 1.f);
                }
                else
                {
                    set_text_and_pause("Nicht genug Geld", 2.f);
                    stateMaschine.hardware.ledManager.blink(5, 0.2f);
                }
                next_state = StateIdentifier::AUSGABE;
                break;

            case 'D':
                // Option Karten_ID anzeigen
                set_text_and_pause(String("ID: " + stateMaschine.hardware.dataManager.get_id()), 10.f);
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
    bool paused, payment_successful;
    Countdown pause;
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
        stateMaschine.hardware.ct.reset();
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
                stateMaschine.hardware.ledManager.blink(3, 0.2f);
                next_state = StateIdentifier::AUSGABE;
                pause.set_new_time(1.f);
                finished = true;
                break;

            case 'A':
                // Aufladen bestätigen
                id = stateMaschine.hardware.dataManager.get_id();
                stateMaschine.hardware.dataManager.pay(betrag.toFloat(), id);
                stateMaschine.hardware.displayManager.set_new_text("Neues Guthaben:");
                stateMaschine.hardware.displayManager.set_new_text(String(stateMaschine.hardware.dataManager.person_guthaben(id)), true);
                stateMaschine.hardware.ledManager.blink(1, 1.5f);
                next_state = StateIdentifier::AUSGABE;
                pause.set_new_time(3.f);
                finished = true;
                break;

            case 'B':
                // Bezahlen bestätigen
                id = stateMaschine.hardware.dataManager.get_id();
                payment_successful = stateMaschine.hardware.dataManager.pay((-1.f) * betrag.toFloat(), id);
                if (payment_successful)
                {
                    stateMaschine.hardware.displayManager.set_new_text("Neues Guthaben:");
                    stateMaschine.hardware.displayManager.set_new_text(String(stateMaschine.hardware.dataManager.person_guthaben(id)), true);
                    stateMaschine.hardware.ledManager.blink(1, 1.5f);
                    pause.set_new_time(3.f);
                }
                else
                {
                    set_text_and_pause("Nicht genug Geld", 2.f);
                    stateMaschine.hardware.displayManager.set_new_text("", true);
                    stateMaschine.hardware.ledManager.blink(5, 0.2f);
                }
                next_state = StateIdentifier::AUSGABE;
                finished = true;
                break;

            // Wird nicht behandelt --------
            case 'C':
                break;
                // -----------------------------

            case 'D':
                betrag.remove(betrag.length() - 1, 2);
                if (is_comma)
                {
                    counter--;
                }
                if (counter == -1)
                {
                    is_comma = false;
                    counter = 0;
                }
                beschreibung = "Betrag: ";
                beschreibung += betrag;
                stateMaschine.hardware.displayManager.set_new_text(beschreibung, true);
                break;

            case '#':
                if (is_comma)
                {
                    break;
                }
                is_comma = true;
                betrag += '.';
                beschreibung = "Betrag: ";
                beschreibung += betrag;
                stateMaschine.hardware.displayManager.set_new_text(beschreibung, true);
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
    void set_text_and_pause(String text, float pause_time)
    {
        stateMaschine.hardware.displayManager.set_new_text(text);
        pause.set_new_time(pause_time);
    };

    String beschreibung, betrag, id;
    StateIdentifier next_state;
    bool finished, is_comma, payment_successful;
    ;
    int counter;
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
        stateMaschine.hardware.ledManager.toggle_permanent(false);
        stateMaschine.switch_state(StateIdentifier::CHIP_AUFLEGEN);
    }
};
