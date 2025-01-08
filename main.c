#include "functions.h"
#include <gtk/gtk.h>

static void ok_clicked(GtkButton *button, gpointer user_data)
{
  GtkWidget **widgets_modify = (GtkWidget **)user_data;
  FILE *file = fopen(FILE_NAME, "r");
  if (!file)
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Could not open file!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  int id = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets_modify[7]));
  // Retrieve the values when the user modifies the data
  ENSTA_Student modified_student;
  strncpy(modified_student.name, gtk_entry_get_text(GTK_ENTRY(widgets_modify[0])), sizeof(modified_student.name) - 1);
  modified_student.name[sizeof(modified_student.name) - 1] = '\0';
  modified_student.birthYear = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets_modify[1]));
  strncpy(modified_student.class, gtk_entry_get_text(GTK_ENTRY(widgets_modify[2])), sizeof(modified_student.class) - 1);
  modified_student.class[sizeof(modified_student.class) - 1] = '\0';
  uppercase(modified_student.class);
  modified_student.grades[0] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets_modify[3]));
  modified_student.grades[1] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets_modify[4]));
  modified_student.grades[2] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets_modify[5]));
  modified_student.grades[3] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets_modify[6]));
  modified_student.average = (modified_student.grades[0] + modified_student.grades[1] + modified_student.grades[2] + modified_student.grades[3]) / 4;

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
    }

    count++;
  }
  fclose(file);

  // Modify the student
  for (int i = 0; i < count; i++)
  {
    if (students[i].id == id && students[i].deleted == 0)
    {

      strncpy(students[i].name, modified_student.name, sizeof(students[i].name) - 1);
      students[i].name[sizeof(students[i].name)] = '\0';

      strncpy(students[i].class, modified_student.class, sizeof(students[i].class) - 1);
      students[i].class[sizeof(students[i].class) - 1] = '\0';

      for (int j = 0; j < NUM_MODULES; j++)
      {
        students[i].grades[j] = modified_student.grades[j];
      }
      students[i].birthYear = modified_student.birthYear;
      students[i].average = grade_average(students[i].grades);
      break;
    }
  }

  // Write all students back to the file
  file = fopen(FILE_NAME, "w");
  if (!file)
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Could not open file!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
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

  fclose(file);
  gtk_widget_destroy(widgets_modify[8]);

  GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Student updated successfully!");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

  return;
}

