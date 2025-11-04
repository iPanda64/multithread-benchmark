#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // For strrchr
#include <windows.h> // For GetModuleFileNameA

#define NUMBER_OF_FILES 128

void read(int file_number) {
  char exe_path[1024];
  char final_path[1024];

  DWORD len = GetModuleFileNameA(NULL, exe_path, sizeof(exe_path));
  if (len == 0 || len >= sizeof(exe_path)) {
    printf("Error: GetModuleFileNameA failed or buffer too small\n");
    return;
  }
  char *dir_end = strrchr(exe_path, '\\');
  if (dir_end == NULL) {
    dir_end = strrchr(exe_path, '/');
  }

  if (dir_end == NULL) {
    printf("Error: Could not find directory from executable path\n");
    return;
  }

  *(dir_end) = '\0';

  snprintf(final_path, sizeof(final_path), "%s\\resources\\file_%d.bin",
           exe_path, file_number);

  FILE *f = fopen(final_path, "rb");
  if (f == NULL) {
    printf("Error: Error opening file: %s\n", final_path);
    return;
  }
  // printf("Starting the reading of file: %d\n ", file_number);
  int buff;
  long bytes_read_count = 0;
  long long bytes_sum = 0;
  while ((buff = fgetc(f)) != EOF) {
    bytes_read_count++;
    bytes_sum +=
        buff; // need so the compiler doesn't optimize the read and the loop
  }
  // printf("Ending the reading of file: %d (Sum of all bytes: %lld) (Number of
  // " "bytes read: %d)\n ",
  //       file_number, bytes_sum, bytes_read_count);
  fclose(f);
}
void read_from_interval(int start, int end) {
  for (int i = start; i <= end; ++i)
    read(i);
}
