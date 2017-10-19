/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
//establish arrays for all functions to use
  
#ifdef EIE2
  static LedNumberType aeCurrentLed[]  =
  {GREEN0, RED0, BLUE0, GREEN0, RED0, BLUE
    0};
  static LedNumberType aeCurrentLed1[]  =
  {GREEN1, RED1, BLUE1, GREEN1, RED1, BLUE
    1};
  static LedNumberType aeCurrentLed2[]  =
  {GREEN2, RED2, BLUE2, GREEN2, RED2, BLUE
    2};
  static LedNumberType aeCurrentLed3[]  =
  {GREEN3, RED3, BLUE3, GREEN3, RED3, BLUE
    3};
#endif /* EIE2 */
  
#ifdef EIE1
  static LedNumberType aeCurrentLed[]  =
  {LCD_GREEN, LCD_RED, LCD_BLUE, LCD_GREEN,
  LCD_RED, LCD_BLUE};
#endif /*EIE 1 */
static bool abLedRateIncreasing[]   =
{TRUE,     FALSE,    TRUE,     FALSE,
TRUE,     FALSE};

static u8 u8CurrentLedIndex  = 0;
static u8 u8LedCurrentLevel  = 0;
static u8 u8DutyCycleCounter = 0;
static u16 u16Counter = COLOR_CYCLE_TIME;


void UserApp1Initialize(void)
{
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;  
  }
    
    /* Start with red LED on 100%, green and
   blue off */
  LedPWM(LCD_RED, LED_PWM_100);
  LedPWM(LCD_GREEN, LED_PWM_0);
  LedPWM(LCD_BLUE, LED_PWM_0);
    
} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();
  
  static bool bCyclingOn = TRUE;
  
  if (bCyclingOn)
  {
    u16Counter--;
  }
/* check for update color every COLOR_CYCLE_TIME ms */
if (u16Counter == 0)
{
  u16Counter = COLOR_CYCLE_TIME;
  
  /* Update the current level based on
  which way it's headed */
  
  /* Update the value to the current
  LED */
  LedPWM( aeCurrentLed[u8CurrentLedIndex],
          u8LedCurrentLevel);
}

/* Toggle cycling on and off */
if (WasButtonPressed(BUTTON0))
{
  ButtonAcknowledge(BUTTON0);
  bCyclingOn = (bool)!bCyclingOn;
}


/* Update the current level based on which
way it's headed */
if (abLedRateIncreasing[u8CurrentLedIndex])
{
  u8LedCurrentLevel++;
}
else
{
  u8LedCurrentLevel--;
}

/* Change direction once we're at the
end */
u8DutyCycleCounter++;
if (u8DutyCycleCounter == 20)
{
  u8DutyCycleCounter = 0;
  
  /* Watch for the indexing variable
  to reset */
  u8CurrentLedIndex++;
  if (u8CurrentLedIndex == sizeof(aeCurrentLed))
  {
    u8CurrentLedIndex = 0;
  }
  
  /* Set the current level based on what
  direction we're now going */
  u8LedCurrentLevel = 20;
  if (abLedRateIncreasing[u8CurrentLedIndex])
  {
    u8LedCurrentLevel = 0;
  }
}

  
} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{

} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
