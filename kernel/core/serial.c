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
#define ESC 27
#define BRACKET 91
#define ENTER_KEY 13
#define BACKSPACE_KEY 127
#define DEL_KEY_1 51
#define DEL_KEY_2 126
#define UP_ARROW 65
#define DOWN_ARROW 66
#define RIGHT_ARROW 67
#define LEFT_ARROW 68
#define BACKSPACE 0x1B5808

//ANSI Escape Seqences
#define SAVE_CURSOR_POS "\033[s"
#define RESTORE_CURSOR_POS "\033[u"
#define CLEAR_LINE "\x1b[K"
#define DEL_SEQ "\x1b[P"

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

#if 0

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

#endif

void outByte(int inb)
{
  outb(COM1, ESC);
  outb(COM1, BRACKET);
  outb(COM1, inb);
}

void delete(char * input, int i)
{

  int count = 0, j = i;

  while ((input[j] = input[j+1]) != '\0')
  {
    outb(COM1, input[j]);
    count++;
    j++;
  }
  input[j] = '\0';
  serial_print(" ");


  while (count >= 0)
  {
    outByte(LEFT_ARROW);
    count--;
  }
}

int getInput(char * input)
{
  int i = 0;
	while(1)
	{
    // serial_print("\033[s"); //saves currsor position

		if(inb(COM1 + 5) & 1)
		{
      int inb = inb(COM1);

        if (inb == ESC)
        {
          inb(COM1); //removes bracket
          inb = inb(COM1);

          if (inb == DEL_KEY_1)
          {
            inb(COM1); //removes tilde
            serial_print(DEL_SEQ);

            delete(input, i);
          }
          else if (inb == UP_ARROW)
          {

          }
          else if (inb == DOWN_ARROW)
          {

          }

          else if (inb == LEFT_ARROW)
          {
            outByte(inb);
            i--;
          }

          else if (inb == RIGHT_ARROW)
          {
            if (input[i+1] == NULL) continue;
            outByte(inb);
          }

        }
        else if (inb == BACKSPACE_KEY)
        {

          outb(COM1, BACKSPACE); //moves cursor left 1
          i--;
          delete(input, i);
        }
        else if (inb == ENTER_KEY)
        {
          serial_print(CLEAR_LINE);
          return 1;
        }
        else if(inb == 122) //used for testing, enter z to escape loop
        {
          break;
        }
        else
        {
          outb(COM1, input[i] = inb);
          i++;
          input[i] = '\0';
        }
    }
    // serial_print("\033[u");
    // serial_print("\x1B[K");
  }
  return 0;
}



#if 0

/*
  Procedure..: GetInputlnWithEcho
  Description..: Get user's input from keyborad.
  Params..: buffer-the pointer to the buffer where store the user's input, buffer_size-the size of that buffer
*/
void GetInputln(char * buffer, const int buffer_size, const int bWithEcho)
{
	int i = 0, cursorPos = 0;
	char userInputChar[] = { 0, 0 }, tempBuffer[buffer_size];

	while(userInputChar[0] != 13 && i < buffer_size - 1)
	{
		if(inb(COM1 + 5) & 1)
		{
			userInputChar[0] = inb(COM1);
			//printf("%d\n", userInputChar[0]);
			if(userInputChar[0] == 27)
			{
				if((inb(COM1 + 5) & 1) && (userInputChar[0] = inb(COM1)) == 91)
				{
					if((inb(COM1 + 5) & 1) && (userInputChar[0] = inb(COM1)) == 65) //arrow up
					{
						//serial_print("");
					}
					else if(userInputChar[0] == 66) //arrow down
					{
						//serial_print("");
					}
					else if(userInputChar[0] == 67 && cursorPos < i) //arrow right
					{
						outb(COM1, 27);
						outb(COM1, 91);
						outb(COM1, 67);
						cursorPos++;
					}
					else if(userInputChar[0] == 68 && cursorPos > 0) //arrow left
					{
						outb(COM1, 27);
						outb(COM1, 91);
						outb(COM1, 68);
						cursorPos--;
					}else if(userInputChar[0] == 51)
					{
						if((inb(COM1 + 5) & 1) && (userInputChar[0] = inb(COM1)) == 126 && cursorPos < i) //delete
						{
							buffer[cursorPos] = 0;
							strcat(&buffer[cursorPos], &buffer[cursorPos + 1]);
							i--;
							EchoInput(&buffer[cursorPos], bWithEcho);
							serial_print(" \b");
							MoveCursorBack(i - cursorPos);
						}
					}
				}else
				{
					memset(buffer, 0, buffer_size);
					serial_print("\n");
					return;
				}
			}
			else if(userInputChar[0] == 127)
			{
				if(cursorPos)
				{
					buffer[cursorPos - 1] = 0;
					strcat(&buffer[cursorPos - 1], &buffer[cursorPos]);
					cursorPos--;
					i--;
					serial_print("\b");
					EchoInput(&buffer[cursorPos], bWithEcho);
					serial_print(" \b");
					MoveCursorBack(i - cursorPos);
				}
				else
				{
					serial_print("");
				}
			}
			else if(userInputChar[0] == 13)
			{
				serial_print("\n");
			}
			else
			{
				strcpy(tempBuffer, &buffer[cursorPos]);
				buffer[cursorPos + 1] = 0;
				strcat(&buffer[cursorPos + 1], tempBuffer);
				buffer[cursorPos] = userInputChar[0];
				buffer[++i] = 0;
				EchoInput(&buffer[cursorPos], bWithEcho);
				cursorPos++;
				MoveCursorBack(i - cursorPos);
			}
		}
	}
}

#endif
