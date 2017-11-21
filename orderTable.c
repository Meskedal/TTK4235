#include <stdio.h>
#include "orderTable.h"

typedef enum tag_elev_lamp_type {
	BUTTON_CALL_UP = 0,
	BUTTON_CALL_DOWN = 1,
	BUTTON_COMMAND = 2
} elev_button_type_t;

typedef enum tag_elev_motor_direction {
	DIRN_DOWN = -1,
	DIRN_STOP = 0,
	DIRN_UP = 1
} elev_motor_direction_t;

void ord_insert(ord_floor_t floor, elev_button_type_t orderType)
{
	orderTable[floor][orderType] = ORD_TRUE;
}

// ALL BUTTONS ON FLOOR
void ord_eraseFloor(ord_floor_t floor)
{
	for (int i = 0; i < N_BUTTONS; ++i) {
		orderTable[floor][i] = ORD_FALSE;
	}
}

void ord_eraseAll()
{
	for (int i = 0; i < N_FLOORS; ++i) {
		ord_eraseFloor(i);
	}
}

int ord_atRequestedFloor(ord_floor_t lastFloor)
{
	// IF LAST DIRECTION IS UPWARDS AND NO ORDERS ABOVE
	// SEARCH THROUGH ALL BUTTONS AT LAST FLOOR
	if (lastDir == DIRN_UP && !(ord_ordersAbove(lastFloor))) {
		for (int b = 0; b < N_BUTTONS; ++b) {
			if (orderTable[lastFloor][b] == ORD_TRUE) {
				return ORD_TRUE;
			}
		}
	} 

	// IF LAST DIRECTION IS UPWARDS AND THERE IS ORDERS ABOVE
	// IGNORE "BUTTON CALL DOWN" ORDERS
	else if (lastDir == DIRN_UP) {
		for (int b = 0; b < N_BUTTONS; ++b) {
			if (b != BUTTON_CALL_DOWN && orderTable[lastFloor][b] == ORD_TRUE) {
				return ORD_TRUE;
			}
		}
	} 

	// IF LAST DIRECTION IS DOWNWARDS AND NO ORDERS BELOW
	// SEARCH THROUGH ALL BUTTONS AT LAST FLOOR
	else if (lastDir == DIRN_DOWN && !(ord_ordersBelow(lastFloor))) {
		for (int b = 0; b < N_BUTTONS; ++b) {
			if (orderTable[lastFloor][b] == ORD_TRUE) {
				return ORD_TRUE;
			}
		}
	} 

	// IF LAST DIRECTION IS DOWNWARDS AND THERE IS ORDERS BELOW
	// IGNORE "BUTTON CALL UP" ORDERS		
	else if (lastDir == DIRN_DOWN) {
		for (int b = 0; b < N_BUTTONS; ++b) {
			if (b != BUTTON_CALL_UP && orderTable[lastFloor][b] == ORD_TRUE) {
				return ORD_TRUE;
			}
		}
	}

	// IF LAST DIRECTION IS STOP
	else {
		for (int b = 0; b < N_BUTTONS; ++b) {
			if (orderTable[lastFloor][b] == ORD_TRUE) {
				return ORD_TRUE;
			}
		}
	}

	return ORD_FALSE;
}

int ord_ordersAbove(ord_floor_t lastFloor)
{
	for (int f = FLOOR_1; f < N_FLOORS; ++f) {
		for (int b = BUTTON_CALL_UP; b < N_BUTTONS; ++b) {
			if (lastFloor < f && orderTable[f][b] == ORD_TRUE) {
				return ORD_TRUE;
			}
		}
	}
	return ORD_FALSE;
}

int ord_ordersBelow(ord_floor_t lastFloor)
{
	for (int f = FLOOR_1; f < N_FLOORS; ++f) {
		for (int b = BUTTON_CALL_UP; b < N_BUTTONS; ++b) {
			if (lastFloor > f && orderTable[f][b] == ORD_TRUE) {
				return ORD_TRUE;
			}
		}
	}
	return ORD_FALSE;
}

// GETS NEXT DIRECTION BASED ON THE ORDER TABLE
int ord_getDir(ord_floor_t lastFloor, int emergencyRecently) 
{
	switch (lastDir) {

		case DIRN_UP:

			// IF STOPPED BETWEEN FLOORS AND THERE ARE ORDERS BELOW OR ORDER AT LAST FLOOR
			if (emergencyRecently && ord_ordersBelow(lastFloor+1)) {
				lastDir = DIRN_DOWN;
				return DIRN_DOWN;
			}
			else if (ord_ordersAbove(lastFloor) == ORD_TRUE) {
				lastDir = DIRN_UP;
				return DIRN_UP;
			} 
			else if (ord_ordersBelow(lastFloor) == ORD_TRUE) {	
				lastDir = DIRN_DOWN;
				return DIRN_DOWN;
			}
			else {
				lastDir = DIRN_STOP;	
				return DIRN_STOP;
			}
			break;

		case DIRN_DOWN:

			// IF STOPPED BETWEEN FLOORS AND THERE ARE ORDERS ABOVE OR ORDER AT LAST FLOOR
			if (emergencyRecently && ord_ordersAbove(lastFloor-1)) {
				lastDir = DIRN_UP;
				return DIRN_UP;
			}
			else if (ord_ordersBelow(lastFloor) == ORD_TRUE) {	
				lastDir = DIRN_DOWN;
				return DIRN_DOWN;
			}
			else if (ord_ordersAbove(lastFloor) == ORD_TRUE) {
					lastDir = DIRN_UP;
					return DIRN_UP;
			}
			else {
				lastDir = DIRN_STOP;	
				return DIRN_STOP;
			}
			break;

		case DIRN_STOP:	

			if (ord_ordersBelow(lastFloor) == ORD_TRUE) {
				lastDir = DIRN_DOWN;
				return DIRN_DOWN;
			} 
			else if (ord_ordersAbove(lastFloor) == ORD_TRUE) {
				lastDir = DIRN_UP;
				return DIRN_UP;
			}
			else {
				lastDir = DIRN_STOP;
				return DIRN_STOP;
			}
			break;
	}
	return DIRN_STOP;
}
