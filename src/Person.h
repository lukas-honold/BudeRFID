#pragma once

class Person {
  public:
    Person() {};

    Person(String &i_name, String &i_id, float i_guthaben) : id(i_id), name(i_name), guthaben(i_guthaben) {

    }

    ~Person() = default;

    // getter für die Variablen einer Person ----------
    String get_name() {
      return name;
    }

    String get_id() {
      return id;
    }

    float get_guthaben(){
      return guthaben;
    }
    // ------------------------------------------------

    // setter für das Guthaben
    void add_guthaben(float money){
      if(guthaben + money >= 0.f){
              guthaben += money;     
      } else {
        // Fehlerbehandlung
      }
        
    }

  private:
    String id;
    String name;
    float guthaben;

};
