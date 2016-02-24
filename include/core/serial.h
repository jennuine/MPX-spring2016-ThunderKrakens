/**
 * @file serial.h
 * @author Thunder Krakens
 * @date February 2nd, 2016
 * @brief Serial - Header
 * @version R1
 */


#ifndef _SERIAL_H
#define _SERIAL_H

#define COM1 0x3f8
#define COM2 0x2f8
#define COM3 0x3e8
#define COM4 0x2e8

//Using echo or nor, for the GetInputln
#define WithoutEcho 0
#define WithEcho 1

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

/**
 * @name  get_input_line
 *
 * @brief Get user's input from keyborad.
 *
 * @param buffer        The pointer to the buffer where store the user's input.
 * @param buffer_size   The size of that buffer.
 * @param bWithEcho     With echo or not
 *
 * @return VOID
 */
void get_input_line(char * buffer, const int buffer_size, const int bWithEcho);
#endif
