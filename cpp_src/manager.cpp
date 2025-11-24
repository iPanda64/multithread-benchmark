#include "manager.h"
#include "algorithms.h"
#include "io.h"
#include "measurements.h"
#include "threads.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

CRITICAL_SECTION critical_section;

void print_info(MeasuredInfo info) {
  if (info.thread_index == GLOBAL_MEASUREMENT_INDEX) {
    printf("Global measurement:\n");
  } else {
    printf("Thread %d:\n", info.thread_index);
  }
  printf("  Elapsed cycles: %lld\n", info.elapsed_cycles);
  printf("  Elapsed seconds: %f\n", info.elapsed_sec);
}

void print_all_info(all_MeasuredInfo all_info) {
  print_info(all_info.global_info);
  for (int i = 0; i < all_info.array_size; i++) {
    print_info(all_info.thread_info_array[i]);
  }
}

void accumulate_measured_info(all_MeasuredInfo *accumulator,
                              all_MeasuredInfo new_info) {
  if (accumulator->array_size != new_info.array_size) {
    printf("Error: Array sizes do not match in accumulate_measured_info.\n");
    exit(1);
  }

  accumulator->global_info.elapsed_cycles +=
      new_info.global_info.elapsed_cycles;
  accumulator->global_info.elapsed_sec += new_info.global_info.elapsed_sec;

  for (int i = 0; i < accumulator->array_size; i++) {
    accumulator->thread_info_array[i].elapsed_cycles +=
        new_info.thread_info_array[i].elapsed_cycles;
    accumulator->thread_info_array[i].elapsed_sec +=
        new_info.thread_info_array[i].elapsed_sec;
  }
}
all_MeasuredInfo io_execution(int num_threads,
                              int measurement_type = CLOCK_MEASUREMENT) {
  int chunk_size = NUMBER_OF_FILES / num_threads;
  HANDLE *hThread = (HANDLE *)malloc(num_threads * sizeof(HANDLE));
  if (hThread == NULL) {
    printf("Error: Memmory alocation of \"hThread\" failed\n");
    exit(1);
  }

  IOData **pData = (IOData **)malloc(num_threads * sizeof(IOData *));
  if (pData == NULL) {
    printf("Error: Memmory alocation of \"pData\" failed\n");
    exit(1);
  }

  MeasurementState globalState;
  start_timer(measurement_type, &globalState);

  for (int i = 0; i < num_threads; i++) {
    pData[i] = (IOData *)calloc(1, sizeof(IOData));
    if (pData[i] == NULL) {
      printf("Error: Memmory alocation of \"pData[i]\" failed\n");
      exit(1);
    }
    pData[i]->start_file_index = i * chunk_size + 1;
    if (i == num_threads - 1) {
      pData[i]->end_file_index = NUMBER_OF_FILES;
    } else {
      pData[i]->end_file_index = (i + 1) * chunk_size;
    }
    pData[i]->thread_index = i;
    pData[i]->measurement_type = 1;

    hThread[i] = CreateThread(NULL, 0, IOThreadFunction, pData[i], 0, NULL);
  }
  WaitForMultipleObjects(num_threads, hThread, TRUE, INFINITE);
  MeasuredInfo globalInfo =
      end_timer(measurement_type, &globalState, GLOBAL_MEASUREMENT_INDEX);
  printf("Successfully read 128 files of 1MB each\n");
  all_MeasuredInfo all_info;
  all_info.global_info = globalInfo;
  all_info.array_size = num_threads;
  all_info.thread_info_array =
      (MeasuredInfo *)malloc(num_threads * sizeof(MeasuredInfo));
  if (all_info.thread_info_array == NULL) {
    printf("Error: Memmory alocation of \"all_info.thread_info_array\" failed\n");
    exit(1);
  }

  for (int i = 0; i < num_threads; i++) {
    all_info.thread_info_array[i] = pData[i]->info;
    CloseHandle(hThread[i]);
    free(pData[i]);
  }

  free(pData);
  free(hThread);

  return all_info;
}
all_MeasuredInfo prime_execution(int num_threads,
                                 int measurement_type = CLOCK_MEASUREMENT) {
  const int array_size = 100000;

  unsigned int *array =
      (unsigned int *)malloc(sizeof(unsigned int) * array_size);
  if (array == NULL) {
    printf("Error: Memmory alocation of \"array\" failed\n");
    exit(1);
  }
  for (int i = 0; i < array_size; ++i) {
    array[i] = i;
  }

  unsigned int *index_array =
      (unsigned int *)malloc(sizeof(unsigned int) * array_size);
  if (index_array == NULL) {
    printf("Error: Memmory alocation of \"index_array\" failed\n");
    exit(1);
  }
  for (int i = 0; i < array_size; ++i) {
    index_array[i] = i;
  }

  shuffle_array(index_array, array_size);

  HANDLE *hThreadArray = (HANDLE *)malloc(num_threads * sizeof(HANDLE));
  if (hThreadArray == NULL) {
    printf("Error: Memmory alocation of \"hThreadArray\" failed\n");
    exit(1);
  }

  PrimeData **pPrimeData =
      (PrimeData **)malloc(sizeof(PrimeData *) * num_threads);
  if (pPrimeData == NULL) {
    printf("Error: Memmory alocation of \"pPrimeData\" failed\n");
    exit(1);
  }

  InitializeCriticalSection(&critical_section);

  MeasurementState globalState;
  start_timer(measurement_type, &globalState);

  int chunk_size = array_size / num_threads;

  for (int i = 0; i < num_threads; ++i) {
    pPrimeData[i] = (PrimeData *)calloc(1, sizeof(PrimeData));
    if (pPrimeData[i] == NULL) {
      printf("Error: Memmory alocation of \"pPrimeData[i]\" failed\n");
      exit(1);
    }

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

  printf("Successfully calculated prime numbers up to %d\n", array_size);

  MeasuredInfo globalInfo =
      end_timer(measurement_type, &globalState, GLOBAL_MEASUREMENT_INDEX);

  all_MeasuredInfo all_info;
  all_info.global_info = globalInfo;
  all_info.array_size = num_threads;
  all_info.thread_info_array =
      (MeasuredInfo *)malloc(num_threads * sizeof(MeasuredInfo));
  if (all_info.thread_info_array == NULL) {
    printf("Error: Memmory alocation of \"all_info.thread_info_array\" failed\n");
    exit(1);
  }

  for (int i = 0; i < num_threads; i++) {
    all_info.thread_info_array[i] = pPrimeData[i]->info;
    CloseHandle(hThreadArray[i]);
    free(pPrimeData[i]);
  }

  DeleteCriticalSection(&critical_section);
  free(hThreadArray);
  free(array);
  free(index_array);
  free(pPrimeData);

  return all_info;
}

all_MeasuredInfo matrix_execution(int num_threads,
                                  int measurement_type = CLOCK_MEASUREMENT) {
  const int m = num_threads, n = 10000 / num_threads, p = 8000;
  unsigned int *matrix_a = (unsigned int *)malloc(m * n * sizeof(unsigned int));
  if (matrix_a == NULL) {
    printf("Error: Memmory alocation of \"matrix_a\" failed\n");
    exit(1);
  }
  unsigned int *matrix_b = (unsigned int *)malloc(n * p * sizeof(unsigned int));
  if (matrix_b == NULL) {
    printf("Error: Memmory alocation of \"matrix_b\" failed\n");
    exit(1);
  }
  unsigned int *matrix_c = (unsigned int *)calloc(m * p, sizeof(unsigned int));
  if (matrix_c == NULL) {
    printf("Error: Memmory alocation of \"matrix_c\" failed\n");
    exit(1);
  }

  srand((unsigned)time(NULL));
  generate_random_array(matrix_a, m * n);
  generate_random_array(matrix_b, n * p);

  //  printf("Matrix A:\n");
  //  print_matrix(matrix_a, m, n);
  //  printf("\nMatrix B:\n");
  //  print_matrix(matrix_b, n, p);

  HANDLE *hThreadArray = (HANDLE *)malloc(num_threads * sizeof(HANDLE));
  if (hThreadArray == NULL) {
    printf("Error: Memmory alocation of \"hThreadArray\" failed\n");
    exit(1);
  }

  MatrixRowData **pDataArray =
      (MatrixRowData **)malloc(num_threads * sizeof(MatrixRowData *));
  if (pDataArray == NULL) {
    printf("Error: Memmory alocation of \"pDataArray\" failed\n");
    exit(1);
  }

  MeasurementState globalState;
  start_timer(measurement_type, &globalState);

  for (int i = 0; i < num_threads; i++) {
    pDataArray[i] = (MatrixRowData *)calloc(1, sizeof(MatrixRowData));
    if (pDataArray[i] == NULL) {
      printf("Error: Memmory alocation of \"pDataArray[i]\" failed\n");
      exit(1);
    }
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

  WaitForMultipleObjects(num_threads, hThreadArray, TRUE, INFINITE);

  printf("Succesfully multiplied 2 matrixes\n");
  //  print_matrix(matrix_c, m, p);

  MeasuredInfo globalInfo =
      end_timer(measurement_type, &globalState, GLOBAL_MEASUREMENT_INDEX);

  all_MeasuredInfo all_info;
  all_info.global_info = globalInfo;
  all_info.array_size = num_threads;
  all_info.thread_info_array =
      (MeasuredInfo *)malloc(num_threads * sizeof(MeasuredInfo));
  if (all_info.thread_info_array == NULL) {
    printf("Error: Memmory alocation of \"all_info.thread_info_array\" failed\n");
    exit(1);
  }

  for (int i = 0; i < num_threads; i++) {
    all_info.thread_info_array[i] = pDataArray[i]->info;
    CloseHandle(hThreadArray[i]);
    free(pDataArray[i]);
  }

  free(matrix_a);
  free(matrix_b);
  free(matrix_c);
  free(hThreadArray);
  free(pDataArray);

  return all_info;
}

all_MeasuredInfo mergesort_execution(int num_threads,
                                     int measurement_type = CLOCK_MEASUREMENT) {
  const int array_size = 2000000;

  unsigned int *array =
      (unsigned int *)malloc(array_size * sizeof(unsigned int));
  if (array == NULL) {
    printf("Error: Memmory alocation of \"array\" failed\n");
    exit(1);
  }

  srand((unsigned)time(NULL));
  generate_random_array(array, array_size);

  HANDLE *hThreadArray = (HANDLE *)malloc(num_threads * sizeof(HANDLE));
  if (hThreadArray == NULL) {
    printf("Error: Memmory alocation of \"hThreadArray\" failed\n");
    exit(1);
  }

  SortArrayData **pDataArray =
      (SortArrayData **)malloc(num_threads * sizeof(SortArrayData *));
  if (pDataArray == NULL) {
    printf("Error: Memmory alocation of \"pDataArray\" failed\n");
    exit(1);
  }

  int *subarray_starts = (int *)malloc(num_threads * sizeof(int));
  int *subarray_ends = (int *)malloc(num_threads * sizeof(int));
  int chunk_size = array_size / num_threads;

  MeasurementState globalState;
  start_timer(measurement_type, &globalState);

  for (int i = 0; i < num_threads; i++) {
    pDataArray[i] = (SortArrayData *)calloc(1, sizeof(SortArrayData));
    if (pDataArray[i] == NULL) {
      printf("Error: Memmory alocation of \"pDataArray[i]\" failed\n");
      exit(1);
    }
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

  merge_sorted_subarrays(array, subarray_starts, subarray_ends, num_threads);

  printf("Successfully sorted array\n");

  MeasuredInfo globalInfo =
      end_timer(measurement_type, &globalState, GLOBAL_MEASUREMENT_INDEX);

  all_MeasuredInfo all_info;
  all_info.global_info = globalInfo;
  all_info.array_size = num_threads;
  all_info.thread_info_array =
      (MeasuredInfo *)malloc(num_threads * sizeof(MeasuredInfo));
  if (all_info.thread_info_array == NULL) {
    printf("Error: Memmory alocation of \"all_info.thread_info_array\" failed\n");
    exit(1);
  }

  for (int i = 0; i < num_threads; i++) {
    all_info.thread_info_array[i] = pDataArray[i]->info;
    CloseHandle(hThreadArray[i]);
    free(pDataArray[i]);
  }

  free(array);
  free(hThreadArray);
  free(pDataArray);
  free(subarray_starts);
  free(subarray_ends);

  return all_info;
}

all_MeasuredInfo start_execution(execution_type type, int num_threads,
                                 int num_iterations,
                                 int measurement_type = CLOCK_MEASUREMENT) {
  all_MeasuredInfo total_sum;
  MeasuredInfo *total_sum_array =
      (MeasuredInfo *)malloc(num_threads * sizeof(MeasuredInfo));
  if (!total_sum_array) {
    printf("Error: Memmory alocation of \"total_sum_array\" failed\n");
    exit(1);
  }
  total_sum.array_size = num_threads;
  total_sum.thread_info_array = total_sum_array;
  total_sum.global_info.elapsed_cycles = 0;
  total_sum.global_info.elapsed_sec = 0;
  total_sum.global_info.thread_index = GLOBAL_MEASUREMENT_INDEX;
  for (int i = 0; i < num_threads; ++i) {
    total_sum.thread_info_array[i].thread_index = i;
    total_sum.thread_info_array[i].elapsed_cycles = 0;
    total_sum.thread_info_array[i].elapsed_sec = 0;
  }

  for (int i = 0; i < num_iterations; i++) {
    all_MeasuredInfo result;
    switch (type) {
    case PRIME:
      result = prime_execution(num_threads, measurement_type);
      break;
    case IO:
      result = io_execution(num_threads, measurement_type);
      break;
    case MATRIX:
      result = matrix_execution(num_threads, measurement_type);
      break;
    case SORT:
      result = mergesort_execution(num_threads, measurement_type);
      break;
    default:
      printf("Execution type not implemented\n");
      total_sum.array_size = 0;
      free(total_sum_array);
      return total_sum;
    }
    accumulate_measured_info(&total_sum, result);
    free(result.thread_info_array);
  }
  total_sum.global_info.elapsed_cycles /= num_iterations;
  total_sum.global_info.elapsed_sec /= num_iterations;

  for (int i = 0; i < total_sum.array_size; i++) {
    total_sum.thread_info_array[i].elapsed_cycles /= num_iterations;
    total_sum.thread_info_array[i].elapsed_sec /= num_iterations;
  }

  return total_sum;
}
