
#define led_pin 7

class CardReader {
public:
    CardReader(int i_cs, int i_reset, int i_framerate) : rfid_scanner(i_cs, i_reset), m_timer(i_framerate) {
        SPI.begin();
        rfid_scanner.PCD_Init();        // Init MFRC522
    };

    void update() {
        if (m_timer.update()) {
            if (rfid_scanner.PICC_IsNewCardPresent()) {
                rfid_scanner.PICC_ReadCardSerial();
                digitalWrite(led_pin, HIGH);
                int x = (int) rfid_scanner.uid.size;
                m_id = "";
                for (int i = 0; i < x; i++) {
                    m_id += String(rfid_scanner.uid.uidByte[i]);
                }
            } else {
                digitalWrite(led_pin, LOW);
            }
        }
    };

    String get_id() {
        return m_id;
    };

private:
    MFRC522 rfid_scanner;
    Timer m_timer;
    String m_id;
};
