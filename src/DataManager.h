#pragma once
#include <SD.h>
#include <SPI.h>
#include <Person.h>

MFRC522 mfrc522(9, 8);  // Create MFRC522 instance

class DataManager
{
public:
    DataManager(int cs_sd, int i_framerate) : m_timer(i_framerate)
    {
        init(cs_sd);
        import_data();
    };

    ~DataManager(){};

    // void dummy_payments(){
    //     personen[0].add_guthaben(-2.f);
    //     personen[1].add_guthaben(2.f);
    // }

    void init(int chipSelect_sd)
    {
        Serial.begin(9600);
        SPI.begin();
        mfrc522.PCD_Init();                // Init MFRC522
        delay(4);                          // Optional delay. Some board do need more time after init to be ready, see Readme
        mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
        Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
        // Open serial communications and wait for port to open:

        while (!Serial)
        {
            ; // wait for serial port to connect. Needed for native USB port only
        }

        Serial.print("Initializing SD card...");

        // see if the card is present and can be initialized:
        if (!SD.begin(chipSelect_sd))
        {
            Serial.println("Card failed, or not present");
            // don't do anything more:
            while (1)
                ;
        }
        Serial.println("card initialized.");

        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        File dataFile = SD.open("datalog.txt");

        // if the file is available, write to it:
        if (dataFile)
        {
            while (dataFile.available())
            {
                Serial.write(dataFile.read());
            }
            dataFile.close();
        }
        // if the file isn't open, pop up an error:
        else
        {
            Serial.println("error opening datalog.txt");
        }
    }

    void update()
    {
        if (m_timer.update())
        {

            if (mfrc522.PICC_IsNewCardPresent())
            {
                m_is_card_present = true;
                mfrc522.PICC_ReadCardSerial();
                int x = (int)mfrc522.uid.size;
                m_id = "";
                for (int i = 0; i < x; i++)
                {
                    m_id += String(mfrc522.uid.uidByte[i]);
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

    bool pay(float money, String id)
    {
        return personen[person_index_by_id(id)].add_guthaben(money);
    }

    float person_guthaben(String id)
    {
        return personen[person_index_by_id(id)].get_guthaben();
    }

    String person_to_string(String id)
    {
        int index = person_index_by_id(id);
        if (index == -1)
        {
            return "Falsche Karte";
        }
        String data;
        data += personen[index].get_name();
        data += ": ";
        data += personen[index].get_guthaben();
        return data;
    };

    void import_data()
    {
        // SD Kartenimplementierung -----------------------------------------------
        // // open the file. note that only one file can be open at a time,
        // // so you have to close this one before opening another.
        // File dataFile = SD.open("datalog.txt");

        // // if the file is available, write to it:
        // String daten = "";
        // counter = 0;

        // if (dataFile) {
        //     while (dataFile.available()) {
        //         char x = (char)dataFile.read();
        //         if (x == ';') {
        //             counter++;
        //         }
        //         daten += x;
        //     }
        //     dataFile.close();
        // }
        // // if the file isn't open, pop up an error:
        // else {
        //     Serial.println("error opening datalog.txt");
        // }

        // Serial.println("Importierte Daten:");
        // Serial.println(daten);
        // Serial.println(counter);
        // --------------------------------------------------------------------------

        daten = "Luedi,1864555133,10.0;\nKirsch,5775247193,20.0;";
        counter = 2;

        int start_index = 0;
        int end_index = 0;
        int start_index_subdaten = 0;
        int end_index_subdaten = 0;
        String subdaten;
        String name;
        String id;
        float guthaben = 0.f;

        // Zeilenweise die Daten pro Person auslesen und ein zugehöriges Objekt erzeugen
        for (int i = 0; i < counter; i++)
        {
            // Auslesen einer Zeile der Datei
            end_index = daten.indexOf(';', start_index);
            subdaten = daten.substring(start_index, end_index);
            start_index = end_index + 2;

            // Subdaten nach Name, ID und Guthaben aufsplitten
            end_index_subdaten = subdaten.indexOf(',', start_index_subdaten);
            name = subdaten.substring(start_index_subdaten, end_index_subdaten);
            start_index_subdaten = end_index_subdaten + 1;
            end_index_subdaten = subdaten.indexOf(',', start_index_subdaten);
            id = subdaten.substring(start_index_subdaten, end_index_subdaten);
            start_index_subdaten = end_index_subdaten + 1;
            guthaben = subdaten.substring(start_index_subdaten, subdaten.length() - 1).toFloat();

            // Zurücksetzen der Subdateindexe für den nächsten Zeilendurchlauf
            start_index_subdaten = 0;
            end_index_subdaten = 0;

            // Initialisierung der Person
            personen[i] = Person(name, id, guthaben);

            // Überprüfung des angelegten Objekts
            Serial.println("Angelegte Person:");
            Serial.println(personen[i].get_name());
            Serial.println(personen[i].get_id());
            Serial.println(personen[i].get_guthaben());
        }
    };

    void export_data()
    {
        String data = "";
        for (int i = 0; i < counter; i++)
        {
            data += personen[i].get_name() + ",";
            data += personen[i].get_id() + ",";
            data += String(personen[i].get_guthaben()) + ";";
            if (i < counter - 1)
            {
                data += "\n";
            }
        }
        Serial.println("Exportierte Daten");
        Serial.println(data);

        // SD Kartenimplementierung -------------------------------
        // SD.remove("datalog.txt");
        // File dataFile = SD.open("datalog.txt", FILE_WRITE);
        // dataFile.print(data);
        // dataFile.close();
        // --------------------------------------------------------
    };

private:
    int person_index_by_id(String id)
    {
        int person_index;
        for (int i = 0; i < counter; i++)
        {
            if (id == personen[i].get_id())
            {
                person_index = i;
                return person_index;
            }
        }
        return -1;
    };

    int counter;
    String daten;
    Person personen[10];

    Timer m_timer;
    String m_id;
    bool m_is_card_present = false;
};
