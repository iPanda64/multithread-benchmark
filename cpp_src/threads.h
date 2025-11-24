#ifndef THREADS_H
#define THREADS_H
#include "measurements.h"
#include <windows.h>

extern CRITICAL_SECTION critical_section;

typedef struct IOData {
  int thread_index;
  int measurement_type;
  MeasuredInfo info;
  int start_file_index;
  int end_file_index;
} IOData;

typedef struct MatrixRowData {
  int thread_index;
  int measurement_type;
  MeasuredInfo info;
  int row_index;
  int n;
  int p;
  unsigned int *matrix_a;
  unsigned int *matrix_b;
  unsigned int *matrix_c;
} MatrixRowData;

typedef struct SortArrayData {
  int thread_index;
  int measurement_type;
  MeasuredInfo info;
  unsigned int *array;
  int left;
  int right;
} SortArrayData;

typedef struct PrimeData {
  int thread_index;
  int measurement_type;
  MeasuredInfo info;
  unsigned int *array;
  unsigned int *index_array;
  int array_size;
  int array_start;
  int array_end;
} PrimeData;

DWORD WINAPI IOThreadFunction(LPVOID lpParam);
DWORD WINAPI MatrixThreadFunction(LPVOID lpParam);
DWORD WINAPI PrimeThreadFunction(LPVOID lpParam);
DWORD WINAPI SortThreadFunction(LPVOID lpParam);

void start_timer(int measurement_type, MeasurementState *state);
MeasuredInfo end_timer(int measurement_type, MeasurementState *state, int thread_index);
#endif
