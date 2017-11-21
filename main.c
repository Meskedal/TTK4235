#include "elev.h"
#include "fsm.h"
#include <unistd.h>

int main() 
{

    // INITIALIZE HARDWARE
    if (!elev_init()) {
      printf("Unable to initialize elevator hardware!\n");
      return 1;
    }

 	  int lastButtonState[N_FLOORS][N_BUTTONS] = { {FALSE} };

 	  fsm_evOn();
 	
    /////////////////
    /// MAIN LOOP ///
    /////////////////

    while (TRUE) {

   		switch(fsm_getCurrentState()) {
     			case STATE_INITIALIZE:
     				  fsm_evInitialize();
     				  break;

     			case STATE_ORDER_REQUEST:
     				  fsm_evCurrentFloor();
     				  break;

     			case STATE_AT_REQUESTED_FLOOR:
     			    if (timer_isTimeOut()) {
     					    fsm_evElevatorStandby();
     				  }
     				  break;

     			case STATE_EMERGENCY:
     				  if (!elev_get_stop_signal()) {
     					    fsm_evEmergencyReleased();
     				  }
     				  break;

     			case STATE_EMERGENCY_WAIT:
     				  if (timer_isTimeOut()) {
     					    fsm_evElevatorStandby();
     				  }
     				  break;

     			case STATE_STANDBY:

              // CONTINUE POLLING
     				  break;
   		}

      //////////////////////
      /// BUTTON POLLING ///
      //////////////////////

      if (fsm_getCurrentState() == STATE_INITIALIZE) {
      // POLL NOTHING
      }
      else if (elev_get_stop_signal()) {
          fsm_evEmergencyPressed();
      } 
      else {

          // POLL ALL ORDER BUTTONS
          for (int f = FLOOR_1; f < N_FLOORS; ++f) {
        		  for (int b = 0; b < N_BUTTONS; ++b) {

                  // READS VALID BUTTONS
        			    if ((!(b == BUTTON_CALL_UP && f == N_FLOORS - 1)) && (!(b == BUTTON_CALL_DOWN && f == FLOOR_1))) {
        				      if (elev_get_button_signal((elev_button_type_t) b, f)) { 
                          lastButtonState[f][b] = TRUE;
                      }

                      // EXECUTES ON RELEASE
                      else if (lastButtonState[f][b] == TRUE) {
                          fsm_evOrderRequest(f, b);
                          lastButtonState[f][b] = FALSE;
                      }
        			    }
        		  }
        	}
      }
  }
    
  return 0;

}
