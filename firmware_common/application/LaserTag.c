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
static u16 u16Count5ms;
static u16 u16countHigh;
static u16 u16countLow;
static u16 u16Lives;
static u16 u16RecoverTime;
static u16 delimiter = 600;
static u16 u16soundCount;

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
    pu32ToggleGPIO = (u32*)(&(AT91C_BASE_PIOA->PIO_SODR));
    LaserTag_Toggle = FALSE;
  }
  else
  {
    pu32ToggleGPIO = (u32*)(&(AT91C_BASE_PIOA->PIO_CODR));
    LaserTag_Toggle = TRUE;
  }
  *pu32ToggleGPIO = PA_10_I2C_SCL;
}
/* end of LaserTagToggler */
/*------------------------------------------------------------
Function: gotShot
Description:
Checks if the modulated signal of 38 kHz is received by the GPIO pin PA_14_BLADE_MOSI.
Requires:
PA_14_BLADE_MOSI is configured correctly as an input pin, must check if receives voltage HIGH
for 5 ms, then VOLTAGE LOW for 5ms, then register that it has been hit and turn an LED on for a bit.

Promises:
Return true if the signal has been received.
*/
void gotShot(void)
{
    u32 *pu32Address;
    pu32Address = (u32*)(&(AT91C_BASE_PIOA->PIO_PDSR));
    u32 u32truthValue = (*pu32Address) & 0x00004000;
    bool rHigh = FALSE;
    if(u32truthValue == 0x00004000)
    {
      rHigh = TRUE;
    }
    if (u16countHigh ==5 && u16countLow==5) 
    {
      u16countHigh = 0;
      u16countLow = 0;
      LedOn(WHITE);
      PWMAudioSetFrequency(BUZZER1, 320);
      u16Lives--;
      LedOff(WHITE);
      LedOn(RED);
     LaserTag_StateMachine = LaserTagSM_Recover;
    }
    else if (rHigh) 
    {
      u16countHigh++;
      LedOff(WHITE);
    }
    else if (!rHigh && u16countHigh!=5) 
    {
      u16countHigh = 0;
      u16countLow = 0;
      LedOff(WHITE);
    }
   else if (!rHigh && u16countHigh==5) 
    {
      u16countLow++;
      LedOff(WHITE);
    }
    
}
void reset(void)
{
  u16Lives = 3;
  u16soundCount = 0;
  LedOff(RED);
  LedOn(CYAN);
  LedOn(GREEN);
  LedOn(YELLOW);
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
  /* Enable the Interrupt Reg's for MOSI */
  
  /* Set counter to 0 to start*/
    u16countHigh = 0;
  /* Set counter to 0 to start*/
    u16countLow = 0;
  /* Set recover count to 0 to start */
    u16RecoverTime = 0;
  /* Set count sound to 0 to start */
    u16soundCount = 0;
   /* Player starts with 3 lives */
    u16Lives = 3;
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
  gotShot();
  LedOff(PURPLE);
  
  if(IsButtonPressed(BUTTON0))
  {
    LaserTag_StateMachine = LaserTagSM_ModulateOn;
  }
  
  if(u16Lives == 3)
  {
      LedOn(CYAN);
      LedOn(GREEN);
      LedOn(YELLOW);
  }
  else if(u16Lives == 2)
  {
      LedOff(CYAN);
      LedOn(GREEN);
      LedOn(YELLOW);
  }
    else if(u16Lives == 1)
  {
      LedOff(CYAN);
      LedOff(GREEN);
      LedOn(YELLOW);
  }
    else if(u16Lives == 0)
  {
      LedOff(CYAN);
      LedOff(GREEN);
      LedOff(YELLOW);
      LaserTag_StateMachine = LaserTagSM_DeadState;
  }
  
 

} /* end LaserTagSM_Idle() */
/*
the proper signal to send to pin and ultimately transmitter
*/
static void LaserTagSM_ModulateOn(void)
{
  gotShot();
  LedOn(PURPLE);
  TimerStart(TIMER_CHANNEL1);
  if(u16Count5ms >= 4)
  {
    u16Count5ms = 0;
    LaserTag_StateMachine = LaserTagSM_ModulateOff;
  }
  else
  {
    u16Count5ms++;
  }
}

static void LaserTagSM_ModulateOff(void)
{
  gotShot();
  LedOff(PURPLE);
  if(u16Count5ms >= 4)
  {
    u16Count5ms = 0;
    if (IsButtonPressed(BUTTON0))
    {
       LaserTag_StateMachine = LaserTagSM_ModulateOn;
    }
    else
    {
       LaserTag_StateMachine = LaserTagSM_Idle;      
    }

  }
  else
  {
    u16Count5ms++;
    TimerStop(TIMER_CHANNEL1);
    LaserTag_Toggle = FALSE;
  }
}

static void LaserTagSM_Recover(void)
{

  u16RecoverTime++;
   if (u16RecoverTime % delimiter == 0)
   {
    LedToggle(RED);
    delimiter -= 25;
   }
  PWMAudioSetFrequency(BUZZER2, 500);
 //first buzz
  if(u16RecoverTime>0 && u16RecoverTime<100)
  {
    PWMAudioOn(BUZZER2);
  }
  else
  {
    PWMAudioOff(BUZZER2);
  }
   if(u16RecoverTime>=5000)
   {
     delimiter = 600;
     LedOff(RED);
     u16RecoverTime = 0;
     LaserTag_StateMachine = LaserTagSM_Idle;
   }
}

/* if you lose all your lives, assumes that all LED's are off upon entering state */
static void LaserTagSM_DeadState(void)
{
  u16soundCount++;
  //first buzz
  if (u16soundCount > 0 && u16soundCount < 100)
  {
    PWMAudioSetFrequency(BUZZER2, 500);
    PWMAudioOn(BUZZER2);
  }
  else
  {
    PWMAudioOff(BUZZER2);
  }
  //second buzz
  if (u16soundCount > 800 && u16soundCount < 1000)
  {
    PWMAudioSetFrequency(BUZZER2, 400);
    PWMAudioOn(BUZZER2);
  }
  else
  {
    PWMAudioOff(BUZZER2);
  }
  //third buzz
   if (u16soundCount > 2000 && u16soundCount < 3000)
  {
    PWMAudioSetFrequency(BUZZER2, 200);
    PWMAudioOn(BUZZER2);
  }
  else
  {
    PWMAudioOff(BUZZER2);
  }
      LedOn(RED);
  if (IsButtonPressed(BUTTON2))
  {
      reset();
      LaserTag_StateMachine = LaserTagSM_Idle;
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
