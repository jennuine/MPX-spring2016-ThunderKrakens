#include "sys_clock.h"
#include "r1.h"
#include <string.h>
#include <core/io.h>
#define RTC_INDEX_SECOND 	0x00
#define RTC_INDEX_SECOND_ALARM 	0x01
#define RTC_INDEX_MINUTE 	0x02
#define RTC_INDEX_MINUTE_ALARM 	0x03
#define RTC_INDEX_HOUR	 	0x04
#define RTC_INDEX_HOUR_ALARM 	0x05
#define RTC_INDEX_DAY_WEEK 0x06
#define RTC_INDEX_DAY_MONTH 0x07
#define RTC_INDEX_MONTH 0x08
#define RTC_INDEX_YEAR 0x09

/*
  Procedure..: set_time_main
  Description..: The main function for set time.
*/
int set_time_main(int argc, char** argv)
{
  if(argc >= 3 && strcmp(argv[2], "--help") != 0)
  {
    error_t errno = set_time_str(argv[2]);
    date_time dateTimeValues;
    switch (errno) {
      case E_NOERROR:
      	get_time(&dateTimeValues);
        printf("System successfully set!\n");
        printf("Current time is: %02d:%02d:%02d\n", dateTimeValues.hour, dateTimeValues.min, dateTimeValues.sec);
      break;

      case E_INVPARA:
        printf("ERROR: The time value you gave is invalid!\n");
      break;

      case E_INVSTRF:
        printf("ERROR: The format you used is incorrect! Please refers to \"settime --help\"\n");
      break;
    }
  }
  else if(argc >= 3 && strcmp(argv[2], "--help") == 0)
  {
    printf("%s", functions[SETTIME].help);
  }
  else
  {
    printf("ERROR: Please type in the arguments! Please refers to \"settime --help\"\n");
  }

  return 0;
}

/*
  Procedure..: get_time_main
  Description..: The main function for get time.
*/
int get_time_main(int argc, char** argv)
{
  if(argc == 2)
  {
    date_time dateTimeValues;
    get_time(&dateTimeValues);
    printf("Current time is: %02d:%02d:%02d\n", dateTimeValues.hour, dateTimeValues.min, dateTimeValues.sec);
  }
  else if(argc >= 2 && strcmp(argv[2], "--help") == 0)
  {
    printf("%s", functions[GETTIME].help);
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

/*
  Procedure..: set_time_str
  Description..: set the system time by using string.
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


/*
  Procedure..: get_time
  Description..: Gets the system time.
  Params..: dateTimeValues-output the date & time value.
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

/*
  Procedure..: set_time
  Description..: Sets the system time.
  Params..: dateTimeValues-input the date & time value.
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

error_t set_date(const date_time * dateTimeValues)
{
	unsigned char month, day, year;
  if(0 <= dateTimeValues->mon &&  dateTimeValues->mon < 12 &&
      0 <= dateTimeValues->day_m && dateTimeValues->day_m < 30 )
  {//If all of the value are in the valid range
    month = dateTimeValues->mon ? (((dateTimeValues->mon / 10) << 4) | (dateTimeValues->mon % 10)) : 0;
  	day = dateTimeValues->day_m ? (((dateTimeValues->day_m / 10) << 4) | (dateTimeValues->day_m % 10)) : 0;
  	year = dateTimeValues->year ? (((dateTimeValues->year / 10) << 4) | (dateTimeValues->year % 10)) : 0;

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

int get_date_main(int argc, char** argv)
{
  if(argc == 2)
  {
    date_time dateTimeValues;
    get_date(&dateTimeValues);
    printf("Current date is: %d/%d/%d\n", dateTimeValues.mon, dateTimeValues.day_m, dateTimeValues.year);
  }
  else if(argc >= 2 && strcmp(argv[2], "--help") == 0)
  {
    printf("%s", functions[GETDATE].help);
  }
  else
  {
    printf("ERROR: Please type in the correct arguments! Please refers to \"getdate --help\"\n");
  }

  return 0;
}
