#ifndef MANAGER_H
#define MANAGER_H

#include "measurements.h"

typedef struct all_MeasuredInfo {
  MeasuredInfo global_info;
  MeasuredInfo *thread_info_array;
  int array_size;
} all_MeasuredInfo;

typedef enum execution_type { PRIME, SORT, MATRIX, IO } execution_type;

void print_info(MeasuredInfo info);
void print_all_info(all_MeasuredInfo all_info);

all_MeasuredInfo start_execution(execution_type type, int num_threads,
                                 int num_iterations, int measurement_type);

#endif