static void modify_clicked(GtkButton *button, gpointer user_data)
{

  GtkWidget *widget8 = GTK_WIDGET(user_data);

  FILE *file = fopen(FILE_NAME, "r");
  if (!file)
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Could not open file!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  int id = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget8));

  ENSTA_Student students[100];
  int count = 0;
  int found = 0;
  int pos = 0;

  // Read all students into memory
  while (fscanf(file, "%d;%[^;];%d;%[^;];%f,%*d;%f,%*d;%f,%*d;%f,%*d;%f;%d\n",
                &students[count].id, students[count].name, &students[count].birthYear, students[count].class,
                &students[count].grades[0], &students[count].grades[1], &students[count].grades[2], &students[count].grades[3],
                &students[count].average, &students[count].deleted) == 10)
  {
    if (students[count].id == id && students[count].deleted == 0)
    {
      found = 1;
      pos = count;
      // nnrmlmnt retun psq cbn l9inah
    }
    count++;
  }
  fclose(file);

  if (!found)
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Student not found");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  GtkWidget *window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  char title[30];
  for (int i = 0; i < count; i++)
  {
    if (students[i].id == id)
    {
      snprintf(title, sizeof(title), "%s", students[i].name); // Set the title to student's name
      break;                                                  // Exit the loop once the student is found
    }
  }
  char *title1 = g_strdup_printf("Modify  %s  Informations", title);

  gtk_window_set_title(GTK_WINDOW(window2), title1);
  gtk_window_set_default_size(GTK_WINDOW(window2), 400, 300);
  gtk_window_set_position(GTK_WINDOW(window2), GTK_WIN_POS_CENTER);

  GtkWidget *modify_grid = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(modify_grid), 20);
  gtk_container_add(GTK_CONTAINER(window2), modify_grid);

  GtkWidget *modify_full_name = gtk_entry_new();
  GtkWidget *modify_birth_year = gtk_spin_button_new_with_range(1990, 2020, 1);
  GtkWidget *modify_class = gtk_entry_new();
  GtkWidget *modify_sfsd = gtk_spin_button_new_with_range(0, 20.0, 0.5);
  GtkWidget *modify_oop = gtk_spin_button_new_with_range(0, 20.0, 0.5);
  GtkWidget *modify_analysis = gtk_spin_button_new_with_range(0, 20.0, 0.5);
  GtkWidget *modify_algebra = gtk_spin_button_new_with_range(0, 20.0, 0.5);

  gtk_grid_attach(GTK_GRID(modify_grid), modify_full_name, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_birth_year, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_class, 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_sfsd, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_oop, 1, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_analysis, 1, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_algebra, 1, 6, 1, 1);

  GtkWidget *modify_labname = gtk_label_new("Enter the Full Name:");
  GtkWidget *modify_labyear = gtk_label_new("Enter the Birth Year:");
  GtkWidget *modify_labclass = gtk_label_new("Enter the Class:");
  GtkWidget *modify_labesfsd = gtk_label_new("Enter the SFSD Grade:");
  GtkWidget *modify_labeoop = gtk_label_new("Enter the OOP Grade:");
  GtkWidget *modify_labeanalyse = gtk_label_new("Enter the Analysis Grade:");
  GtkWidget *modify_labalgebra = gtk_label_new("Enter the Algebra Grade:");

  gtk_grid_attach(GTK_GRID(modify_grid), modify_labname, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_labyear, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_labclass, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_labesfsd, 0, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_labeoop, 0, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_labeanalyse, 0, 5, 1, 1);
  gtk_grid_attach(GTK_GRID(modify_grid), modify_labalgebra, 0, 6, 1, 1);

  // Retrieve the values;
  // Initialize fields with the current student's data
  gtk_entry_set_text(GTK_ENTRY(modify_full_name), students[pos].name);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(modify_birth_year), students[pos].birthYear);
  gtk_entry_set_text(GTK_ENTRY(modify_class), students[pos].class);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(modify_sfsd), students[pos].grades[0]);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(modify_oop), students[pos].grades[1]);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(modify_analysis), students[pos].grades[2]);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(modify_algebra), students[pos].grades[3]);

  GtkWidget **widgets_modify = malloc(9 * sizeof(GtkWidget *));
  widgets_modify[0] = GTK_WIDGET(modify_full_name);
  widgets_modify[1] = GTK_WIDGET(modify_birth_year);
  widgets_modify[2] = GTK_WIDGET(modify_class);
  widgets_modify[3] = GTK_WIDGET(modify_sfsd);
  widgets_modify[4] = GTK_WIDGET(modify_oop);
  widgets_modify[5] = GTK_WIDGET(modify_analysis);
  widgets_modify[6] = GTK_WIDGET(modify_algebra);
  widgets_modify[7] = widget8;
  widgets_modify[8] = GTK_WIDGET(window2);

  GtkWidget *ok_modify_button = gtk_button_new_with_label("OK");
  gtk_grid_attach(GTK_GRID(modify_grid), ok_modify_button, 0, 7, 1, 1);
  g_signal_connect(ok_modify_button, "clicked", G_CALLBACK(ok_clicked), widgets_modify);

  gtk_widget_show_all(window2);
}
static void extract_clicked(GtkButton *button, gpointer user_data)
{
  GtkWidget *widget9 = GTK_WIDGET(user_data);

  FILE *file = fopen(FILE_NAME, "r");
  if (!file)
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Could not open file!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  // variable declarations
  char classm[3];
  ENSTA_Student student[100];
  int match = 0;

  strncpy(classm, gtk_entry_get_text(GTK_ENTRY(widget9)), sizeof(classm) - 1);
  classm[sizeof(classm) - 1] = '\0';
  if (classm[0] == '\0')
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Enter the class");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  uppercase(classm);

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
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "No students were found in the class %s.", classm);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  // Create a new window (window1) to display the student list
  GtkWidget *window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window1), "Student List");
  gtk_window_set_default_size(GTK_WINDOW(window1), 400, 300);
  gtk_window_set_position(GTK_WINDOW(window1), GTK_WIN_POS_CENTER);

  // Create a scrolled window and add it to the window1
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(window1), scrolled_window);

  // Create a text view and add it to the scrolled window
  GtkWidget *text_view = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD);
  gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

  // Get the text buffer of the text view to append the student information
  GtkTextBuffer *text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

  // printing the sorted students
  GtkTextIter iter;
  char line[50]; // Temporary buffer for each formatted line
  for (int i = 0; i < match; i++)
  {
    // Format the student's data into the line buffer
    sprintf(line, "ID: %d, Name: %s, Average: %.2f\n", student[i].id, student[i].name, student[i].average);

    // Get the end of the text buffer
    gtk_text_buffer_get_end_iter(text_buffer, &iter);

    // Append the formatted line to the text buffer
    gtk_text_buffer_insert(text_buffer, &iter, line, -1);
  }

  // Show the window and all its contents
  gtk_widget_show_all(window1);

  // Connect the delete-event signal to destroy the window when closed
  g_signal_connect(window1, "delete-event", G_CALLBACK(gtk_widget_destroy), NULL);
}

