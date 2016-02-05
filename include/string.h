/**
 * @file string.h
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @brief String Handling - Header
 *
 */

#ifndef _STRING_H
#define _STRING_H

#include <system.h>

/** @brief  Name: isspace.
 *
 * Description: Identifies if its space
 * @param const char *c - A constant character
 */
 
int isspace(const char *c);

/** @brief  Name: memset.
 *
 * Description: Sets region of memory
 * @param s - destination
 * @param c - byte to write
 * @param n - count
 * @return VOID
 */

void* memset(void *s, int c, size_t n);

/** @brief  Name: strcpy.
 *
 * Description: Copies one string to another.
 * @param s1 - Destination string
 * @param s2 - Source string
 * @return  s1 - Destination String
 */
 
char* strcpy(char *s1, const char *s2);

/** @brief  Name: strcat.
 *
 * Description: Concatenate the contents of one string onto another.
 * @param s1 - Destination string
 * @param s2 - Source string
 * @return  s1 - Destination String
 */

char* strcat(char *s1, const char *s2);

 /** @brief  Name: strlen.
 *
 * Description: Returns the length of a string.
 * @param s - String input.
 * @return count - Length of the String
 */
 
int   strlen(const char *s);

/** @brief  Name: strcmp.
 *
 * Description: String comparison.
 * @param s1 - First string to use for the compare.
 * @param s2 - Second string to use for the compare.
 * @return whether they are the same or not.
 */
 
int   strcmp(const char *s1, const char *s2);

/** @brief  Name: strtok.
 *
 * Description: Split string into tokens.
 * @param s1 - String
 * @param s2 - Delimiter
 */

char* strtok(char *s1, const char *s2);

/** @brief  Name: atoi.
 *
 * Description: Convert an ASCII string to an integer.
 * @param const char *s - String.
 * @return integer - The converted integer.
 */
 
int atoi(const char *s);

/** @brief  Name: sprintf.
 *
 * Description: Generate a formatted string.
 *
 *	%[-x]c		output a character, '-' - align right, x - the output width
 *
 *	%[-x]s		output a string, '-' - align right, x - the output width
 *
 *	%[{-,+}x]d	output a character, '-' - align right, '+' - align right and display '+' sign, x - the output width
 *
 *	%[-x]X		(capital 'X') output a hexadecimal number, '-' - align right, x - the output width
 *
 * @note Output width will be ignored if width is smaller than actual length.
 *
 * @param str - Output string.
 * @param format - The format of the string.
 * @param ... - All of the additional parameters.
 * @return vsprintf(str, format, ap) - Return the string with its format and pointer.
 */
 
int sprintf(char *str, const char *format, ...);

/** @brief  Name: printf.
 *
 * Description: Print out a formatted string.
 *
 *	%[-x]c		output a character, '-' - align right, x - the output width
 *
 *	%[-x]s		output a string, '-' - align right, x - the output width
 *
 *	%[{-,+}x]d	output a character, '-' - align right, '+' - align right and display '+' sign, x - the output width
 *
 *	%[-x]X		(capital 'X') output a hexadecimal number, '-' - align right, x - the output width
 *
 * @note Output width will be ignored if width is smaller than actual length.
 *
 * @param str - Output string.
 * @param format - The format of the string.
 * @param ... - All of the additional parameters.
 * @return vsprintf(str, format, ap) - Return the string with its format and pointer.
 */
 
int printf(const char *format, ...);
#endif
