#pragma once

#include "Timer.h"
#include "Countdown.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "KeypadManager.h"
#include "LEDManager.h"
#include "PowerManager.h"

#define relay_pin A3
#define chip_select_rfid 9
#define reset_rfid 8
#define chip_select_sd 10
#define led_pin A0

class Hardware
{
public:
    Hardware(int countdown) : dataManager(chip_select_sd, 4),
                              keypadManager(10), displayManager(0x27, 20, 4, 5),
                              ct(countdown), ledManager(led_pin, 10), powerManager(relay_pin){};

    DataManager dataManager;
    KeypadManager keypadManager;
    DisplayManager displayManager;
    Countdown ct;
    LEDManager ledManager;
    PowerManager powerManager;

    void update()
    {
        keypadManager.update();
        dataManager.update();
        displayManager.update();
        ledManager.update();
    }
};
