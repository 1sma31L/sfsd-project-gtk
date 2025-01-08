/**
 * @file
 * @brief This is the main file for the ENSTA Student Management System.
 * It contains the main menu and the implementation of the functions to manage students.
 * @authors BOUSSEKINE Mohamed Ismail, NACERI Rim Serine, FERKIOUI Akram, HARIZI Raouank, AMEDHKOUH Darine, HAMMOUTI Walid
 * @date 2024-12-27
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "functions.h"

#define FILE_NAME "ENSTA_Students.txt"
#define NUM_MODULES 4
#define MAX_NAME_LENGTH 40

const char *MODULE_NAMES[] = {"SFSD", "OOP", "Mathematical Analysis", "Linear Algebra"};
const int MODULE_COEFFICIENTS[] = {4, 3, 5, 2};

/**
 * @struct ENSTA_Student
 * @brief Structure representing a student.
 */
typedef struct
{
  int id;
  char name[MAX_NAME_LENGTH];
  int birthYear;
  char class[3];
  float grades[NUM_MODULES];
  float average;
  bool deleted;
} ENSTA_Student;

/**
 * @brief Procedure to add a student to the file
 * @author Akram
 * @bug Infinite loop when entering a non-integer value
 */
void add_student();

/**
 * @brief Function to check if a student with a given ID exists
 * @author Ismail
 * @param id The ID of the student to check
 * @return A pointer to the student if found, NULL otherwise
 * @bug None
 */
ENSTA_Student *student_exists(int id); // DONE by Ismail

/**
 * @brief Procedure to search for a student by ID
 * @author Ismail
 * @bug None
 */
void search_student();

/**
 * @brief Procedure to modify a student's information
 * @author Soundouss
 * @bug None
 */
void modify_student();

/**
 * @brief Procedure to delete a student from the file
 * @author Raounek
 * @bug None
 */
void delete_student();

/**
 * @brief Additional procedure sed in extract_by_class() to uppercase the class name (could be unnecessary?)
 * @author Serine
 * @param class The class name to extract students from
 * @bug None
 */
void uppercase(char class[3]);

/**
 * @brief Procedure to extract students by class
 * @author Serine
 * @bug None
 */
void extract_by_class();

/**
 * @brief Procedure to reorganize the file by removing logically deleted entries
 * @author Walid
 * @bug None
 */
void reorganize_file();

/**
 * @brief Function to calculate the average of the grades
 * @author Darine
 * @param grades The grades of the student
 * @return The average of the grades
 * @bug None
 */
float grade_average(float grades[4]);

int main()
{
  int choice;
  FILE *file = fopen(FILE_NAME, "a");
  do
  {
    printf("\n--- Student Management System ---\n");
    printf("1. Add Student\n2. Search Student\n3. Modify Student\n");
    printf("4. Delete Student\n5. Extract by Class\n6. Reorganize File\n0. Exit\n");
    printf("Enter your choice: ");
    while (scanf("%d", &choice) != 1)
    {
      printf("Invalid input. Choice must be an integer (0-6), try again: ");
      while (getchar() != '\n')
        ;
    }
    switch (choice)
    {
    case 1:
      add_student();
      break;
    case 2:
      search_student();
      break;
    case 3:
      modify_student();
      break;
    case 4:
      delete_student();
      break;
    case 5:
      extract_by_class();
      break;
    case 6:
      reorganize_file();
      break;
    case 0:
      printf("Exiting...\n");
      fclose(file);
      break;
    default:
      printf("Invalid choice. Try again.\n");
    }
  } while (choice != 0);
  return 0;
}

