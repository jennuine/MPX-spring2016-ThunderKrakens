/**
 * @file cmd_orders.h
 * @author Thunder Krakens
 * @date February 7nd, 2016
 * @version R5
 *
 * @brief This file contains orders & index of all the commands.
 */
 
#define WITH_R2_TEMP_CMD  0
#define WITH_R3_TEMP_CMD  0
#define WITH_R5_TEMP_CMD  1

/**------------------------------------------------------------------**/
#define FUNCTIONS_BEGIN     0
#define HELP            0

/**------------------------------------------------------------------**/
#define MPX_FUNCTIONS_BEGIN 1

#define VERSION       MPX_FUNCTIONS_BEGIN+0
#define GETTIME       MPX_FUNCTIONS_BEGIN+1
#define SETTIME       MPX_FUNCTIONS_BEGIN+2
#define GETDATE       MPX_FUNCTIONS_BEGIN+3
#define SETDATE       MPX_FUNCTIONS_BEGIN+4
#define SHUTDOWN      MPX_FUNCTIONS_BEGIN+5
#define LOADR3        MPX_FUNCTIONS_BEGIN+6
#if WITH_R3_TEMP_CMD
    #define YIELD         MPX_FUNCTIONS_BEGIN+7
    #define MPX_FUNC_END  MPX_FUNCTIONS_BEGIN+7
#else
    #define MPX_FUNC_END  MPX_FUNCTIONS_BEGIN+6
#endif


/**------------------------------------------------------------------**/

#define PCB_FUNCTIONS_BEGIN     MPX_FUNC_END+1

#define SUSPDPCB      PCB_FUNCTIONS_BEGIN+0
#define RESUMEPCB     PCB_FUNCTIONS_BEGIN+1
#define SETPCBPRIO    PCB_FUNCTIONS_BEGIN+2
#define SHOWPCB       PCB_FUNCTIONS_BEGIN+3
#if WITH_R2_TEMP_CMD
    #define CREATEPCB   PCB_FUNCTIONS_BEGIN+4
    #define DELPCB      PCB_FUNCTIONS_BEGIN+5
    #define BLOCKPCB    PCB_FUNCTIONS_BEGIN+6
    #define UNBLKPCB    PCB_FUNCTIONS_BEGIN+7
    #define PCB_FUNC_END    PCB_FUNCTIONS_BEGIN+7
#else
    #define PCB_FUNC_END    PCB_FUNCTIONS_BEGIN+3
#endif

/**------------------------------------------------------------------**/

#define MCB_FUNCTIONS_BEGIN PCB_FUNC_END+1

#define SHOWMCB         MCB_FUNCTIONS_BEGIN+0
#if WITH_R5_TEMP_CMD
    #define INITMCB         MCB_FUNCTIONS_BEGIN+1
    #define ALLOCMCB        MCB_FUNCTIONS_BEGIN+2
    #define FREEMCB         MCB_FUNCTIONS_BEGIN+3
    #define ISMCBEMPT       MCB_FUNCTIONS_BEGIN+4
    #define MCB_FUNC_END    MCB_FUNCTIONS_BEGIN+4
#else
    #define MCB_FUNC_END    MCB_FUNCTIONS_BEGIN+0
#endif

/**------------------------------------------------------------------**/

#define NUM_OF_FUNCTIONS    MCB_FUNC_END+1

