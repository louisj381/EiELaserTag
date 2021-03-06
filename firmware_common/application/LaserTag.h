/**********************************************************************************************************************
File: LaserTag.h                                                                

----------------------------------------------------------------------------------------------------------------------

6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
TODO: Investigate the configuration.h file as stated above.
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for LaserTag.c

**********************************************************************************************************************/

#ifndef __LASER_TAG_H
#define __LASER_TAG_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/
/* Required constants for ant channel config */
//Device is a MASTER
#define ANT_CHANNEL_TYPE_USERAPP        CHANNEL_TYPE_MASTER
//Channel 0 - 7
#define ANT_CHANNEL_USERAPP             ANT_CHANNEL_0
// LO; 0X0001 - 0X7FFF
#define ANT_CHANNEL_PERIOD_LO_USERAPP   (u8)0x00
// HI; 0x0001 - 0x7fff
#define ANT_CHANNEL_PERIOD_HI_USERAPP   (u8)0x20
// Device # Low byte (unique)
#define ANT_DEVICEID_LO_USERAPP         (u8)0x89
// Device # High byte (unique)
#define ANT_DEVICEID_HI_USERAPP         (u8)0x19
// 1 - 255
#define ANT_DEVICE_TYPE_USERAPP         (u8)1
// 1-127; MSB is pairing
#define ANT_TRANSMISSION_TYPE_USERAPP   (u8)1
// 2400MHz + 0 - 99 MHz
#define ANT_FREQUENCY_USERAPP           (u8)50
// Max tx power
#define ANT_TX_POWER_USERAPP            RADIO_TX_POWER_4DBM
/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/
void LaserTagToggler(void);
void gotShot(void);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void LaserTagInitialize(void);
void LaserTagRunActiveState(void);
void EnableInterruptReg(void);
void reset(void);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void LaserTagSM_Idle(void);    

static void LaserTagSM_Error(void);         

static void LaserTagSM_ModulateOn(void);

static void LaserTagSM_ModulateOff(void);

static void LaserTagSM_Recover(void);
static void LaserTagSM_DeadState(void);

#endif /* __LASER_TAG_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
