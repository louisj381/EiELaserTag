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
static fnCode_type LaserTag_StateMachine;            /* The state machine function pointer */
static u32 LaserTag_u32Timeout;                      /* Timeout counter used across states */
static AntAssignChannelInfoType LaserTag_sChannelInfo;
static u8 LaserTag_au8MessageFail[] = "\n\r***ANT channel setup failed***\n\n\r";
static u8 au8TestMessage[] = {0, 0, 0, 0,
0xA5, 0, 0, 0};
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
Function: LaserTagInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void LaserTagInitialize(void)
{
 
  u8 au8WelcomeMessage[] = "ANT Master";

  /* Set a message up on the LCD. Delay is required to let the clear command send. */
  LCDCommand(LCD_CLEAR_CMD);
  for(u32 i = 0; i < 10000; i++);
  LCDMessage(LINE1_START_ADDR, au8WelcomeMessage);

 /* Configure ANT for this application */
  LaserTag_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP;
  LaserTag_sChannelInfo.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP;
  LaserTag_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  LaserTag_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
 
  LaserTag_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  LaserTag_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  LaserTag_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  LaserTag_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  LaserTag_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP;
  LaserTag_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP;

  LaserTag_sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    LaserTag_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
  
  /* Attempt to queue the ANT channel setup */
  if( AntAssignChannel(&LaserTag_sChannelInfo) )
  {
    LaserTag_u32Timeout = G_u32SystemTime1ms;
    LaserTag_StateMachine = LaserTagSM_AntChannelAssign;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    DebugPrintf(LaserTag_au8MessageFail);
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

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ANT channel assignment */
static void LaserTagSM_AntChannelAssign()
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    // meaning channel assignment was successful,
    // so open channel and proceed to Idle state
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    LaserTag_StateMachine = LaserTagSM_Idle;
  }
  // watch for time out
  if (IsTimeUp(&LaserTag_u32Timeout, 3000))
  {
    DebugPrintf(LaserTag_au8MessageFail);
    LaserTag_StateMachine = LaserTagSM_Error;
  }
}

static void LaserTagSM_Idle(void)
{
  if (AntReadAppMessageBuffer())
  {
    // new message from ANT task: check what it is
    if (G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      //we got some data
    }
    else if (G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      //A channel period has gone by:
      // typically this is when new data should
      //be queued to be sent
      // update and queue the new message data
      au8TestMessage[7]++;
      if (au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if (au8TestMessage[6] =
      }
  }
} /* end LaserTagSM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void LaserTagSM_Error(void)          
{
  
} /* end LaserTagSM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
