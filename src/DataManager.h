#pragma once
class DataManager {
  public:
    DataManager(int cs) {
      //        Serial.print("Start");
      //        if (!SD.begin(cs)) {
      //            Serial.println("Card failed, or not present");
      //            while (1) {};
      //        } else {
      //            Serial.println("card initialized.");
      //        }
    };

    ~DataManager() {
    };

    void import_data() {
      //        File dataFile = SD.open("datalog.txt");
      //        if (dataFile) {
      //            counter = 0;
      //            while (dataFile.available()) {
      //                char x = (char) dataFile.read();
      //                if (x == ';') {
      //                    counter++;
      //                }
      //                daten += x;
      //            }
      //            dataFile.close();
      //        } else {
      //            Serial.println("Fuck");
      //        }


      daten = "Luedi,12345678,10.0;\nKirsch,87654321,20.0;";
      counter = 2;

      //      Serial.println(daten);
      //      Serial.println(counter);

      int start_index = 0;
      int end_index = 0;
      int start_index_subdaten = 0;
      int end_index_subdaten = 0;
      String subdaten;
      String name;
      String id;
      float guthaben = 0.f;

      // Zeilenweise die Daten pro Person auslesen und ein zugehöriges Objekt erzeugen
      for (int i = 0; i < counter; i++) {
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
        Serial.println(personen[i].get_name());
        Serial.println(personen[i].get_id());
        Serial.println(personen[i].get_guthaben());

      }



    };


  private:
    int counter = 0;
    String daten;
    Person personen[10];
};
