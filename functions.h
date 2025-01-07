#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <gtk/gtk.h>
// Macro Definitions
#define FILE_NAME "ENSTA_Students.txt"
#define NUM_MODULES 4
#define MAX_NAME_LENGTH 40

// Module Names and Coefficients
const char *MODULE_NAMES[] = {"SFSD", "OOP", "Mathematical Analysis", "Linear Algebra"};
const int MODULE_COEFFICIENTS[] = {4, 3, 5, 2};

// Structure for student details
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

void add_student();
void modify_student();
void delete_student();
void extract_by_class();

void uppercase(char class[3])
{
  for (int i = 0; class[i] != '\0'; i++)
  {
    class[i] = toupper(class[i]);
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

void search_student();

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

#endif // MACRO
