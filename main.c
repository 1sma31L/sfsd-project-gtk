#include "functions.h"
#include <gtk/gtk.h>

static void add_student_clicked(GtkButton *button, gpointer user_data) {
    FILE *file = fopen(FILE_NAME, "a+");

    // Retrieve the user data (entry_widgets) passed in
    GtkWidget **widgets = (GtkWidget **)user_data;

    // Retrieve the values;
    ENSTA_Student student;

    student.id = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets[0]));
    strncpy(student.name ,gtk_entry_get_text(GTK_ENTRY(widgets[1])),sizeof(student.name) - 1); 
student.name[sizeof(student.name) - 1] = '\0';
    student.birthYear = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets[2]));
    strncpy(student.class ,gtk_entry_get_text(GTK_ENTRY(widgets[3])), sizeof(student.class) - 1);  
student.class[sizeof(student.class) - 1] = '\0';  

    for (int i = 0; i < 4; i++) {
        student.grades[i] = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widgets[i + 4]));
    }   

    student.average = grade_average(student.grades);

    student.deleted = 0;

    //write in the fucking file;

    fprintf(file, "%d;%s;%d;%s;", student.id, student.name, student.birthYear, student.class);
    for (int i = 0; i < NUM_MODULES; i++)
    {
    fprintf(file, "%.2f,%d;", student.grades[i], MODULE_COEFFICIENTS[i]);
    }
    fprintf(file, "%.2f;%d\n", student.average, student.deleted);

    fclose(file);
   
}



static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid,*button,*add_modify_box,*search_delete_box,*extract_box,*reorganize_box;
    GtkWidget *search_button,*add_button,*Modify_button,*Extract_button,*reorganize_button,*delete_button;
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
    gtk_widget_set_margin_top(grid, 40);
    gtk_widget_set_margin_bottom(grid, 40);
    gtk_widget_set_margin_start(grid, 30);
    gtk_widget_set_margin_end(grid, 30);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // creat the boxes;
    GtkWidget *title_box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    GtkWidget *label = gtk_label_new("Welcome To SMS");
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
    add_button = gtk_button_new_with_label("Add Student");
    search_button = gtk_button_new_with_label("Search Student");
    Modify_button = gtk_button_new_with_label("Modify Student");
    Extract_button = gtk_button_new_with_label("Extract by Class");
    reorganize_button = gtk_button_new_with_label("Reorganize File");
    delete_button = gtk_button_new_with_label("Delete Student");

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
    gtk_widget_set_margin_top(reorganize_box, 100);

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
    GtkWidget *id = gtk_spin_button_new_with_range(0,G_MAXDOUBLE,1);
    GtkWidget *full_name = gtk_entry_new();
    GtkWidget *birth_year = gtk_spin_button_new_with_range(1990,2020,1);
    GtkWidget *classe = gtk_entry_new();
    GtkWidget *sfsd = gtk_spin_button_new_with_range(0.0, 20.0, 0.5);
    GtkWidget *oop = gtk_spin_button_new_with_range(0.0, 20.0, 0.5);
    GtkWidget *analysis = gtk_spin_button_new_with_range(0.0, 20.0, 0.5);
    GtkWidget *algebra = gtk_spin_button_new_with_range(0.0, 20.0, 0.5);
    gtk_grid_attach(GTK_GRID(grid1), id , 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), full_name, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), birth_year, 6, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), classe, 8, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), sfsd, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), oop, 4, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), analysis, 6, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), algebra, 8, 1, 1, 1);

// add entries and spins of search_delete_box ;

    GtkWidget *search_delete_id = gtk_spin_button_new_with_range(0,G_MAXDOUBLE,1);
    gtk_grid_attach(GTK_GRID(grid2), search_delete_id , 3, 1, 2, 1);

// // add entries and spins of search_delete_box ;
    GtkWidget *extract_class = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid3), extract_class , 3, 0, 2, 1);

//entries resize
    gtk_widget_set_size_request(sfsd, 100, 70);// the remining entries oop analysis algebra follow the sfsd entry size;


// buttons labels:
    GtkWidget *labid = gtk_label_new("Enter an unique ID:");
    GtkWidget *labname = gtk_label_new("Enter the Full Name:");
    GtkWidget *labyear = gtk_label_new("Enter the Birth Year:");
    GtkWidget *labclass = gtk_label_new("Enter the Class:");
    GtkWidget *labesfsd = gtk_label_new("Enter the SFSD Grade:");
    GtkWidget *labeoop = gtk_label_new("Enter the OOP Grade:");
    GtkWidget *labeanalyse = gtk_label_new("Enter the Analysis Grade:");
    GtkWidget *labalgebra = gtk_label_new("Enter the Algebra Grade:");
    GtkWidget *labsearch_delete_id = gtk_label_new("Enter the ID:");
    GtkWidget *labextract_class = gtk_label_new("Enter the Class:");


    gtk_grid_attach(GTK_GRID(grid1), labid , 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), labname , 3, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), labyear , 5, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), labclass , 7, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), labesfsd , 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), labeoop , 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), labeanalyse , 5, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid1), labalgebra , 7, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid2), labsearch_delete_id , 1, 1, 1, 1);
    gtk_widget_set_margin_end(labsearch_delete_id, 45);
    gtk_grid_attach(GTK_GRID(grid3), labextract_class , 1, 0, 1, 1);
    gtk_widget_set_margin_end(labextract_class, 20);


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



    GtkWidget **widgets = malloc(8 * sizeof(GtkWidget *));
    widgets[0] = GTK_WIDGET(id);
    widgets[1] = GTK_WIDGET(full_name);
    widgets[2] = GTK_WIDGET(birth_year);
    widgets[3] = GTK_WIDGET(classe);
    widgets[4] = GTK_WIDGET(sfsd);
    widgets[5] = GTK_WIDGET(oop);
    widgets[6] = GTK_WIDGET(analysis);
    widgets[7] = GTK_WIDGET(algebra);

// Connect buttons to callback functions;

    g_signal_connect(add_button, "clicked", G_CALLBACK(add_student_clicked), widgets);

    gtk_widget_show_all(window);

}


int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
return status;        
}