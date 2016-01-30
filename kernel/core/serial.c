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
  Procedure..: GetInputlnWithEcho
  Description..: Get user's input from keyborad with echo on the screen.
  Params..: buffer-the pointer to the buffer where store the user's input, buffer_size-the size of that buffer
*/
void GetInputlnWithEcho(char * buffer, const int buffer_size)
{
	int i = 0;
	char userInputChar[] = { 0, 0 };
	while(userInputChar[0] != 13 && i < buffer_size)
	{
		if(inb(COM1 + 5) & 1)
		{
			userInputChar[0] = inb(COM1);
			buffer[i++] = (userInputChar[0] == 13) ? 0 : userInputChar[0];
			serial_print((userInputChar[0] == 13) ? "\n" : userInputChar);
		}
	}
}

/*
  Procedure..: GetInputlnWithoutEcho
  Description..: Get user's input from keyborad with echo on the screen.
  Params..: buffer-the pointer to the buffer where store the user's input, buffer_size-the size of that buffer
*/
void GetInputlnWithoutEcho(char * buffer, const int buffer_size)
{
	int i = 0;
	char userInputChar = 0;
	while(userInputChar != 13 && i < (buffer_size - 1))
	{
		if(inb(COM1 + 5) & 1)
		{
			userInputChar = inb(COM1);
			buffer[i++] = (userInputChar == 13) ? 0 : userInputChar;
			serial_print("*");
		}
	}
	serial_print("\n");
}
