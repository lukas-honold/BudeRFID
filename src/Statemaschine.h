#pragma once
#include "Hardware.h"

enum class StateIdentifier
{
    CHIP_AUFLEGEN,
    WARTEN,
    ID_GELESEN,
    AUFLADEN,
    AUSGABE
};

class StateMaschine;

class State
{
public:
    State(StateMaschine &stm) : stateMaschine(stm){};
    virtual void update() = 0;
    virtual void init()=0;
    StateIdentifier state_id;
    StateMaschine &stateMaschine;
};

class StateMaschine
{
public:
    StateMaschine(Hardware& hrdw):hardware(hrdw) {};  // seconds of inactivity after which the device goes automatically into standby

    void run(State **array_of_possible_states, int nbr_states, StateIdentifier start_state)
    {
        dyn_array = array_of_possible_states;
        current_state = start_state;
        m_nbr_states = nbr_states;
        switch_state(current_state);
        while (hardware.ct.alive())
        {
            hardware.update();
            current_state_running->update();
        }
        hardware.dataManager.export_data();
        hardware.powerManager.switch_off();
    };

    void switch_state(StateIdentifier new_state)
    {
        //run through array of states and find the one with the right identifier
        for (int x = 0; x < m_nbr_states; x++)
        {
            if (dyn_array[x]->state_id == new_state)
            {
                current_state = new_state;
                current_state_running = dyn_array[x];
                current_state_running->init(); //init the new state
                break;
            }
        }
    };

    StateIdentifier current_state;
    State *current_state_running;
    Hardware& hardware;
    int m_nbr_states;
    State **dyn_array;

private:
    void update()
    {
        current_state_running->update();
    };
};
