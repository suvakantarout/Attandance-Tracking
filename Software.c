#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>  

#define MAX_STUDENTS 100
#define MAX_LINE_LENGTH 1024

int teacher_login() {
    char username[50], password[50];
    printf("\nTeacher Login\n");
    printf("Username: ");
    scanf("%49s", username);
    printf("Password: ");
    scanf("%49s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "password") == 0) {
        return 1;
    }
    printf("Invalid credentials!\n");
    return 0;
}

void create_attendance_sheet() {
    char class_name[50];
    printf("\nCreate New Attendance Sheet\n");
    printf("Enter class name: ");
    scanf("%49s", class_name);

    char filename[60];
    sprintf(filename, "%s.csv", class_name);

    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        fclose(fp);
        printf("Attendance sheet already exists for this class!\n");
        return;
    }

    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error creating attendance sheet!\n");
        return;
    }

    fprintf(fp, "Name,RegNo\n");
    fclose(fp);
    printf("Attendance sheet created successfully!\n");
}

void add_student() {
    char class_name[50];
    printf("\nAdd Student\n");
    printf("Enter class name: ");
    scanf("%49s", class_name);

    char filename[60];
    sprintf(filename, "%s.csv", class_name);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Class does not exist!\n");
        return;
    }

    char header[MAX_LINE_LENGTH];
    fgets(header, MAX_LINE_LENGTH, fp);
    int columns = 0;
    char *token = strtok(header, ",");
    while (token != NULL) {
        columns++;
        token = strtok(NULL, ",");
    }
    fclose(fp);

    char name[50], regno[20];
    printf("Enter student name: ");
    scanf(" %49[^\n]", name);
    printf("Enter registration number: ");
    scanf("%19s", regno);

    fp = fopen(filename, "a");
    fprintf(fp, "%s,%s", name, regno);

    for (int i = 2; i < columns; i++) {
        fprintf(fp, ",A");
    }
    fprintf(fp, "\n");
    fclose(fp);
    printf("Student added successfully!\n");
}

void get_current_date(char *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void take_attendance() {
    char class_name[50];
    printf("\nTake Attendance\n");
    printf("Enter class name: ");
    scanf("%49s", class_name);

    char filename[60];
    sprintf(filename, "%s.csv", class_name);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Class does not exist!\n");
        return;
    }

    char date[20];
    get_current_date(date);

    char lines[MAX_STUDENTS][MAX_LINE_LENGTH];
    int line_count = 0;
    while (fgets(lines[line_count], MAX_LINE_LENGTH, fp)) {
        lines[line_count][strcspn(lines[line_count], "\n")] = '\0';
        line_count++;
    }
    fclose(fp);

    if (line_count == 0) {
        printf("Empty attendance sheet!\n");
        return;
    }

    char *header = lines[0];
    if (strstr(header, date) != NULL) {
        printf("Attendance already taken for today (%s)!\n", date);
        return;
    }

    strcat(header, ",");
    strcat(header, date);

    for (int i = 1; i < line_count; i++) {
        char name[50], regno[20];
        char *ptr = lines[i];
        
        sscanf(ptr, "%[^,],%[^,]", name, regno);
        
        printf("Attendance for %s (%s) [P/A]: ", name, regno);
        char status;
        scanf(" %c", &status);
        status = toupper(status);
        
        while (status != 'P' && status != 'A') {
            printf("Invalid input! Enter P or A: ");
            scanf(" %c", &status);
            status = toupper(status);
        }
        
        strcat(lines[i], ",");
        strncat(lines[i], &status, 1);
    }

    fp = fopen(filename, "w");
    for (int i = 0; i < line_count; i++) {
        fprintf(fp, "%s\n", lines[i]);
    }
    fclose(fp);
    printf("Attendance recorded successfully for %s!\n", date);
}
void remove_student() {
    char class_name[50], regno[20];
    printf("\nRemove Student\n");
    printf("Enter class name: ");
    scanf("%49s", class_name);
    printf("Enter registration number: ");
    scanf("%19s", regno);

    char filename[60];
    sprintf(filename, "%s.csv", class_name);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Class does not exist!\n");
        return;
    }

    char lines[MAX_STUDENTS][MAX_LINE_LENGTH];
    int line_count = 0;
    int found = 0;

    while (fgets(lines[line_count], MAX_LINE_LENGTH, fp)) {
        lines[line_count][strcspn(lines[line_count], "\n")] = '\0';
        
        if (line_count > 0) {
            char current_regno[20];
            sscanf(lines[line_count], "%*[^,],%[^,]", current_regno);
            if (strcmp(current_regno, regno) == 0) {
                found = 1;
                line_count--; 
            }
        }
        line_count++;
    }
    fclose(fp);

    if (!found) {
        printf("Student not found!\n");
        return;
    }

    fp = fopen(filename, "w");
    for (int i = 0; i < line_count; i++) {
        fprintf(fp, "%s\n", lines[i]);
    }
    fclose(fp);
    printf("Student removed successfully!\n");
}

void show_attendance() {
    char class_name[50];
    printf("\nShow Attendance\n");
    printf("Enter class name: ");
    scanf("%49s", class_name);

    char filename[60];
    sprintf(filename, "%s.csv", class_name);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Class does not exist!\n");
        return;
    }

    printf("\n%-20s %-15s", "Name", "RegNo");
    
    char line[MAX_LINE_LENGTH];
    fgets(line, MAX_LINE_LENGTH, fp);
    line[strcspn(line, "\n")] = '\0';
    
    char *date = strtok(line, ",");
    date = strtok(NULL, ",");
    while ((date = strtok(NULL, ",")) != NULL) {
        printf(" %-10s", date);
    }
    printf("\n");

    while (fgets(line, MAX_LINE_LENGTH, fp)) {
        line[strcspn(line, "\n")] = '\0';
        char *token = strtok(line, ",");
        printf("%-20s", token);
        token = strtok(NULL, ",");
        printf(" %-15s", token);
        
        while ((token = strtok(NULL, ",")) != NULL) {
            printf(" %-10s", token);
        }
        printf("\n");
    }
    fclose(fp);
}

int main() {
    if (!teacher_login()) {
        return 1;
    }

    int choice;
    do {
        printf("\nAttendance Management System\n");
        printf("1. Create Attendance Sheet\n");
        printf("2. Add Student\n");
        printf("3. Take Attendance\n");
        printf("4. Remove Student\n");
        printf("5. Show Attendance\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: create_attendance_sheet(); break;
            case 2: add_student(); break;
            case 3: take_attendance(); break;
            case 4: remove_student(); break;
            case 5: show_attendance(); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 6);

    return 0;
}