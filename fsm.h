#ifndef fsm_h
#define fsm_h

#include <stdio.h>
#include "orderTable.h"
#include "elev.h"
#include "timer.h"

typedef enum tag_fsm_state {
        STATE_INITIALIZE = 0,
        STATE_STANDBY = 1,
        STATE_ORDER_REQUEST = 2,
        STATE_AT_REQUESTED_FLOOR = 3,
        STATE_EMERGENCY = 4,
        STATE_EMERGENCY_WAIT = 5
} fsm_state;

typedef enum tag_onOFF {
	FALSE = 0,
	TRUE = 1
} onOFF;

static int lastFloor;
static int currentDest;
static int currentState;
static int emergencyRecently; // SPECIAL CASE FOR THE NEXT ORDER	

void fsm_evOn();
void fsm_evInitialize();
void fsm_evElevatorStandby();
void fsm_evOrderRequest(ord_floor_t floor, elev_button_type_t orderType); 
void fsm_evAtRequestedFloor();
void fsm_evEmergencyPressed();
void fsm_evEmergencyReleased();
void fsm_evCurrentFloor();
int fsm_getCurrentState();

#endif