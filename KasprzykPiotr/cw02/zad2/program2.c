#define _XOPEN_SOURCE 500 // in order to use strptime, must be before #include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <time.h>
#include <ftw.h>
#include <dirent.h>

int operator; // assumed marks: '<' is 1, '=' is 2, '>' is 3
struct tm date_argument;

/**
 * Prints help with examplary program calls
 */
void print_help() {
    printf("exemplary program call: ./program2 /etc/ '>' 'mar 19 2019' search_directory\n");
    printf("exemplary program call: ./program2 /tmp/ '>' 'mar 19 2019' search_directory\n");
    printf("exemplary program call: ./program2 /etc/ '>' 'mar 19 2019' nftw\n");
    printf("exemplary program call: ./program2 /tmp/ '>' 'mar 19 2019' nftw\n");
}

/**
 * Compares two tm structures (years and days are taken under consideration only)
 * @param file_date date describing file (i.e. recent access date or recent modification date)
 * @param user_date date provided by program user as an argument
 * @return number that that coresponds with one of the operator: '<' is 1, '=' is 2, '>' is 3
 */
int dates_comparison(struct tm* file_date, struct tm* user_date) {

    if(file_date -> tm_year > user_date -> tm_year
       || (file_date -> tm_year == user_date -> tm_year && file_date -> tm_yday > user_date -> tm_yday))
        return 3; // operator '>'

    if(file_date -> tm_year == user_date -> tm_year && file_date -> tm_yday == user_date -> tm_yday)
        return 2; // operator '='

    return 1; //operator '<'
}

/**
 * Prints file stats
 * @param file_path_buffer
 * @param stat_buffer
 */
void process_file(const char *file_path_buffer, const struct stat *stat_buffer) {
    struct tm *stat_date_recent_modification;
    struct tm *stat_date_recent_access;
    int stat_file_type;
    stat_date_recent_modification = localtime(&(stat_buffer->st_mtime));
    stat_date_recent_access = localtime(&(stat_buffer->st_atime));
    stat_file_type = stat_buffer->st_mode;

    // file is not processed if file recent modification date does not satisfy defined by operator time range
    if (dates_comparison(stat_date_recent_modification, &date_argument) != operator)
        return;

    // file is not processed if file recent access date does not satisfy defined by operator time range
    if (dates_comparison(stat_date_recent_access, &date_argument) != operator)
        return;

    char recent_modification_date_string[20];
    strftime(recent_modification_date_string, 20, "%b %d %Y", localtime(&(stat_buffer -> st_mtime)));

    char recent_access_date_string[20];
    strftime(recent_access_date_string, 20, "%b %d %Y", localtime(&(stat_buffer -> st_atime)));


    printf("File: %s\n", file_path_buffer);
    printf("-type: ");
    switch (stat_file_type & S_IFMT) {
        case S_IFREG:
            printf("regular file");
            break;
        case S_IFDIR: //not used
            printf("directory");
            break;
        case S_IFCHR:
            printf("character device");
            break;
        case S_IFBLK:
            printf("block device");
            break;
        case S_IFLNK: //not used
            printf("symbolic link");
            break;
        case S_IFIFO: //not used
            printf("pipe");
            break;
        case S_IFSOCK:
            printf("socket");
            break;
        default:
            printf("unknown");
    }
    printf("\t-size: %ld", stat_buffer->st_size);
    printf("\t-modification date: %s", recent_modification_date_string);
    printf("\t-access date: %s\n\n", recent_access_date_string);

}

/**
 * Search through all entries in a given directory (subdirectories are processed recursively)
 * @param directory_path_buffer is a path to a directory that will be search
 * @param directory_path_buffer_size
 */