static void delete_clicked(GtkButton *button, gpointer user_data)
{

  GtkWidget *widget8 = GTK_WIDGET(user_data);

  FILE *file = fopen(FILE_NAME, "r+");
  if (!file)
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Could not open file!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  int id = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget8));

  int valid_id = 0; // Flag for existing ID
  ENSTA_Student student;
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
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "The entered ID does not exist or the student is already deleted.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
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

      GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Student with ID %d deleted successfully.", id);
      gtk_dialog_run(GTK_DIALOG(dialog));
      gtk_widget_destroy(dialog);
      break;
    }
  }

  fclose(file);
}

static void reorganize_clicked(GtkButton *button, gpointer user_data)
{
  FILE *f = fopen(FILE_NAME, "r");

  if (!f)
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Could not open file!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  FILE *tmp = fopen("tmp.txt", "w");

  if (!tmp)
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Could not open temp file!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    fclose(tmp);
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
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: couldn't delete the file");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
  if (rename("tmp.txt", FILE_NAME) != 0)
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error renaming the temporary file");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
  else
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Reorganization and deletion completed successfully.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
  }
  return;
}

/**
 * @brief Search for a student by ID and display their information.
 * @param button
 * @param user_data
 */
static void search_student_clicked(GtkButton *button, gpointer user_data)
{
  // Get the GtkSpinButton widget (passed as user_data)
  GtkWidget *widget8 = GTK_WIDGET(user_data);

  // Retrieve the entered ID
  int id = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget8));

  // Check if the student exists
  ENSTA_Student *found_student = student_exists(id);
  if (!found_student)
  {
    // Show an error dialog if the student ID does not exist
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_OK,
                                               "Error: Student ID %d does not exist!", id);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  // Prepare a buffer to store the student's data
  char info[1024]; // Adjust size as needed to fit all the data

  // Format the student details into the buffer
  snprintf(info, sizeof(info),
           "ID: %d\n"
           "Name: %s\n"
           "Birth Year: %d\n"
           "Class: %s\n",
           found_student->id,
           found_student->name,
           found_student->birthYear,
           found_student->class);

  // Append module data to the buffer
  for (int i = 0; i < 4; i++)
  {
    char module_info[128];
    snprintf(module_info, sizeof(module_info),
             "Module: %s\nNote: %.2f (Coefficient: %d)\n",
             MODULE_NAMES[i], found_student->grades[i], MODULE_COEFFICIENTS[i]);
    strncat(info, module_info, sizeof(info) - strlen(info) - 1);
  }

  // Append the average to the buffer
  char average_info[64];
  snprintf(average_info, sizeof(average_info), "Average: %.2f\n", found_student->average);
  strncat(info, average_info, sizeof(info) - strlen(info) - 1);

  // Create a message dialog to display the student information
  GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                             GTK_DIALOG_MODAL,
                                             GTK_MESSAGE_INFO,
                                             GTK_BUTTONS_OK,
                                             "%s", info);

  // Show the dialog and wait for the user to respond
  gtk_dialog_run(GTK_DIALOG(dialog));

  // Destroy the dialog after it is closed
  gtk_widget_destroy(dialog);
}

