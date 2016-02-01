#include "R1.h"
#include "../mpx_supt.h"
#include <string.h>
#include <core/serial.h>

int commhand()
{
	printf("In R1 commhand...\n");
	printf("input > ");
	char userInput[100];
	GetInputln(userInput, 100, 1);
	printf("Your inputs: %s\n", userInput);
	return 0;
}

