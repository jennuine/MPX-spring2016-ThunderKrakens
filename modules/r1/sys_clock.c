/**
 * @file sys_clock.c
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @version R1
 *
 * @brief The main file that manipulates and controls the system's clock.
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

/**
 * @name set_time_main.
 *
 * @brief Sets the time for the system.
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int set_time_main(int argc, char** argv)
{
  if(argc >= 3)
  {//If num of arguments is more than 3.
    if(strcmp(argv[2], "--help") == 0)
    {//If it is "--help"
      if(argc <= 3)
      {//If there is nothing after "--help"
        print_help(SETTIME);
      }
      else
      {
        printf("ERROR: Too many arguments you had gave! Please refer to \"settime --help\"\n");
      }
    }
    else
    {//If it is not "--help", which means set time.
      if(argc <= 3)
      {//If there is nothing after the time value.
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
      else
      {
        printf("ERROR: Too many arguments you had given! Please refer to \"settime --help\"\n");
      }
    }
  }
  else
  {
	   printf("ERROR: Please type in the arguments! Please refer to \"settime --help\"\n");
  }

  return 0;
}

/**
 * @name get_time_main.
 *
 * @brief Retrieves system's current time.
 * @param argc  The number of tokens found.
 * @param argv  The array of tokens.
 * @return  0
 */
int get_time_main(int argc, char** argv)
{
  if(argc == 2)
  {
  	date_time dateTimeValues;
  	get_time(&dateTimeValues);
  	printf("\tCurrent time is: %d:%d:%d\n", dateTimeValues.hour, dateTimeValues.min, dateTimeValues.sec);
  }
  else if(argc >= 3 && strcmp(argv[2], "--help") == 0)
  {
    if(argc <= 3)
    {
      print_help(GETTIME);
    }
    else
    {
      printf("ERROR: Too many arguments you had given! Please refer to \"settime --help\"\n");
    }
  }
  else
  {
	   printf("ERROR: Please type in the correct arguments! Please refers to \"gettime --help\"\n");
  }

  return 0;
}

/**
 * @name is_digit
 *
 * @brief determines if a character represents a digit.
 *
 * @param ch  The character
 *
 * @return 1 if it is digit, otherwise returns 0.
 */
static int is_digit(const char ch)
{
  return '0' <= ch && ch <= '9';
}

/**
 * @name set_time_str.
 *
 * @brief Sets the time for the system by string.
 * @param timeStr   The string type of current Time.
 * @return 0 if there is no error, otherwise return a error code.
 */
error_t set_time_str(const char * timeStr){
  static char tempStr[10];
  date_time dateTimeValues;
  int bIsValid = 1;

  strcpy(tempStr, timeStr);
  char * tok = strtok(tempStr, ":");

  if(bIsValid && (bIsValid = (strlen(tok) <= 2 && *tok && is_digit(*tok) &&   //first char must digit
                        	(!*(tok + 1) || is_digit(*(tok + 1))))))  //second char either not exsit or a digit
  {
	   dateTimeValues.hour = atoi(tok);
  }

  tok = strtok(NULL, ":");
  if(bIsValid && (bIsValid = (strlen(tok) <= 2 && *tok && is_digit(*tok) &&   //first char must digit
                        	(!*(tok + 1) || is_digit(*(tok + 1))))))  //second char either not exsit or a digit
  {
	   dateTimeValues.min = atoi(tok);
  }

  tok = strtok(NULL, ":");
  if(bIsValid && (bIsValid = (strlen(tok) <= 2 && *tok && is_digit(*tok) &&   //first char must digit
                        	(!*(tok + 1) || is_digit(*(tok + 1))))))  //second char either not exsit or a digit
  {
	   dateTimeValues.sec = atoi(tok);
  }

  if(bIsValid)
	 return set_time(&dateTimeValues);
  else
	 return E_INVSTRF;
}


/**
 * @name get_time.
 *
 * @brief Retrieves system's current time and date.
 * @param dateTimeValues  The value of current time and date
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

/**
 * @name set_time.
 *
 * @brief Sets the time for the system by date_time struct.
 * @param dateTimeValues   The struct that holds the time values.
 * @return 0 if there is no error, otherwise return a error code.
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

/**
 * @name get_date.
 *
 * @brief Retrieves system's current date.
 * @param dateTimeValues  The struct that holds the value of current date
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

  if(dateTimeValues->year >= 70)
  {
    dateTimeValues->year += 1900;
  }
  else
  {
    dateTimeValues->year += 2000;
  }
}

/**
 * @name is_date_value_valid.
 *
 * @brief Check if the date specified is valid, which means year should between 1970 ~ 1969,
          month should between 1 ~ 12, while the range of the day is based on the month and year.
 * @param year  The value of the year
 * @param mon  The value of the month
 * @param day  The value of the day of month
 * @return VOID
 */
