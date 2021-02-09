#pragma once

#include "Timer.h"
#include "Countdown.h"
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

    void update(){
        keypadManager.update();
        cardReader.update();
        displayManager.update();
        ledManager.update();
    }
};