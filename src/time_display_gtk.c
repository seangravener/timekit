#include <gtk/gtk.h>
#include <time.h>

static gboolean update_time(gpointer label) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char buf[100];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);

    gtk_label_set_text(GTK_LABEL(label), buf);
    return TRUE;  // Continue calling
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Current Time");
    gtk_window_set_default_size(GTK_WINDOW(window), 250, 100);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *label = gtk_label_new("Loading...");
    gtk_container_add(GTK_CONTAINER(window), label);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_timeout_add_seconds(1, update_time, label);  // Update every 1 sec

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
