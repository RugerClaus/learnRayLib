#include "../../include/state/appstate.h"

void logStateTransitions(APPSTATE from_state, APPSTATE to_state, const char* context)
{
    printf("Transitioning from %d to %d in context %s\n", from_state, to_state, context);
}

void initStateManager(StateManager* sm)
{
    sm->appState = MAIN_MENU;
    sm->previousAppState = MAIN_MENU;

    for (int i = 0; i < NUM_STATES; i++) {
        for (int j = 0; j < NUM_STATES; j++) {
            sm->appAllowedTransitions[i][j] = false;
        }
    }

    sm->appAllowedTransitions[MAIN_MENU][GAME_ACTIVE] = true;
    sm->appAllowedTransitions[MAIN_MENU][QUIT] = true;
    
    sm->appAllowedTransitions[GAME_ACTIVE][MAIN_MENU] = true;
    sm->appAllowedTransitions[GAME_ACTIVE][QUIT] = true;
}

bool isTransitionAllowed(StateManager* sm, APPSTATE current_state, APPSTATE new_state) {
    for (int i = 0; i < NUM_STATES; i++) {
        if (sm->appAllowedTransitions[current_state][i] == new_state) {
            return true;
        }
    }
    return false;
}


void setAppState(StateManager *sm, APPSTATE new_state) {
    if (new_state == sm->appState) {
        return;
    }
    if (isTransitionAllowed(sm, sm->appState, new_state)) {
        logStateTransitions(sm->appState, new_state, "APPSTATE");
        sm->previousAppState = sm->appState;
        sm->appState = new_state;
        printf("App state: %d\n", sm->appState);
    } else {
        printf("Transition not allowed from %d to %d\n", sm->appState, new_state);
    }
}

bool isAppState(StateManager* sm, APPSTATE state)
{
    return sm->appState == state;
}

