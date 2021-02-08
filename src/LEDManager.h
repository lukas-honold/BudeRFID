#pragma once
#include <Arduino.h>
#include <Timer.h>
#include <Countdown.h>

class LEDManager{
public:
    LEDManager(int ledPin,int framerate):update_timer(framerate){
        pinMode(ledPin,OUTPUT);
        tgl(LOW);
        m_led_pin = ledPin;
    };

    void update(){
        if (update_timer.update())
        {
            if(is_blinking){
                if(!blink_countdown.alive()&&to_blink>0)
                {
                    // decrement the counter of blinks, toggle the lampstatus and reset the countdown timer
                    to_blink--;
                    blink_countdown.reset();
                    tgl(!current_state);
                }
                if(to_blink<=0)
                    is_blinking = false; //switch the blinking off when all blinks are gone
            }
        }
    };

    void toggle_permanent(bool state){
        permanent_on = state;
        to_blink=0;
        is_blinking = false; //no more blinking when permanent on
        tgl(state);
    };

    void blink(int nbr_blinks,float intervall=0.1f){
        is_blinking = true;
        blink_countdown.init(intervall);
        to_blink = nbr_blinks*2; //2* because on and off is one state each so for one time on and off you need two "blinks"
        permanent_on = false; //no permanent light when blinking
        tgl(true);
    }


private:

    void tgl(bool state){
        digitalWrite(m_led_pin,state);
        current_state = state;
    }

    bool current_state;

    bool permanent_on = false;
    bool is_blinking=false;
    int to_blink = 0;
    int milliseconds_per_blink = 0;
    int m_led_pin;
    Timer update_timer;
    Countdown blink_countdown;
};
