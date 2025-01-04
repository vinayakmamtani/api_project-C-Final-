#include<stdio.h>
#include<stdlib.h>  // for malloc
#include<string.h>  // for string manipulation

// max length for a line
#define MAX_LINE_LEN 1024  
// max length for a field
#define MAX_FIELD_LEN 256  

// define structure for person
typedef struct {
    int roll_num;          //roll number
    char name[MAX_FIELD_LEN];   // name
    char gender[MAX_FIELD_LEN];  // gender
    char place[MAX_FIELD_LEN];   // place
    float cgpa;            // cgpa
} Person;

// trim function to remove leading/trailing spaces
void trim (char *str) {
    int start = 0, end = strlen(str) - 1;

    // trim leading spaces
    while (str[start] == ' ' || str[start] == '\t') {
        start++; 
    }

    // trim trailing spaces
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
        end--;
    }

    // shift string to start without leading spaces
    for (int i = start; i <= end; i++) {
        str[i - start] = str[i]; 
    }

    // null terminate the string
    str[end - start + 1] = '\0';
}

//fetch person details by roll number
Person* get_person_info(const char *file, int roll) {
    FILE *f = fopen(file, "r");
    if (f == NULL) {
        printf("Error opening file.\n"); 
        return NULL;
    }

    char line[MAX_LINE_LEN];

    // skip header line
    if (fgets(line, sizeof(line), f) == NULL) {
        fclose(f);
        return NULL;
    }

    //read each line to find person by roll number
    while (fgets(line, sizeof(line), f)) // file gets string 
     
     
                 {
        Person p; 
        char *token = strtok(line, ","); //split string

        if (token != NULL) {
            //roll number extractingg
            p.roll_num = atoi(token); //convert string- integer

            //extract name
            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(p.name, token, MAX_FIELD_LEN);//used to copy string
                trim(p.name); 
            }

            // extracting gender
            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(p.gender, token, MAX_FIELD_LEN);
                trim(p.gender); 
            }

            // extract places
            token = strtok(NULL, ",");
            if (token != NULL) {
                strncpy(p.place, token, MAX_FIELD_LEN);
                trim(p.place); 
            }

            // extract cgpa
            token = strtok(NULL, ",");
            if (token != NULL) {
                p.cgpa = atof(token);
            }
        }

        // roll number matches, return person info
        if (p.roll_num == roll) {
            fclose(f);
            Person *res = (Person *)malloc(sizeof(Person));  //allocate block of memory
            *res = p; 
            return res;
        }
    }

    fclose(f);
    return NULL;  
}

//display person's information
void display_person_info(Person *p) {
    if (p != NULL) {
        // print details
        printf("Roll Number: %d\n", p->roll_num);
        printf("Name: %s\n", p->name);
        printf("Gender: %s\n", p->gender);
        printf("Place: %s\n", p->place);
        printf("CGPA: %.2f\n", p->cgpa); 
    } else {
        printf("Person not found.\n"); 
    }
}

