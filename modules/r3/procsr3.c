#include <system.h>
#include <core/serial.h>

#include "../mpx_supt.h"

#define RC_1 1
#define RC_2 2
#define RC_3 3
#define RC_4 4
#define RC_5 5

void proc1()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_1; i++){
      serial_println("proc1 dispatched");
      sys_req(IDLE);
    }
    sys_req(EXIT);
    serial_println("proc1 ran after it was terminated");
  }
}

void proc2()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_2; i++){
      serial_println("proc2 dispatched");
      sys_req(IDLE);
    }
    sys_req(EXIT);
    serial_println("proc2 ran after it was terminated");
  }
}

void proc3()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_3; i++){
      serial_println("proc3 dispatched");
      sys_req(IDLE);
    }
    sys_req(EXIT);
    serial_println("proc3 ran after it was terminated");
  }
}

void proc4()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_4; i++){
      serial_println("proc4 dispatched");
      sys_req(IDLE);
    }
    sys_req(EXIT);
    serial_println("proc4 ran after it was terminated");
  }
}

void proc5()
{
  int i;

  // repeat forever if termination fails
  while(1){
    for(i=0; i<RC_5; i++){
      serial_println("proc5 dispatched");
      sys_req(IDLE);
    }
    sys_req(EXIT);
    serial_println("proc5 ran after it was terminated");
  }
}
