
#pragma once
class DisplayManager {
  public:
    DisplayManager(int address, int p1, int p2, int i_framerate) : lcd(address, p1, p2), m_timer(i_framerate) {
      lcd.init();
      lcd.backlight();
      lcd.setCursor(3, 0);
    };

    ~DisplayManager() = default;

    void set_new_text(String to_print) {
      next_input = to_print;
      change_made = true;
    };

    void update() {
      if (m_timer.update()) {
          if(change_made){
              change_made = false;
              test += 1;
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(next_input);
          }

      }
    };

  private:
    bool change_made = false;
    Timer m_timer;
    String next_input;
    LiquidCrystal_I2C lcd;  // set the LCD address to 0x27 for a 16 chars and 2 line display
    float frametime = 0.f;
    int test = 0;
};
