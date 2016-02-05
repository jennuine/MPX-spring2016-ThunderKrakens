/**
 * @file string.c
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @brief String Handling
 * @version R1
 */
#include <system.h>
#include <core/serial.h>
#include "../modules/mpx_supt.h"
#include <string.h>
#include <core/serial.h>


/* ****************************** *
 * ********* Your Task: ********* *
 * ******** complete the ******** *
 * * following string functions * *
 * ****************************** */

 /** @brief  Name: strlen.
 *
 * Description: Returns the length of a string.
 * @param s - String input.
 * @return count - Length of the String
 */

int strlen(const char *s)
{
  int count = 0;
  for(; *s; s++, count++)
  {
  }

  return count; // return length of string
}

/** @brief  Name: strcpy.
 *
 * Description: Copies one string to another.
 * @param s1 - Destination string
 * @param s2 - Source string
 * @return  s1 - Destination String
 */

char* strcpy(char *s1, const char *s2)
{
  while ((*s1++ = *s2++) != '\0');
  return s1;// return pointer to destination string
}

/** @brief  Name: atoi.
 *
 * Description: Convert an ASCII string to an integer.
 * @param const char *s - String.
 * @return integer - The converted integer.
 */

int atoi(const char *s)
{
  int sign, i, integer = 0;

  sign = ((i = (s[0] == '-')) ? -1 : 1);

  while (s[i])
  {
    if (s[i] >= '0' && s[i] <= '9')
      integer = integer * 10 + (s[i] - '0');
    else
    {
      serial_println("Error (atoi): that was not a valid integer.");
      return 0;
    }
    i++;
  }

  integer = sign * integer;

  return integer; // return integer
}

/** @brief  Name: strcmp.
 *
 * Description: String comparison.
 * @param s1 - First string to use for the compare.
 * @param s2 - Second string to use for the compare.
 * @return whether they are the same or not.
 */

int strcmp(const char *s1, const char *s2)
{
  for(; (*s1 == *s2) && *s1 && *s2; s1++, s2++)
  {
  }

  return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
}

/** @brief  Name: ParsePadding.
 *
 * Description: Parse the number for padding. (static - Only can be access within this file).
 * @param str - Paddling String
 * @param width - Paddling Width
 * @param DecWidth - Width of decimal part.
 * @param bIsRight - Is align right.
 * @param bHasSign - Has + / -.
 * @return bIsValid - Returns the validity.
 */

static int ParsePadding(const char * Str, int * Width, int * DecWidth, int * bIsRight, int * bHasSign)
{
	*Width = *DecWidth = *bIsRight = *bHasSign = 0;
	int bIsValid = 1, bIsAfterDot = 0, i = 0;

	for(i = 0; *Str && bIsValid; Str++, i++)
	{
		if(i == 0 && (*Str == '+' || *Str == '-'))
		{
			*bIsRight = 1;
			*bHasSign = *Str == '+' ? 1 : 0;
		}
		else if('0' <= *Str && *Str <= '9')
		{
			if(!bIsAfterDot)
			{
				*Width = (*Width * 10) + (*Str - '0');
			}
			else
			{
				*DecWidth = (*DecWidth * 10) + (*Str - '0');
			}
		}
		else if(*Str == '.' && !bIsAfterDot)
		{
			bIsAfterDot = 1;
		}
		else
		{
			bIsValid = 0;
		}
	}
	return bIsValid;
}

/** @brief  Name: AddPad.
 *
 * Description: Add a certain number of paddings (static - Only can be access within this file).
 * @param str - In string.
 * @param count - Number of whitespace.
 * @return VOID
 */

static void AddPad(char * Str, const unsigned int count)
{
	unsigned int i = 0;
	for(i = 0; i < count; i++, Str++)
	{
		*Str = ' ';
	}
}

/** @brief  Name: itoa.
 *
 * Description: Convert integer into string (static - Only can be access within this file).
 * @param str - In string.
 * @param Value - Value of the integer.
 * @param width - Paddling width.
 * @param bIsRight - Is align right
 * @param bHasSign - Has sign + / -
 * @return VOID
 */

