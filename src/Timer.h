#pragma once

class Timer {
public:
    Timer(int framerate) {
        m_frametime = 1000.f / framerate;
        last_time = millis();
    }

    Timer(){};

    ~Timer() = default;

    void init(int framerate){
        m_frametime = 1000.f/framerate;
        last_time = millis();
    }

    void set_frametime_in_ms(int new_fr_time){
        m_frametime=new_fr_time;
    }

    bool update() {
        if (millis() > last_time + m_frametime) {
            last_time = millis();
            return true;
        }
        return false;
    }

private:
    float m_frametime =0;
    float last_time;
};