void add_student()
{
  FILE *file = fopen(FILE_NAME, "r+"); // Open file in read/write mode
  if (!file)
  {
    perror("Error opening file");
    return;
  }

  ENSTA_Student student;
  int idExists;

  do
  {
    idExists = 0; // Reset flag for ID existence

    printf("Enter student ID: ");
    while (scanf("%d", &student.id) != 1)
    {
      printf("Invalid input. Please enter a numeric value for the ID : ");
      while (getchar() != '\n')
        ; // Clear invalid input from buffer
      continue;
    }
    getchar(); // Clear newline from buffer

    // Verify ID uniqueness
    FILE *readFile = fopen(FILE_NAME, "r");
    if (!readFile)
    {
      perror("Error opening file for ID verification");
      fclose(file);
      return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), readFile))
    {
      int existingID;

      // Parse the line to extract the ID
      if (sscanf(buffer, "%d;%*[^\n]", &existingID) == 1)
      {
        if (existingID == student.id)
        {
          idExists = 1;
          printf("Error: Student ID %d already exists. Please enter a unique ID.\n", student.id);
          break;
        }
      }
    }
    fclose(readFile);
  } while (idExists);

  printf("Enter student name (FirstName LastName): ");
  fgets(student.name, MAX_NAME_LENGTH, stdin);
  student.name[strcspn(student.name, "\n")] = '\0';

  do
  {
    printf("Enter student birth year (between 1990 and 2020): ");
    while (scanf("%d", &student.birthYear) != 1)
    {
      printf("Invalid input. Please enter a numeric value : ");
      while (getchar() != '\n')
        ; // Clear invalid input from buffer
      continue;
    }
    if (student.birthYear < 1990 || student.birthYear > 2020)
    {
      printf("Invalid birth year. Please enter a value between 1990 and 2020.\n");
    }
  } while (student.birthYear < 1990 || student.birthYear > 2020);

  printf("Enter student class (e.g., 1A, 2B): ");
  scanf("%s", student.class);
  uppercase(student.class);

  printf("Enter grades for the following modules (between 0 and 20):\n");
  int CorGrade = 1;
  for (int i = 0; i < NUM_MODULES; i++)
  {
    do
    {
      printf("%s: ", MODULE_NAMES[i]);
      if (scanf("%f", &student.grades[i]) != 1)
      {
        printf("Invalid input. Please enter a numeric value.\n");
        CorGrade = 0;
        while (getchar() != '\n')
          ;       // Clear invalid input from buffer
        continue; // Prompt again for the same module
      }
      else
        CorGrade = 1;
      if (student.grades[i] < 0 || student.grades[i] > 20)
      {
        printf("Invalid grade. Please enter a value between 0 and 20.\n");
      }
    } while (student.grades[i] < 0 || student.grades[i] > 20 || CorGrade == 0);
  }

  student.average = grade_average(student.grades);
  student.deleted = 0;

  fprintf(file, "%d;%s;%d;%s;", student.id, student.name, student.birthYear, student.class);
  for (int i = 0; i < NUM_MODULES; i++)
  {
    fprintf(file, "%.2f,%d;", student.grades[i], MODULE_COEFFICIENTS[i]);
  }
  fprintf(file, "%.2f;%d\n", student.average, student.deleted);

  printf("Student added successfully!\n");

  fclose(file);
}

ENSTA_Student *student_exists(int id)
{
  static ENSTA_Student student;
  FILE *file = fopen(FILE_NAME, "r");
  if (!file)
  {
    perror("Error opening file");
    return NULL;
  }

  while (fscanf(file, "%d;%[^;];%d;%[^;];%f,%*d;%f,%*d;%f,%*d;%f,%*d;%f;%d\n",
                &student.id, student.name, &student.birthYear, student.class,
                &student.grades[0], &student.grades[1], &student.grades[2], &student.grades[3],
                &student.average, &student.deleted) == 10)
  {
    if (student.id == id && student.deleted == 0)
    {
      fclose(file);
      return &student;
    }
  }

  fclose(file);
  return NULL;
}

void search_student()
{
  int id;
  printf("Enter the ID of the student to search: ");
  while (scanf("%d", &id) != 1)
  {
    printf("Invalid input. Please enter a valid numeric ID: ");
    while (getchar() != '\n')
      ; // Clear invalid input from buffer
  }

  if (student_exists(id))
  {
    ENSTA_Student *found_student = student_exists(id);
    printf("\nStudent Found:\n");
    printf("ID: %d, Name: %s, Birth Year: %d, Class: %s\n", found_student->id, found_student->name, found_student->birthYear, found_student->class);
    for (int i = 0; i < 4; i++)
    {
      printf("Module: %s Note: %.2f (Coefficient %d)\n", MODULE_NAMES[i], found_student->grades[i], MODULE_COEFFICIENTS[i]);
    }
    printf("Average: %.2f\n", found_student->average);
  }
  else
  {
    printf("Student not found.\n");
  }
}

