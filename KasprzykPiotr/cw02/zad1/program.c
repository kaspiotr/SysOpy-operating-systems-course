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
 * Sorts records in a given file using insertion sort
 * @param argc is a number of the program input arguments
 * @param argv contains an array of pointers to input arguments
 */
void sort(int argc, char **argv) {

    if (argc < 6) {
        fprintf(stderr, "wrong function input argument format\n");
        exit(-1);
    }

    char *file_name = argv[2];
    int records_number = atoi(argv[3]);
    int record_size = atoi(argv[4]);
    char *variant = argv[5];

    char *first_unsorted_record = malloc(record_size * sizeof(char));
    char *swap_buffer = malloc(record_size * sizeof(char));

    if (strcmp(variant, "sys") == 0) {
        int fd = open(file_name, O_RDWR);

        if (fd < 0) {

            fprintf(stderr, "error while opening file: %s\n", file_name);
            exit(-1);
        }

        int i;
        int next_record_position;
        int target_record_position;
        for (i = 1; i < records_number; i++) {
            next_record_position = i * record_size;
            target_record_position = 0; // record position after insertion sort is complete

            // setting fd to to the beginning of the next record - next_record_position
            if (lseek(fd, next_record_position * sizeof(char), SEEK_SET) == 0) {
                fprintf(stderr, "error while opening file: %s\n", file_name);
                exit(-1);
            }

            // loading next record from file to first_unsorted_record array
            if(read(fd, first_unsorted_record, record_size * sizeof(char)) != record_size * sizeof(char)) {
                fprintf(stderr, "error while reading from file: %s\n", file_name);
                exit(-1);
            }

            // search for position of the record, where it should be after insertion sort is completed
            int j = i;
            int record_position;
            while (j > 0) {
                record_position = record_size * j;

                // finding position of previous record
                if (lseek(fd, (record_position - record_size) * sizeof(char), SEEK_SET) < 0) {
                    fprintf(stderr, "error while setting pointer in a file: %s\n", file_name);
                    exit(-1);
                }

                // loading record from file into swap_buffer
                if (read(fd, swap_buffer, record_size * sizeof(char)) != record_size * sizeof(char)) {
                    fprintf(stderr, "error while reading from file: %s\n", file_name);
                    exit(-1);
                }

                // loop ends when proper position is found
                if ((unsigned char) first_unsorted_record[0] >= (unsigned char) swap_buffer[0]) { // we have found target record position
                    target_record_position = record_position;
                    break;
                } else { // if record position does not point to target_record_position
                    if(lseek(fd, record_position * sizeof(char), SEEK_SET) < 0) {
                        fprintf(stderr, "error while setting pointer in a file: %s\n", file_name);
                        exit(-1);
                    }

                    if(write(fd, swap_buffer, record_size * sizeof(char)) != record_size * sizeof(char)){
                        fprintf(stderr, "error while writing to file: %s\n", file_name);
                        exit(-1);
                    }
                }
                j--;
            }

            // inserting record into target_record_position in file
            if(lseek(fd, target_record_position * sizeof(char), SEEK_SET) < 0) {
                fprintf(stderr, "error while setting pointer in file: %s\n", file_name);
                exit(-1);
            }

            if(write(fd, first_unsorted_record, record_size * sizeof(char)) != record_size * sizeof(char)){
                fprintf(stderr, "error while writing into file: %s\n", file_name);
                exit(-1);
            }
        }

        // closing file
        if(close(fd) != 0) {
            fprintf(stderr, "error while closing file: %s\n", file_name);
            exit(-1);
        }
    } else if (strcmp(variant, "lib") == 0) {

        FILE *file = fopen(file_name, "r+");

        if (file == NULL) {
            fprintf(stderr, "error while opening file: %s\n", file_name);
            exit(-1);
        }

        int i;
        int next_record_position;
        int target_record_position;
        for(i=0; i < records_number; i++) {
            next_record_position = i * record_size;
            target_record_position = 0;

            if(fseek(file, next_record_position * sizeof(char), SEEK_SET) != 0) {
                fprintf(stderr, "error while setting file pointer in file: %s\n", file_name);
                exit(-1);
            }

            // loading next record from file to first_unsorted_record array
            if(fread(first_unsorted_record, sizeof(char), record_size, file) != record_size) {
                fprintf(stderr, "error while reading from file: %s\n", file_name);
                exit(-1);
            }

            // search for position of the record, where it should be after insertion sort is completed
            int j = i;
            int record_position;
            while (j > 0) {
                record_position = record_size * j;

                // finding position of previous record
                if (fseek(file, (record_position - record_size) * sizeof(char), SEEK_SET) != 0) {
                    fprintf(stderr, "error while setting pointer in a file: %s\n", file_name);
                    exit(-1);
                }

                // loading record from file into swap_buffer
                if (fread(swap_buffer, sizeof(char), record_size, file) != record_size) {
                    fprintf(stderr, "error while reading from file: %s\n", file_name);
                    exit(-1);
                }

                // loop ends when proper position is found
                if ((unsigned char) first_unsorted_record[0] >= (unsigned char) swap_buffer[0]) { // we have found target record position
                    target_record_position = record_position;
                    break;
                } else { // if record position does not point to target_record_position
                    if(fseek(file, record_position * sizeof(char), SEEK_SET) != 0) {
                        fprintf(stderr, "error while setting pointer in a file: %s\n", file_name);
                        exit(-1);
                    }

                    if(fwrite(swap_buffer, sizeof(char), record_size, file) != record_size){
                        fprintf(stderr, "error while writing to file: %s\n", file_name);
                        exit(-1);
                    }
                }

                j--;
            }

            // inserting record into target_record_position in file
            if(fseek(file, target_record_position * sizeof(char), SEEK_SET) != 0) {
                fprintf(stderr, "error while setting pointer in file: %s\n", file_name);
                exit(-1);
            }

            if(fwrite(first_unsorted_record, sizeof(char), record_size, file) != record_size){
                fprintf(stderr, "error while writing into file: %s\n", file_name);
                exit(-1);
            }
        }

        // closing file
        if(fclose(file) != 0) {

            fprintf(stderr, "error while closing file: %s\n", file_name);
            exit(-1);
        }

    } else {
        fprintf(stderr, "wrong function input argument format\n");
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
    } else if (strcmp(argv[1], "sort") == 0) {
        sort(argc, argv);

    } else if (strcmp(argv[1], "copy") == 0) {

        copy(argc, argv);
    } else {

        fprintf(stderr, "unknown command: %s\n", argv[1]);
        exit(-1);
    }

    return 0;
}