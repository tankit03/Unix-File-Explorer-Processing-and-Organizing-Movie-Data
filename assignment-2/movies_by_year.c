#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>         
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>

struct movies 
{
    char *title;
    int year;
    char *language;
    double rating_v;
    struct movies *next;

};

struct movies *createmovies(char *currLine)
{
    struct movies *currmovies = malloc(sizeof(struct movies));

    if (currmovies == NULL) {
        // Handle memory allocation failure
        perror("Failed to allocate memory for movies");
        exit(EXIT_FAILURE); // Or return an error code, depending on your needs
    }

    // For use with strtok_r
    char *saveptr;

    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    if (token == NULL) {
        // Handle improper CSV formatting
        fprintf(stderr, "Error: Invalid CSV format - missing title\n");
        free(currmovies);
        return NULL; // Return NULL to indicate failure
    }
    currmovies->title = calloc(strlen(token) + 1, sizeof(char));
    if (currmovies->title == NULL) {
        // Handle memory allocation failure
        perror("Failed to allocate memory for title");
        free(currmovies);
        exit(EXIT_FAILURE); // Or return an error code
    }
    strcpy(currmovies->title, token);

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);
    if (token != NULL) {
        currmovies->year = atoi(token);
    } else {
        // Handle improper CSV formatting or missing year
        fprintf(stderr, "Error: Invalid CSV format - missing year\n");
        free(currmovies->title);
        free(currmovies);
        return NULL;
    }

    // The next token is the language
    token = strtok_r(NULL, ",", &saveptr);
    if (token == NULL) {
        // Handle improper CSV formatting or missing language
        fprintf(stderr, "Error: Invalid CSV format - missing language\n");
        free(currmovies->title);
        free(currmovies);
        return NULL;
    }
    currmovies->language = calloc(strlen(token) + 1, sizeof(char));
    if (currmovies->language == NULL) {
        // Handle memory allocation failure
        perror("Failed to allocate memory for language");
        free(currmovies->title);
        free(currmovies);
        exit(EXIT_FAILURE);
    }
    strcpy(currmovies->language, token);

    // The last token is the rating
    token = strtok_r(NULL, "\n", &saveptr);
    if (token != NULL) {
        currmovies->rating_v = strtod(token, NULL);
    } else {
        // Handle improper CSV formatting or missing rating
        fprintf(stderr, "Error: Invalid CSV format - missing rating\n");
        free(currmovies->title);
        free(currmovies->language);
        free(currmovies);
        return NULL;
    }

    // Set the next node to NULL in the newly created movies entry
    currmovies->next = NULL;

    return currmovies;
}

struct  movies *processFile(char *filePath)
{
    
    // Open the specified file for reading only
    FILE *moviesFile = fopen(filePath, "r");

    if (moviesFile == NULL) {
        perror("Failed to open the file");
        exit(EXIT_FAILURE);
    }

    char *currLine = NULL;
    size_t len = 0;
    size_t nread;
    char *token;
    int counter = 0;

    // this is the head of the linked list
    struct movies *head = NULL;
    // this is the tail of the linked list
    struct movies *tail = NULL;

    int firstLine = 1; // this helps us identify the first line
    

    // the code below basically reads everything line by line
    while ((nread = getline(&currLine, &len, moviesFile)) != -1)
    {
        if(firstLine){

            firstLine = 0; //set the flag to 0 so it skips the first line
            continue; //this skips the first line

        }
        //printf("kfnerhgiuehrg");
        // Get a new student node corresponding to the current line
        struct movies *newNode = createmovies(currLine);

        // if there is nothing in the head it sets the head and tail to a new node
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
        counter++;
    }

    // tells the user the files been processed. 

    printf("Now processing the chosen file named %s", filePath);
    printf("\n");
    free(currLine);
    fclose(moviesFile);

    return head;
}

void printmovies(struct movies* aMovies){
    // this prints the movie's title, release year, language, and rating. It's also formatted
    
    while (aMovies != NULL) {
        printf("Title: %s, Year: %d, Language: %s, Rating: %.1f\n",
               aMovies->title, aMovies->year, aMovies->language, aMovies->rating_v);
        aMovies = aMovies->next;
    }
}
/*
* creates a directory with a different name based on the provided identifiers
*/

