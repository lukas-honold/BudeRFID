#pragma once

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {{'1', '2', '3', 'A'},
                         {'4', '5', '6', 'B'},
                         {'7', '8', '9', 'C'},
                         {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {3, 8, 9, 10}; // Row pinouts of the keypad

byte colPins[COLS] = {A0, A1, A2, 2}; // Column pinouts of the keypad

class KeypadManager
{
public:
  KeypadManager(int framerate) : keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS), timer(framerate){};

  ~KeypadManager() = default;

  void update()
  {
    if (timer.update())
    {
      key = keypad.getKey();
      if (key)
      {
        pressed = true;
      }
      else
      {
        pressed = false;
      }
    }
  }

  char get_key()
  {
    pressed = false;
    return key;
  }

  bool is_pressed()
  {
    return pressed;
  }

private:
  Timer timer;
  char key;
  bool pressed = false;
  Keypad keypad;
};
