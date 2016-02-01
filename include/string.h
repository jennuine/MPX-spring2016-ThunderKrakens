#ifndef _STRING_H
#define _STRING_H

#include <system.h>

/*
  Procedure..: isspace
  Description..: Determine if a character is whitespace.
  Params..: c-character to check
*/
int isspace(const char *c);

/*
  Procedure..: memset
  Description..: Set a region of memory.
  Params..: s-destination, c-byte to write, n-count
*/
void* memset(void *s, int c, size_t n);

/*
  Procedure..: strcpy
  Description..: Copy one string to another.
  Params..: s1-destination, s2-source
*/
char* strcpy(char *s1, const char *s2);

/*
  Procedure..: strcat
  Description..: Concatenate the contents of one string onto another.
  Params..: s1-destination, s2-source
*/
char* strcat(char *s1, const char *s2);

/*
  Procedure..: strlen
  Description..: Returns the length of a string.
  Params..: s-input string
*/
int   strlen(const char *s);

/*
  Procedure..: strcmp
  Description..: String comparison
  Params..: s1-string 1, s2-string 2
*/
int   strcmp(const char *s1, const char *s2);

/*
  Procedure..: strtok
  Description..: Split string into tokens
  Params..: s1-string, s2-delimiter
*/
char* strtok(char *s1, const char *s2);

/*
  Procedure..: atoi
  Description..: Convert an ASCII string to an integer
  Params..: const char *s -- String
*/
int atoi(const char *s);

/*
  Procedure..: sprintf
  Description..: generate a formatted string.
	%[-x]c		output a character, '-' - align right, x - the output width
	%[-x]s		output a string, '-' - align right, x - the output width
	%[{-,+}x]d	output a character, '-' - align right, '+' - align right and display '+' sign, x - the output width
	%[-x]X		(capital 'X') output a hexadecimal number, '-' - align right, x - the output width
	*note: output width will be ignored if width is smaller than actual length.
  Params..: str-output string, format-the format of the string, ...-the additional parameters.
*/
int sprintf(char *str, const char *format, ...);

/*
  Procedure..: convertFormat
  Description..: stores the formatted string in storage.
  Params..: format-the format of the string, storage - the char array to store the formatted string
*/
void convertFormat(const char *format, char *storage, ...);

/*
  Procedure..: printf
  Description..: print out a formatted string.
	%[-x]c		output a character, '-' - align right, x - the output width
	%[-x]s		output a string, '-' - align right, x - the output width
	%[{-,+}x]d	output a character, '-' - align right, '+' - align right and display '+' sign, x - the output width
	%[-x]X		(capital 'X') output a hexadecimal number, '-' - align right, x - the output width
	*note: output width will be ignored if width is smaller than actual length.
  Params..: format-the format of the string, ...-the additional parameters.
*/
int printf(const char *format, ...);
#endif
