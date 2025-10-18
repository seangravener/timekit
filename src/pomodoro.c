#include <gtk/gtk.h>
#include <time.h>
#include <stdlib.h>

#define DEFAULT_WORK_MIN      25
#define DEFAULT_SHORT_MIN     5
#define DEFAULT_LONG_MIN      15
#define LONG_BREAK_INTERVAL   4

typedef enum {
    MODE_WORK,
    MODE_SHORT_BREAK,
    MODE_LONG_BREAK
} TimerMode;

static TimerMode current_mode = MODE_WORK;
static int session_count = 0;
static int remaining_seconds = DEFAULT_WORK_MIN * 60;
static gboolean running = FALSE;

static GtkWidget *label;
static GtkWidget *counter_label;
static GtkWidget *mode_label;
static GtkWidget *spin_button;

// 🔔 Play sound using external command (portable logic)
void play_sound() {
#if defined(__APPLE__)
    system("afplay /System/Library/Sounds/Glass.aiff &");
#elif defined(_WIN32)
    Beep(750, 300);
#else
    system("paplay /usr/share/sounds/freedesktop/stereo/complete.oga &");
#endif
}

void update_display() {
    int minutes = remaining_seconds / 60;
    int seconds = remaining_seconds % 60;

    char time_str[16];
    snprintf(time_str, sizeof(time_str), "%02d:%02d", minutes, seconds);
    gtk_label_set_text(GTK_LABEL(label), time_str);
}

// 🕒 Timer loop
gboolean update_timer(gpointer data) {
    if (running && remaining_seconds > 0) {
        remaining_seconds--;
        update_display();
    } else if (running && remaining_seconds == 0) {
        running = FALSE;
        play_sound();

        if (current_mode == MODE_WORK) {
            session_count++;
            if (session_count % LONG_BREAK_INTERVAL == 0) {
                current_mode = MODE_LONG_BREAK;
                remaining_seconds = DEFAULT_LONG_MIN * 60;
            } else {
                current_mode = MODE_SHORT_BREAK;
                remaining_seconds = DEFAULT_SHORT_MIN * 60;
            }
        } else {
            current_mode = MODE_WORK;
            remaining_seconds = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_button)) * 60;
        }

        // Update labels
        char counter_str[64];
        snprintf(counter_str, sizeof(counter_str), "Sessions Completed: %d", session_count);
        gtk_label_set_text(GTK_LABEL(counter_label), counter_str);

        const char *mode_str =
            current_mode == MODE_WORK ? "Work Session" :
            current_mode == MODE_SHORT_BREAK ? "Short Break" : "Long Break";
        gtk_label_set_text(GTK_LABEL(mode_label), mode_str);

        update_display();
        running = TRUE;  // auto-start next mode
    }

    return TRUE;
}

void start_clicked(GtkWidget *widget, gpointer data) {
    if (!running && remaining_seconds == 0) {
        remaining_seconds = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_button)) * 60;
        current_mode = MODE_WORK;
        gtk_label_set_text(GTK_LABEL(mode_label), "Work Session");
    }
    running = TRUE;
}

void pause_clicked(GtkWidget *widget, gpointer data) {
    running = FALSE;
}

void reset_clicked(GtkWidget *widget, gpointer data) {
    running = FALSE;
    session_count = 0;
    current_mode = MODE_WORK;

    int minutes = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_button));
    remaining_seconds = minutes * 60;

    gtk_label_set_text(GTK_LABEL(mode_label), "Work Session");
    gtk_label_set_text(GTK_LABEL(counter_label), "Sessions Completed: 0");
    update_display();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Pomodoro Timer");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 250);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 20);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    mode_label = gtk_label_new("Work Session");
    gtk_widget_set_halign(mode_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), mode_label, FALSE, FALSE, 5);

    label = gtk_label_new("25:00");
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 5);

    counter_label = gtk_label_new("Sessions Completed: 0");
    gtk_widget_set_halign(counter_label, GTK_ALIGN_CENTER);
    gtk_box_pack_start(GTK_BOX(vbox), counter_label, FALSE, FALSE, 5);

    GtkWidget *duration_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), duration_box, FALSE, FALSE, 0);
    GtkWidget *duration_label = gtk_label_new("Work Duration (min):");
    gtk_box_pack_start(GTK_BOX(duration_box), duration_label, FALSE, FALSE, 0);
    spin_button = gtk_spin_button_new_with_range(5, 60, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_button), 25);
    gtk_box_pack_start(GTK_BOX(duration_box), spin_button, FALSE, FALSE, 0);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    GtkWidget *start_button = gtk_button_new_with_label("Start");
    GtkWidget *pause_button = gtk_button_new_with_label("Pause");
    GtkWidget *reset_button = gtk_button_new_with_label("Reset");

    g_signal_connect(start_button, "clicked", G_CALLBACK(start_clicked), NULL);
    g_signal_connect(pause_button, "clicked", G_CALLBACK(pause_clicked), NULL);
    g_signal_connect(reset_button, "clicked", G_CALLBACK(reset_clicked), NULL);

    gtk_box_pack_start(GTK_BOX(hbox), start_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), pause_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), reset_button, TRUE, TRUE, 0);

    g_timeout_add_seconds(1, update_timer, NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
