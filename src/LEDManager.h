#pragma once
#define ledpin 6
#include <Arduino.h>


class LEDManager{
    LEDManager(){
        pinMode(ledpin,OUTPUT);
    };



};