void search_directory(char *directory_path_buffer, int *directory_path_buffer_size) {
    // adding slash sign at the end of directory path
    strcat(directory_path_buffer, "/");

    // saving index of the last character in directory_path_buffer NULL character is added at the end of directory_path_buffer
    int directory_path_buffer_last_idx = strlen(directory_path_buffer);

    // to be sure that copying from the buffer and copying to the buffer wil be held properly
    directory_path_buffer[directory_path_buffer_last_idx] = '\0';

    // amortisation of directory_path_buffer_array -if array is full in more than a half it is reallocated to be two times longer
    if ((*directory_path_buffer_size) < 2 * directory_path_buffer_last_idx) {
        directory_path_buffer = realloc(directory_path_buffer, 2 * (*directory_path_buffer_size) * sizeof(char));
        *directory_path_buffer_size = *directory_path_buffer_size * 2;
    }

    // opening directory
    DIR *current_direcotory_pointer;
    if ((current_direcotory_pointer = opendir(directory_path_buffer)) == NULL) {
        fprintf(stderr, "error while opening directory %s\n", directory_path_buffer);
        exit(-1);
    }

    struct dirent *entry;
    struct stat stat_buffer;

    // reading directory until all entries are searched
    while ((entry = readdir(current_direcotory_pointer)) != NULL) {

        // ignoring '..' and '." entries
        if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0)
            continue;

        // creating path to entry, adding file name to it
        strcat(directory_path_buffer, entry->d_name);

        // saving entry metadata into stat_buffer
        if (lstat(directory_path_buffer, &stat_buffer) < 0) {
            fprintf(stderr, "error while getting file %s statistics\n", directory_path_buffer);
            exit(-1);
        }

        // processing regular files, socket files, character device files, block device files,
        if (S_ISREG(stat_buffer.st_mode) || S_ISSOCK(stat_buffer.st_mode) || S_ISCHR(stat_buffer.st_mode) || S_ISBLK(stat_buffer.st_mode)) {
            process_file(directory_path_buffer, &stat_buffer);
        } else { //processing directories recursively
            search_directory(directory_path_buffer, directory_path_buffer_size);
        }
        directory_path_buffer[directory_path_buffer_last_idx] = '\0';
    }

    // closing directory
    if (closedir(current_direcotory_pointer) != 0) {
        fprintf(stderr, "error while closing directory %s\n", directory_path_buffer);
        exit(-1);
    }
}

/**
 * Utility function used as a parameter to nftw function
 * @param directory_path_buffer
 * @param stat_buffer
 * @param entry_info
 * @param ftw_str
 * @return
 */
int nftw_util(const char* directory_path_buffer, const struct stat* stat_buffer, int entry_info, struct FTW* ftw_str) {

    if(entry_info == FTW_F)
        process_file(directory_path_buffer, stat_buffer);

    return 0;
}

int main(int argc, char **argv) {

    if (argc < 5) {
        fprintf(stderr, "wrong arguments format\n");
        print_help();
        exit(-1);
    }

    char *directory_path_buffer = malloc(1000 * sizeof(char));
    directory_path_buffer[0] = '\0'; //NULL character added at

    // in case given argument is an absolute path
    if (argv[1][0] == '/') {
        strcpy(directory_path_buffer, argv[1]);
    } else { // if given argument is not an absolute path it is concatenated with current working directory to be so
        getcwd(directory_path_buffer, 1000);
        strcat(directory_path_buffer, "/");
        strcat(directory_path_buffer, argv[1]);
    }

    // checking operator
    if (strcmp(argv[2], "<") == 0)
        operator = 1;
    else if (strcmp(argv[2], "=") == 0)
        operator = 2;
    else if (strcmp(argv[2], ">") == 0)
        operator = 3;
    else {
        fprintf(stderr, "invalid operator %s\n", argv[2]);
        print_help();
        exit(-1);
    }

    int *buffer_length = malloc(sizeof(int));
    *buffer_length = 1000;

    // parsing date
    strptime(argv[3], "%b $d %Y", &date_argument);

    if (strcmp(argv[4], "search_directory") == 0) {
        printf("searching with search_directory function:\n");
        search_directory(directory_path_buffer, buffer_length);
    } else if (strcmp(argv[4], "nftw") == 0) {
        printf("searching with nftw:\n");
        nftw(directory_path_buffer, nftw_util, 10, FTW_PHYS); //FTW_PHYS = not following symbolic links
    } else {
        fprintf(stderr, "invalid argument %s\n", argv[4]);
        print_help();
        exit(-1);
    }

    return 0;
}