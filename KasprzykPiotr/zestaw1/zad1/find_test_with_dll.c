#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include <dlfcn.h>
#include "find.h"

void print_time(double time) {
    int minutes = (int) (time / 60);
    double seconds = time - minutes * 60;
    printf("%dm %.9fs\n", minutes, seconds);
}

double count_time_range(struct timeval end_time, struct timeval start_time) {
    double tmp_end_time = ((double) end_time.tv_sec) + (((double) end_time.tv_usec) / 1000000);
    double tmp_start_time = ((double) start_time.tv_sec) + (((double) start_time.tv_usec) / 1000000);
    return tmp_end_time - tmp_start_time;
}

void print_function_execution_time(char* function_name, clock_t real_start, clock_t real_end, struct timeval sys_start, struct timeval sys_end, struct timeval user_start, struct timeval user_end) {
    printf("%s function execution time:\n", function_name);
    printf("real:\t");
    print_time(((double) real_end - real_start) / CLOCKS_PER_SEC);
    printf("system:\t");
    print_time(count_time_range(sys_end, sys_start));
    printf("user:\t");
    print_time(count_time_range(user_end, user_start));
    printf("\n");
}

int main(int argc, char **argv) {

    void* bib_handle = dlopen("./libfind.so", RTLD_LAZY);
    if(!bib_handle)
    {
        fprintf(stderr, "error while loading library:\n");
        fprintf(stderr, "%s\n", dlerror());
        exit(1);
    }

    void (*d_create_blocks_array)(blocks_array*, int);
    d_create_blocks_array = dlsym(bib_handle, "create_blocks_array");

    void (*d_save_find_result_to_temp_file)(char*, char*, char*);
    d_save_find_result_to_temp_file = dlsym(bib_handle, "save_find_result_to_temp_file");

    void (*d_remove_block)(blocks_array*, int);
    d_remove_block = dlsym(bib_handle, "remove_block");

    void (*d_save_temp_file_to_block)(blocks_array*, char*);
    d_save_temp_file_to_block = dlsym(bib_handle, "save_temp_file_to_block");

    void (*d_delete_blocks_array)(blocks_array*);
    d_delete_blocks_array = dlsym(bib_handle, "delete_blocks_array");

    if (argc < 2) {
        fprintf(stderr, "wrong input format\n");
        exit(1);
    }

    blocks_array array;
    int array_size;
    clock_t real_start, real_end;
    struct rusage ru_start, ru_end;
    struct timeval sys_start, sys_end, user_start, user_end;

    if (strcmp(argv[1], "create_table") == 0) {
        array_size = atoi(argv[2]);

        real_start = clock();
        getrusage(RUSAGE_SELF, &ru_start);
        d_create_blocks_array(&array, array_size);
        real_end = clock();
        getrusage(RUSAGE_SELF, &ru_end);
        sys_start = ru_start.ru_stime;
        user_start = ru_start.ru_utime;
        sys_end = ru_end.ru_stime;
        user_end = ru_end.ru_utime;
        print_function_execution_time("create_table", real_start, real_end, sys_start, sys_end, user_start, user_end);

    } else {
        fprintf(stderr, "cannot perform any further operations on uninitialized table\n");
        exit(1);
    }

    int i = 3;
    while (i < argc) {
        char *buffer = argv[i];

        if (strcmp(argv[i], "search_directory") == 0) {
            char* directory_path = argv[i+1];
            char* file_name = argv[i+2];
            char* temp_file_name = argv[i+3];

            real_start = clock();
            getrusage(RUSAGE_SELF, &ru_start);
            d_save_find_result_to_temp_file(directory_path, file_name, temp_file_name);
            real_end = clock();
            getrusage(RUSAGE_SELF, &ru_end);
            sys_start = ru_start.ru_stime;
            user_start = ru_start.ru_utime;
            sys_end = ru_end.ru_stime;
            user_end = ru_end.ru_utime;
            print_function_execution_time("save_find_result_to_temp_file", real_start, real_end, sys_start, sys_end, user_start, user_end);

            i += 4;
        } else if (strcmp(argv[i], "remove_block") == 0) {
            int index = atoi(argv[i+1]);

            real_start = clock();
            getrusage(RUSAGE_SELF, &ru_start);
            d_remove_block(&array, index);
            real_end = clock();
            getrusage(RUSAGE_SELF, &ru_end);
            sys_start = ru_start.ru_stime;
            user_start = ru_start.ru_utime;
            sys_end = ru_end.ru_stime;
            user_end = ru_end.ru_utime;
            print_function_execution_time("remove_block", real_start, real_end, sys_start, sys_end, user_start, user_end);

            i += 2;
        } else if (strcmp(argv[i], "save_temp_file_to_block") == 0) {
            char *temp_file_name = argv[i+1];

            real_start = clock();
            getrusage(RUSAGE_SELF, &ru_start);
            d_save_temp_file_to_block(&array, temp_file_name);
            real_end = clock();
            getrusage(RUSAGE_SELF, &ru_end);
            sys_start = ru_start.ru_stime;
            user_start = ru_start.ru_utime;
            sys_end = ru_end.ru_stime;
            user_end = ru_end.ru_utime;
            print_function_execution_time("save_temp_file_to_block", real_start, real_end, sys_start, sys_end, user_start, user_end);

            i += 2;
        }
    }

    real_start = clock();
    getrusage(RUSAGE_SELF, &ru_start);
    d_delete_blocks_array(&array);
    real_end = clock();
    getrusage(RUSAGE_SELF, &ru_end);
    sys_start = ru_start.ru_stime;
    user_start = ru_start.ru_utime;
    sys_end = ru_end.ru_stime;
    user_end = ru_end.ru_utime;
    print_function_execution_time("delete_blocks_array", real_start, real_end, sys_start, sys_end, user_start, user_end);

    dlclose(bib_handle);
    return 0;
}