#ifndef FSM_H__
#define FSM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "fsm_event.h"
#include "fsm_state.h"

typedef void (*fsm_action_t)(void);

typedef struct {
  fsm_action_t start;
  fsm_action_t stop;

  fsm_action_t edit_steps;
  fsm_action_t update_edit_steps;

  fsm_action_t start_menu;
  fsm_action_t update_start_menu;

  fsm_action_t view;
  fsm_action_t update_view;

  fsm_action_t main_menu;
  fsm_action_t update_main_menu;

  fsm_action_t progress;
  fsm_action_t update_progress;

  fsm_action_t edit_step;
  fsm_action_t update_edit_step;

  fsm_action_t edit_time;
  fsm_action_t update_edit_time;

  fsm_action_t edit_temp;
  fsm_action_t update_edit_temp;

  fsm_action_t edit_smoke;
  fsm_action_t update_edit_smoke;

  fsm_action_t edit_set_time;
  fsm_action_t edit_set_temp;
  fsm_action_t edit_set_smoke;

  fsm_action_t edit_next_step;

  void (*no_action)(fsm_state_t, fsm_event_t);
} fsm_actions_t;

void fsm_init(fsm_actions_t const *actions);
void fsm_event(fsm_event_t event);

fsm_state_t fsm_get_state(void);

#ifdef __cplusplus
}
#endif

#endif // FSM_H__
