#ifndef orderTable_h
#define orderTable_h

#define N_FLOORS 4
#define N_BUTTONS 3

typedef enum tag_ord_floor {
	FLOOR_UNDEFINED = -1,
	FLOOR_1 = 0,
	FLOOR_2 = 1,
	FLOOR_3 = 2,
	FLOOR_4 = 3
} ord_floor_t;

typedef enum tag_ord_orderTable {
	ORD_FALSE = 0,
	ORD_TRUE = 1
} ord_orderTable_t;

// PREVENTS REDEFINITION FROM elev.c
typedef enum tag_elev_lamp_type elev_button_type_t;
typedef enum tag_elev_motor_direction elev_motor_direction_t;

static int orderTable[N_FLOORS][N_BUTTONS] = { {0} };
static int lastDir = 0; //IGNORES MOMENTARY STOPS

void ord_insert(ord_floor_t floor, elev_button_type_t orderType);
void ord_eraseFloor(ord_floor_t floor);
void ord_eraseAll();
int ord_atRequestedFloor(ord_floor_t lastFloor);
int ord_ordersAbove(ord_floor_t lastFloor);
int ord_ordersBelow(ord_floor_t lastFloor);
int ord_getDir(ord_floor_t lastFloor, int emergencyRecently);

#endif