#include "threads.h"
#include "algorithms.h"
#include "io.h"
#include "measurements.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

void start_timer(int measurement_type, MeasurementState *state) {
  measurement_state_init(state);

  if (measurement_type == CLOCK_MEASUREMENT)
    clock_start(state);
  else
    rdtsc_start(state);
}

MeasuredInfo end_timer(int measurement_type, MeasurementState *state,
                       int thread_index) {
  MeasuredInfo info;
  if (measurement_type == CLOCK_MEASUREMENT)
    info = clock_end(state, thread_index);
  else
    info = rdtsc_end(state, thread_index);
  return info;
}

DWORD WINAPI IOThreadFunction(LPVOID lpParam) {
  IOData *data = (IOData *)lpParam;

  SetThreadAffinityForCore(data->thread_index);

  MeasurementState state;
  start_timer(data->measurement_type, &state);

  read_from_interval(data->start_file_index, data->end_file_index);

  data->info = end_timer(data->measurement_type, &state, data->thread_index);

  return 0;
}

DWORD WINAPI MatrixThreadFunction(LPVOID lpParam) {
  MatrixRowData *data = (MatrixRowData *)lpParam;
  SetThreadAffinityForCore(data->thread_index);

  MeasurementState state;
  start_timer(data->measurement_type, &state);

  compute_matrix_row(data->matrix_a, data->matrix_b, data->matrix_c,
                     data->row_index, data->n, data->p);

  data->info = end_timer(data->measurement_type, &state, data->thread_index);

  return 0;
}

DWORD WINAPI PrimeThreadFunction(LPVOID lpParam) {
  PrimeData *data = (PrimeData *)lpParam;
  SetThreadAffinityForCore(data->thread_index);

  MeasurementState state;
  start_timer(data->measurement_type, &state);

  unsigned int *array = data->array;
  unsigned int *index_array = data->index_array;

  for (int k = data->array_start; k < data->array_end; k++) {
    unsigned int current_value = array[index_array[k]];
    if (current_value != 0 && is_prime(current_value)) {
      for (unsigned int m = 2; m * current_value < data->array_size; ++m) {

        EnterCriticalSection(&critical_section);
        array[m * current_value] = 0;
        LeaveCriticalSection(&critical_section);
      }
    }
  }

  data->info = end_timer(data->measurement_type, &state, data->thread_index);

  return 0;
}

DWORD WINAPI SortThreadFunction(LPVOID lpParam) {
  SortArrayData *data = (SortArrayData *)lpParam;
  SetThreadAffinityForCore(data->thread_index);

  MeasurementState state;
  start_timer(data->measurement_type, &state);

  mergesort(data->array, data->left, data->right);

  data->info = end_timer(data->measurement_type, &state, data->thread_index);

  return 0;
}