/**
 * @brief Add a new student to the file.
 * @param button
 * @param user_data
 */
static void add_student_clicked(GtkButton *button, gpointer user_data)
{
  // Retrieve the user data (entry_widgets) passed in
  GtkWidget **widgets = (GtkWidget **)user_data;

  // Retrieve the values;
  ENSTA_Student student;
  student.id = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets[0]));
  strncpy(student.name, gtk_entry_get_text(GTK_ENTRY(widgets[1])), sizeof(student.name) - 1);
  student.name[sizeof(student.name) - 1] = '\0';
  student.birthYear = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets[2]));
  strncpy(student.class, gtk_entry_get_text(GTK_ENTRY(widgets[3])), sizeof(student.class) - 1);
  student.class[sizeof(student.class) - 1] = '\0';

  for (int i = 0; i < 4; i++)
  {
    student.grades[i] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets[i + 4]));
  }

  student.average = grade_average(student.grades);

  student.deleted = 0;
  uppercase(student.class);
  if (student.name[0] == '\0')
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error:Enter the Full name");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  if (student.class[0] == '\0')
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error:Enter the class");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }
  // Check for ID uniqueness in the file
  FILE *readFile = fopen(FILE_NAME, "r");
  if (!readFile)
  {
    // If the file does not exist, proceed with adding the student
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Could not open file!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  int idExists = 0;
  char buffer[256];
  while (fgets(buffer, sizeof(buffer), readFile))
  {
    int existingID;
    // Parse the line to extract the ID
    if (sscanf(buffer, "%d;%*[^\n]", &existingID) == 1)
    {
      if (existingID == student.id)
      {
        // ID already exists, show error message
        idExists = 1;
        break;
      }
    }
  }
  fclose(readFile);

  if (idExists)
  {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Student ID already exists!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;
  }

  // write in the file;
  FILE *writefile = fopen(FILE_NAME, "a+");

  fprintf(writefile, "%d;%s;%d;%s;", student.id, student.name, student.birthYear, student.class);
  for (int i = 0; i < NUM_MODULES; i++)
  {
    fprintf(writefile, "%.2f,%d;", student.grades[i], MODULE_COEFFICIENTS[i]);
  }
  fprintf(writefile, "%.2f;%d\n", student.average, student.deleted);

  fclose(writefile);
  GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Student added successfully!");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

/**
 * @brief Activate the application and create the main window.
 * @param app
 * @param user_data
 */
static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *grid, *button, *add_modify_box, *search_delete_box, *extract_box, *reorganize_box;
  GtkWidget *search_button, *add_button, *Modify_button, *Extract_button, *reorganize_button, *delete_button;
  // creat a programme window;
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "STUDENT MANAGEMENT SYSTEM");
  gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);
  // Get screen dimensions using the newer GdkDisplay API
  GdkDisplay *display = gdk_display_get_default();
  GdkMonitor *monitor = gdk_display_get_primary_monitor(display); // Primary monitor
  GdkRectangle monitor_geometry;
  gdk_monitor_get_geometry(monitor, &monitor_geometry);
  gint screen_width = monitor_geometry.width;
  gint screen_height = monitor_geometry.height;

  // Set the window size to adapt to screen size
  gtk_window_set_default_size(GTK_WINDOW(window), screen_width * 0.8, screen_height * 0.8); // 80% of screen size
  // creat a grid;
  grid = gtk_grid_new();

  gtk_container_add(GTK_CONTAINER(window), grid);

  GtkCssProvider *provider = gtk_css_provider_new();
  GdkScreen *screen = gdk_display_get_default_screen(display);

  gtk_style_context_add_provider_for_screen(
      screen,
      GTK_STYLE_PROVIDER(provider),
      GTK_STYLE_PROVIDER_PRIORITY_USER);

  gtk_css_provider_load_from_path(provider, "style.css", NULL);
  g_object_unref(provider);

  // creat the boxes;
  GtkWidget *title_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
  GtkWidget *label = gtk_label_new("Student Managment System");
  gtk_container_add(GTK_CONTAINER(title_box), label);

  /// the first box:
  add_modify_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
  GtkWidget *grid1 = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(add_modify_box), grid1);
  gtk_grid_set_row_spacing(GTK_GRID(grid1), 20);
  gtk_grid_set_column_spacing(GTK_GRID(grid1), 30);

  /// the second box
  search_delete_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
  GtkWidget *grid2 = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(search_delete_box), grid2);
  gtk_grid_set_row_spacing(GTK_GRID(grid2), 20);
  gtk_grid_set_column_spacing(GTK_GRID(grid2), 30);

  /// the third box
  extract_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget *grid3 = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(extract_box), grid3);
  gtk_grid_set_row_spacing(GTK_GRID(grid3), 20);
  gtk_grid_set_column_spacing(GTK_GRID(grid3), 30);

  /// the fourth box
  reorganize_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  // add the boxes to the main grid
  gtk_grid_attach(GTK_GRID(grid), title_box, 0, 0, 3, 1);
  gtk_grid_attach(GTK_GRID(grid), add_modify_box, 0, 1, 3, 2);
  gtk_grid_attach(GTK_GRID(grid), search_delete_box, 0, 3, 3, 2);
  gtk_grid_attach(GTK_GRID(grid), extract_box, 0, 5, 3, 1);
  gtk_grid_attach(GTK_GRID(grid), reorganize_box, 0, 6, 3, 1);

  // Create buttons for each operation
  add_button = gtk_button_new_with_label("Add Student  ➕");
  search_button = gtk_button_new_with_label("Search Student  🔍");
  Modify_button = gtk_button_new_with_label("Modify Student  ✍️");
  Extract_button = gtk_button_new_with_label("Extract by Class  📤");
  reorganize_button = gtk_button_new_with_label("Reorganize File  🗄️");
  delete_button = gtk_button_new_with_label("Delete Student  🚮");

  // add buttons to the boxes;
  gtk_grid_attach(GTK_GRID(grid1), add_button, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid2), Modify_button, 0, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid2), search_button, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid2), delete_button, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid3), Extract_button, 0, 0, 1, 1);
  gtk_box_pack_start(GTK_BOX(reorganize_box), reorganize_button, false, false, 0);

  // add margine between the boxes;
  gtk_widget_set_margin_top(search_delete_box, 100);
  gtk_widget_set_margin_top(extract_box, 100);
  gtk_widget_set_margin_top(reorganize_box, 50);

  // buttons resize;
  gtk_widget_set_size_request(add_button, 100, 70);
  gtk_widget_set_size_request(Modify_button, 100, 70);
  gtk_widget_set_margin_end(add_button, 70);
  gtk_widget_set_margin_end(Modify_button, 70);
  gtk_widget_set_size_request(search_button, 100, 70);
  gtk_widget_set_size_request(delete_button, 100, 70);
  gtk_widget_set_margin_end(search_button, 70);
  gtk_widget_set_margin_end(delete_button, 70);
  gtk_widget_set_size_request(Extract_button, 100, 70);
  gtk_widget_set_margin_end(Extract_button, 70);
  gtk_widget_set_size_request(reorganize_button, 350, 70);

  // add entries and spins of add_modify_box ;
  GtkWidget *id = gtk_spin_button_new_with_range(0, G_MAXDOUBLE, 1);
  GtkWidget *full_name = gtk_entry_new();
  GtkWidget *birth_year = gtk_spin_button_new_with_range(1990, 2020, 1);
  GtkWidget *classe = gtk_entry_new();
  GtkWidget *sfsd = gtk_spin_button_new_with_range(0, 20.0, 0.5);
  GtkWidget *oop = gtk_spin_button_new_with_range(0, 20.0, 0.5);
  GtkWidget *analysis = gtk_spin_button_new_with_range(0, 20.0, 0.5);
  GtkWidget *algebra = gtk_spin_button_new_with_range(0, 20.0, 0.5);

  gtk_spin_button_set_value(GTK_SPIN_BUTTON(birth_year), 2005);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(sfsd), 10.0);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(oop), 10.0);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(analysis), 10.0);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(algebra), 10.0);

  gtk_grid_attach(GTK_GRID(grid1), id, 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), full_name, 4, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), birth_year, 6, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), classe, 8, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), sfsd, 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), oop, 4, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), analysis, 6, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), algebra, 8, 1, 1, 1);

  // a entries and spins of search_delete_box ;

  GtkWidget *search_delete_id = gtk_spin_button_new_with_range(0, G_MAXDOUBLE, 1);
  gtk_grid_attach(GTK_GRID(grid2), search_delete_id, 3, 1, 2, 1);

  // // add entries and spins of extract_box ;
  GtkWidget *extract_class = gtk_entry_new();
  gtk_grid_attach(GTK_GRID(grid3), extract_class, 3, 0, 2, 1);

  // entries resize
  gtk_widget_set_size_request(sfsd, 100, 70); // the remining entries oop analysis algebra follow the sfsd entry size;

  // buttons labels:
  GtkWidget *labid = gtk_label_new("ID:");
  GtkWidget *labname = gtk_label_new("Full Name:");
  GtkWidget *labyear = gtk_label_new("Birth Year:");
  GtkWidget *labclass = gtk_label_new("Class:");
  GtkWidget *labesfsd = gtk_label_new("SFSD Grade:");
  GtkWidget *labeoop = gtk_label_new("OOP Grade:");
  GtkWidget *labeanalyse = gtk_label_new("Analysis Grade:");
  GtkWidget *labalgebra = gtk_label_new("Algebra Grade:");
  GtkWidget *labsearch_delete_id = gtk_label_new("ID:");
  GtkWidget *labextract_class = gtk_label_new("Class:");

  gtk_grid_attach(GTK_GRID(grid1), labid, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), labname, 3, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), labyear, 5, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), labclass, 7, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), labesfsd, 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), labeoop, 3, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), labeanalyse, 5, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid1), labalgebra, 7, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid2), labsearch_delete_id, 1, 1, 1, 1);
  gtk_widget_set_margin_end(labsearch_delete_id, 45);
  gtk_grid_attach(GTK_GRID(grid3), labextract_class, 1, 0, 1, 1);
  gtk_widget_set_margin_end(labextract_class, 20);

  // apply css in buttons;
  gtk_widget_set_name(add_button, "add_button");
  gtk_widget_set_name(search_button, "search_button");
  gtk_widget_set_name(Modify_button, "Modify_button");
  gtk_widget_set_name(Extract_button, "Extract_button");
  gtk_widget_set_name(reorganize_button, "reorganize_button");
  gtk_widget_set_name(delete_button, "delete_button");
  // NEW ONES
  gtk_widget_set_name(id, "id");
  gtk_widget_set_name(full_name, "full_name");
  gtk_entry_set_placeholder_text(GTK_ENTRY(full_name), "John Doe");
  gtk_widget_set_name(birth_year, "birth_year");
  gtk_widget_set_name(classe, "classe");
  gtk_entry_set_placeholder_text(GTK_ENTRY(classe), "A1");
  gtk_widget_set_name(sfsd, "sfsd");
  gtk_widget_set_name(oop, "oop");
  gtk_widget_set_name(analysis, "analysis");
  gtk_widget_set_name(algebra, "algebra");
  gtk_widget_set_name(search_delete_id, "search_delete_id");
  gtk_widget_set_name(extract_class, "extract_class");
  gtk_entry_set_placeholder_text(GTK_ENTRY(extract_class), "A1");

  // NEW NEW ONES
  gtk_widget_set_name(labid, "labid");
  gtk_widget_set_name(labname, "labname");
  gtk_widget_set_name(labyear, "labyear");
  gtk_widget_set_name(labclass, "labclass");
  gtk_widget_set_name(labesfsd, "labesfsd");
  gtk_widget_set_name(labeoop, "labeoop");
  gtk_widget_set_name(labeanalyse, "labeanalyse");
  gtk_widget_set_name(labalgebra, "labalgebra");
  gtk_widget_set_name(labsearch_delete_id, "labsearch_delete_id");
  gtk_widget_set_name(labextract_class, "labextract_class");
  gtk_widget_set_name(label, "label");
  // Set the horizontal alignment of the label to the start of the cell
  gtk_widget_set_halign(labid, GTK_ALIGN_START);
  gtk_widget_set_halign(labname, GTK_ALIGN_START);
  gtk_widget_set_halign(labyear, GTK_ALIGN_START);
  gtk_widget_set_halign(labclass, GTK_ALIGN_START);
  gtk_widget_set_halign(labesfsd, GTK_ALIGN_START);
  gtk_widget_set_halign(labeoop, GTK_ALIGN_START);
  gtk_widget_set_halign(labeanalyse, GTK_ALIGN_START);
  gtk_widget_set_halign(labalgebra, GTK_ALIGN_START);
  gtk_widget_set_halign(labsearch_delete_id, GTK_ALIGN_START);
  gtk_widget_set_halign(labextract_class, GTK_ALIGN_START);

  GtkWidget **widgets = malloc(10 * sizeof(GtkWidget *));
  widgets[0] = GTK_WIDGET(id);
  widgets[1] = GTK_WIDGET(full_name);
  widgets[2] = GTK_WIDGET(birth_year);
  widgets[3] = GTK_WIDGET(classe);
  widgets[4] = GTK_WIDGET(sfsd);
  widgets[5] = GTK_WIDGET(oop);
  widgets[6] = GTK_WIDGET(analysis);
  widgets[7] = GTK_WIDGET(algebra);
  widgets[8] = GTK_WIDGET(search_delete_id);
  widgets[9] = GTK_WIDGET(extract_class);
  // Connect buttons to callback functions;

  g_signal_connect(add_button, "clicked", G_CALLBACK(add_student_clicked), widgets);
  g_signal_connect(search_button, "clicked", G_CALLBACK(search_student_clicked), widgets[8]);
  g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_clicked), widgets[8]);
  g_signal_connect(reorganize_button, "clicked", G_CALLBACK(reorganize_clicked), NULL);
  g_signal_connect(Extract_button, "clicked", G_CALLBACK(extract_clicked), widgets[9]);
  g_signal_connect(Modify_button, "clicked", G_CALLBACK(modify_clicked), widgets[8]);

  gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;
  app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}