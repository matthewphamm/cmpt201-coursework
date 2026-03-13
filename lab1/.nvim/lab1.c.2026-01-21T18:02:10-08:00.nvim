#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *buff = NULL;
  size_t size = 0;

  char *saveptr;

  printf("Please enter some text: ");
  ssize_t characters = getline(&buff, &size, stdin);

  if (characters == -1) {
    perror("getline failed");
    exit(EXIT_FAILURE);
  }

  char *token = strtok_r(buff, " ", &saveptr);

  printf("Tokens:\n");

  while (token != NULL) {
    printf(" %s\n", token);

    token = strtok_r(NULL, " ", &saveptr);
  }

  free(buff);
  return 0;
}
