#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "file_utils.h"
#include "extension_group.h"

int main() {
    char directory[MAX_PATH] = {0};

    const char *osascript_cmd =
        "osascript -e 'try' -e 'POSIX path of (choose folder with prompt \"Select folder to sort\")' -e 'on error' -e 'return \"CANCELLED\"' -e 'end try'";

    FILE *fp = popen(osascript_cmd, "r");
    if (!fp) {
        fprintf(stderr, "Failed to run osascript\n");
        return 1;
    }

    if (!fgets(directory, sizeof(directory), fp)) {
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

    if (!allocate_extension_groups(groups, MAX_EXTENSIONS)) {
        free(groups);
        closedir(dir);
        return 1;
    }

    int group_count = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            const char *ext = get_file_extension(entry->d_name);
            int idx = find_or_add_extension(groups, &group_count, ext);
            if (idx == -1) break;

            if (groups[idx].count < MAX_FILES) {
                strncpy(groups[idx].files[groups[idx].count].name, entry->d_name, MAX_PATH);
                groups[idx].files[groups[idx].count].name[MAX_PATH - 1] = '\0';
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

    free_extension_groups(groups, MAX_EXTENSIONS);
    free(groups);
    printf("Sorting done.\n");
    return 0;
}
