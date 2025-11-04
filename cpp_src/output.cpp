#include "output.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void write_json_output(all_MeasuredInfo info) {
  char exe_path[1024];
  char final_path[1024];

  DWORD len = GetModuleFileNameA(NULL, exe_path, sizeof(exe_path));
  if (len == 0 || len >= sizeof(exe_path)) {
    printf("Error: GetModuleFileNameA failed or buffer too small\n");
    return;
  }
  char *dir_end = strrchr(exe_path, '\\');
  if (dir_end == NULL) {
    dir_end = strrchr(exe_path, '/');
  }

  if (dir_end == NULL) {
    printf("Error: Could not find directory from executable path\n");
    return;
  }

  *(dir_end) = '\0';

  snprintf(final_path, sizeof(final_path), "%s\\output.json", exe_path);

  FILE *f = fopen(final_path, "w");
  if (f == NULL) {
    printf("Error: Error opening file: %s\n", final_path);
    return;
  }

  fprintf(f, "{\n");
  fprintf(f, "  \"global_info\": {\n");
  fprintf(f, "    \"elapsed_cycles\": %lld,\n",
          info.global_info.elapsed_cycles);
  fprintf(f, "    \"elapsed_sec\": %f\n", info.global_info.elapsed_sec);
  fprintf(f, "  },\n");
  fprintf(f, "  \"thread_info\": [\n");

  for (int i = 0; i < info.array_size; i++) {
    fprintf(f, "    {\n");
    fprintf(f, "      \"thread_index\": %d,\n",
            info.thread_info_array[i].thread_index);
    fprintf(f, "      \"elapsed_cycles\": %lld,\n",
            info.thread_info_array[i].elapsed_cycles);
    fprintf(f, "      \"elapsed_sec\": %f\n",
            info.thread_info_array[i].elapsed_sec);
    fprintf(f, "    }");
    if (i < info.array_size - 1) {
      fprintf(f, ",\n");
    } else {
      fprintf(f, "\n");
    }
  }

  fprintf(f, "  ]\n");
  fprintf(f, "}\n");

  fclose(f);
  printf("----------------------------\n");
  printf("Successfully wrote output to:\n %s\n", final_path);
}
