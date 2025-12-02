# Multithreaded File Search Program

## Authors

**Institution:** HWR Berlin - Hochschule für Wirtschaft und Recht
**Course:** Operating Systems
**Date:** December 2024

| Name           | Student ID  |
|----------------|-------------|
| Domenik Wilhelm | 77207300494 |
| Theo Leuthardt | 77205844868 |

## Overview

This program implements a parallel file search solution using POSIX threads or in short pthread. 
It searches for a specified pattern across multiple text files simultaneously for a specified search pattern in a directory.

## Features

- **Pattern Search**: Searches for a configurable text pattern in all files
- **Parallel Processing**: Uses multiple threads (configurable number) to search files concurrently
- **Thread-Safe Operations**: Implements mutex locks for safe access to shared resources
- **Linked List Management**: Files are managed in a singly linked list with thread-safe access
- **Individual Thread Logging**: Each thread maintains its own log file for debugging and monitoring

## Building the Program

### Prerequisites
- GCC compiler with C11 support
- POSIX threads library (pthread)
- Make utility

### Compilation

To compile the program:
```bash
make
```

This creates an executable named `search`.

### Building and Running

To compile and run in one step:
```bash
make run
```

### Cleaning

To remove compiled files and logs:
```bash
make clean
```

## Configuration

You can modify the following constants in `search.h`:

- `SEARCH_PATTERN`: The text pattern to search for (default: "content")
- `NUM_THREADS`: Number of worker threads (default: 8)
- `SEARCH_DIR`: Directory containing files to search (default: "./ToSearch")
- `MAX_FILENAME_LENGTH`: Maximum length for filenames (default: 256)
- `MAX_LINE_LENGTH`: Maximum line length in files (default: 1024)

## Usage

1. Create a directory named `ToSearch` in the program directory
2. Place text files to search in the `ToSearch` directory
3. Modify `SEARCH_PATTERN` in `search.h` if needed
4. Build and run the program:
   ```bash
   make run
   ```

## Output

The program produces:

1. **Console output**: Shows program progress and final results table
2. **Thread log files**: `thread_1.log`, `thread_2.log`, etc., containing detailed search operations
3. **Results table**: Displays each file, the thread that processed it, and whether the pattern was found

### Example Output

```
=== Initializing search program ===
Search pattern: TODO
Number of threads: 6
Search directory: ./ToSearch

Reading files from directory './ToSearch'...
Added 20 files to the search list.

=== Starting 6 threads ===
Thread 1 created.
Thread 2 created.
...

=== Waiting for threads to complete ===
Thread 1 completed. Processed 4 files.
Thread 2 completed. Processed 3 files.
...

=== Search Results ===
Filename                       Thread     Pattern Found
-----------------------------------------------------------
file1.txt                      1          YES
file2.txt                      2          NO
...
-----------------------------------------------------------
Total files: 20 | Files with pattern: 8

=== Program completed successfully ===
```

## Notes

- The program skips hidden files (starting with '.')
- The program skips '.' and '..' directory entries
- All threads execute the same worker function
- Thread-safe operations ensure no race conditions
- Memory is properly allocated and freed

## License

This is an educational project for the Operating Systems course at HWR Berlin.
