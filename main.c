#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]) {

  for (int i = 1; i < argc; i++) {
    if (access(argv[i], F_OK) != 0) {
      printf("File %s does not exist.\n", argv[i]);
      return 1;
    }

    printf("------------------------------------\nFile: %s\n------------------------------------\n", argv[i]);

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
