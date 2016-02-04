#ifndef _SYS_CLOCK_H
#define _SYS_CLOCK_H

#include <system.h>
#include "../errno.h"

/*
  Procedure..: set_time_main
  Description..: The main function for set time.
*/
int set_time_main(int argc, char** argv);

/*
  Procedure..: get_time_main
  Description..: The main function for get time.
*/
int get_time_main(int argc, char** argv);

/*
  Procedure..: set_time_help
  Description..: print out the help message for settime.
*/
void set_time_help();

/*
  Procedure..: get_time_help
  Description..: print out the help message for gettime.
*/
void get_time_help();

/*
  Procedure..: set_time_str
  Description..: set the system time by using string.
*/
error_t set_time_str(const char * timeStr);

/*
  Procedure..: get_time
  Description..: Gets the system time.
  Params..: dateTimeValues-output the date & time value.
*/
void get_time(date_time * dateTimeValues);

/*
  Procedure..: set_time
  Description..: Sets the system time.
  Params..: dateTimeValues-input the date & time value.
*/
error_t set_time(const date_time * dateTimeValues);

int set_date_main(int argc, char** argv);

int get_date_main(int argc, char** argv);


#endif
