#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

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

int compare_names(const void *a, const void *b) {
    return strcmp(((FileEntry *)a)->name, ((FileEntry *)b)->name);
}

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
    groups[*group_count].ext[sizeof(groups[*group_count].ext)-1] = '\0';
    groups[*group_count].count = 0;
    (*group_count)++;
    return (*group_count) - 1;
}

int main() {
    char directory[MAX_PATH] = {0};

    const char *osascript_cmd =
        "osascript -e 'try' -e 'POSIX path of (choose folder with prompt \"Select folder to sort\")' -e 'on error' -e 'return \"CANCELLED\"' -e 'end try'";

    FILE *fp = popen(osascript_cmd, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to run osascript\n");
        return 1;
    }

    if (fgets(directory, sizeof(directory), fp) == NULL) {
        fprintf(stderr, "No folder selected.\n");
        pclose(fp);
        return 1;
    }
    pclose(fp);

    size_t len = strlen(directory);
    if (len > 0 && directory[len - 1] == '\n') {
        directory[len - 1] = '\0';
    }

    if (strcmp(directory, "CANCELLED") == 0) {
        printf("Selection cancelled by user.\n");
        return 0;
    }

    printf("Folder selected: %s\n", directory);

    DIR *dir = opendir(directory);
    if (!dir) {
        perror("Cannot open folder");
        return 1;
    }

    ExtensionGroup *groups = malloc(sizeof(ExtensionGroup) * MAX_EXTENSIONS);
    if (!groups) {
        fprintf(stderr, "Memory allocation failed\n");
        closedir(dir);
        return 1;
    }
    int group_count = 0;

    for (int i = 0; i < MAX_EXTENSIONS; i++) {
        groups[i].files = malloc(sizeof(FileEntry) * MAX_FILES);
        if (!groups[i].files) {
            fprintf(stderr, "Memory allocation failed for files\n");
            // Libérer ce qui a déjà été alloué
            for (int j = 0; j < i; j++) {
                free(groups[j].files);
            }
            free(groups);
            closedir(dir);
            return 1;
        }
        groups[i].count = 0;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            const char *ext = get_file_extension(entry->d_name);
            int idx = find_or_add_extension(groups, &group_count, ext);
            if (idx == -1) {
                fprintf(stderr, "Error: too many extensions, stopping.\n");
                break;
            }
            if (groups[idx].count < MAX_FILES) {
                strncpy(groups[idx].files[groups[idx].count].name, entry->d_name, MAX_PATH);
                groups[idx].files[groups[idx].count].name[MAX_PATH-1] = '\0';
                groups[idx].count++;
            } else {
                fprintf(stderr, "Warning: too many files for extension %s\n", ext);
            }
        }
    }
    closedir(dir);

    for (int i = 0; i < group_count; i++) {
        char ext_folder[MAX_PATH];
        snprintf(ext_folder, sizeof(ext_folder), "%s/%s", directory, groups[i].ext);
        create_dir_if_needed(ext_folder);

        qsort(groups[i].files, groups[i].count, sizeof(FileEntry), compare_names);

        for (int j = 0; j < groups[i].count; j++) {
            char old_path[MAX_PATH], new_path[MAX_PATH];
            snprintf(old_path, sizeof(old_path), "%s/%s", directory, groups[i].files[j].name);
            snprintf(new_path, sizeof(new_path), "%s/%03d.%s", ext_folder, j + 1, groups[i].ext);

            if (rename(old_path, new_path) != 0) {
                perror("rename");
            } else {
                printf("Moved and renamed: %s -> %s\n", groups[i].files[j].name, new_path);
            }
        }
    }

    for (int i = 0; i < MAX_EXTENSIONS; i++) {
        free(groups[i].files);
    }
    free(groups);

    printf("Sorting done.\n");
    return 0;
}
