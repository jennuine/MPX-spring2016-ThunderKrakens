/**
 * @brief System Clock and Date Header
 * @file sys_clock.h
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @version R1
 *
 * The header of the main file that manipulates and controls the system's clock.
 *
 */
#ifndef _SYS_CLOCK_H
#define _SYS_CLOCK_H

#include <system.h>
#include "../errno.h"

/** @brief  Name: set_time_main.
 *
 * Description: The main fuction of time set for the system.
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 */
int set_time_main(int argc, char** argv);

/** @brief  Name: get_time_main.
 *
 * Description: Retrieves system's current time.
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 *
 */
int get_time_main(int argc, char** argv);

/** @brief  Name: set_time_help.
 *
 * Description: Print out the help message for settime.
 * @param
 * @return  VOID
 */
void set_time_help();


/** @brief  Name: get_time_help.
 *
 * Description: Print out the help message for settime.
 * @param
 * @return  VOID
 */
void get_time_help();

/** @brief  Name: set_time_str.
 *
 * Description: Sets the time for the system by string.
 * @param timeStr - The string type of current Time.
 * @return dateTimeValues - Returns the set time of the system
 * @return E_INVSTRF - Invalid String
 */
error_t set_time_str(const char * timeStr);

/** @brief  Name: get_time.
 *
 * Description: Retrieves system's current time and date.
 * @param dateTimeValues - The value of current time and date
 * @return VOID
 */
void get_time(date_time * dateTimeValues);

/** @brief  Name: error_t set_time.
 *
 * Description: Error handling of set_time function has invalid value, or invalid format.
 * @param dateTimeValues - The value of current time and date
 * @return E_NOERROR - When no error was detected
 * @return E_INVPARA - Invalid Parameter
 */
error_t set_time(const date_time * dateTimeValues);

/** @brief  Name: set_date_main.
 *
 * Description: Sets system's date.
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 * @return  0
 */
int set_date_main(int argc, char** argv);

/** @brief  Name: get_date_main.
 *
 * Description: Retrieves system's current date.
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 * @return  0
 */
int get_date_main(int argc, char** argv);

#endif
