#include <system.h>
#include <string.h>
#include <core/serial.h>


/* ****************************** *
 * ********* Your Task: ********* *
 * ******** complete the ******** *
 * * following string functions * *
 * ****************************** */

/*
  Procedure..: strlen
  Description..: Returns the length of a string.
  Params..: s-input string
*/
int strlen(const char *s)
{
  int count = 0;
  for(; *s; s++, count++)
  {
  }
  
  return count; // return length of string
}

/*
  Procedure..: strcpy
  Description..: Copy one string to another.
  Params..: s1-destination, s2-source
*/
char* strcpy(char *s1, const char *s2)
{
  while ((*s1++ = *s2++) != '\0');
  return s1;// return pointer to destination string
}


/*
  Procedure..: atoi
  Description..: Convert an ASCII string to an integer
  Params..: const char *s -- String
*/
int atoi(const char *s)
{
  int sign, i, integer = 0;

  if (s[0] == '-')
    sign = 1;

  i = sign;

  while ( s[i] != '\0')
  {
    if (s[i] >= '0' && s[i] <= '9')
      integer = integer * 10 + (s[i] - '0');
    else
    {
      serial_println("Error: that was not a valid integer.\n");
      return 0;
    }
    i++;
  }

  if (sign == 1)
    integer = -integer;

  return integer; // return integer
}

/*
  Procedure..: strcmp
  Description..: String comparison
  Params..: s1-string 1, s2-string 2
*/
int strcmp(const char *s1, const char *s2)
{
  for(; (*s1 == *s2) && *s1 && *s2; s1++, s2++)
  {
  } 
  
  return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
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
