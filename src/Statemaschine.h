#pragma once
#include "Hardware.h"

enum class StateIdentifier {
    State1, State2, State3
};

class StateMaschine;

class State {
public:
    State(StateMaschine *stm):m_stm(stm) {};

    virtual void update() = 0;

    StateIdentifier state_id;
    StateMaschine* m_stm;
};

class StateMaschine {
public:
    StateMaschine( Hardware* hrdw) {};

    void run(State** array_of_possible_states,int nbr_states,StateIdentifier start_state) {
        dyn_array = array_of_possible_states;
        current_state = start_state;
        m_nbr_states = nbr_states;
        switch_state(current_state);
        while (hardware->ct.alive()){
            hardware->update();
            current_state_running->update();
        }
    };

    void switch_state(StateIdentifier new_state) {
        //run through array of states and find the one with the right identifier
        for(int x =0;x< m_nbr_states;x++)
        {
            if(dyn_array[x]->state_id==new_state)
            {
                current_state=new_state;
                current_state_running = dyn_array[x];
                break;
            }
        }
        //wenn wir hier ankommen dann haben wir ein problem
    };
    StateIdentifier current_state;
    State* current_state_running;
    Hardware* hardware;
    int m_nbr_states;
    State** dyn_array;
private:
    void update() {
        //hw update
        current_state_running->update();
    };

};

class Test1State : public State {
public:
    Test1State(StateMaschine *stm):State(stm) {
        state_id = StateIdentifier::State1;
    };

    void update(){
        //hier sachen machen
        Serial.println("State1");
        m_stm->switch_state(StateIdentifier::State2);
    }
};

class Test2State : public State {
public:
    Test2State(StateMaschine *stm):State(stm) {
        state_id = StateIdentifier::State2;
    };
    void update(){
        Serial.println("State2");
        m_stm->switch_state(StateIdentifier::State1);
    }
};