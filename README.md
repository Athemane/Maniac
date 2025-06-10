##############################################
#                                            #
#                  MANIAC                    #
#           File Sorter by Extension         #
#                                            #
##############################################

Program Description:

MANIAC is a clean, efficient C program designed for macOS users to organize files effortlessly. It opens a native Finder window to let you select a folder, then sorts all regular files inside by their extension.

For each detected file extension, MANIAC creates a dedicated subfolder (e.g., txt, pdf, jpg), moves the files into these folders, and renames them with a neat, zero-padded sequential numbering format (001.txt, 002.pdf, ...).

Designed with modularity and dynamic memory management, MANIAC can handle large numbers of files safely and efficiently.

Features:
- Native macOS Finder folder selection via AppleScript
- Groups files by their extension into respective folders
- Renames files with consistent 3-digit numbering for better order
- Dynamic memory management to handle up to 100 extensions and 1000 files per extension by default
- Simple command-line interface, no complicated setup required

Project Structure:
- main.c: Main program logic and user interaction
- file_utils.h / file_utils.c: Utility functions for file extension extraction, directory creation, and file sorting
- extension_group.h / extension_group.c: Management of file groups by extension with memory allocation and lookup

Requirements:
- macOS operating system (for Finder folder selection)
- Terminal access
- GCC compiler (available via Xcode Command Line Tools)

Compilation:
Run this command in Terminal from the folder containing source files:

    gcc main.c file_utils.c extension_group.c -o maniac

Usage:
Execute the compiled program:

    ./maniac

A Finder window will prompt you to select the folder to organize. Once selected, the files will be sorted and renamed automatically.

If folder selection is canceled, the program will exit gracefully.

Limits & Configuration:
- Supports up to 100 different file extensions by default
- Supports up to 1000 files per extension
- Limits can be changed by adjusting MAX_EXTENSIONS and MAX_FILES in extension_group.h
- Only regular files are processed (folders and special files are ignored)

Author:
Athemane alias Hammoud Boualem

License:
Free to use and modify.

Thank you for using MANIAC!
Feel free to report issues or suggest improvements.
