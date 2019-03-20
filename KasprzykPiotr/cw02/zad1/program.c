#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

/**
 * Populates given record with char's (a - z) in number of record_size_in_bytes
 * @param record is an array of chars that will be populated
 * @param record_size_in_bytes is a number of bytes that record is going to have
 */
void populate_random_record(char *record, int record_size_in_bytes) {
    int i;
    for (i = 0; i < record_size_in_bytes; i++) {
        record[i] = (char) ('a' + rand() % ('z' - 'a' + 1));
    }
}

/**
 * Creates file with records and randomly generated content
 * @param argc is a number of the program input arguments
 * @param argv contains an array of pointers to input arguments
 */
void generate(int argc, char **argv) {

    if (argc < 5) {
        fprintf(stderr, "wrong function input argument format\n");
        exit(-1);
    }

    char *file_name = argv[2];
    int records_number = atoi(argv[3]);
    int record_size = atoi(argv[4]);

    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        fprintf(stderr, "error while opening the file: %s\n", file_name);
        exit(-1);
    }

    char *record = malloc(record_size * sizeof(char));

    int i;
    for (i = 0; i < records_number; i++) {
        populate_random_record(record, record_size);

        if (fwrite(record, sizeof(char), record_size, file) != record_size) {
            fprintf(stderr, "error while writing records to file: %s\n", file_name);
            exit(-1);
        }
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "error while writing records into file: %s\n", file_name);
        exit(-1);
    }
}

/**
 * Copies one file to another
 * @param argc is a number of the program input arguments
 * @param argv contains an array of pointers to input arguments
 */
void copy(int argc, char **argv) {

    if (argc < 7) {
        fprintf(stderr, "wrong function input argument format\n");
        exit(-1);
    }

    char *file_name = argv[2];
    char *copy_file_name = argv[3];
    int records_number = atoi(argv[4]);
    int buffer_size = atoi(argv[5]);
    char *variant = argv[6];

    char *record_buffer = malloc(buffer_size * sizeof(char));

    if (strcmp(variant, "sys") == 0) {

        int fd = open(file_name, O_RDONLY);

        if (fd < 0) {
            fprintf(stderr, "error while opening the file: %s\n", file_name);
            exit(-1);
        }

        int copy_fd = open(copy_file_name, O_WRONLY | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);

        if (copy_fd < 0) {
            fprintf(stderr, "error while opening the copy if the file: %s\n", copy_file_name);
            exit(-1);
        }

        int i;
        // reading a record from a given file and writing a copy of that record to another file
        for (i = 0; i < records_number; i++) {
            if (read(fd, record_buffer, buffer_size * sizeof(char)) != buffer_size) {
                fprintf(stderr, "error while reading the file: %s\n", file_name);
                exit(-1);
            }

            if (write(copy_fd, record_buffer, buffer_size * sizeof(char)) != buffer_size) {
                fprintf(stderr, "error while writing to the file: %s\n", copy_file_name);
                exit(-1);
            }
        }

        if (close(fd) < 0) {
            fprintf(stderr, "error while closing the file: %s\n", file_name);
            exit(-1);
        }

        if (close(copy_fd) < 0) {
            fprintf(stderr, "error while closing the copy %s of the file: %s\n", copy_file_name, file_name);
            exit(-1);
        }

    } else if (strcmp(variant, "lib") == 0) {

        FILE *file = fopen(file_name, "r");

        if (file == NULL) {
            fprintf(stderr, "error while opening the file: %s\n", file_name);
            exit(-1);
        }

        FILE *file_copy = fopen(copy_file_name, "w");

        if (file_copy == NULL) {
            fprintf(stderr, "error while opening the copy %s of the file: %s\n", copy_file_name, file_name);
            exit(-1);
        }

        // reading a record from a given file and writing a copy of that record to another file
        int i;
        for (i = 0; i < records_number; i++) {

            if (fread(record_buffer, sizeof(char), buffer_size, file) != buffer_size) {
                fprintf(stderr, "error while reading the file: %s", file_name);
                exit(-1);
            }

            if (fwrite(record_buffer, sizeof(char), buffer_size, file_copy) != buffer_size) {
                fprintf(stderr, "error while writing to the file: %s", copy_file_name);
                exit(-1);
            }
        }

    } else {
        fprintf(stderr, "wrong variant entered, it should be sys or lib\n");
        exit(-1);
    }

}

int main(int argc, char **argv) {

    srand(time(NULL));

    if (strcmp(argv[1], "generate") == 0) {
        generate(argc, argv);
    } else if(strcmp(argv[1], "sort") == 0) {


    }

    else if(strcmp(argv[1], "copy") == 0) {

        copy(argc, argv);
    }

    else {

        fprintf(stderr, "unknown command: %s\n", argv[1]);
        exit(-1);
    }

    return 0;
}