static void itoa(char * Str, const int Value, const int Width, const int bIsRight, const int bHasSign)
{
	int ActW = (Value == 0 ? 1: 0) + ((bHasSign || Value < 0) ? 1 : 0), TempV = Value;
	while(TempV != 0)
	{
		TempV = TempV / 10;
		ActW++;
	}
	if(bIsRight && Width > ActW)
	{
		AddPad(Str, (Width - ActW));
		Str += (Width - ActW);
	}
	if(bHasSign || Value < 0)
	{
		*Str = (Value < 0) ? '-' : '+';
		Str++;
	}

	TempV = Value >= 0 ? Value : -1 * Value;
	Str += (ActW - 1 - ((bHasSign || Value < 0) ? 1 : 0));
	do
	{
		*Str = (TempV % 10) + '0';
		Str--;
		TempV = TempV / 10;
	}while(TempV != 0);
	Str += (ActW + 1 - ((bHasSign || Value < 0) ? 1 : 0));

	if(!bIsRight && Width > ActW)
	{
		AddPad(Str, (Width - ActW));
		Str += (Width - ActW);
	}
	*Str = 0;
}

static char ByteMap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };


/**
 *  @brief NibbleToChar
 *
 *  Description: convert a nibble into a single hexadecimal (static - Only can be access within this file)
 *  @param value - The value of the nibble
 *  @return ByteMap[Value] - Hexadecimal Map
 *  @return * - Invalid
 */

static char NibbleToChar(unsigned char Value)
{
	if(Value < 16)
	{
		return ByteMap[Value];
	}
	return '*';
}

/** @brief  Name: bytesToHexString.
 *
 * Description: Convert bytes into a hexadecimal string (static - Only can be access within this file).
 * @param OutStr - Output string.
 * @param Value - The value of bytes.
 * @return VOID
 */


static void bytesToHexString(char * OutStr, const int Value)
{
	int i = 0, Size = sizeof(Value), TempV = Value;
	unsigned char * Bytes = (unsigned char *)&TempV;

	for(i = 0; i < Size; i++) {
		OutStr[(2 * Size) - (i * 2) - 2] = NibbleToChar((Bytes[i] >> 4));
		OutStr[(2 * Size) - (i * 2) - 1] = NibbleToChar((Bytes[i] & 0x0f));
	}

	OutStr[2 * i] = '\0';
}

/** @brief  Name: vsprintf.
 *
 * Description: The actual function that perform the "printf" and "sprintf" function (static - Only can be access within this file).
 * @param str - Output string.
 * @param format - The format of the string.
 * @param ap - the pointer of the first additional parameter.
 * @return 0
 */

