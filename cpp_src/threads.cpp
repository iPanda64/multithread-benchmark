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

  if (measurement_type = CLOCK_MEASUREMENT)
    clock_start(state);
  else
    rdtsc_start(state);
}

MeasuredInfo end_timer(int measurement_type, MeasurementState *state,
                       int thread_index) {
  MeasuredInfo info;
  if (measurement_type = CLOCK_MEASUREMENT)
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

// --- Example Functions ---
void io_example() {
  const int num_threads = 4;
  int chunk_size = NUMBER_OF_FILES / num_threads;
  HANDLE *hThread = (HANDLE *)malloc(num_threads * sizeof(HANDLE));
  if (hThread == NULL)
    exit(1);

  IOData **pData = (IOData **)malloc(num_threads * sizeof(IOData *));
  if (pData == NULL)
    exit(1);

  for (int i = 0; i < num_threads; i++) {
    pData[i] = (IOData *)calloc(1, sizeof(IOData));
    if (pData[i] == NULL)
      exit(1);
    pData[i]->start_file_index = i * chunk_size;
    if (i == num_threads - 1) {
      pData[i]->end_file_index = NUMBER_OF_FILES - 1;
    } else {
      pData[i]->end_file_index = (i + 1) * chunk_size - 1;
    }
    pData[i]->thread_index = i;
    pData[i]->measurement_type = 1;

    hThread[i] = CreateThread(NULL, 0, IOThreadFunction, pData[i], 0, NULL);
  }
  WaitForMultipleObjects(num_threads, hThread, TRUE, INFINITE);

  for (int i = 0; i < num_threads; i++) {
    CloseHandle(hThread[i]);
    free(pData[i]);
  }
  free(pData);
  free(hThread);
}
void matrix_example() {
  const int m = 4, n = 300, p = 400;
  unsigned int *matrix_a = (unsigned int *)malloc(m * n * sizeof(unsigned int));
  if (matrix_a == NULL)
    exit(1);
  unsigned int *matrix_b = (unsigned int *)malloc(n * p * sizeof(unsigned int));
  if (matrix_b == NULL)
    exit(1);
  unsigned int *matrix_c = (unsigned int *)calloc(m * p, sizeof(unsigned int));
  if (matrix_c == NULL)
    exit(1);

  srand((unsigned)time(NULL));
  generate_random_array(matrix_a, m * n);
  generate_random_array(matrix_b, n * p);

  printf("Matrix A:\n");
  print_matrix(matrix_a, m, n);
  printf("\nMatrix B:\n");
  print_matrix(matrix_b, n, p);

  HANDLE *hThreadArray = (HANDLE *)malloc(m * sizeof(HANDLE));
  if (hThreadArray == NULL)
    exit(1);

  MatrixRowData **pDataArray =
      (MatrixRowData **)malloc(m * sizeof(MatrixRowData *));
  if (pDataArray == NULL)
    exit(1);

  for (int i = 0; i < m; i++) {
    pDataArray[i] = (MatrixRowData *)calloc(1, sizeof(MatrixRowData));
    if (pDataArray[i] == NULL)
      exit(1);
    pDataArray[i]->thread_index = i;
    pDataArray[i]->measurement_type = 1;
    pDataArray[i]->row_index = i;
    pDataArray[i]->n = n;
    pDataArray[i]->p = p;
    pDataArray[i]->matrix_a = matrix_a;
    pDataArray[i]->matrix_b = matrix_b;
    pDataArray[i]->matrix_c = matrix_c;
    hThreadArray[i] =
        CreateThread(NULL, 0, MatrixThreadFunction, pDataArray[i], 0, NULL);
  }

  WaitForMultipleObjects(m, hThreadArray, TRUE, INFINITE);

  for (int i = 0; i < m; i++) {
    CloseHandle(hThreadArray[i]);
    free(pDataArray[i]);
  }

  printf("\nResult Matrix C:\n");
  print_matrix(matrix_c, m, p);

  free(matrix_a);
  free(matrix_b);
  free(matrix_c);
  free(hThreadArray);
  free(pDataArray);
}

void mergesort_example() {
  const int array_size = 10;
  const int num_threads = 4;

  unsigned int *array =
      (unsigned int *)malloc(array_size * sizeof(unsigned int));
  if (array == NULL)
    exit(1);

  srand((unsigned)time(NULL));
  generate_random_array(array, array_size);

  printf("Random array:\n");
  for (int i = 0; i < array_size; ++i)
    printf("%u, ", array[i]);
  HANDLE *hThreadArray = (HANDLE *)malloc(num_threads * sizeof(HANDLE));
  if (hThreadArray == NULL)
    exit(1);

  SortArrayData **pDataArray =
      (SortArrayData **)malloc(num_threads * sizeof(SortArrayData *));
  if (pDataArray == NULL)
    exit(1);

  int *subarray_starts = (int *)malloc(num_threads * sizeof(int));
  int *subarray_ends = (int *)malloc(num_threads * sizeof(int));
  int chunk_size = array_size / num_threads;

  for (int i = 0; i < num_threads; i++) {
    pDataArray[i] = (SortArrayData *)calloc(1, sizeof(SortArrayData));
    if (pDataArray[i] == NULL)
      exit(1);
    subarray_starts[i] = i * chunk_size;
    if (i == num_threads - 1) {
      subarray_ends[i] = array_size - 1;
    } else {
      subarray_ends[i] = (i + 1) * chunk_size - 1;
    }
    pDataArray[i]->array = array;
    pDataArray[i]->left = subarray_starts[i];
    pDataArray[i]->right = subarray_ends[i];
    pDataArray[i]->thread_index = i;
    pDataArray[i]->measurement_type = 1;

    hThreadArray[i] =
        CreateThread(NULL, 0, SortThreadFunction, pDataArray[i], 0, NULL);
  }

  WaitForMultipleObjects(num_threads, hThreadArray, TRUE, INFINITE);

  for (int i = 0; i < num_threads; i++) {
    CloseHandle(hThreadArray[i]);
    free(pDataArray[i]);
  }

  merge_sorted_subarrays(array, subarray_starts, subarray_ends, num_threads);

  printf("Sorted array:\n");
  for (int i = 0; i < array_size; ++i)
    printf("%u, ", array[i]);

  free(array);
  free(hThreadArray);
  free(pDataArray);
  free(subarray_starts);
  free(subarray_ends);
}

void prime_example() {
  const int array_size = 1000;
  const int num_threads = 4;

  unsigned int *array =
      (unsigned int *)malloc(sizeof(unsigned int) * array_size);
  if (array == NULL)
    exit(1);
  for (int i = 0; i < array_size; ++i) {
    array[i] = i;
  }

  unsigned int *index_array =
      (unsigned int *)malloc(sizeof(unsigned int) * array_size);
  if (index_array == NULL)
    exit(1);
  for (int i = 0; i < array_size; ++i) {
    index_array[i] = i;
  }

  shuffle_array(index_array, array_size);

  HANDLE *hThreadArray = (HANDLE *)malloc(num_threads * sizeof(HANDLE));
  if (hThreadArray == NULL)
    exit(1);

  PrimeData **pPrimeData =
      (PrimeData **)malloc(sizeof(PrimeData *) * num_threads);
  if (pPrimeData == NULL)
    exit(1);

  InitializeCriticalSection(&critical_section);

  int chunk_size = array_size / num_threads;

  for (int i = 0; i < num_threads; ++i) {
    pPrimeData[i] = (PrimeData *)calloc(1, sizeof(PrimeData));
    if (pPrimeData[i] == NULL)
      exit(1);

    int start_index = i * chunk_size;
    int end_index = (i == num_threads - 1) ? array_size : (i + 1) * chunk_size;

    pPrimeData[i]->array = array;
    pPrimeData[i]->index_array = index_array;
    pPrimeData[i]->array_start = start_index;
    pPrimeData[i]->array_end = end_index;
    pPrimeData[i]->array_size = array_size;
    pPrimeData[i]->thread_index = i;
    pPrimeData[i]->measurement_type = 1;

    hThreadArray[i] =
        CreateThread(NULL, 0, PrimeThreadFunction, pPrimeData[i], 0, NULL);
  }

  WaitForMultipleObjects(num_threads, hThreadArray, TRUE, INFINITE);

  for (int i = 0; i < num_threads; i++) {
    CloseHandle(hThreadArray[i]);
    free(pPrimeData[i]);
  }

  printf("Prime numbers found:\n");
  for (int i = 2; i < array_size; ++i) {
    if (array[i] != 0) {
      printf("%u ", array[i]);
    }
  }
  printf("\n");

  DeleteCriticalSection(&critical_section);
  free(hThreadArray);
  free(array);
  free(index_array);
  free(pPrimeData);
}

// int main() {
//   // matrix_example();
//   // mergesort_example();
//   // prime_example();
//   io_example();
// }