// function to add new person to file
void add_new_person(const char *file, Person *new_p) {
    FILE *f = fopen(file, "a");
    if (f == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // write new person details to file
    fprintf(f, "%d,%s,%s,%s,%.2f\n", new_p->roll_num, new_p->name,
            new_p->gender, new_p->place, new_p->cgpa); 

    fclose(f); 
    printf("New person added.\n");
}

// function to update an existing person detail
void update_person_info(const char *file, Person *upd_p) {
    FILE *f = fopen(file, "r");
    if (f == NULL) {
        printf("Error opening file.\n");
        return;
    }

    FILE *temp_f = fopen("temp.csv", "w");
    if (temp_f == NULL) {
        printf("Error opening temp file.\n");
        fclose(f);
        return;
    }

    char line[MAX_LINE_LEN]; 
    int found = 0; 

    // write header to temporary file
    fprintf(temp_f, "roll_num,name,gender,place,cgpa\n");

    // read each line from file
    while (fgets(line, sizeof(line), f)) {
        Person p; 
        char *token = strtok(line, ","); 

        // skip header line
        if (line[0] == 'r' && line[1] == 'o') {
            continue; 
        }

        if (token != NULL) {
            p.roll_num = atoi(token); 

            // extract name
            token = strtok(NULL, ","); 
            if (token != NULL) {
                strncpy(p.name, token, MAX_FIELD_LEN); 
            }

            // extract gender
            token = strtok(NULL, ","); 
            if (token != NULL) {
                strncpy(p.gender, token, MAX_FIELD_LEN); 
            }

            // extract place
            token = strtok(NULL, ","); 
            if (token != NULL) {
                strncpy(p.place, token, MAX_FIELD_LEN); 
            }

            // extract cgpa
            token = strtok(NULL, ","); 
            if (token != NULL) {
                p.cgpa = atof(token); 
            }
        }

        //roll number matches, update the details
        if (p.roll_num == upd_p->roll_num) {
            // write updated details to temp file
            fprintf(temp_f, "%d,%s,%s,%s,%.2f\n", upd_p->roll_num,
                    upd_p->name, upd_p->gender, upd_p->place, upd_p->cgpa);
            found = 1; 
        } else {
            // write existing data to temp file
            fprintf(temp_f, "%d,%s,%s,%s,%.2f\n", p.roll_num,
                    p.name, p.gender, p.place, p.cgpa);
        }
    }

    fclose(f);
    fclose(temp_f);

    // replace original file with updated file
    if (found) {
        remove(file); 
        rename("temp.csv", file); 
        printf("Person details updated.\n");
    } else {
        printf("Person not found.\n"); 
    }
}

// function to remove a person from the file
void remove_person(const char *file, int roll) {
    FILE *f = fopen(file, "r");
    if (f == NULL) {
        printf("Error opening file.\n");
        return;
    }

    FILE *temp_f = fopen("temp.csv", "w");
    if (temp_f == NULL) {
        printf("Error opening temp file.\n");
        fclose(f);
        return;
    }

    char line[MAX_LINE_LEN]; 
    int found = 0; 

    // write header to temp file
    fprintf(temp_f, "roll_num,name,gender,place,cgpa\n");

    // read lines from the file
    while (fgets(line, sizeof(line), f)) {
        Person p; 
        char *token = strtok(line, ","); 

        // skip header line
        if (line[0] == 'r' && line[1] == 'o') { 
            continue; 
        }

        if (token != NULL) {
            p.roll_num = atoi(token); 

            // extract name
            token = strtok(NULL, ","); 
            if (token != NULL) {
                strncpy(p.name, token, MAX_FIELD_LEN); 
            }

            // extract gender
            token = strtok(NULL, ","); 
            if (token != NULL) {
                strncpy(p.gender, token, MAX_FIELD_LEN); 
            }

            // extract place
            token = strtok(NULL, ","); 
            if (token != NULL) {
                strncpy(p.place, token, MAX_FIELD_LEN); 
            }

            // extract cgpa
            token = strtok(NULL, ","); 
            if (token != NULL) {
                p.cgpa = atof(token); 
            }
        }

        // if roll number doesn't match, keep the person
        if (p.roll_num != roll) {
            // write existing person to temp file
            fprintf(temp_f, "%d,%s,%s,%s,%.2f\n", p.roll_num,
                    p.name, p.gender, p.place, p.cgpa);
        } else {
            found = 1; 
        }
    }

    fclose(f); 
    fclose(temp_f); 

    // replace the original file with updated file
    if (found) {
        remove(file); 
        rename("temp.csv", file); 
        printf("Person removed.\n");
    } else {
        printf("Person not found.\n"); 
    }
}

// main function
int main() {
    int roll, choice;

    // input roll number
    printf("Enter roll number: ");
    scanf("%d", &roll);
    getchar(); 

    // fetch and display person's info
    Person *p = get_person_info("person.csv", roll);
    display_person_info(p);

    // menu for actions
    printf("Choose action:\n");
    printf("1. Add new person\n");
    printf("2. Update person\n");
    printf("3. Remove person\n");
    printf("Choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        // add new person
        Person new_p;
        printf("Enter roll number: ");
        scanf("%d", &new_p.roll_num);
        getchar(); 
        printf("Enter name: ");
        fgets(new_p.name, sizeof(new_p.name), stdin);
        trim(new_p.name);

        printf("Enter gender: ");
        fgets(new_p.gender, sizeof(new_p.gender), stdin);
        trim(new_p.gender);

        printf("Enter place: ");
        fgets(new_p.place, sizeof(new_p.place), stdin);
        trim(new_p.place);

        printf("Enter CGPA: ");
        scanf("%f", &new_p.cgpa);

        add_new_person("person.csv", &new_p); 
    } else if (choice == 2) {
        // update person's info
        Person upd_p;
        upd_p.roll_num = roll;
        getchar(); 
        printf("Enter updated name: ");
        fgets(upd_p.name, sizeof(upd_p.name), stdin);
        trim(upd_p.name);

        printf("Enter updated gender: ");
        fgets(upd_p.gender, sizeof(upd_p.gender), stdin);
        trim(upd_p.gender);

        printf("Enter updated place: ");
        fgets(upd_p.place, sizeof(upd_p.place), stdin);
        trim(upd_p.place);

        printf("Enter updated CGPA: ");
        scanf("%f", &upd_p.cgpa);

        update_person_info("person.csv", &upd_p); 
    } else if (choice == 3) {
        // remove person
        remove_person("person.csv", roll); 
    }

    free(p); 
    return 0;     
}
