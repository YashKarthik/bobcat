#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_GRAY    "\x1b[90m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main (int argc, char *argv[]) {
  int isTerminal;
  if (isatty(fileno(stdout))) {
    isTerminal = 1;
  } else {
    isTerminal = 0;
  }

  struct winsize windowSize;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize);

  for (int i = 1; i < argc; i++) {
    if (access(argv[i], F_OK) != 0) {
      printf("File %s does not exist.\n", argv[i]);
      return 1;
    }

    if (isTerminal) {
      for (int w = 0; w < windowSize.ws_col; w++) {
        printf(ANSI_COLOR_GRAY "-");
      }
      printf(ANSI_COLOR_RESET "\nFile: %s\n", argv[i]);
      for (int w = 0; w < windowSize.ws_col; w++) {
        printf(ANSI_COLOR_GRAY "-");
      }
      printf(ANSI_COLOR_RESET "\n");
    }


    FILE *f = fopen(argv[i], "r");

    fseek(f, 0, SEEK_END);
    long fileSize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = malloc(fileSize+1);
    if (buffer == NULL) {
      printf("Could not allocate memory for buffer.\n");
      fclose(f);
      return 1;
    }

    size_t bytesRead = fread(buffer, 1, fileSize, f);
    if (bytesRead != fileSize) {
      printf("Error reading file.\n");
      free(buffer);
      fclose(f);
      return 1;
    }

    buffer[fileSize] = '\0';

    printf("%s", buffer);
    free(buffer);
    fclose(f);
  }
  printf("\n");

  return 0;
}
