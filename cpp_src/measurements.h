#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H
#include <time.h>

// TO DO: adjust to your CPU frequency (2.8 GHz is used as a default)
#define FREQUENCY 2800000000.0
#define INVALID_MEASUREMENT -2
#define GLOBAL_MEASUREMENT_INDEX -1
#define RDTSC_MEASUREMENT 0
#define CLOCK_MEASUREMENT 1

typedef struct MeasuredInfo {
  long long elapsed_cycles;
  double elapsed_sec;
  int thread_index;
} MeasuredInfo;

typedef struct {
  unsigned cycles_high1, cycles_low1;
  unsigned cycles_high2, cycles_low2;
  unsigned __int64 temp_cycles1, temp_cycles2;
  __int64 total_cycles;
  clock_t start, end;
} MeasurementState;

void measurement_state_init(MeasurementState *state);
void compute_cpuid_overhead();
void SetThreadAffinityForCore(int threadIndex);

void rdtsc_start(MeasurementState *state);
MeasuredInfo rdtsc_end(MeasurementState *state, int thread_index_result);

void clock_start(MeasurementState *state);
MeasuredInfo clock_end(MeasurementState *state, int thread_index_result);

#endif
