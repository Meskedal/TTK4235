#include "fsm.h"

// CHECKS FOR INVALID FLOOR
void fsm_evOn()
{ 
	lastFloor = elev_get_floor_sensor_signal();

	if (lastFloor == FLOOR_UNDEFINED) {
		currentState = STATE_INITIALIZE;	
	} 
	else {
		elev_set_floor_indicator(lastFloor);
		currentState = STATE_STANDBY;
	}
}

int fsm_getCurrentState()
{
	return currentState;
}

// SENDS ELEVATOR TO DEFINED FLOOR
void fsm_evInitialize()
{
	elev_set_motor_direction(DIRN_DOWN);

	if (elev_get_floor_sensor_signal() != FLOOR_UNDEFINED) {
   		elev_set_motor_direction(DIRN_STOP);
   		lastFloor = elev_get_floor_sensor_signal();
   		elev_set_floor_indicator(lastFloor);
   		currentState = STATE_STANDBY;
   	} 
}

// UPDATES LAST FLOOR
void fsm_evCurrentFloor()
{
	int currentFloor = elev_get_floor_sensor_signal();

	if (currentFloor != FLOOR_UNDEFINED) {
		lastFloor = currentFloor;
		elev_set_floor_indicator(lastFloor);

		// IF ALSO AT REQUESTED FLOOR
		if (ord_atRequestedFloor(currentFloor)) {
			elev_set_motor_direction(DIRN_STOP);
			fsm_evAtRequestedFloor();
		}
	} 
}

// INSERTS ORDER, SETS NEXT DIRECTION
void fsm_evOrderRequest(ord_floor_t floor, elev_button_type_t orderType)
{
	ord_insert(floor, orderType);
	elev_set_button_lamp(orderType, floor, TRUE);

	// DON'T SET NEXT DIRECTION BEFORE DOOR IS CLOSED, UNLESS UNCHANGED DESTINATION
	if (((currentState != STATE_AT_REQUESTED_FLOOR && currentState != STATE_EMERGENCY_WAIT)) || (ord_getDir(lastFloor, emergencyRecently) == DIRN_STOP)) {
		currentState = STATE_ORDER_REQUEST;
		elev_set_motor_direction(ord_getDir(lastFloor, emergencyRecently));
		emergencyRecently = FALSE;
	} 
}

void fsm_evAtRequestedFloor()
{
	if (lastFloor != FLOOR_4) {
		elev_set_button_lamp(BUTTON_CALL_UP, lastFloor, FALSE); 
	}
	if (lastFloor != FLOOR_1) {
		elev_set_button_lamp(BUTTON_CALL_DOWN, lastFloor, FALSE);
	}
	elev_set_button_lamp(BUTTON_COMMAND, lastFloor, FALSE);

	ord_eraseFloor(lastFloor);
	elev_set_door_open_lamp(TRUE);
	timer_start(3);

	currentState = STATE_AT_REQUESTED_FLOOR;
}

void fsm_evEmergencyPressed()
{
	elev_set_motor_direction(DIRN_STOP);
	elev_set_stop_lamp(TRUE);
	ord_eraseAll();

	for (int f = 1; f < N_FLOORS-1; ++f) {
        for (int b = 0; b < N_BUTTONS; ++b) {
				elev_set_button_lamp(b, f, FALSE);
		}
	}
    elev_set_button_lamp(BUTTON_CALL_UP, FLOOR_1, FALSE);
    elev_set_button_lamp(BUTTON_COMMAND, FLOOR_1, FALSE);
    elev_set_button_lamp(BUTTON_CALL_DOWN, FLOOR_4, FALSE);
    elev_set_button_lamp(BUTTON_COMMAND, FLOOR_4, FALSE);
  
	if (elev_get_floor_sensor_signal() != FLOOR_UNDEFINED) {
		elev_set_door_open_lamp(TRUE);
	}

	currentState = STATE_EMERGENCY;
}

void fsm_evEmergencyReleased()
{
	elev_set_stop_lamp(FALSE);

	if (elev_get_floor_sensor_signal() != FLOOR_UNDEFINED) {
		timer_start(3);
		currentState = STATE_EMERGENCY_WAIT;
	} 

	// WHEN BETWEEN FLOORS
	else {
		emergencyRecently = TRUE; 
	}
}

void fsm_evElevatorStandby()
{
	elev_set_door_open_lamp(FALSE);
	elev_set_motor_direction(ord_getDir(lastFloor, emergencyRecently));
	emergencyRecently = FALSE;
	
	if (ord_getDir(lastFloor, emergencyRecently) != DIRN_STOP) {
		currentState = STATE_ORDER_REQUEST;
	}
	else {
		currentState = STATE_STANDBY; 
	}
}
	
