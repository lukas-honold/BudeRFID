#pragma once
#include <Arduino.h>

class PowerManager{
public:
    PowerManager(int relay_pin){
        pinMode(relay_pin,OUTPUT);
        digitalWrite(relay_pin,LOW);
        m_rel_pin = relay_pin;
    }

    void switch_off(){
        digitalWrite(m_rel_pin,HIGH);
    }

private:
    int m_rel_pin;
};
