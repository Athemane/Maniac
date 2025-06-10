#ifndef FILE_UTILS_H
#define FILE_UTILS_H

const char* get_file_extension(const char *filename);
void create_dir_if_needed(const char *path);
int compare_names(const void *a, const void *b);

#endif
