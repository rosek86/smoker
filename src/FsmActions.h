#ifndef FSM_ACTIONS_H__
#define FSM_ACTIONS_H__

void fsmActionsInit(void);

void fsmActionsSetTemperature(float temperature);
void fsmActionsSetEncoder(int encoder);
void fsmActionsButtonPressed(void);

void fsmActionsRun(void);

#endif // FSM_ACTIONS_H__
