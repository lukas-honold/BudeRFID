//internal arduino libs
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

//external libs
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>

#include "Timer.h"
#include "Countdown.h"
#include "Person.h"

#include "Hardware.h"
#include "Statemaschine.h"


#define led_pin 7


/*

class StateMaschine
{
public:
    enum class States
    {
        WARTEN,
        CHIP_AUFLEGEN,
        ID_GELESEN,
        AUFLADEN,
        AUSGABE,
        AUSSCHALTEN
    };

    StateMaschine(int countdown_init) : hardware(countdown_init){};

    void update()
    {
        hardware.cardReader.update();
        hardware.keypadManager.update();
        hardware.displayManager.update();
        hardware.ledManager.update();
    };

    void run()
    {
        hardware.dataManager.import_data();
        while (hardware.ct.alive())
        {
            update();
            switch (current_state)
            {

            case States::CHIP_AUFLEGEN:
                // Brüßungstext
                hardware.displayManager.set_new_text("Chip auflegen");
                current_state = States::WARTEN;
                break;

            case States::WARTEN:

                // Karte auslesen und LED an- bzw. ausschalten
                if (hardware.cardReader.is_card_present())
                {
                    hardware.ledManager.blink(1, 3.f);
                    hardware.displayManager.set_new_text(hardware.dataManager.person_to_string(hardware.cardReader.get_id()));
                    current_state = States::ID_GELESEN;
                }

                break;

            case States::ID_GELESEN:
                if (hardware.keypadManager.is_pressed())
                {
                    // Option Karten_ID anzeigen
                    if (hardware.keypadManager.get_key() == '*')
                    {
                        set_text_and_pause("Abbruch", 0.5f);
                        current_state = States::AUSGABE;
                    }

                    // Option Geld aufladen
                    if (hardware.keypadManager.get_key() == 'A')
                    {
                        hardware.displayManager.set_new_text("Betrag: ");
                        current_state = States::AUFLADEN;
                    }

                    // Option 1.00€ abbuchen
                    if (hardware.keypadManager.get_key() == 'B')
                    {
                        hardware.dataManager.pay(-1.f, hardware.cardReader.get_id());
                        set_text_and_pause(hardware.dataManager.person_to_string(hardware.cardReader.get_id()), 2.f);
                        current_state = States::AUSGABE;
                    }

                    // Option 0.50€ abbuchen
                    if (hardware.keypadManager.get_key() == 'C')
                    {
                        hardware.dataManager.pay(-0.5f, hardware.cardReader.get_id());
                        set_text_and_pause(hardware.dataManager.person_to_string(hardware.cardReader.get_id()), 2.f);
                        current_state = States::AUSGABE;
                    }

                    // Option Karten_ID anzeigen
                    if (hardware.keypadManager.get_key() == 'D')
                    {
                        set_text_and_pause(hardware.cardReader.get_id(), 10.f);
                        current_state = States::AUSGABE;
                    }
                }
                break;

            case States::AUFLADEN:




                break;
                
            case States::AUSGABE:
                if (!pause.alive())
                {
                    hardware.ct.reset();
                    current_state = States::CHIP_AUFLEGEN;
                }
                break;
            }
        }
        hardware.powerManager.switch_off();
    };

private:
    void set_text_and_pause(String text, float pause_time){
        hardware.displayManager.set_new_text(text);
        pause = Countdown(pause_time);
    }


    States current_state = States::CHIP_AUFLEGEN;
    Hardware hardware;
    Countdown pause;
};

*/



void setup()
{
    Hardware hrdw(20);
    Serial.begin(9600);

    StateMaschine stm(&hrdw);

    State* states[]={
            new Test1State(&stm),
            new Test2State(&stm),
    };

    stm.run(states,2,StateIdentifier::State1);
}

void loop()
{
}