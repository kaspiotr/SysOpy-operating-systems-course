#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

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

int main(int argc, char **argv) {

    if (strcmp(argv[1], "generate") == 0) {
        generate(argc, argv);
    }

    return 0;
}