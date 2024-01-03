# Unix-File-Explorer-Processing-and-Organizing-Movie-Data

This README provides instructions for compiling and running a Unix-based program developed for Assignment 2. The program is designed to handle file and directory operations, including reading and processing CSV files, and creating and writing to new directories and files.

## Instructions

### Program Overview

The program performs the following operations:
- Reads and processes entries from a directory.
- Finds files based on specific criteria.
- Processes data from selected CSV files.
- Creates a new directory and files, writing processed data to these files.

### Format of the CSV File

Your program will handle CSV files with the following structure:
- First row: Column headers (not movie data).
- Subsequent rows: Movie data in columns separated by commas.
- Columns: Title, Year, Languages, Rating Value.

### Program Functionality

#### Main Menu

The program starts with two options:
1. Select file to process.
2. Exit the program.

#### Selecting a File to Process

Based on the user's choice, the program allows selection of a file to process in one of three ways:
1. Choose the largest file.
2. Choose the smallest file.
3. Specify the name of a file.

#### Processing the File

After a file is chosen, the program:
- Creates a new directory named `your_onid.movies.random`.
- Sets appropriate permissions for the directory.
- Parses the CSV file and creates a file for each year with movies released.
- Writes movie titles to respective year files.

### Sample Program Execution

The program follows a structured flow to process files and handle user choices. Refer to the detailed example provided in the assignment description.

## Hints & Resources

- Incrementally develop and test your code.
- Use version control (e.g., Git) for tracking changes.
- Utilize `random()` or `srandom()` for generating random numbers.
- Use `sprintf()` for formatted output to strings.

## Compilation and Execution Instructions

### Setting up the Environment

1. Connect to the Oregon State University servers.

### Compiling the Code

1. Navigate to the directory containing the unzipped file.
2. Compile the program:
   ```
   gcc --std=gnu99 -o movies movies_by_year.c
   ```

### Running the Program

1. Execute the program with a CSV file:
   ```
   ./movies movies_sample_1.csv
   ```
   (Or replace `movies_sample_1.csv` with any desired CSV file having the prefix `movies_`).

## Additional Notes

- Ensure that the CSV file is in the correct format as specified.
- Follow the prompts in the program to navigate through its functionalities.
- The directory and files created will follow the naming and permission conventions outlined in the assignment.

---

**Developer:** Tanish Hupare
**Assignment:** Files & Directories (Assignment 2)
