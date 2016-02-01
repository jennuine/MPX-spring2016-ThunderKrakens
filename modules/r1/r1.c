#include "r1.h"
#include "../mpx_supt.h"
// #include "sys_time.h"
#include <string.h>
#include <core/serial.h>


int commhand()
{
	printf("In R1 commhand...\n\n");

	char userInput[100];

	while (getInput(userInput))
	{
		char str[100];
		convertFormat(userInput, str);
		printf("Your inputs: %s\n", str);
	}
	return 0;
}
