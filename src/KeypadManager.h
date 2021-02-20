#pragma once

#define ROWS 4
#define COLS 4

const char keys[ROWS][COLS]  = {{'1', '2', '3', 'A'},
                         {'4', '5', '6', 'B'},
                         {'7', '8', '9', 'C'},
                         {'*', '0', '#', 'D'}};

 byte rowPins[ROWS]= {3,2,17,18};  // Row pinouts of the keypad

 byte colPins[COLS]  = {7,6,5,4};  // Column pinouts of the keypad

class KeypadManager {
   public:
    KeypadManager(int framerate) : keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS), timer(framerate){};

    ~KeypadManager() = default;

    void update() {
        if (timer.update()) {
            key = keypad.getKey();
            if (key) {
                pressed = true;
            } else {
                pressed = false;
            }
        }
    }

    char get_key() {
        pressed = false;
        return key;
    }

    bool is_pressed() {
        return pressed;
    }

   private:
    Timer timer;
    char key;
    bool pressed = false;
    Keypad keypad;
};
