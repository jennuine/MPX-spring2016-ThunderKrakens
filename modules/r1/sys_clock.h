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
 * @name set_time_main.
 *
 * @brief Sets the time for the system.
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int set_time_main(int argc, char** argv);

/**
 * @name get_time_main.
 *
 * @brief Retrieves system's current time.
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int get_time_main(int argc, char** argv);

/**
 * @name set_time_str.
 *
 * @brief Sets the time for the system by string.
 * @param timeStr   The string type of current Time.
 * @return 0 if there is no error, otherwise return a error code.
 */
error_t set_time_str(const char * timeStr);

/**
 * @name get_time.
 *
 * @brief Retrieves system's current time and date.
 * @param dateTimeValues  The value of current time and date
 * @return VOID
 */
void get_time(date_time * dateTimeValues);

/**
 * @name set_time.
 *
 * @brief Sets the time for the system by date_time struct.
 * @param dateTimeValues   The struct that holds the time values.
 * @return 0 if there is no error, otherwise return a error code.
 */
error_t set_time(const date_time * dateTimeValues);

/**
 * @name set_date_main.
 *
 * @brief Sets system's date.
 * @param argc  The number of tokens.
 * @param argv  The array of tokens.
 * @return 0
 */
int set_date_main(int argc, char** argv);

/**
 * @name get_date_main.
 *
 * @brief Retrieves system's current date.
 * @param argc  The number of tokens.
 * @param argv  The array of tokens.
 * @return  0
 */
int get_date_main(int argc, char** argv);

/**
 * @name get_date.
 *
 * @brief Retrieves system's current date.
 * @param dateTimeValues  The struct that holds the value of current date
 * @return VOID
 */
void get_date(date_time * dateTimeValues);

/**
 * @name set_date_str.
 *
 * @brief Sets the date for the system by string.
 * @param str   The string type of current date.
 * @return 0 if there is no error, otherwise return a error code.
 */
int set_date_str(const char * str);

/**
 * @name: set_date.
 *
 * @brief Sets the date of the system.
 * @param dateTimeValues  The struct that holds the value of date
 * @return 0 if there is no error, otherwise return a error code.
 */
error_t set_date(const date_time * dateTimeValues);

#endif
