##############################################
#                                            #
#              R E A D   M E                 #
#                                            #
##############################################

# Program Name: 
MANIAC

# Description: 
This C program lets you pick a folder using a macOS Finder window, 
then sorts the files inside by their extension.


It creates subfolders for each extension and renames the files 
inside each folder with numbered filenames like 001.txt, 002.pdf, etc. 

This program uses dynamic memory allocation to safely handle many files 
and avoid crashes on macOS.

# Features:
- GUI folder selection using macOS Finder window
- Groups files by extension into folders
- Renames files with aligned numbering
- Handles large number of files safely with dynamic memory
- Command-line executable, easy to use

# How to Compile:
Open Terminal and run:

A Finder window will open for you to select the folder to sort.

# Requirements:
- macOS system
- Terminal access
- gcc compiler installed (comes with Xcode command line tools)

# Notes:
- The program only sorts regular files (ignores folders).
- If you cancel folder selection, the program exits gracefully.
- Max 100 different extensions and 1000 files per extension supported by default.
- You can adjust MAX_EXTENSIONS and MAX_FILES constants in the source code if needed.

# Author:
Athemane alias Hammoud Boualem

# License:
Free to use and modify.

---

Thank you for using this program!  
Feel free to report issues or suggest improvements.
