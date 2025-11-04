#ifndef INPUT_H
#define INPUT_H

struct ProgramOptions {
  int thread_count;
  const char *algorithm;
  const char *timer_type;
  bool enable_io;
};
void print_options(const ProgramOptions &options);
ProgramOptions input_management(int argc, char *argv[]);
#endif
