#include <stdio.h>  
#include <stdlib.h> // For memory allocation and general utilities
#include <string.h> 

// Define constants for maximum line and field lengths
#define MAX_LINE_LENGTH 1024    // Maximum length of a line in the CSV file
#define MAX_FIELD_LENGTH 256    // Maximum length of each field (name, gender, etc.)

// Define a structure to hold a person's data
typedef struct {
    int roll_number;               
    char name[MAX_FIELD_LENGTH];   
    char gender[MAX_FIELD_LENGTH]; 
    char religion[MAX_FIELD_LENGTH]; 
    float cgpa;                    
} Person;

// Function to remove leading and trailing spaces from a string
void trim(char *str) {
    int start = 0, end = strlen(str) - 1;

    // Remove leading spaces or tabs
    while (str[start] == ' ' || str[start] == '\t') {
        start++;
    }

    // Remove trailing spaces, tabs, or newlines
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
        end--;
    }

    // Shift the valid part of the string to the beginning
    for (int i = start; i <= end; i++) {
        str[i - start] = str[i];
    }

    // Null-terminate the trimmed string
    str[end - start + 1] = '\0';
}

// Function to get a person's information based on their roll number

Person* get_person_info(const char *filename, int roll_number) {
    FILE *file = fopen(filename, "r"); 
    if (file == NULL) {
        printf("Error opening file.\n"); 
        return NULL;
    }

    char line[MAX_LINE_LENGTH]; // Buffer to hold each line from the file


//The fgets() function in C is used to read a line of text from a specified stream (like a file or standard input) and store it in a character array (string).

    // Read and skip the header line
    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return NULL; 
    }

    // Read each line from the file
    while (fgets(line, sizeof(line), file)) {
        Person person;
        char *token = strtok(line, ","); 

        if (token != NULL) {
            // Parse(To analyze a string of characters) roll number
            person.roll_number = atoi(token);

            // Parse and trim each field
            token = strtok(NULL, ","); //strtok is used to split a string according to some conditions.
            if (token != NULL) {
                strncpy(person.name, token, MAX_FIELD_LENGTH); //The strncpy() function in C is a predefined function in the string.h library used to copy a specified number of characters from one string to another.
                trim(person.name);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(person.gender, token, MAX_FIELD_LENGTH);
                trim(person.gender);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(person.religion, token, MAX_FIELD_LENGTH);
                trim(person.religion);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                person.cgpa = atof(token); // Convert CGPA to float
            }
        }

        // If the roll number matches, return the person's information
        if (person.roll_number == roll_number) {
            fclose(file);
            Person *result = (Person *)malloc(sizeof(Person)); // Allocate memory for result
            *result = person; // Copy person's data into allocated memory
            return result;
        }
    }

    fclose(file); // Close the file after reading
    return NULL;  // Return NULL if no match is found
}

// Function to display a person's information
void display_person_info(Person *person) {
    if (person != NULL) {
        // Print person details if found(Here person is the roll number/ID)
        printf("Roll Number: %d, Name: %s, Gender: %s, Religion: %s, CGPA: %.2f\n",
               person->roll_number, person->name, person->gender, person->religion, person->cgpa);
    } else {
        printf("Person not found.\n");
    }
}

// Function to add a new person's information to the CSV file
void add_new_person(const char *filename, Person *new_person) {
    FILE *file = fopen(filename, "a"); // Open file in append mode
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Write the new person's data as a new line
    fprintf(file, "%d,%s,%s,%s,%.2f\n", new_person->roll_number, new_person->name,
            new_person->gender, new_person->religion, new_person->cgpa);

    fclose(file); // Close the file
    printf("New person added successfully.\n");
}

