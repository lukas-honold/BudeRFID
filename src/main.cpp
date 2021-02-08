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
#include "LEDManager.h"
#include "PowerManager.h"

#define relay_pin A3
#define chip_select_rfid 5
#define reset_rfid 4
#define chip_select_sd 6
#define led_pin 7

class Hardware
{
public:
    Hardware(int countdown) : dataManager(chip_select_sd), keypadManager(10),
                              cardReader(chip_select_rfid, reset_rfid, 4), displayManager(0x27, 20, 4, 5),
                              ct(countdown), ledManager(led_pin, 10), powerManager(relay_pin){};
    DataManager dataManager;
    KeypadManager keypadManager;
    CardReader cardReader;
    DisplayManager displayManager;
    Countdown ct;
    LEDManager ledManager;
    PowerManager powerManager;
};

class StateMaschine
{
public:
    enum class States
    {
        WARTEN,
        CHIP_AUFLEGEN,
        ID_GELESEN,
        ABGEBUCHT,
        AUFLADEN,
        ID_AUSGEGEBEN,
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
        hardware.ledManager.blink(5, 1.f);
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
                    hardware.ledManager.toggle_permanent(true);
                    hardware.displayManager.set_new_text(hardware.cardReader.get_id());
                    current_state = States::ID_GELESEN;
                }

                // Testkommentar
                break;

            case States::ID_GELESEN:
                hardware.ledManager.toggle_permanent(hardware.cardReader.is_card_present());

                if (hardware.keypadManager.is_pressed())
                {
                    hardware.ct.reset();
                    hardware.displayManager.set_new_text(String(hardware.keypadManager.get_key()));
                    if (hardware.keypadManager.get_key() == '*')
                    {
                        hardware.displayManager.set_new_text(String(hardware.keypadManager.get_key()));
                        current_state = States::ID_AUSGEGEBEN;
                    }
                }
                break;
            case States::ABGEBUCHT:
                break;
            case States::AUFLADEN:
                break;
            case States::ID_AUSGEGEBEN:
                break;
            }
        }
        hardware.powerManager.switch_off();
    };

private:
    States current_state = States::CHIP_AUFLEGEN;
    Hardware hardware;
};

void setup()
{
    Serial.begin(9600);
    StateMaschine stm(20);
    stm.run();
}

void loop()
{
}