void modify_student()
{
  FILE *file = fopen(FILE_NAME, "r");
  if (!file)
  {
    perror("Error opening file");
    return;
  }

  int id;
  printf("Enter the ID of the student to modify: ");
  while (scanf("%d", &id) != 1)
  {
    printf("Invalid input. Please enter a valid numeric ID: ");
    while (getchar() != '\n')
      ; // Clear invalid input from buffer
  }

  ENSTA_Student students[100];
  int count = 0;
  int found = 0;

  // Read all students into memory
  while (fscanf(file, "%d;%[^;];%d;%[^;];%f,%*d;%f,%*d;%f,%*d;%f,%*d;%f;%d\n",
                &students[count].id, students[count].name, &students[count].birthYear, students[count].class,
                &students[count].grades[0], &students[count].grades[1], &students[count].grades[2], &students[count].grades[3],
                &students[count].average, &students[count].deleted) == 10)
  {
    if (students[count].id == id && students[count].deleted == 0)
    {
      found = 1;
      // nnrmlmnt retun psq cbn l9inah
    }
    count++;
  }
  fclose(file);

  if (!found)
  {
    printf("Student not found.\n");
    return;
  }

  // Modify the student
  for (int i = 0; i < count; i++)
  {
    if (students[i].id == id && students[i].deleted == 0)
    {
      printf("Modify Name (current: %s): ", students[i].name);
      getchar(); // Clear buffer
      fgets(students[i].name, MAX_NAME_LENGTH, stdin);
      students[i].name[strcspn(students[i].name, "\n")] = '\0';

      printf("Modify Class (current: %s): ", students[i].class);
      scanf("%s", students[i].class);

      printf("Modify Grades:\n");
      for (int j = 0; j < NUM_MODULES; j++)
      {
        printf("%s (current: %.2f): ", MODULE_NAMES[j], students[i].grades[j]);
        scanf("%f", &students[i].grades[j]);
      }

      students[i].average = grade_average(students[i].grades);
      break;
    }
  }

  // Write all students back to the file
  file = fopen(FILE_NAME, "w");
  if (!file)
  {
    perror("Error opening file");
    return;
  }

  for (int i = 0; i < count; i++)
  {
    fprintf(file, "%d;%s;%d;%s;", students[i].id, students[i].name, students[i].birthYear, students[i].class);
    for (int j = 0; j < NUM_MODULES; j++)
    {
      fprintf(file, "%.2f,%d;", students[i].grades[j], MODULE_COEFFICIENTS[j]);
    }
    fprintf(file, "%.2f;%d\n", students[i].average, students[i].deleted);
  }

  printf("Student updated successfully!\n");

  fclose(file);
}

void delete_student()
{
  FILE *file = fopen(FILE_NAME, "r+");
  if (!file)
  {
    perror("Error opening file");
    return;
  }

  int id;
  int valid_input = 0; // Flag for valid integer input
  int valid_id = 0;    // Flag for existing ID
  ENSTA_Student student;

  // Ask for valid ID until provided
  printf("Enter the ID of the student to delete: ");
  while (!valid_input)
  {

    if (scanf("%d", &id) != 1) // Check if input is not an integer
    {
      printf("Invalid input. Please enter a valid numeric ID:\n");
      while (getchar() != '\n')
        ; // Clear invalid input from buffer
      continue;
    }
    valid_input = 1; // Mark input as valid if an integer is entered
  }

  rewind(file); // Reset file pointer to the beginning

  // Check if the entered ID exists and is not already deleted
  while (fscanf(file, "%d;%[^;];%d;%[^;];%f,%*d;%f,%*d;%f,%*d;%f,%*d;%f;%d\n",
                &student.id, student.name, &student.birthYear, student.class,
                &student.grades[0], &student.grades[1], &student.grades[2], &student.grades[3],
                &student.average, &student.deleted) == 10)
  {
    if (student.id == id && student.deleted == 0)
    {
      valid_id = 1;
      break;
    }
  }

  if (!valid_id) // If no valid ID is found, exit the function
  {
    printf("The entered ID does not exist or the student is already deleted.\n");
    fclose(file);
    return;
  }

  // Reset file pointer to modify the student record
  rewind(file);

  long pos;
  while ((pos = ftell(file)) >= 0 && fscanf(file, "%d;%[^;];%d;%[^;];%f,%*d;%f,%*d;%f,%*d;%f,%*d;%f;%d\n",
                                            &student.id, student.name, &student.birthYear, student.class,
                                            &student.grades[0], &student.grades[1], &student.grades[2], &student.grades[3],
                                            &student.average, &student.deleted) == 10)
  {
    if (student.id == id && student.deleted == 0)
    {
      student.deleted = 1;
      fseek(file, pos, SEEK_SET);
      fprintf(file, "%d;%s;%d;%s;%.2f,0;%.2f,0;%.2f,0;%.2f,0;%.2f;%d\n",
              student.id, student.name, student.birthYear, student.class,
              student.grades[0], student.grades[1], student.grades[2], student.grades[3],
              student.average, student.deleted);
      printf("Student with ID %d deleted successfully.\n", id);
      break;
    }
  }

  fclose(file);
}

