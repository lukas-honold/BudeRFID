#pragma once


class CardReader
{
public:
    CardReader(int i_cs, int i_reset, int i_framerate) : rfid_scanner(i_cs, i_reset), m_timer(i_framerate)
    {
        SPI.begin();
        rfid_scanner.PCD_Init(); // Init MFRC522
    };

    void update()
    {
        if (m_timer.update())
        {

            if (rfid_scanner.PICC_IsNewCardPresent())
            {
                m_is_card_present = true;
                rfid_scanner.PICC_ReadCardSerial();
                int x = (int)rfid_scanner.uid.size;
                m_id = "";
                for (int i = 0; i < x; i++)
                {
                    m_id += String(rfid_scanner.uid.uidByte[i]);
                }
            }
            else
            {
                m_is_card_present = false;
            }
        }
    };

    String get_id()
    {
        return m_id;
    };

    bool is_card_present()
    {
        return m_is_card_present;
    };

private:
    MFRC522 rfid_scanner;
    Timer m_timer;
    String m_id;
    bool m_is_card_present = false;
};
