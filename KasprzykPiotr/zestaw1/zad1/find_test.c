#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>
#include "find.h"

const char *report_file = "raport2.txt";

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

void log_print_time_to_file(FILE *file_pointer, double time) {
    int minutes = (int) (time / 60);
    double seconds = time - minutes * 60;
    fprintf(file_pointer, "%dm %.9fs\n", minutes, seconds);
}

void clear_file(char* file_name) {
    FILE *file_pointer;
    file_pointer = fopen(file_name, "w+");
    fprintf(file_pointer, "");
    fclose(file_pointer);
}

void log_function_execution_time_to_file(char* file_name, char* function_name, clock_t real_start, clock_t real_end, struct timeval sys_start, struct timeval sys_end, struct timeval user_start, struct timeval user_end) {

    FILE *file_pointer;
    file_pointer = fopen(file_name, "a");

    char file_content[100];
    stpcpy(file_content, function_name);
    strcat(file_content, " function execution time:\n");

    strcat(file_content, "real:\t");
    fprintf(file_pointer, file_content);
    log_print_time_to_file(file_pointer, ((double) real_end - real_start) / CLOCKS_PER_SEC);

    stpcpy(file_content, "system:\t");
    fprintf(file_pointer, file_content);
    log_print_time_to_file(file_pointer, count_time_range(sys_end, sys_start));

    stpcpy(file_content, "user:\t");
    fprintf(file_pointer, file_content);
    log_print_time_to_file(file_pointer, count_time_range(user_end, user_start));
    stpcpy(file_content, "\n");
    fprintf(file_pointer, file_content);

    fclose(file_pointer);
}

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "wrong input format\n");
        exit(1);
    }

    blocks_array array;
    int array_size;
    clock_t real_start, real_end;
    struct rusage ru_start, ru_end;
    struct timeval sys_start, sys_end, user_start, user_end;

    clear_file(report_file);
    if (strcmp(argv[1], "create_table") == 0) {
        array_size = atoi(argv[2]);

        real_start = clock();
        getrusage(RUSAGE_SELF, &ru_start);
        create_blocks_array(&array, array_size);
        real_end = clock();
        getrusage(RUSAGE_SELF, &ru_end);
        sys_start = ru_start.ru_stime;
        user_start = ru_start.ru_utime;
        sys_end = ru_end.ru_stime;
        user_end = ru_end.ru_utime;
        print_function_execution_time("create_table", real_start, real_end, sys_start, sys_end, user_start, user_end);
        log_function_execution_time_to_file(report_file, "create_table", real_start, real_end, sys_start, sys_end, user_start, user_end);

    } else {
        fprintf(stderr, "cannot perform any further operations on uninitialized table\n");
        exit(1);
    }

    int i = 3;
    while (i < argc) {

        if (strcmp(argv[i], "search_directory") == 0) {
            char* directory_path = argv[i+1];
            char* file_name = argv[i+2];
            char* temp_file_name = argv[i+3];

            real_start = clock();
            getrusage(RUSAGE_SELF, &ru_start);
            save_find_result_to_temp_file(directory_path, file_name, temp_file_name);
            real_end = clock();
            getrusage(RUSAGE_SELF, &ru_end);
            sys_start = ru_start.ru_stime;
            user_start = ru_start.ru_utime;
            sys_end = ru_end.ru_stime;
            user_end = ru_end.ru_utime;
            print_function_execution_time("save_find_result_to_temp_file", real_start, real_end, sys_start, sys_end, user_start, user_end);
            log_function_execution_time_to_file(report_file, "save_find_result_to_temp_file", real_start, real_end, sys_start, sys_end, user_start, user_end);

            i += 4;
        } else if (strcmp(argv[i], "remove_block") == 0) {
            int index = atoi(argv[i+1]);

            real_start = clock();
            getrusage(RUSAGE_SELF, &ru_start);
            remove_block(&array, index);
            real_end = clock();
            getrusage(RUSAGE_SELF, &ru_end);
            sys_start = ru_start.ru_stime;
            user_start = ru_start.ru_utime;
            sys_end = ru_end.ru_stime;
            user_end = ru_end.ru_utime;
            print_function_execution_time("remove_block", real_start, real_end, sys_start, sys_end, user_start, user_end);
            log_function_execution_time_to_file(report_file, "remove_block", real_start, real_end, sys_start, sys_end, user_start, user_end);

            i += 2;
        } else if (strcmp(argv[i], "save_temp_file_to_block") == 0) {
            char *temp_file_name = argv[i+1];

            real_start = clock();
            getrusage(RUSAGE_SELF, &ru_start);
            save_temp_file_to_block(&array, temp_file_name);
            real_end = clock();
            getrusage(RUSAGE_SELF, &ru_end);
            sys_start = ru_start.ru_stime;
            user_start = ru_start.ru_utime;
            sys_end = ru_end.ru_stime;
            user_end = ru_end.ru_utime;
            print_function_execution_time("save_temp_file_to_block", real_start, real_end, sys_start, sys_end, user_start, user_end);
            log_function_execution_time_to_file(report_file, "save_temp_file_to_block", real_start, real_end, sys_start, sys_end, user_start, user_end);

            i += 2;
        } else if (strcmp(argv[i], "repeat_save_to_block_and_remove") == 0) {
            char *temp_file_name = argv[i+1];
            int reps = atoi(argv[i+2]);

            printf("Repeated operations of saving temp. file to array of blocks will be done %d times:\n", reps);
            real_start = clock();
            getrusage(RUSAGE_SELF, &ru_start);
            while (reps > 0) {
                int index = save_temp_file_to_block(&array, temp_file_name);
                remove_block(&array, index);
                reps--;
            }
            real_end = clock();
            getrusage(RUSAGE_SELF, &ru_end);
            sys_start = ru_start.ru_stime;
            user_start = ru_start.ru_utime;
            sys_end = ru_end.ru_stime;
            user_end = ru_end.ru_utime;
            print_function_execution_time("repeat_save_to_block_and_remove", real_start, real_end, sys_start, sys_end, user_start, user_end);
            log_function_execution_time_to_file(report_file, "repeat_save_to_block_and_remove", real_start, real_end, sys_start, sys_end, user_start, user_end);

            i += 3;
        }
    }

    real_start = clock();
    getrusage(RUSAGE_SELF, &ru_start);
    delete_blocks_array(&array);
    real_end = clock();
    getrusage(RUSAGE_SELF, &ru_end);
    sys_start = ru_start.ru_stime;
    user_start = ru_start.ru_utime;
    sys_end = ru_end.ru_stime;
    user_end = ru_end.ru_utime;
    print_function_execution_time("delete_blocks_array", real_start, real_end, sys_start, sys_end, user_start, user_end);
    log_function_execution_time_to_file("report2.txt", "delete_blocks_array", real_start, real_end, sys_start, sys_end, user_start, user_end);

    return 0;
}
