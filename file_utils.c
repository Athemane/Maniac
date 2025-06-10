#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "file_utils.h"

const char* get_file_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "others";
    return dot + 1;
}

void create_dir_if_needed(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        if (mkdir(path, 0755) != 0) {
            perror("mkdir failed");
        }
    }
}

int compare_names(const void *a, const void *b) {
    return strcmp(((const FileEntry *)a)->name, ((const FileEntry *)b)->name);
}
