#include "algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool is_prime(unsigned int n) {
  if (n < 2)
    return false;
  for (unsigned int i = 2; i < n; i++) {
    if (n % i == 0)
      return false;
  }
  return true;
}
void check_primes(unsigned int array[], int array_start, int array_end) {
  for (int k = array_start; k < array_end; k++) {
    if (is_prime(array[k]) == true) {
      for (int j = 2 * array[k] - 2; j < array_end;
           j += array[k]) { // array_end e doar la subarray
        array[j] = 0;
      }
    } else {
      array[k] = 0;
    }
  }
}

void merge(unsigned int a[], int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  unsigned int *L = (unsigned int *)malloc(n1 * sizeof(unsigned int));
  if (L == NULL) {
    exit(1);
  }
  unsigned int *R = (unsigned int *)malloc(n2 * sizeof(unsigned int));
  if (R == NULL) {
    free(L);
    exit(1);
  }

  for (int i = 0; i < n1; i++)
    L[i] = a[left + i];
  for (int j = 0; j < n2; j++)
    R[j] = a[mid + 1 + j];

  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      a[k++] = L[i++];
    } else {
      a[k++] = R[j++];
    }
  }

  while (i < n1)
    a[k++] = L[i++];
  while (j < n2)
    a[k++] = R[j++];

  free(L);
  free(R);
}

void mergesort(unsigned int a[], int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    mergesort(a, left, mid);
    mergesort(a, mid + 1, right);
    merge(a, left, mid, right);
  }
}

#include <limits.h>

void merge_sorted_subarrays(unsigned int a[], int subarray_starts[],
                            int subarray_ends[], int num_parts) {
  int total_size = subarray_ends[num_parts - 1] - subarray_starts[0] + 1;
  unsigned int *result =
      (unsigned int *)malloc(total_size * sizeof(unsigned int));
  if (result == NULL)
    exit(1);

  for (int k = 0; k < total_size; k++) {
    int min_part = -1;
    unsigned int min_val = UINT_MAX;

    for (int i = 0; i < num_parts; i++) {
      if (subarray_starts[i] <= subarray_ends[i]) {
        unsigned int val = a[subarray_starts[i]];
        if (min_part == -1 || val < min_val) {
          min_part = i;
          min_val = val;
        }
      }
    }

    result[k] = min_val;
    if (min_part != -1) {
      subarray_starts[min_part]++;
    }
  }

  for (int i = 0; i < total_size; i++)
    a[i] = result[i];

  free(result);
}

void generate_random_array(unsigned int a[], int n, int max_val) {
  for (int i = 0; i < n; i++) {
    a[i] = (unsigned int)(rand() % max_val);
  }
}
void shuffle_array(unsigned int arr[], int n) {
  srand((unsigned)time(NULL));

  for (int i = n - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    unsigned int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

void matrix(int m, int n, int p) {
  srand((unsigned)time(NULL));

  unsigned int *matrix_a = (unsigned int *)malloc(m * n * sizeof(unsigned int));
  unsigned int *matrix_b = (unsigned int *)malloc(n * p * sizeof(unsigned int));
  unsigned int *matrix_c = (unsigned int *)calloc(m * p, sizeof(unsigned int));

  if (matrix_a == NULL || matrix_b == NULL || matrix_c == NULL)
    exit(1);

  generate_random_array(matrix_a, m * n, 10);
  generate_random_array(matrix_b, n * p, 10);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      for (int k = 0; k < n; k++) {
        matrix_c[i * p + j] += matrix_a[i * n + k] * matrix_b[k * p + j];
      }
    }
  }

  free(matrix_a);
  free(matrix_b);
  free(matrix_c);
}

void print_matrix(unsigned int *matrix, int m, int p) {
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++)
      printf("%u ", matrix[i * p + j]);
    printf("\n");
  }
}

void compute_matrix_row(unsigned int *matrix_a, unsigned int *matrix_b,
                        unsigned int *matrix_c, int row_index, int n, int p) {
  for (int j = 0; j < p; j++) {
    unsigned int sum = 0;
    for (int k = 0; k < n; k++) {
      sum += matrix_a[row_index * n + k] * matrix_b[k * p + j];
    }
    matrix_c[row_index * p + j] = sum;
  }
}
