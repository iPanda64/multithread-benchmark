#include "input.h"
#include <stdio.h>
#include <stdlib.h> // For exit()
#include <string.h>
#include <thread>

// Function to print the help message
void print_help() {
  printf("Usage: program.exe [OPTIONS]\n");
  printf("\n");
  printf("Options:\n");
  printf("\n");
  printf("  -h, --help             Display this help message and exit.\n");
  printf("\n");
  printf("  --threads <number|auto>  Set the number of threads.\n");
  printf("                         'auto' uses hardware concurrency.\n");
  printf("                         (Default: hardware concurrency or 4 if "
         "unavailable)\n");
  printf("\n");
  printf("  --algorithm <type>     Set the algorithm to run.\n");
  printf("                         Available types: sorting, matrix, prime\n");
  printf("                         (Default: matrix)\n");
  printf("\n");
  printf("  --timer <type>         Set the timer type.\n");
  printf("                         (Default: clock)\n");
  printf("\n");
  printf("  --io                   Enable I/O mode.\n");
  printf("                         (Default: disabled)\n");
  exit(0); // Exit after printing help
}

void print_options(const ProgramOptions &options) {
  printf("--- Program Configuration ---\n");
  printf("Thread Count: %d\n", options.thread_count);
  printf("Algorithm:    %s\n", options.algorithm);
  printf("Timer Type:   %s\n", options.timer_type);
  printf("I/O Enabled:  %s\n", (options.enable_io ? "Yes" : "No"));
  printf("----------------------------\n");
}

int form_number(const char *string) {
  int number = 0;
  for (int i = 0; i < strlen(string); ++i) {
    if (string[i] < '0' || string[i] > '9')
      return -1;
    else {
      number = number * 10 + (string[i] - '0');
    }
  }
  return number;
}

bool valid_algorithm(const char *string) {
  if (strcmp(string, "sorting") == 0 || strcmp(string, "matrix") == 0 ||
      strcmp(string, "prime") == 0)
    return true;
  return false;
}

ProgramOptions input_management(int argc, char *argv[]) {

  unsigned int default_threads = std::thread::hardware_concurrency();
  if (default_threads == 0) {
    default_threads = 4;
  }

  ProgramOptions options = {(int)default_threads, "matrix", "clock", false};

  // printf("Default options set.\n");

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--threads") == 0) {
      if (i + 1 < argc) {
        const char *value = argv[i + 1];
        i++;
        if (strcmp(value, "auto") == 0) {
          options.thread_count = (int)default_threads;
        } else {
          options.thread_count = form_number(value);
          if (options.thread_count == 0 || options.thread_count == -1 ||
              options.thread_count > default_threads) {
            printf("Warning: Invalid or zero number for --threads. Using "
                   "default.\n");
            options.thread_count = (int)default_threads;
          }
        }
      } else {
        printf(
            "Warning: Invalid or zero number for --threads. Using default.\n");
        options.thread_count = (int)default_threads;
      }
    } else if (strcmp(argv[i], "--algorithm") == 0) {
      if (i + 1 < argc) {
        if (valid_algorithm(argv[i + 1]))
          options.algorithm = argv[i + 1];
        else {
          printf("Warning: Invalid --algorithm. Using default.\n");
          options.algorithm = "matrix";
        }
        i++;
      } else {
        options.algorithm = "matrix";
      }
    } else if (strcmp(argv[i], "--timer") == 0) {
      if (i + 1 < argc) {
        options.timer_type = argv[i + 1];
        i++;
      } else {
        printf("Warning: Invalid --timer. Using default.\n");
      }
    } else if (strcmp(argv[i], "--io") == 0) {
      options.enable_io = true;
    } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_help();
    } else {
      printf("Warning: Unknown option ignored\n");
    }
  }

  if (options.enable_io == true)
    for (int i = 1; i < argc; ++i) {
      if (strcmp(argv[i], "--algorithm") == 0 ||
          strcmp(argv[i], "--timer") == 0) {
        printf("Warning: I/O mode doesn't use --algorithm \n");
        break;
      }
    }
  return options;
}