char* createdir(const char *onid){
    //generate random number to ensure uniqueness
    srand((unsigned)time(NULL));
    int rand_n = rand() % 10000;

    //create the directory name by combining the base identifiers and random numbers
    char dir_name[256];
    snprintf(dir_name, sizeof(dir_name), "%s.movies.%d", onid,rand_n);
    
    //try to create the directory with the spcfifc permissions
    if(mkdir(dir_name, 0711) == -1){
        perror("Error creating directory");
        exit(1);
    }

    // print this message with the directory and it was successfully created
    printf("created directory: %s\n", dir_name);
    printf("\n");

    //set permissions for the user and group

    if (chmod(dir_name, S_IRWXU | S_IRGRP | S_IXGRP) != 0) {
            perror("Error setting directory permissions");
    }
    char *result = strdup(dir_name);
    
    return result;
}

/*
*add movies informtions to the text files within the given directory
*/

void writeTitleFile(struct movies* list, const char* dir){
   
    struct movies* current = list;
    
    // iterate through the linked list of movies
    while(current != NULL){
        // create a filename for the movies using its release year
        char filename[256];
        snprintf(filename, sizeof(filename), "%s/%d.txt",dir, current->year);
        //open the file for appending
        FILE *file = fopen(filename, "a");

        if(file != NULL){

            //write movie information to the file               
            fprintf(file, "Title: %s, Year: %d, Language: %s, Rating: %.1f\n",
            current->title, current->year, current->language, current->rating_v);
            fclose(file);

        
            // Set file permissons to allow user and group access
            if (chmod(filename, S_IRUSR | S_IWUSR | S_IRGRP) != 0) {
            perror("Error setting directory permissions");
            }
        } 
        //move to the next movie in the linked list
        current = current->next;
    }
}

/*
*this function checks the prefixs then compares the size with the other files and returns the checked file name 
*/

char* findLargestFileWithPrefix(const char *directory, const char *prefix, const char *extension) {
    // Initialize variables to keep track of the largest file and its size.
    char *largestFile = NULL;
    long long largestSize = 0;

    // Open the specified directory for reading.
    DIR *dir;
    struct dirent *entry;
    dir = opendir(directory);

    // Check if the directory couldn't be opened.
    if (dir == NULL) {
        perror("opendir"); // Print an error message and return NULL.
        return NULL;
    }

    // Iterate through each file in the directory.
    while ((entry = readdir(dir)) != NULL) {
        // Check if the file name starts with the specified prefix and contains the given extension.
        if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0 && strstr(entry->d_name, extension) != NULL) {
            // Build the full file path by concatenating the directory and file name.
            char filepath[FILENAME_MAX];
            snprintf(filepath, sizeof(filepath), "%s/%s", directory, entry->d_name);

            // Open the file in binary read mode.
            FILE *file = fopen(filepath, "rb");

            // Check if the file couldn't be opened.
            if (file == NULL) {
                perror("fopen"); // Print an error message and continue to the next file.
                continue;
            }

            // Determine the size of the file by seeking to the end and using stat.
            struct stat fileInfo;

            if(stat(filepath, &fileInfo) == 1){
                perror("stat");
                continue;
            }

            long long fileSize = fileInfo.st_size;


            // If the current file is larger than the previously found largest file, update the variables.
            if (fileSize > largestSize) {
                largestSize = fileSize;
                if (largestFile != NULL) {
                    free(largestFile); // Free previously allocated memory for the largest file.
                }
                largestFile = strdup(entry->d_name); // Duplicate and store the name of the largest file.
            }
        }
    }

    // Close the directory and return the name of the largest file (or NULL if none found).
    closedir(dir);
    return largestFile;

}

/*
*this function checks the prefixs then compares the size with the other files and returns the checked file name if its the smallest file
*/

