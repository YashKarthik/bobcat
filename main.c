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
    if (access(argv[i], F_OK) != 0 || argv[i] == NULL) {
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
    char *line = NULL; // getlines will allocate buffer and assign pointer to this
    size_t len = 0;
    ssize_t read;
    int lineNum = 0;

    while ((read = getline(&line, &len, f)) != -1) {
      if (isTerminal) {
        printf(ANSI_COLOR_GRAY " %i  | " ANSI_COLOR_RESET, lineNum);
      }
      printf("%s", line);
      lineNum++;
    }

    free(line);
    fclose(f);
  }
  printf("\n");

  return 0;
}
