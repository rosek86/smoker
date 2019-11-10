#ifndef FSM_EVENTS_H__
#define FSM_EVENTS_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  FSM_EVT_CLICK_VIEW,
  FSM_EVT_CLICK_EXIT,
  FSM_EVT_CLICK_EDIT,
  FSM_EVT_CLICK_START,
  FSM_EVT_CLICK_STOP,
  FSM_EVT_CLICK,
  FSM_EVT_ENCODER,
  FSM_EVT_TIMER,
  FSM_EVT_CLICK_TIME,
  FSM_EVT_CLICK_TEMP,
  FSM_EVT_CLICK_SMOKE,
  FSM_EVT_CLICK_NEXT,
  FSM_EVT_CLICK_PREV,
  FSM_EVT_COUNT
} fsm_event_t;

#ifdef __cplusplus
}
#endif

#endif // FSM_EVENTS_H__