static int is_date_value_valid(const int year, const int mon, const int day)
{
  int Result = 1;
  int DayMax = 0;

  Result = Result && (1 <= mon && mon <= 12);
  Result = Result && (1970 <= year && year <= 2069);

  switch(mon)
  {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      DayMax = 31;
    break;

    case 4:
    case 6:
    case 9:
    case 11:
      DayMax = 30;
    break;

    case 2:
      if(year % 4 == 0)
      {
        DayMax = 29;
      }
      else
      {
        DayMax = 28;
      }
  }
  Result = Result && (1 <= day && day <= DayMax);
  return Result;
}


/**
 * @name set_date.
 *
 * @brief Sets the date of the system.
 * @param dateTimeValues  The struct that holds the value of date
 * @return 0 if there is no error, otherwise return a error code.
 */
error_t set_date(const date_time * dateTimeValues)
{
  unsigned char month, day, year;

  if(is_date_value_valid(dateTimeValues->year, dateTimeValues->mon, dateTimeValues->day_m))
    {//If all of the value are in the valid range

      month = ((dateTimeValues->mon / 10) << 4) | (dateTimeValues->mon % 10);
      day = ((dateTimeValues->day_m / 10) << 4) | (dateTimeValues->day_m % 10);
      year = dateTimeValues->year % 100;
      year = ((year / 10) << 4) | (year % 10);

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

/**
 * @name get_date_main.
 *
 * @brief Retrieves system's current date.
 * @param argc  The number of tokens.
 * @param argv  The array of tokens.
 * @return  0
 */
int get_date_main(int argc, char** argv)
{
  if(argc == 2)
  {
    date_time dateTimeValues;
    get_date(&dateTimeValues);
    printf("\tCurrent date is: %d/%d/%d\n", dateTimeValues.mon, dateTimeValues.day_m, dateTimeValues.year);
  }
  else if(argc >= 3 && strcmp(argv[2], "--help") == 0)
  {
    if(argc <= 3)
    {
      print_help(GETDATE);
    }
    else
    {
      printf("ERROR: Too many arguments you had given! Please refer to \"settime --help\"\n");
    }
  }
  else
  {
    printf("ERROR: Please type in the correct arguments! Please refers to \"getdate --help\"\n");
  }

  return 0;
}

/**
 * @name set_date_str.
 *
 * @brief Sets the date for the system by string.
 * @param str   The string type of current date.
 * @return 0 if there is no error, otherwise return a error code.
 */
int set_date_str(const char * str)
{
  char temp[11];
  date_time dateTimeValues;

  strcpy(temp, str);
  char *tok = strtok(temp, "/");

  if (strlen(tok) == 2 && is_digit(tok[0]) && (is_digit(tok[1])))
    dateTimeValues.mon = atoi(tok);
  else
    return E_INVSTRF;

  tok = strtok(NULL, "/");

  if (strlen(tok) == 2 && is_digit(tok[0]) && (is_digit(tok[1])))
    dateTimeValues.day_m = atoi(tok);
  else
    return E_INVSTRF;

  tok = strtok(NULL, "/");

  if (strlen(tok) == 4 && is_digit(tok[0]) && is_digit(tok[1]) && is_digit(tok[2]) && is_digit(tok[3]))
    dateTimeValues.year = atoi(tok);
  else
    return E_INVSTRF;

  return set_date(&dateTimeValues);
}

/**
 * @name set_date_main.
 *
 * @brief Sets system's date.
 * @param argc  The number of tokens.
 * @param argv  The array of tokens.
 * @return 0
 */
int set_date_main(int argc, char **argv)
{
  if(argc >= 3 && strcmp(argv[2], "--help") != 0)
  {
    if(argc <= 3)
    {
      error_t errno = set_date_str(argv[2]);
      date_time dateTimeValues;

      switch (errno)
      {
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
    else
    {
      printf("ERROR: Too many arguments you had given! Please refer to \"setdate --help\"\n");
    }
  }
  else if(argc >= 3 && strcmp(argv[2], "--help") == 0)
  {
    if(argc <= 3)
    {
      print_help(SETDATE);
    }
    else
    {
      printf("ERROR: Too many arguments you had given! Please refer to \"setdate --help\"\n");
    }
  }
  else
  {
    printf("ERROR: Please type in the correct arguments! Please refer to \"setdate --help\"\n");
  }

  return 0;
}