void uppercase(char class[3])
{
  for (int i = 0; class[i] != '\0'; i++)
  {
    class[i] = toupper(class[i]);
  }
}

void extract_by_class()
{
  // opening the file and checking for its existence
  FILE *file = fopen(FILE_NAME, "r");
  if (!file)
  {
    perror("Error opening file");
    return;
  }

  // variable declarations
  char classm[3];
  ENSTA_Student student[300];
  int match = 0;

  // getting user input (class name)
  printf("Enter the class name to extract (e.g., 3a, 3B): ");
  scanf("%s", classm);

  // checking for class name validation
  if (strlen(classm) != 2)
  {
    do
    {
      printf("Invalid class name. Please enter a valid one: ");
      scanf("%s", classm);

    } while (strlen(classm) != 2);
  }
  uppercase(classm);

  // scanning for class matches and simultaneously storing them in an array
  while (fscanf(file, "%d;%[^;];%d;%[^;];%f,%*d;%f,%*d;%f,%*d;%f,%*d;%f;%d\n",
                &student[match].id, student[match].name, &student[match].birthYear, student[match].class,
                &student[match].grades[0],
                &student[match].grades[1],
                &student[match].grades[2],
                &student[match].grades[3],
                &student[match].average, &student[match].deleted) == 10)
  {
    if (strcmp(student[match].class, classm) == 0 && student[match].deleted == 0)
    {
      match++; // incrementing the match counter to store the next student with a matching class
    }
  }

  fclose(file);

  if (match == 0)
  {
    printf("No students were found in the class %s. \n", classm);
    return;
  }

  // sorting students by their average in descending order
  for (int i = 1; i < match; i++)
  {
    ENSTA_Student pivot = student[i]; // picking the current student as the pivot (to compare to)
    int j = i - 1;                    // setting the previous student from the sorted part of the array as the comparator

    // moving the elements (student[0] .. student[j]) that have an average less than pivot.average one position to the right
    while (j >= 0 && student[j].average < pivot.average)
    {
      student[j + 1] = student[j]; // shifting student[j] to the right
      j -= 1;                      // moving one position to left to keep comparing (in the sorted part of the array) until j = -1
    }

    student[j + 1] = pivot; // placing the pivot in its correct position withing the sorted part of the array
  }

  // printing the sorted students
  printf("\nStudents in class %s sorted by average in decreasing order are:\n", classm);
  for (int i = 0; i < match; i++)
  {
    printf("ID: %d, Name: %s, Average: %.2f\n", student[i].id, student[i].name, student[i].average);
  }
}

void reorganize_file()
{

  FILE *f = fopen(FILE_NAME, "r");

  if (!f)
  {
    printf("ERROR:can't open the file %s", FILE_NAME);
    return;
  }

  FILE *tmp = fopen("tmp.txt", "w");

  if (!tmp)
  {
    printf("ERROR:can't open the file tmp.txt");
    fclose(f);
    return;
  }

  char StudentInfo[120];
  while (fgets(StudentInfo, sizeof(StudentInfo), f))
  {
    if (StudentInfo[strlen(StudentInfo) - 2] == '0')
    {

      fputs(StudentInfo, tmp);
    }
  }
  fclose(f);
  fclose(tmp);
  if (remove(FILE_NAME) != 0)
  {
    printf("ERROR:can't delete the file %s", FILE_NAME);
  }
  if (rename("tmp.txt", FILE_NAME) != 0)
  {
    perror("Error renaming the temporary file");
  }
  else
  {
    printf("Reorganization and deletion completed successfully.\n");
  }
  return;
}

float grade_average(float grades[NUM_MODULES])
{
  float sum = 0;
  int sum_of_coefficients = 0;
  for (int i = 0; i < NUM_MODULES; i++)
  {
    sum += grades[i] * MODULE_COEFFICIENTS[i];
    sum_of_coefficients += MODULE_COEFFICIENTS[i];
  }
  return sum / sum_of_coefficients;
}