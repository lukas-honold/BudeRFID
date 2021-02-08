#pragma once
#include <Arduino.h>
#include <Timer.h>

class LEDManager{
public:
    LEDManager(int ledPin){
        pinMode(ledPin,OUTPUT);
        digitalWrite(ledPin,LOW);
        m_led_pin = ledPin;
    };

    void update(){
        if(is_blinking){

        }


    };

    void toggle_permanent(bool state){
        permanent_on = state;
        to_blink=0;
        is_blinking = false; //no more blinking when permanent on
        tgl(state);
    };

    void blink(int nbr_blinks,int intervall=100){
        permanent_on = false; //no permanent light when blinking

        tgl(true);
    }


private:

    void tgl(bool state){
        digitalWrite(m_led_pin,state);
    }
    bool permanent_on = false;
    bool is_blinking=false;
    int to_blink = 0;
    int milliseconds_per_blink = 0;
    int m_led_pin;

};
