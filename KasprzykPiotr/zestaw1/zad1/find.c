#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "find.h"

void save_find_result_to_file(char *directory_path, char *file_name, char *tmp_file_name) {
    char command[100];
    stpcpy(command, "find ");
    strcat(command, directory_path);
    strcat(command, " -name ");
    strcat(command, file_name);
    strcat(command, " > ");
    strcat(command, tmp_file_name);
    strcat(command, " 2>&1");
    printf("%s\n", command);
    system(command);
}