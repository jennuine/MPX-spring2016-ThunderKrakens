/**
 * @file sys_clock.h
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @version R1
 *
 * @brief The main file that manipulates and controls the system's clock.
 */
#ifndef _SYS_CLOCK_H
#define _SYS_CLOCK_H

#include <system.h>
#include "../errno.h"

/**
 * @name set_time_main
 *
 * @brief The main set time argument handler for set time function.
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0 when finished with execution.
 */
int set_time_main(int argc, char** argv);

/**
 * @name get_time_main
 *
 * @brief The main get time argument handler for the get time function.
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0 when finished with execution.
 */
int get_time_main(int argc, char** argv);

/**
 * @name set_time_str
 *
 * @brief Sets the time for the system by string.
 * @param timeStr   The string type of current Time.
 * @return The appropiate error code. See @ref errno.h for details.
 */
error_t set_time_str(const char * timeStr);

/**
 * @name get_time
 *
 * @brief Retrieves the system's current time and date.
 * @param dateTimeValues  The value of current time and date
 */
void get_time(date_time * dateTimeValues);

/**
 * @name set_time
 *
 * @brief Sets the time for the system by date_time structure.
 * @param dateTimeValues   The structure that holds the time values.
 * @return The appropiate error number. See @ref errno.h for details.
 */
error_t set_time(const date_time * dateTimeValues);

/**
 * @name set_date_main
 *
 * @brief The set date argument handler for the set date function.
 * @param argc  The number of tokens.
 * @param argv  The array of tokens.
 * @return 0 when finished execution.
 */
int set_date_main(int argc, char** argv);

/**
 * @name get_date_main
 *
 * @brief The get date argument handler for the get date function.
 * @param argc  The number of tokens.
 * @param argv  The array of tokens.
 * @return  0 when finished with execution.
 */
int get_date_main(int argc, char** argv);

/**
 * @name get_date
 *
 * @brief Retrieves system's current date.
 * @param dateTimeValues  The structure that holds the value of current date.
 */
void get_date(date_time * dateTimeValues);

/**
 * @name set_date_str
 *
 * @brief Sets the date for the system by string.
 * @param str   The string type of current date.
 * @return  0 when finished with execution.
 */
int set_date_str(const char * str);

/**
 * @name set_date.
 *
 * @brief Sets the date of the system.
 * @param dateTimeValues  The structure that holds the value of date
 * @return The appropiate error number. See @ref errno.h for details.
 */
error_t set_date(const date_time * dateTimeValues);

#endif
