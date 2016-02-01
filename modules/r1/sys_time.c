#include <system.h>
#include <string.h>

int checkTimeFormat(int argc, char argv[]) {
  char *input = *(argv + 1);
  printf("enter");
  char *tok = strtok(input, ":");

  while ( tok != NULL) {
    printf(" %s\n", tok);
    tok = strtok(NULL, ":");
  }
  return 0;
}