static int vsprintf(char *str, const char *format, const int * ap)
{
	char * StrPtr = str;

	for(; *format; format++)
	{
		if(*format == '%')
		{
			char PadValue[100] = { 0 };
			int Width = 0, DecWidth = 0, bIsRight = 0, bHasSign = 0, bIsValid = 1;
			format++;
			if(*format == '+' || *format == '-' || *format == '.' || ('0' <= *format && *format <= '9'))
			{
				int i = 0;
				do
				{
					PadValue[i++] = *format++;
				}while(*format == '.' || ('0' <= *format && *format <= '9'));
				PadValue[i] = '\0';
				bIsValid = ParsePadding(PadValue, &Width, &DecWidth, &bIsRight, &bHasSign);
			}

			if(bIsValid && *format == 'c')
			{
				if(bIsRight && Width > 1)
				{
					AddPad(StrPtr, Width - 1);
					StrPtr += (Width - 1);
				}
				*StrPtr++ = (char)*ap++;
				if(!bIsRight && Width > 1)
				{
					AddPad(StrPtr, Width - 1);
					StrPtr += (Width - 1);
				}
			}
			else if(bIsValid && *format == 's')
			{
				char * InputStr = (char *)*ap++;
				int StrSize = strlen(InputStr);
				if(bIsRight && Width > StrSize)
				{
					AddPad(StrPtr, Width - StrSize);
					StrPtr += (Width - StrSize);
				}
				strcat(StrPtr, InputStr);
				StrPtr += StrSize;
				if(!bIsRight && Width > StrSize)
				{
					AddPad(StrPtr, Width - StrSize);
					StrPtr += (Width - StrSize);
				}
			}
			else if(bIsValid && *format == 'd')
			{
				char ValueStr[100] = { 0 };
				int ActValue = (int)*ap++;
				itoa(ValueStr, ActValue, Width, bIsRight, bHasSign);
				strcat(StrPtr, ValueStr);
				StrPtr += strlen(ValueStr);
			}
			else if(bIsValid && *format == 'X')
			{
				char ValueStr[100] = { 0 };
				ValueStr[0] = '0';
				ValueStr[1] = 'x';
				int ActValue = (int)*ap++;
				bytesToHexString(&ValueStr[2], ActValue);
				int HexStrSize = strlen(ValueStr);
				if(bIsRight && Width > HexStrSize)
				{
					AddPad(StrPtr, Width - HexStrSize);
					StrPtr += (Width - HexStrSize);
				}
				strcat(StrPtr, ValueStr);
				StrPtr += HexStrSize;
				if(!bIsRight && Width > HexStrSize)
				{
					AddPad(StrPtr, Width - HexStrSize);
					StrPtr += (Width - HexStrSize);
				}
			}
			else
			{
				char * PadPtr = PadValue;
				*StrPtr++ = '%';
				while( *PadPtr && (*StrPtr++ = *PadPtr++) );
				*StrPtr++ = *format;
			}
		}
		else
		{
			*StrPtr++ = *format;
		}
	}
	StrPtr = '\0';
	return 0;
}

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

int sprintf(char *str, const char *format, ...)
{
	int * ap = (int *)&format;
	ap++;
	return vsprintf(str, format, ap);
}

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

int printf(const char *format, ...)
{
	int * ap = (int *)&format;
	ap++;
	static char tempStr[5000];
	memset(tempStr, 0, 5000);
	int result = vsprintf(tempStr, format, ap);
	serial_print(tempStr);
	return result;
}

/* ---------------------------------------
    Functions below this point are given.
    No need to tamper with these!
   --------------------------------------- */

/*
  Procedure..: strcat
  Description..: Concatenate the contents of one string onto another.
  Params..: s1-destination, s2-source
*/
char* strcat(char *s1, const char *s2)
{
  char *rc = s1;
  if (*s1) while(*++s1);
  while( (*s1++ = *s2++) );
  return rc;
}

/*
  Procedure..: isspace
  Description..: Determine if a character is whitespace.
  Params..: c-character to check
*/
int isspace(const char *c)
{
  if (*c == ' '  ||
      *c == '\n' ||
      *c == '\r' ||
      *c == '\f' ||
      *c == '\t' ||
      *c == '\v'){
    return 1;
  }
  return 0;
}

/*
  Procedure..: memset
  Description..: Set a region of memory.
  Params..: s-destination, c-byte to write, n-count
*/
void* memset(void *s, int c, size_t n)
{
  unsigned char *p = (unsigned char *) s;
  while(n--){
    *p++ = (unsigned char) c;
  }
  return s;
}

/*
  Procedure..: strtok
  Description..: Split string into tokens
  Params..: s1-string, s2-delimiter
*/
char* strtok(char *s1, const char *s2)
{
  static char *tok_tmp = NULL;
  const char *p = s2;

  //new string
  if (s1!=NULL){
    tok_tmp = s1;
  }
  //old string cont'd
  else {
    if (tok_tmp==NULL){
      return NULL;
    }
    s1 = tok_tmp;
  }

  //skip leading s2 characters
  while ( *p && *s1 ){
    if (*s1==*p){
      ++s1;
      p = s2;
      continue;
    }
    ++p;
  }

  //no more to parse
  if (!*s1){
    return (tok_tmp = NULL);
  }

  //skip non-s2 characters
  tok_tmp = s1;
  while (*tok_tmp){
    p = s2;
    while (*p){
      if (*tok_tmp==*p++){
	*tok_tmp++ = '\0';
	return s1;
      }
    }
    ++tok_tmp;
  }

  //end of string
  tok_tmp = NULL;
  return s1;
}
