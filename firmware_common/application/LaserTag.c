/**********************************************************************************************************************
File: LaserTag.c                                                                



Description:
This is a LaserTag.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void LaserTagInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void LaserTagRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_LaserTag"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32LaserTagFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern u32 G_u32AntApiCurrentMessageTimeStamp;
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "LaserTag_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type LaserTag_StateMachine; 
static bool LaserTag_Toggle;
static u16 u16ToggleOn;
static u16 u16Count5ms;

/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
/*
Function: LasterTagToggler
Description:
Changes the truth value of the boolean LaserTagToggler every time the function
is called.
*/
void LaserTagToggler(void)
{
  u32 *pu32ToggleGPIO;
  if(LaserTag_Toggle)
  {
    LedOn(PURPLE);
    pu32ToggleGPIO = (u32*)(&(AT91C_BASE_PIOA->PIO_SODR));
    LaserTag_Toggle = FALSE;
    u16ToggleOn++;
   // *pu32ToggleGPIO += 1;
  }
  else
  {
    LedOff(PURPLE);
    pu32ToggleGPIO = (u32*)(&(AT91C_BASE_PIOA->PIO_CODR));
    LaserTag_Toggle = TRUE;
    //*pu32ToggleGPIO += 2;
  }
  *pu32ToggleGPIO = PA_10_I2C_SCL;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: LaserTagInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -LaserTag Toggler to be active

Promises:
  -
*/
void LaserTagInitialize(void)
{
  u16ToggleOn = 0;
    /* Set 5ms counter to 0 to start*/
  u16Count5ms = 0;
     /* Set Toggle to false to start. */
  LaserTag_Toggle = FALSE;
   /* Set Timer with 5 tick period before inturrupt. */
  TimerSet(TIMER_CHANNEL1, 0x0005);
   /* Sets LaserTagToggler to the timer function ptr */
  TimerAssignCallback(TIMER_CHANNEL1, LaserTagToggler);
   /* Starts the timer */
  TimerStart(TIMER_CHANNEL1);
  if( 1 )
  {
    LaserTag_StateMachine = LaserTagSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    LaserTag_StateMachine = LaserTagSM_Error;
  }

} /* end LaserTagInitialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function LaserTagRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void LaserTagRunActiveState(void)
{
  LaserTag_StateMachine();

} /* end LaserTagRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*

*/
static void LaserTagSM_Idle(void)
{
  if(IsButtonPressed(BUTTON0))
  {
    LedOn(CYAN);
    LedOff(RED);
    LaserTag_StateMachine = LaserTagSM_ModulateOn;
  }
  else
  {
    LedOff(CYAN);
    LedOn(RED);
    LaserTag_StateMachine = LaserTagSM_ModulateOff;
  }
} /* end LaserTagSM_Idle() */
/*
the proper signal to send to pin and ultimately transmitter
*/
static void LaserTagSM_ModulateOn(void)
{
 TimerStart(TIMER_CHANNEL1);
  if(u16Count5ms >= 4)
  {
    u16Count5ms = 0;
    LaserTag_StateMachine = LaserTagSM_Idle;
  }
  else
  {
    u16Count5ms++;
  }
}

static void LaserTagSM_ModulateOff(void)
{
  if(u16Count5ms >= 4)
  {
    u16Count5ms = 0;
    LaserTag_StateMachine = LaserTagSM_Idle;
  }
  else
  {
    u16Count5ms++;
    TimerStop(TIMER_CHANNEL1);
    LaserTag_Toggle = FALSE;
  }
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void LaserTagSM_Error(void)          
{

  
} /* end LaserTagSM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
