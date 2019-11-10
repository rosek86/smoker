#ifndef FSM_STATE_H__
#define FSM_STATE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  FSM_STATE_START_MENU,
  FSM_STATE_VIEW_CONFIG_1,
  FSM_STATE_EDIT_STEPS,
  FSM_STATE_RUNNING,
  FSM_STATE_MAIN_MENU,
  FSM_STATE_VIEW_CONFIG_2,
  FSM_STATE_EDIT_STEP,
  FSM_STATE_EDIT_TIME,
  FSM_STATE_EDIT_TEMP,
  FSM_STATE_EDIT_SMOKE,
  FSM_STATE_COUNT
} fsm_state_t;

#ifdef __cplusplus
}
#endif

#endif // FSM_STATE_H__
