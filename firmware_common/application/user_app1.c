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

extern u8 G_au8DebugScanfBuffer[];
extern u8 G_u8DebugScanfCharCount;

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u32 UserApp1_u32Notes[] = {C3, C3S, D3, D3S, E3, F3, F3S, G3, G3S, A3, A3S, B3};
static u8 UserApp_au8UserInputBuffer[U16_USER_INPUT_BUFFER_SIZE];
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
  
  UserApp_au8UserInputBuffer[U16_USER_INPUT_BUFFER_SIZE] = '\0';
  
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
  u8 charcount = 0;
  static u32 u32Index = 12; 
  charcount = DebugScanf(UserApp_au8UserInputBuffer);
  
if (UserApp_au8UserInputBuffer[0] == 'q')
{
  u32Index = 0;
}
else if (UserApp_au8UserInputBuffer[0] == 'w')
{
  u32Index = 1;
}
else if (UserApp_au8UserInputBuffer[0] == 'e')
{
  u32Index = 2;
}
else if (UserApp_au8UserInputBuffer[0] == 'r')
{
  u32Index = 3;
}
else if (UserApp_au8UserInputBuffer[0] == 't')
{
  u32Index = 4;
}
else if (UserApp_au8UserInputBuffer[0] == 'y')
{
  u32Index = 5;
}
else if (UserApp_au8UserInputBuffer[0] == 'u')
{
  u32Index = 6;
}
else if (UserApp_au8UserInputBuffer[0] == 'i')
{
  u32Index = 7;
}
else if (UserApp_au8UserInputBuffer[0] == 'o')
{
  u32Index = 8;
}
else if (UserApp_au8UserInputBuffer[0] == 'p')
{
  u32Index = 9;
}
else if (UserApp_au8UserInputBuffer[0] == '[')
{
  u32Index = 10;
}
else if (UserApp_au8UserInputBuffer[0] == ']')
{
  u32Index = 11;
}
if (UserApp_au8UserInputBuffer[0] == 'z')       //to save our ears
{
  PWMAudioOff(BUZZER1);
}
else if (u32Index < 12)
{
  PWMAudioOn(BUZZER1);
  PWMAudioSetFrequency(BUZZER1, UserApp1_u32Notes[u32Index]);
}



if (WasButtonPressed(BUTTON1))
{
  ButtonAcknowledge(BUTTON1);
  PWMAudioSetFrequency(BUZZER1, 294);
}
if (WasButtonPressed(BUTTON2))
{
  ButtonAcknowledge(BUTTON2);
  //PWMAudioSetFrequency(BUZZER1, the enum array[INDEX]);
}
if (WasButtonPressed(BUTTON3))
{
  ButtonAcknowledge(BUTTON3);
  PWMAudioSetFrequency(BUZZER1, 392);
}
if (IsButtonPressed(BUTTON0) || IsButtonPressed(BUTTON1) || IsButtonPressed(BUTTON2) || IsButtonPressed(BUTTON3))
{
  PWMAudioOn(BUZZER1);
}
else
{
  PWMAudioOff(BUZZER1);
}
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
