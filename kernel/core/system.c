#include <string.h>
#include <system.h>

#include <core/serial.h>

#include <core/io.h>
#define RTC_INDEX_SECOND 	0x00
#define RTC_INDEX_SECOND_ALARM 	0x01
#define RTC_INDEX_MINUTE 	0x02
#define RTC_INDEX_MINUTE_ALARM 	0x03
#define RTC_INDEX_HOUR	 	0x04
#define RTC_INDEX_HOUR_ALARM 	0x05

/*
  Procedure..: klogv
  Description..: Kernel log messages. Sent to active
      serial device.
*/
void klogv(const char *msg)
{
  char logmsg[64] = {'\0'}, prefix[] = "klogv: ";
  strcat(logmsg, prefix);
  strcat(logmsg, msg);
  serial_println(logmsg);
}

/*
  Procedure..: kpanic
  Description..: Kernel panic. Prints an error message
      and halts.
*/
void kpanic(const char *msg)
{
  cli(); //disable interrupts
  char logmsg[64] = {'\0'}, prefix[] = "Panic: ";
  strcat(logmsg, prefix);
  strcat(logmsg, msg);
  klogv(logmsg);
  hlt(); //halt
}

/*
  Procedure..: GetTime
  Description..: Gets the system time.
  Params..: hour-output hour, minute-output minute, second-output second.
*/
void GetTime(unsigned int * hour, unsigned int * minute, unsigned int * second)
{
	unsigned char sec, min, hr;
	outb(0x70, RTC_INDEX_SECOND);
	sec = inb(0x71);
	outb(0x70, RTC_INDEX_MINUTE);
	min = inb(0x71);
	outb(0x70, RTC_INDEX_HOUR);
	hr = inb(0x71);
	
	*hour = (hr >> 4) * 10 + (hr & 0x0f);
	*minute = (min >> 4) * 10 + (min & 0x0f);
	*second = (sec >> 4) * 10 + (sec & 0x0f);
	
}

/*
  Procedure..: PrintTime
  Description..: Print out the system time.
  Params..: None
*/
void PrintTime()
{
	unsigned int hour, minute, second;
	GetTime(&hour, &minute, &second);
	
	printf("Current Time (GMT +0) is: %-2d:%-2d:%-2d\n", hour, minute, second);
}

/*
  Procedure..: SetTime
  Description..: Sets the system time.
  Params..: hour-input hour, minute-input minute, second-input second.
*/
void SetTime(const unsigned int hour, const unsigned int minute, const unsigned int second)
{
	unsigned char sec, min, hr;
	hr = hour ? (((hour / 10) << 4) | (hour % 10)) : 0;
	min = minute ? (((minute / 10) << 4) | (minute % 10)) : 0;
	sec = second ? (((second / 10) << 4) | (second % 10)) : 0;
	
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
	
	PrintTime();
}

