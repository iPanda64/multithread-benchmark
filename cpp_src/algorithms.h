#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#define MAX 100000

void generate_random_array(unsigned int a[], int n, int max_val = MAX);
void compute_matrix_row(unsigned int *matrix_a, unsigned int *matrix_b,
                        unsigned int *matrix_c, int row_index, int n, int p);
void mergesort(unsigned int a[], int left, int right);
void merge_sorted_subarrays(unsigned int a[], int subarray_starts[],
                            int subarray_ends[], int num_parts);
void print_matrix(unsigned int *matrix, int m, int p);
void shuffle_array(unsigned int arr[], int n);

bool is_prime(unsigned int n);

#endif
