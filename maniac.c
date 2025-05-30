#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH 1024        // Maximum path length
#define MAX_FILES 1000       // Maximum files per extension group
#define MAX_EXTENSIONS 100   // Maximum different extensions handled

// Structure to store a file name
typedef struct {
    char name[MAX_PATH];
} FileEntry;

// Structure to store a group of files with the same extension
typedef struct {
    char ext[64];       // Extension name (e.g. "txt")
    FileEntry *files;   // Dynamic array of files with this extension
    int count;          // Number of files in this group
} ExtensionGroup;

// Compare function for sorting files by name (used by qsort)
int compare_names(const void *a, const void *b) {
    return strcmp(((FileEntry *)a)->name, ((FileEntry *)b)->name);
}

// Get the file extension from a filename string
const char* get_file_extension(const char *filename) {
    const char *dot = strrchr(filename, '.'); // Find last dot in filename
    if (!dot || dot == filename) return "others"; // No extension found
    return dot + 1; // Return pointer after the dot
}

// Create a directory if it does not already exist
void create_dir_if_needed(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) { // Check if directory exists
        if (mkdir(path, 0755) != 0) { // Create directory with rwxr-xr-x permissions
            perror("mkdir failed");
        }
    }
}

// Find the index of an extension group or add a new one if it does not exist
int find_or_add_extension(ExtensionGroup groups[], int *group_count, const char *ext) {
    for (int i = 0; i < *group_count; i++) {
        if (strcmp(groups[i].ext, ext) == 0)
            return i; // Found existing group index
    }
    if (*group_count >= MAX_EXTENSIONS) { // Too many different extensions
        fprintf(stderr, "Too many different extensions!\n");
        return -1;
    }
    // Add new extension group
    strncpy(groups[*group_count].ext, ext, sizeof(groups[*group_count].ext));
    groups[*group_count].ext[sizeof(groups[*group_count].ext)-1] = '\0'; // Ensure null-termination
    groups[*group_count].count = 0; // Initialize count
    (*group_count)++;
    return (*group_count) - 1; // Return new group's index
}

int main() {
    char directory[MAX_PATH] = {0};

    // AppleScript command to open macOS folder picker and get folder path
    const char *osascript_cmd =
        "osascript -e 'try' -e 'POSIX path of (choose folder with prompt \"Select folder to sort\")' -e 'on error' -e 'return \"CANCELLED\"' -e 'end try'";

    // Run AppleScript and read folder path from output
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

    // Remove trailing newline from the directory string
    size_t len = strlen(directory);
    if (len > 0 && directory[len - 1] == '\n') {
        directory[len - 1] = '\0';
    }

    // Check if user cancelled the folder selection
    if (strcmp(directory, "CANCELLED") == 0) {
        printf("Selection cancelled by user.\n");
        return 0;
    }

    printf("Folder selected: %s\n", directory);

    // Open the selected directory for reading
    DIR *dir = opendir(directory);
    if (!dir) {
        perror("Cannot open folder");
        return 1;
    }

    // Allocate memory for extension groups dynamically
    ExtensionGroup *groups = malloc(sizeof(ExtensionGroup) * MAX_EXTENSIONS);
    if (!groups) {
        fprintf(stderr, "Memory allocation failed\n");
        closedir(dir);
        return 1;
    }
    int group_count = 0;

    // Allocate memory for files array inside each extension group
    for (int i = 0; i < MAX_EXTENSIONS; i++) {
        groups[i].files = malloc(sizeof(FileEntry) * MAX_FILES);
        if (!groups[i].files) {
            fprintf(stderr, "Memory allocation failed for files\n");
            // Free already allocated memory before exiting
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

    // Read all entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Process only regular files
            const char *ext = get_file_extension(entry->d_name); // Get file extension
            int idx = find_or_add_extension(groups, &group_count, ext); // Find or create group
            if (idx == -1) {
                fprintf(stderr, "Error: too many extensions, stopping.\n");
                break;
            }
            if (groups[idx].count < MAX_FILES) { // Check if group can hold more files
                // Copy filename to group's file list
                strncpy(groups[idx].files[groups[idx].count].name, entry->d_name, MAX_PATH);
                groups[idx].files[groups[idx].count].name[MAX_PATH-1] = '\0'; // Ensure null-termination
                groups[idx].count++;
            } else {
                fprintf(stderr, "Warning: too many files for extension %s\n", ext);
            }
        }
    }
    closedir(dir);

    // For each extension group, create folder, sort files, and rename/move them
    for (int i = 0; i < group_count; i++) {
        char ext_folder[MAX_PATH];
        snprintf(ext_folder, sizeof(ext_folder), "%s/%s", directory, groups[i].ext);
        create_dir_if_needed(ext_folder); // Create folder for this extension if needed

        // Sort files alphabetically by name
        qsort(groups[i].files, groups[i].count, sizeof(FileEntry), compare_names);

        // Rename and move each file to its extension folder with numbered name
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

    // Free allocated memory for file lists and groups
    for (int i = 0; i < MAX_EXTENSIONS; i++) {
        free(groups[i].files);
    }
    free(groups);

    printf("Sorting done.\n");
    return 0;
}
