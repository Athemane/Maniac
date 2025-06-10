#ifndef EXTENSION_GROUP_H
#define EXTENSION_GROUP_H

#define MAX_PATH 1024
#define MAX_FILES 1000
#define MAX_EXTENSIONS 100

typedef struct {
    char name[MAX_PATH];
} FileEntry;

typedef struct {
    char ext[64];
    FileEntry *files;
    int count;
} ExtensionGroup;

int find_or_add_extension(ExtensionGroup groups[], int *group_count, const char *ext);
void free_extension_groups(ExtensionGroup groups[], int count);
int allocate_extension_groups(ExtensionGroup groups[], int size);

#endif