// Function to update a person's information in the CSV file
void update_person_info(const char *filename, Person *updated_person) {
    FILE *file = fopen(filename, "r"); // Open original file for reading
    if (file == NULL) {
        printf("Error opening file for updating.\n");
        return;
    }

    FILE *temp_file = fopen("temp.csv", "w"); // Open a temporary file for writing
    if (temp_file == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int found = 0;

    // Write the header to the temporary file
    fprintf(temp_file, "Roll Number,Name,Gender,Religion,CGPA\n");

    //Print in the format of Roll Number,Name,...
    while (fgets(line, sizeof(line), file)) {
        Person person;
        char *token = strtok(line, ",");

        if (line[0] == 'R' && line[1] == 'o') {
            continue; // Skip header line in original file
        }

        if (token != NULL) {
            person.roll_number = atoi(token);

            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(person.name, token, MAX_FIELD_LENGTH);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(person.gender, token, MAX_FIELD_LENGTH);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(person.religion, token, MAX_FIELD_LENGTH);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                person.cgpa = atof(token);
            }
        }

        // Check if this is the person to update
        if (person.roll_number == updated_person->roll_number) {
            // Write updated information to temp file
            fprintf(temp_file, "%d,%s,%s,%s,%.2f\n", updated_person->roll_number,
                    updated_person->name, updated_person->gender,
                    updated_person->religion, updated_person->cgpa);
            found = 1;
        } else {
            // Write original data for others
            fprintf(temp_file, "%d,%s,%s,%s,%.2f\n", person.roll_number,
                    person.name, person.gender, person.religion, person.cgpa);
        }
    }

    fclose(file);      // Close original file
    fclose(temp_file); // Close temporary file

    if (found) {
        // Replace original file with updated temporary file
        remove(filename);
        rename("temp.csv", filename);
        printf("Person information updated successfully.\n");
    } else {
        printf("Person not found for update.\n");
    }
}

// Function to remove a person from the CSV file
void remove_person(const char *filename, int roll_number) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    FILE *temp_file = fopen("temp.csv", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int found = 0;

    // Write the header to the temporary file
    fprintf(temp_file, "Roll Number,Name,Gender,Religion,CGPA\n");

    while (fgets(line, sizeof(line), file)) {
        Person person;
        char *token = strtok(line, ",");

        if (line[0] == 'R' && line[1] == 'o') {
            continue; // Skip header line
        }

        if (token != NULL) {
            person.roll_number = atoi(token);

            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(person.name, token, MAX_FIELD_LENGTH);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(person.gender, token, MAX_FIELD_LENGTH);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(person.religion, token, MAX_FIELD_LENGTH);
            }

            token = strtok(NULL, ",");
            if (token != NULL) {
                person.cgpa = atof(token);
            }
        }

        if (person.roll_number != roll_number) {
            // Write original data for non-matching records
            fprintf(temp_file, "%d,%s,%s,%s,%.2f\n", person.roll_number,
                    person.name, person.gender, person.religion, person.cgpa);
        } else {
            found = 1; // Mark record for deletion
        }
    }

    fclose(file);
    fclose(temp_file);

    if (found) {
        remove(filename);       // Remove original file
        rename("temp.csv", filename); // Rename temp file to original name
        printf("Person removed successfully.\n");
    } else {
        printf("Person with roll number %d not found.\n", roll_number);
    }
}

// Main function
int main() {
    int roll_number, choice;

    printf("Enter roll number: ");
    scanf("%d", &roll_number);
    getchar(); // Consume newline character left by scanf

    // Fetch and display information for the entered roll number
    Person *person = get_person_info("person.csv", roll_number);
    display_person_info(person);

    printf("Choose an action:\n");
    printf("1. Add new person\n");
    printf("2. Update existing person\n");
    printf("3. Remove person\n");
    printf("Choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        // Add a new person's information
        Person new_person;
        printf("Enter roll number: ");
        scanf("%d", &new_person.roll_number);
        getchar(); // Consume newline
        printf("Enter name: ");
        fgets(new_person.name, sizeof(new_person.name), stdin);
        trim(new_person.name);

        printf("Enter gender: ");
        fgets(new_person.gender, sizeof(new_person.gender), stdin);
        trim(new_person.gender);

        printf("Enter religion: ");
        fgets(new_person.religion, sizeof(new_person.religion), stdin);
        trim(new_person.religion);

        printf("Enter CGPA: ");
        scanf("%f", &new_person.cgpa);

        add_new_person("person.csv", &new_person);
    } else if (choice == 2) {
        // Update an existing person's information
        Person updated_person;
        updated_person.roll_number = roll_number;
        getchar(); // Consume newline
        printf("Enter updated name: ");
        fgets(updated_person.name, sizeof(updated_person.name), stdin);
        trim(updated_person.name);

        printf("Enter updated gender: ");
        fgets(updated_person.gender, sizeof(updated_person.gender), stdin);
        trim(updated_person.gender);

        printf("Enter updated religion: ");
        fgets(updated_person.religion, sizeof(updated_person.religion), stdin);
        trim(updated_person.religion);

        printf("Enter updated CGPA: ");
        scanf("%f", &updated_person.cgpa);

        update_person_info("person.csv", &updated_person);
    } else if (choice == 3) {
        // Remove a person's information
        remove_person("person.csv", roll_number);
    }

    free(person); // Free allocated memory for person
    return 0;     // End program
}
