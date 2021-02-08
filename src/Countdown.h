class Countdown {
public:
    Countdown(float seconds) {
        init(seconds);
    }

    Countdown()=default;

    void init(float seconds){
        init_time = seconds;
        last_time = millis();
        time_left = seconds * 1000.f;
    }

    void set_new_time(int new_init_time){
        init_time = new_init_time;
        reset();
    }

    void reset() {
        time_left = init_time * 1000.f;
    };

    void reset(int to_add_seconds) {
        time_left += to_add_seconds * 1000.f;
    };

    bool alive() {
        time_left -= millis() - last_time;
        last_time = millis();
        return time_left > 0.f;
    }

    float getTimeLeft() { return time_left; }

private:
    float last_time;
    float init_time;
    float time_left;
};
