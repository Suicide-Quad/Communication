#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>
#include <gtk/gtk.h>

#define BUFFER_SIZE 512
#define SERVER_PORT "2048" // port utilisé pour la connexion
#define BACKLOG 10          // nombre maximal de connexions en attente

typedef struct UserInterface
{
    GtkWindow *window;
    GtkImage *image;
    GtkButton *change;
    gchar *path;
    gint x;
    gint y;
} UserInterface;

void change_image(GtkWidget *widget, gpointer data)
{
    if (widget == 0){
        g_print("widget");
    }
    UserInterface *ui = (UserInterface *)data;

    //Image *image = importImage(ui->path);
    g_print("Change \n");
    // on print ui->x et ui->y
    g_print("x = %d, y = %d \n", ui->x, ui->y);
    //saveImage(image, ui->path);
    //gtk_image_set_from_file(GTK_IMAGE(ui->image), ui->path);
}

void charge_image(GtkWidget *widget, gpointer data)
{
    UserInterface *ui = (UserInterface *)data;
    g_print("Loading the image\n");
    // on mets la progress bar à 10
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(ui->progress), 0.1);

    g_print("%s \n", gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(widget)));

    gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(widget));

    // gchar *filename2 = filename + 1;
    Image *image = importImage(filename);
    
    image = resizeImage(image, 750);
    saveImage(image, "gui.bmp");
    

    gtk_image_set_from_file(GTK_IMAGE(ui->image), filename);

    ui->path = "gui.bmp";
}

void save_image(GtkWidget *widget, gpointer data)
{
    if (widget == 0){
        g_print("widget");
    }
    //UserInterface *ui = (UserInterface *)data;
    g_print("Save \n");
    //saveImage(image, ui->path);
}

int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

        // Constructs a GtkBuilder instance.
    GtkBuilder *builder = gtk_builder_new();

        // Loads the UI description.
        // (Exits if an error occurs.)
    GError *error = NULL;
    if (gtk_builder_add_from_file(builder, "ui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window.main"));
    GtkImage *image = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
    GtkButton *change = GTK_BUTTON(gtk_builder_get_object(builder, "change"));
    GtkButton *save = GTK_BUTTON(gtk_builder_get_object(builder, "save"));
    GtkButton *exit = GTK_BUTTON(gtk_builder_get_object(builder, "exit"));
    GtkFileChooserButton *filechooserbutton1 = GTK_FILE_CHOOSER_BUTTON(gtk_builder_get_object(builder, "file-chooser"));
    gchar *chemin = "gui2.png";
        // Creates the "Game" structure.
    UserInterface ui =
            {
                .window = window,
                .image = image,
                .change = change,
                .path = chemin,
                .x = 0,
                .y = 0
            };
    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(change, "clicked", G_CALLBACK(change_image), &ui);
    g_signal_connect(save, "clicked", G_CALLBACK(save_image), &ui);
    g_signal_connect(exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(filechooserbutton1, "file-set", G_CALLBACK(charge_image), &ui);
    gtk_main();

    struct addrinfo hints, *server_info, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status;
    if ((status = getaddrinfo(NULL, SERVER_PORT, &hints, &server_info)) != 0) {
        errx(EXIT_FAILURE, "%s", gai_strerror(status));
    }

    int server_socket;
    for (p = server_info; p != NULL; p = p->ai_next) {
        if ((server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }

        int yes = 1;
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            err(EXIT_FAILURE, "Failed to set socket options");
        }

        if (bind(server_socket, p->ai_addr, p->ai_addrlen) == -1) {
            close(server_socket);
            continue;
        }

        break;
    }

    if (p == NULL) {
        errx(EXIT_FAILURE, "Failed to bind server socket");
    }

    freeaddrinfo(server_info);

    if (listen(server_socket, BACKLOG) == -1) {
        err(EXIT_FAILURE, "Failed to listen for connections");
    }

    printf("Server waiting for connections on port %s...\n", SERVER_PORT);

    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof client_addr;

    int client_socket;
    char buffer[256];
    ssize_t bytes_received;

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == -1) {
            err(EXIT_FAILURE, "Failed to accept connection");
        }

        bytes_received = recv(client_socket, buffer, sizeof buffer, 0);
        if (bytes_received == -1) {
            err(EXIT_FAILURE, "Failed to receive message from client");
        }

        buffer[bytes_received] = '\0';
        printf("Received message from client: %s\n", buffer);

        char *position1;
        char *position2;
        int test = 0;
        for (int i = 0; i < bytes_received; i++) {
            if (buffer[i] == ':'){
                test = 1;
            }
            else{
                if (test){
                    position1+=buffer[i];
                }
                else {
                    position2+=buffer[i];
                }
            }
        }
        ui.x = atoi(position1);
        ui.y = atoi(position2);
        change_image(NULL, &ui);
        printf("x = %d, y = %d\n", ui.x, ui.y);
        close(client_socket);
    }

    close(server_socket);
}
