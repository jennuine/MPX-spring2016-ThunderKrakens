/*
  ----- serial.c -----

  Description..: Contains methods and variables used for
    serial input and output.
*/

#include <stdint.h>
#include <string.h>

#include <core/io.h>
#include <core/serial.h>

#define NO_ERROR 0

//ANSI Escape ASCII Characters
#define ESC_KEY 27
#define BRACKET_KEY 91
#define ENTER_KEY 13
#define BACKSPACE_KEY 127
#define DEL_KEY_SEQ_3 51
#define DEL_KEY_SEQ_4 126
#define UP_ARROW 65
#define DOWN_ARROW 66
#define RIGHT_ARROW 67
#define LEFT_ARROW 68


// Active devices used for serial I/O
int serial_port_out = 0;
int serial_port_in = 0;

/*
  Procedure..: init_serial
  Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device)
{
  outb(device + 1, 0x00); //disable interrupts
  outb(device + 3, 0x80); //set line control register
  outb(device + 0, 115200/9600); //set bsd least sig bit
  outb(device + 1, 0x00); //brd most significant bit
  outb(device + 3, 0x03); //lock divisor; 8bits, no parity, one stop
  outb(device + 2, 0xC7); //enable fifo, clear, 14byte threshold
  outb(device + 4, 0x0B); //enable interrupts, rts/dsr set
  (void)inb(device);      //read bit to reset port
  return NO_ERROR;
}

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg)
{
  int i;
  for(i=0; *(i+msg)!='\0'; i++){
    outb(serial_port_out,*(i+msg));
  }
  outb(serial_port_out,'\r');
  outb(serial_port_out,'\n');
  return NO_ERROR;
}

/*
  Procedure..: serial_print
  Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg)
{
  int i;
  for(i=0; *(i+msg)!='\0'; i++){
    outb(serial_port_out,*(i+msg));
  }
  if (*msg == '\r') outb(serial_port_out,'\n');
  return NO_ERROR;
}

/*
  Procedure..: set_serial_out
  Description..: Sets serial_port_out to the given device address.
    All serial output, such as that from serial_println, will be
    directed to this device.
*/
int set_serial_out(int device)
{
  serial_port_out = device;
  return NO_ERROR;
}

/*
  Procedure..: set_serial_in
  Description..: Sets serial_port_in to the given device address.
    All serial input, such as console input via a virtual machine,
    QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device)
{
  serial_port_in = device;
  return NO_ERROR;
}

/*
  Procedure..: MoveCursorBackchar
  Description..: Move the cursor back for specific times.
  Params..: num-The number of times that needs to move back.
*/
static void MoveCursorBack(int num)
{
	while(num-- > 0)
		serial_print("\b");
}

/*
  Procedure..: PrintStars
  Description..: Print out the '*' for specific times.
  Params..: num-The number of times that needs to print.
*/
static void PrintStars(int num)
{
	while(num-- > 0)
		serial_print("*");
}

/*
  Procedure..: EchoInput
  Description..: Decides to print out the original string or stars.
  Params..: InputStr-The string, bWithEcho-Turn on the echo or not.
*/
static void EchoInput(const char * InputStr, const int bWithEcho)
{
	if(bWithEcho)
		serial_print(InputStr);
	else
		PrintStars(strlen(InputStr));
}


/*
  Procedure..: GetInputlnWithEcho
  Description..: Get user's input from keyborad.
  Params..: buffer-the pointer to the buffer where store the user's input, buffer_size-the size of that buffer
*/
void GetInputln(char * buffer, const int buffer_size, const int bWithEcho)
{
	int i = 0, cursorPos = 0;
	char userInputChar[] = { 0, 0 }, tempBuffer[buffer_size];

	while(userInputChar[0] != ENTER_KEY && i < buffer_size - 1)
	{//if user did not press enter, and we have enough space to store inputs.
		if(inb(COM1 + 5) & 1)
		{ //if there is inputs avalible
			userInputChar[0] = inb(COM1); //read in the input char
			if(userInputChar[0] == ESC_KEY)
			{ //if we receive a ESC Key
				if((inb(COM1 + 5) & 1) && (userInputChar[0] = inb(COM1)) == BRACKET_KEY)
				{ //check and receive the next ESC sequence.
					if((inb(COM1 + 5) & 1) && (userInputChar[0] = inb(COM1)) == UP_ARROW) //check and receive the next ESC sequence.
					{//It was Arrow up
						//serial_print("");
					}
					else if(userInputChar[0] == DOWN_ARROW)
					{//It was Arrow down
						//serial_print("");
					}
					else if(userInputChar[0] == RIGHT_ARROW && cursorPos < i)
					{//It was Arrow right
						serial_print("\x1B\x5B\x43"); //Let arrow move right
						cursorPos++;
					}
					else if(userInputChar[0] == LEFT_ARROW && cursorPos > 0)
					{//It was Arrow left
						serial_print("\x1B\x5B\x44"); //Let arrow move left
						cursorPos--;
					}else if(userInputChar[0] == DEL_KEY_SEQ_3)
					{//It was delete sequence 3.
						if((inb(COM1 + 5) & 1) && (userInputChar[0] = inb(COM1)) == DEL_KEY_SEQ_4 && cursorPos < i)
						{//It was delete key
							buffer[cursorPos] = '\0'; //delete the char at current position
							strcat(&buffer[cursorPos], &buffer[cursorPos + 1]); //move the rest of string forward
							i--; //string length - 1
							EchoInput(&buffer[cursorPos], bWithEcho); //override the old echo
							serial_print(" \b"); //remove the last useless char
							MoveCursorBack(i - cursorPos); //move the cursor back to original position
						}
					}
				}else
				{//It is only a ESC key, return without any input string.
					memset(buffer, '\0', buffer_size); //clean all the input chars
					serial_print("\n");
					return; 
				}
			}
			else if(userInputChar[0] == BACKSPACE_KEY) //if we receive a Backspace Key
			{
				if(cursorPos)
				{
					buffer[cursorPos - 1] = 0; //delete the char at previous position
					strcat(&buffer[cursorPos - 1], &buffer[cursorPos]); //move the rest of string forward
					i--; //string length - 1
					serial_print("\b"); //move cursor left
					cursorPos--; //move cursor left
					EchoInput(&buffer[cursorPos], bWithEcho);  //override the old echo
					serial_print(" \b"); //remove the last useless char
					MoveCursorBack(i - cursorPos); //move the cursor back to original position
				}
				else //if the cursor is in the first position, there is nothing to delete.
				{
					serial_print("");
				}
			}
			else if(userInputChar[0] == ENTER_KEY) //if we recive a Enter key
			{
				serial_print("\n");
			}
			else //otherwise, we consider it as a normal key, insert char into current position, and store it to user input string.
			{
				strcpy(tempBuffer, &buffer[cursorPos]); //copy the rest of string.
				buffer[cursorPos + 1] = '\0'; 
				strcat(&buffer[cursorPos + 1], tempBuffer); //paste it to next position.
				buffer[cursorPos] = userInputChar[0]; //insert user input
				buffer[++i] = '\0';
				EchoInput(&buffer[cursorPos], bWithEcho); //override the old echo
				cursorPos++;
				MoveCursorBack(i - cursorPos);
			}
		}
	}
}

