#ifndef PROJECT_KASPIOTR_FINDLIB_H
#define PROJECT_KASPIOTR_FINDLIB_H

typedef struct blocks_array {
    int blocks_array_size;
    char **blocks;
    int *block_sizes;
} blocks_array;

void create_blocks_array(blocks_array *array, int blocks_array_size);

void delete_blocks_array(blocks_array *array);

int save_temp_file_to_block(blocks_array *array, char *temp_file_name);

void remove_block(blocks_array *array, int block_index);

void save_find_result_to_temp_file(char *directory_path, char *file_name, char *temp_file_name);

#endif //PROJECT_KASPIOTR_FINDLIB_H
