#include "input.h"
#include "manager.h"
#include "output.h" // Added
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  // ProgramOptions default_options = {4, "matrix", "clock", false};

  // printf("Default options set.\n");
  // print_options(default_options);
  // printf("\n");
  ProgramOptions options = input_management(argc, argv);
  printf("----------------------------\n");
  printf("Final options after parsing command line:\n");
  print_options(options);

  int measurement_type = 0;
  if (strcmp(options.timer_type, "rdtsc"))
    measurement_type = RDTSC_MEASUREMENT;
  else if (strcmp(options.timer_type, "clock"))
    measurement_type = CLOCK_MEASUREMENT;
  else {
    printf("Timer incorectly defined. Using clock().\n");
    measurement_type = CLOCK_MEASUREMENT;
  }

  all_MeasuredInfo result;
  if (options.enable_io) {
    result = start_execution(IO, options.thread_count, 10, measurement_type);
  } else {
    if (strcmp(options.algorithm, "mergersort"))
      result =
          start_execution(SORT, options.thread_count, 10, measurement_type);
    else if (strcmp(options.algorithm, "matrix"))
      result =
          start_execution(MATRIX, options.thread_count, 10, measurement_type);
    else if (strcmp(options.algorithm, "prime"))
      result =
          start_execution(PRIME, options.thread_count, 10, measurement_type);
    else {
      printf("Algorithm incorectly defined. Using sort.\n");
      result =
          start_execution(SORT, options.thread_count, 10, measurement_type);
    }
  }

  printf("----------------------------\n");
  if (result.array_size > 0) {
    printf("Average after %d iterations:\n\n", 10);
    print_all_info(result);
    write_json_output(result);
    free(result.thread_info_array);
  }
  return 0;
}
