#ifndef _SERIAL_H
#define _SERIAL_H

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

/*
  Procedure..: init_serial
  Description..: Initializes devices for user interaction, logging, ...
*/
int init_serial(int device);

/*
  Procedure..: serial_println
  Description..: Writes a message to the active serial output device.
    Appends a newline character.
*/
int serial_println(const char *msg);

/*
  Procedure..: serial_print
  Description..: Writes a message to the active serial output device.
*/
int serial_print(const char *msg);

/*
  Procedure..: set_serial_out
  Description..: Sets serial_port_out to the given device address.
    All serial output, such as that from serial_println, will be
    directed to this device.
*/
int set_serial_out(int device);

/*
  Procedure..: set_serial_in
  Description..: Sets serial_port_in to the given device address.
    All serial input, such as console input via a virtual machine,
    QEMU/Bochs/etc, will be directed to this device.
*/
int set_serial_in(int device);

/*
  Procedure..: GetInputlnWithEcho
  Description..: Get user's input from keyborad with echo on the screen.
  Params..: buffer-the pointer to the buffer where store the user's input, buffer_size-the size of that buffer
*/
void GetInputlnWithEcho(char * buffer, const int buffer_size);

/*
  Procedure..: GetInputlnWithoutEcho
  Description..: Get user's input from keyborad with echo on the screen.
  Params..: buffer-the pointer to the buffer where store the user's input, buffer_size-the size of that buffer
*/
void GetInputlnWithoutEcho(char * buffer, const int buffer_size);
#endif
