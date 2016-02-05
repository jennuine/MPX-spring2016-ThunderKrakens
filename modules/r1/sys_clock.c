/**
 * @brief System Clock and Date
 * @file sys_clock.c
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @version R1
 *
 * The main file that manipulates and controls the system's clock.
 */


#include "sys_clock.h"
#include "r1.h"
#include <string.h>
#include <core/io.h>
#define RTC_INDEX_SECOND     0x00
#define RTC_INDEX_SECOND_ALARM     0x01
#define RTC_INDEX_MINUTE     0x02
#define RTC_INDEX_MINUTE_ALARM     0x03
#define RTC_INDEX_HOUR         0x04
#define RTC_INDEX_HOUR_ALARM     0x05
#define RTC_INDEX_DAY_WEEK 0x06
#define RTC_INDEX_DAY_MONTH 0x07
#define RTC_INDEX_MONTH 0x08
#define RTC_INDEX_YEAR 0x09

/** @brief  Name: set_time_main.
 *
 * Description: Sets the time for the system.
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 * @return  0
 */
int set_time_main(int argc, char** argv)
{
  if(argc >= 3 && strcmp(argv[2], "--help") != 0)
  {
  	error_t errno = set_time_str(argv[2]);
  	date_time dateTimeValues;
  	switch (errno)
    {
    	case E_NOERROR:
   		 get_time(&dateTimeValues);
      	printf("\n\tSystem time had been successfully set!\n");
      	printf("\tCurrent time is: %d:%d:%d\n\n", dateTimeValues.hour, dateTimeValues.min, dateTimeValues.sec);
    	break;

    	case E_INVPARA:
      	printf("ERROR: The time value you gave is invalid!\n");
    	break;

    	case E_INVSTRF:
      	printf("ERROR: The format you used is incorrect! Please refer to \"settime --help\"\n");
    	break;
  	}
  }
  else if(argc >= 3 && strcmp(argv[2], "--help") == 0)
  {
	   print_help(SETTIME);
  }
  else
  {
	   printf("ERROR: Please type in the arguments! Please refer to \"settime --help\"\n");
  }

  return 0;
}

/** @brief  Name: get_time_main.
 *
 * Description: Retrieves system's current time.
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 * @return  0
 */
int get_time_main(int argc, char** argv)
{
  if(argc == 2)
  {
  	date_time dateTimeValues;
  	get_time(&dateTimeValues);
  	printf("\tCurrent time is: %02d:%02d:%02d\n", dateTimeValues.hour, dateTimeValues.min, dateTimeValues.sec);
  }
  else if(argc >= 2 && strcmp(argv[2], "--help") == 0)
  {
	   print_help(GETTIME);
  }
  else
  {
	   printf("ERROR: Please type in the correct arguments! Please refers to \"gettime --help\"\n");
  }

  return 0;
}

static int is_digit(const char ch)
{
  return '0' <= ch && ch <= '9';
}

/** @brief  Name: set_time_str.
 *
 * Description: Sets the time for the system by string.
 * @param timeStr - The string type of current Time.
 * @return dateTimeValues - Returns the set time of the system
 * @return E_INVSTRF - Invalid String
 */
error_t set_time_str(const char * timeStr){
  static char tempStr[10];
  date_time dateTimeValues;
  int bIsValid = 1;

  strcpy(tempStr, timeStr);
  char * tok = strtok(tempStr, ":");

  if(bIsValid && (bIsValid = (*tok && is_digit(*tok) &&   //first char must digit
                        	(!*(tok + 1) || is_digit(*(tok + 1))))))  //second char either not exsit or a digit
  {
	   dateTimeValues.hour = atoi(tok);
  }

  tok = strtok(NULL, ":");
  if(bIsValid && (bIsValid = (*tok && is_digit(*tok) &&   //first char must digit
                        	(!*(tok + 1) || is_digit(*(tok + 1))))))  //second char either not exsit or a digit
  {
	   dateTimeValues.min = atoi(tok);
  }

  tok = strtok(NULL, ":");
  if(bIsValid && (bIsValid = (*tok && is_digit(*tok) &&   //first char must digit
                        	(!*(tok + 1) || is_digit(*(tok + 1))))))  //second char either not exsit or a digit
  {
	   dateTimeValues.sec = atoi(tok);
  }

  if(bIsValid)
	 return set_time(&dateTimeValues);
  else
	 return E_INVSTRF;
}


/** @brief  Name: get_time.
 *
 * Description: Retrieves system's current time and date.
 * @param dateTimeValues - The value of current time and date
 * @return VOID
 */
void get_time(date_time * dateTimeValues)
{
    unsigned char sec, min, hr;
    outb(0x70, RTC_INDEX_SECOND);
    sec = inb(0x71);
    outb(0x70, RTC_INDEX_MINUTE);
    min = inb(0x71);
    outb(0x70, RTC_INDEX_HOUR);
    hr = inb(0x71);

    dateTimeValues->hour = (hr >> 4) * 10 + (hr & 0x0f);
    dateTimeValues->min = (min >> 4) * 10 + (min & 0x0f);
    dateTimeValues->sec = (sec >> 4) * 10 + (sec & 0x0f);
}

/** @brief  Name: set_time_str.
 *
 * Description: Sets the time for the system by string.
 * @param timeStr - The string type of current Time.
 * @return dateTimeValues - Returns the set time of the system
 * @return E_INVSTRF - Invalid String
 */

