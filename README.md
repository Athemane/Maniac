╔════════════════════════════════════════════════════════════════════╗
║                📁 MANIAC (macOS)               ║
╠════════════════════════════════════════════════════════════════════╣

This project is a C utility that automatically sorts files in a folder
by their extension. Each file type is moved into a subfolder named after
its extension and renamed sequentially (001.ext, 002.ext, etc.).
The tool uses a graphical dialog to select the folder to sort
(macOS only).

╔════════════════════════════════════════════════════════════════════╗
║                           FEATURES                               ║
╠════════════════════════════════════════════════════════════════════╣
- Graphical folder selection (macOS)
- Automatic sorting of files by extension
- Creation of subfolders for each detected extension
- Sequential renaming of files in each subfolder
- Error handling (memory, folder access, etc.)

╔════════════════════════════════════════════════════════════════════╗
║                        PROJECT STRUCTURE                         ║
╠════════════════════════════════════════════════════════════════════╣

  .
  ├── main.c
  ├── file_utils.h
  ├── file_utils.c
  ├── extension_group.h
  ├── extension_group.c

╔════════════════════════════════════════════════════════════════════╗
║                     COMPILATION & USAGE                          ║
╠════════════════════════════════════════════════════════════════════╣

Compilation:
  gcc main.c file_utils.c extension_group.c -o Maniac

Usage:
  ./Maniac

A dialog will open to let you choose the folder to sort.
Files will be moved and renamed into subfolders created at
the root of the selected folder.

╔════════════════════════════════════════════════════════════════════╗
║                              EXAMPLE                             ║
╠════════════════════════════════════════════════════════════════════╣

Before:
  /Users/you/Documents/sort
  ├── doc1.pdf
  ├── doc2.pdf
  ├── image1.jpg
  ├── script.py

After:
  /Users/you/Documents/sort
  ├── pdf/
  │   ├── 001.pdf
  │   └── 002.pdf
  ├── jpg/
  │   └── 001.jpg
  ├── py/
  │   └── 001.py

╔════════════════════════════════════════════════════════════════════╗
║                           LIMITATIONS                            ║
╠════════════════════════════════════════════════════════════════════╣
- macOS only (uses osascript)
- Limited to 1000 files per extension and 100 extensions
- Only handles files in the root folder (no recursion)
- Files without extension go into the 'others' folder

╔════════════════════════════════════════════════════════════════════╗
║                        CUSTOMIZATION                             ║
╠════════════════════════════════════════════════════════════════════╣
- Edit the constants in extension_group.h to handle more files/extensions.
- To support other OSes, replace the graphical folder selection.

╔════════════════════════════════════════════════════════════════════╗
║                              AUTHOR                              ║
╠════════════════════════════════════════════════════════════════════╣
- Athemane.

╔════════════════════════════════════════════════════════════════════╗
║                              LICENSE                             ║
╠════════════════════════════════════════════════════════════════════╣
- Oppen Source 

╚════════════════════════════════════════════════════════════════════╝

                    ★★★ HAPPY SORTING! ★★★
