/**
 * @file string.h
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @brief Many usefull functions that used for handling string.
 * @version R1
 */

 #ifndef _STRING_H
 #define _STRING_H

 #include <system.h>

 /**
  * @name isspace.
  *
  * @brief Identifies if its space
  *
  * @param  A constant character
  *
  * @return 1 if it is space, otherwise return 0.
  */

 int isspace(const char *c);

 /**
  * @name memset.
  *
  * @brief Sets region of memory
  * @param s  destination
  * @param c  byte to write
  * @param n  count
  * @return the pointer to the memory space.
  */
 void* memset(void *s, int c, size_t n);

 /**
  * @name: strcpy.
  *
  * @brief Copies one string to another.
  *
  * @param s1  Destination string
  * @param s2  Source string
  *
  * @return  pointer to the destination String
  */
 char* strcpy(char *s1, const char *s2);

 /**
  * @name strcat.
  *
  * @brief Concatenate the contents of one string onto another.
  * @param s1   Destination string
  * @param s2   Source string
  * @return  pointer to destination String
  */
 char* strcat(char *s1, const char *s2);

 /**
 * @name: strlen.
 *
 * @brief Returns the length of a string.
 *
 * @param s   String input.
 *
 * @return count    Length of the String
 */
 int   strlen(const char *s);

 /**
  * @name: strcmp.
  *
  * @brief String comparison.
  *
  * @param s1    First string to use for the compare.
  * @param s2    Second string to use for the compare.
  *
  * @return whether they are the same or not.
  */
 int   strcmp(const char *s1, const char *s2);

 /**
  * @name strtok.
  *
  * @brief Split string into tokens.
  * @param s1   String
  * @param s2   Delimiter
  * @return the pointer to the token.
  */
 char* strtok(char *s1, const char *s2);

 /**
  * @name: atoi.
  *
  * @brief Convert an ASCII string to an integer.
  *
  * @param s   String.
  *
  * @return The converted integer.
  */
 int atoi(const char *s);

 /**
  * @name: sprintf.
  *
  * @brief Generate a formatted string.
  *
  *	%[-x]c		output a character, '-' - align right, x - the output width
  *
  *	%[-x]s		output a string, '-' - align right, x - the output width
  *
  *	%[{-,+}x]d	output a character, '-' - align right, '+' - align right and display '+' sign, x - the output width
  *
  *	%[-x]X		(capital 'X') output a hexadecimal number, '-' - align right, x - the output width
  *
  * note: Output width will be ignored if width is smaller than actual length.
  *
  * @param str - Output string.
  * @param format - The format of the string.
  * @param ... - All of the additional parameters.
  * @return vsprintf(str, format, ap) - Return the string with its format and pointer.
  */
 int sprintf(char *str, const char *format, ...);

 /**
  * @name printf.
  *
  * @brief Print out a formatted string.
  *
  *	%[-x]c		output a character, '-' - align right, x - the output width
  *
  *	%[-x]s		output a string, '-' - align right, x - the output width
  *
  *	%[{-,+}x]d	output a character, '-' - align right, '+' - align right and display '+' sign, x - the output width
  *
  *	%[-x]X		(capital 'X') output a hexadecimal number, '-' - align right, x - the output width
  *
  * note: Output width will be ignored if width is smaller than actual length.
  *
  * @param str - Output string.
  * @param format - The format of the string.
  * @param ... - All of the additional parameters.
  * @return vsprintf(str, format, ap) - Return the string with its format and pointer.
  */
 int printf(const char *format, ...);
 #endif