char* findSmallestFileWithPrefix(const char *directory, const char *prefix, const char *extension) {
    // Initialize variables to keep track of the smallest file and its size.
    char *smallestFile = NULL;
    long long smallestSize = LLONG_MAX;

    // Open the specified directory for reading.
    DIR *dir;
    struct dirent *entry;
    dir = opendir(directory);

    // Check if the directory couldn't be opened.
    if (dir == NULL) {
        perror("opendir"); // Print an error message and return NULL.
        return NULL;
    }

    // Iterate through each file in the directory.
    while ((entry = readdir(dir)) != NULL) {
        // Check if the file name starts with the specified prefix and contains the given extension.
        if (strncmp(entry->d_name, prefix, strlen(prefix)) == 0 && strstr(entry->d_name, extension) != NULL) {
            // Build the full file path by concatenating the directory and file name.
            char filepath[FILENAME_MAX];
            snprintf(filepath, sizeof(filepath), "%s/%s", directory, entry->d_name);

            // Open the file in binary read mode.
            FILE *file = fopen(filepath, "rb");

            // Check if the file couldn't be opened.
            if (file == NULL) {
                perror("fopen"); // Print an error message and continue to the next file.
                continue;
            }

            // Determine the size of the file by seeking to the end and using stat.
            struct stat fileInfo;
            if (stat(filepath, &fileInfo) == -1) {
                perror("stat");
                fclose(file);
                continue;
            }

            long long fileSize = fileInfo.st_size;

            // If the current file is smaller than the previously found smallest file, update the variables.
            if (fileSize < smallestSize) {
                smallestSize = fileSize;
                if (smallestFile != NULL) {
                    free(smallestFile); // Free previously allocated memory for the smallest file.
                }
                smallestFile = strdup(filepath); // Duplicate and store the path of the smallest file.
            }

            fclose(file);
        }
    }

    // Close the directory and return the path to the smallest file (or NULL if none found).
    closedir(dir);
    return smallestFile;
}

/*
*this function checks the prefixs then compares the size with the other files and returns the checked file name 
*/

void FindInputFilewithPrefix(){

 
    char Name[256];
    const char *onid = "huparet";
    int checker = 0;

    do{

        printf("Enter in the file name: ");
        scanf("%s", Name);

        if(access(Name, F_OK) == -1){
        printf("The file %s was not found. try again \n", Name);
        printf("\n");
        }else{
            checker = 1;
        }

    }while(checker == 0); 
        
            // Open the chosen file for reading
            FILE* currentFile = fopen(Name, "r");
            printf("Process the choose file: %s\n", Name);

            // Process the file to obtain movie information
            struct movies* list = processFile(Name);

            //Create directory with a specific prefix and write movie information to text files
            char* dir = createdir(onid);
            if(list != NULL){
                writeTitleFile(list, dir); 
            }
            //close the current file
            fclose(currentFile);

}

/*
* Prompt the user to select and process a file based on their choice
*/

void ProcessFilePrompt(){

    int choice;
    const char *onid = "huparet"; // Prefix for the directory name

        //Prompt the user to enter a choice from 1 to 3.
        printf("Which file you want to process? \n");
        printf("Enter 1 to pick the largest file \n");
        printf("Enter 2 to pick the smallest file \n");
        printf("Enter 3 to specify the name of a file \n");
        printf("\n");
        printf("Enter a choice from 1 to 3 :");
        scanf("%i", &choice);

        // Validate the user's choice
        while(choice > 3 || choice < 1){

            printf("invalid option. Please input a valid input through 1-3: ");
            scanf("%i", &choice);
        }

     switch (choice){
            case 1:
                // Find and process the largest file with a specific prefix and extenion
                char *largestFile = findLargestFileWithPrefix(".","movies_","csv");
                if(largestFile != NULL){
                    struct movies* list = processFile(largestFile);
                    char* dir = createdir(onid);
                    writeTitleFile(list, dir);    
                }
                break;
            case 2:
                //find and process the smallest with a specific prefix and extenion
                char *fileToProcess = findSmallestFileWithPrefix(".", "movies_", ".csv");
                if (fileToProcess != NULL){
                    struct movies* list = processFile(fileToProcess);
                    char* dir = createdir(onid);
                    writeTitleFile(list, dir);
                }                
                break;
            case 3: 
                //Allow the user to specific the name of a file
                FindInputFilewithPrefix();
                break;
            default:
            
            printf("invalid choice. Please eneter a valid option. \n");
            printf("\n");
    }
    
}

/*
* Prompt the user to select and process a file or exist program
*/

void userInterface(){
    
    int option;

    do{
        //Prompt the user to enter a choice from 1 to 4.
        printf("1. Select file to process \n");
        printf("2. Exit the program \n");
        printf("\n");
        printf("Enter a choice from 1 to 2: ");
        scanf("%i", &option);

        switch (option){
            case 1:
                printf("\n");
                // allow the user to select and process a file
                ProcessFilePrompt(); 
                break;
            case 2:
                printf("You have existed the program. \n");
                break;
            default:
            
            printf("invalid choice. Please eneter a valid option. \n");
            printf("\n");
        }
    }while (option != 2);
}

int main (int argc, char*argv[]){

    printf("\n");
    userInterface();
    
    return 0;

}