error_t set_time(const date_time * dateTimeValues)
{
  unsigned char sec, min, hr;
  if(0 <= dateTimeValues->hour && dateTimeValues->hour < 24 &&
	0 <= dateTimeValues->min && dateTimeValues->min < 60 &&
	0 <= dateTimeValues->sec && dateTimeValues->sec < 60 )
  {//If all of the value are in the valid range
	hr = dateTimeValues->hour ? (((dateTimeValues->hour / 10) << 4) | (dateTimeValues->hour % 10)) : 0;
      min = dateTimeValues->min ? (((dateTimeValues->min / 10) << 4) | (dateTimeValues->min % 10)) : 0;
      sec = dateTimeValues->sec ? (((dateTimeValues->sec / 10) << 4) | (dateTimeValues->sec % 10)) : 0;

    //Disable interrupts
    cli();

    outb(0x70, RTC_INDEX_SECOND);
    outb(0x71, sec);
    outb(0x70, RTC_INDEX_MINUTE);
    outb(0x71, min);
    outb(0x70, RTC_INDEX_HOUR);
    outb(0x71, hr);

    //Enable interrupts
    sti();

    return E_NOERROR;
  }

  return E_INVPARA;
}

/** @brief  Name: get_date.
 *
 * Description: Retrieves system's current date.
 * @param dateTimeValues - The value of current date
 * @return VOID
 */

void get_date(date_time * dateTimeValues)
{
  unsigned char day, month, year;
  outb(0x70, RTC_INDEX_DAY_MONTH);
  day = inb(0x71);
  outb(0x70, RTC_INDEX_MONTH);
  month = inb(0x71);
  outb(0x70, RTC_INDEX_YEAR);
  year = inb(0x71);

  dateTimeValues->mon = (month >> 4) * 10 + (month & 0x0f);
  dateTimeValues->day_m = (day >> 4) * 10 + (day & 0x0f);
  dateTimeValues->year = (year >> 4) * 10 + (year & 0x0f);
}

/** @brief  Name: set_date.
 *
 * Description: Sets the date of the system.
 * @param dateTimeValues - The value of current time and date
 * @return E_NOERROR - When no error was detected
 * @return E_INVPARA - Invalid Parameter
 */

error_t set_date(const date_time * dateTimeValues)
{
  unsigned char month, day, year;

  if(0 <= dateTimeValues->mon &&  dateTimeValues->mon <= 12 &&
    0 <= dateTimeValues->day_m && dateTimeValues->day_m <= 31 )
    {//If all of the value are in the valid range

      month = dateTimeValues->mon;
      day = dateTimeValues->day_m ;
      year = dateTimeValues->year ;

      //Disable interrupts
      cli();

      outb(0x70, RTC_INDEX_MONTH);
      outb(0x71, month);
      outb(0x70, RTC_INDEX_DAY_MONTH);
      outb(0x71, day);
      outb(0x70, RTC_INDEX_YEAR);
      outb(0x71, year);

      //Enable interrupts
      sti();
      return E_NOERROR;
    }
    return E_INVPARA;
}

/** @brief  Name: get_date_main.
 *
 * Description: Retrieves system's current date.
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 * @return  0
 */

int get_date_main(int argc, char** argv)
{
  if(argc == 2)
  {
    date_time dateTimeValues;
    get_date(&dateTimeValues);
    printf("\tCurrent date is: %2d/%2d/%2d\n", dateTimeValues.mon, dateTimeValues.day_m, dateTimeValues.year);
  }
  else if(argc >= 2 && strcmp(argv[2], "--help") == 0)
  {
    print_help(GETDATE);
  }
  else
  {
    printf("ERROR: Please type in the correct arguments! Please refers to \"getdate --help\"\n");
  }

  return 0;
}

/** @brief  Name: get_date_main.
 *
 * Description: Retrieves system's current date.
 * @param argc - The number of tokens found.
 * @return  0
 */

int set_date_str(const char * str)
{
  char temp[10];
  date_time dateTimeValues;

  strcpy(temp, str);
  char *tok = strtok(temp, "/");

  if (is_digit(tok[0]) && (is_digit(tok[1])))
    dateTimeValues.mon = atoi(tok);
  else
    return E_INVPARA;

  tok = strtok(NULL, "/");

  if (is_digit(tok[0]) && (is_digit(tok[1])))
    dateTimeValues.day_m = atoi(tok);
  else
    return E_INVPARA;

  tok = strtok(NULL, "/");

  if (is_digit(tok[0]) && (is_digit(tok[1])))
    dateTimeValues.year = atoi(tok);
  else
    return E_INVPARA;

  return set_date(&dateTimeValues);
}

/** @brief  Name: set_date_str.
 *
 * Description: Sets the date for the system by string.
 * @param argc - The number of tokens found.
 * @param argv - The array of tokens.
 * @return 0
 */

int set_date_main(int argc, char **argv)
{
  if(argc >= 3 && strcmp(argv[2], "--help") != 0)
  {
    error_t errno = set_date_str(argv[2]);
    date_time dateTimeValues;

    switch (errno) {
      case E_NOERROR:
        get_date(&dateTimeValues);
        printf("\n\tSystem date successfully set!\n");
        printf("\tCurrent date is: %d/%d/%d\n\n", dateTimeValues.mon, dateTimeValues.day_m, dateTimeValues.year);
        break;
      case E_INVPARA:
        printf("ERROR: The date value you gave is invalid!\n");
        break;
      case E_INVSTRF:
        printf("ERROR: The format you used is incorrect! Please refer to \"setdate --help\"\n");
        break;
    }
  }
  else if(argc >= 3 && strcmp(argv[2], "--help") == 0)
  {
    print_help(SETDATE);
  }
  else
  {
    printf("ERROR: Please type in the arguments! Please refer to \"settime --help\"\n");
  }

  return 0;
}
