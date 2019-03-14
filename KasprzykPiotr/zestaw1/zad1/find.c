#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "find.h"

void create_blocks_array(blocks_array *array, int blocks_array_size) {
    array->blocks_array_size = blocks_array_size;
    array->blocks = calloc(blocks_array_size, sizeof(char *));
    array->block_sizes = calloc(blocks_array_size, sizeof(int));
}

void delete_blocks_array(blocks_array *array) {
    for (int i = 0; i < array->blocks_array_size; i++) {
        free(array->blocks[i]);
    }
    free(array->blocks);
    free(array->block_sizes);
}

void remove_block(blocks_array *array, int block_index) {
    free(array->blocks[block_index]);
    array->blocks[block_index] = NULL;
    array->block_sizes[block_index] = 0;
}

int save_temp_file_to_block(blocks_array *array, char *temp_file_name) {

    int blocks_array_size = array->blocks_array_size;
    int i = 0;

    while (i < blocks_array_size && array->blocks[i] != NULL) {
        i++;
    }

    if (i == blocks_array_size) {
        fprintf(stderr, "array of blocks is already full\n");
        return -1;
    }

    struct stat sb;
    int file_size = 0;
    if (stat(temp_file_name, &sb) == -1) {
        fprintf(stderr, "file not found\n");
        return -1;
    } else {
        file_size = (int) sb.st_size;
    }

    if (file_size != 0) {
        array->blocks[i] = calloc(file_size, sizeof(char));

        FILE *fp;

        fp = fopen(temp_file_name, "r"); // read mode

        if (fp == NULL)
        {
            perror("error while opening the file\n");
            exit(EXIT_FAILURE);
        }

        char ch;
        int j=0;
        while((ch = fgetc(fp)) != EOF) {
            array->blocks[i][j] = ch;
            j++;
        }

        fclose(fp);
    }

    array -> block_sizes[i] = file_size;
    return i;
}

void save_find_result_to_temp_file(char *directory_path, char *file_name, char *temp_file_name) {
    char command[100];
    stpcpy(command, "find ");
    strcat(command, directory_path);
    strcat(command, " -name ");
    strcat(command, file_name);
    strcat(command, " > ");
    strcat(command, temp_file_name);
    strcat(command, " 2>&1");
    printf("%s\n", command);
    int returned_value = system(command);
    if (returned_value == -1) {
        fprintf(stderr, "given commend is incorrect\n");
    }
}