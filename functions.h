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
void uppercase();
void reorganize_file();
ENSTA_Student *student_exists(int id);
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
