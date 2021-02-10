
#pragma once

struct Cursor {
    Cursor(int m_x,int m_y):x(m_x),y(m_y){};
    Cursor() = default;
    int x = 0;
    int y = 0;
};

class CustomCharGenerator{
public:
    CustomCharGenerator(){
        byte customChar[8] = {
                B00000,
                B00000,
                B00000,
                B00000,
                B00000,
                B00000,
                B00000,
                B00000
        };
        byte full = B11111;
        for(int x=7;x<=0;x--){
            customChar[x]= full;
            for(int y=0;y<8;y++)
                new_chars[x][y]=customChar[y];
        }
    }
    byte new_chars[8][8];
};



class DisplayManager {
public:
    DisplayManager(int address, int p1, int p2, int i_framerate) : lcd(address, p1, p2), m_timer(i_framerate) {
        lcd.init();
        lcd.backlight();
        lcd.setCursor(3, 0);
        cursor_line1.x = 0;
        cursor_line2.y = 0;
        cursor_line2.x = 0;
        cursor_line2.y = 1;
        CustomCharGenerator gen;
        for(int x=0;x<8;x++){
            lcd.createChar(x, gen.new_chars[x]);
        }
    };
    void write_percentage_char(Cursor position,int percent){
        custom_char_cursor = position;
        current_custom_char = byte(map(percent,0,100,0,7));//map percentage to respective char
    }
    ~DisplayManager() = default;

    void set_new_text(String to_print, bool line = false) {
        if (line) {
            line2 = to_print;
        } else {
            line1 = to_print;
        }
        changes_made = true;
    };

    void update() {
        if (m_timer.update()) {
            update_display();
        }
    };

    bool custom_char_gets_drawn = false;


private:
    void update_display(){
            if (changes_made) {
                changes_made = false;
                lcd.clear();
                lcd.setCursor(cursor_line1.x, cursor_line1.y);
                lcd.print(line1);
                lcd.setCursor(cursor_line2.x, cursor_line2.y);
                lcd.print(line2);
                if(custom_char_gets_drawn){
                    lcd.setCursor(custom_char_cursor.x,custom_char_cursor.y);
                    lcd.write(current_custom_char);
                }
            }
    };




    String line1, line2;
    Cursor cursor_line1, cursor_line2,custom_char_cursor;
    byte current_custom_char=0;
    Timer m_timer;
    LiquidCrystal_I2C lcd;  // set the LCD address to 0x27 for a 16 chars and 2 line display
    float frametime = 0.f;
    bool changes_made = false;
    bool changes_made_second_line = false;
};
