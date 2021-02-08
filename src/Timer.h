class Timer {
public:
    Timer(int framerate) {
        frametime = 1000.f / framerate;
        last_time = millis();
    }

    ~Timer() = default;

    bool update() {
        if (millis() > last_time + frametime) {
            last_time = millis();
            return true;
        }
        return false;
    }

private:
    float frametime;
    float last_time;
};
