#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extension_group.h"

int find_or_add_extension(ExtensionGroup groups[], int *group_count, const char *ext) {
    for (int i = 0; i < *group_count; i++) {
        if (strcmp(groups[i].ext, ext) == 0)
            return i;
    }
    if (*group_count >= MAX_EXTENSIONS) {
        fprintf(stderr, "Too many different extensions!\n");
        return -1;
    }
    strncpy(groups[*group_count].ext, ext, sizeof(groups[*group_count].ext));
    groups[*group_count].ext[sizeof(groups[*group_count].ext) - 1] = '\0';
    groups[*group_count].count = 0;
    (*group_count)++;
    return (*group_count) - 1;
}

int allocate_extension_groups(ExtensionGroup groups[], int size) {
    for (int i = 0; i < size; i++) {
        groups[i].files = malloc(sizeof(FileEntry) * MAX_FILES);
        if (!groups[i].files) {
            fprintf(stderr, "Memory allocation failed for files\n");
            for (int j = 0; j < i; j++) {
                free(groups[j].files);
            }
            return 0;
        }
        groups[i].count = 0;
    }
    return 1;
}

void free_extension_groups(ExtensionGroup groups[], int size) {
    for (int i = 0; i < size; i++) {
        free(groups[i].files);
    }
}
