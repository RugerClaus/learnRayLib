#ifndef APPSTATE_H
#define APPSTATE_H

#include <stdio.h>
#include <stdbool.h>

typedef enum
{
    MAIN_MENU,
    GAME_ACTIVE,
    QUIT,
    NUM_STATES
} APPSTATE;

typedef struct StateManager{
    APPSTATE appState;
    APPSTATE previousAppState;
    bool appAllowedTransitions[NUM_STATES][NUM_STATES];
} StateManager;

void logStateTransitions(APPSTATE from_state, APPSTATE to_state, const char* context);
void initStateManager(StateManager* sm);
bool isTransitionAllowed(StateManager* sm, APPSTATE current_state, APPSTATE new_state);
void setAppState(StateManager *sm, APPSTATE new_state);
bool isAppState(StateManager* sm, APPSTATE state);
#endif