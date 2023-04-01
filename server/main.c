#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <err.h>
#include "data.h"
#include <gtk/gtk.h>

#define BUFFER_SIZE 512
#define SERVER_PORT "2048" // port utilisé pour la connexion
#define BACKLOG 10          // nombre maximal de connexions en attente

typedef struct UserInterface
{
    GtkWindow *window;
    GtkImage *image;
    GtkButton *change;
    GtkEntry *entry;
    Gchar *path;
} UserInterface;

void change_image(GtkWidget *widget, gpointer data)
{
    if (widget == 0){
        g_print("widget");
    }
    UserInterface *ui = (UserInterface *)data;

    //Image *image = importImage(ui->path);
    //double angleRotation = (double)gtk_range_get_value(GTK_RANGE(ui->scale));
    g_print("Change \n");
    //saveImage(image, ui->path);
    gtk_image_set_from_file(GTK_IMAGE(ui->image), ui->path);
}

void entry_changed(GtkWidget *widget, gpointer data)
{
    UserInterface *ui = (UserInterface *)data;
    const gchar *sText;

    sText = gtk_entry_get_text(GTK_ENTRY(widget));
    g_print("Entry changed : %s\n", sText);

    ui->entry = GTK_ENTRY(widget);
}

void rewrite(int fd, const void *buf, size_t count)
{
    size_t nbyte = 0;
    while(count > nbyte)
    {
        int temp;
        temp = write(fd, buf, count);
        if (temp == -1)
            errx(1, "Error while writing the file");
        buf += temp;
        nbyte += temp;
    }
}

struct info *echo(int fd_in, int fd_out)
{
    char buffer[BUFFER_SIZE];
    int nbyte;
    while(1)
    {
        nbyte = read(fd_in, buffer, BUFFER_SIZE);
        if (nbyte == 0)
           break;
        if (nbyte == -1)
            errx(1, "Error while reading the file");
        return data(fd_out, buffer, nbyte);
    }
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
    GtkEntry *entry = GTK_ENTRY(gtk_builder_get_object(builder, "seuil"));
    gchar *chemin = "gui2.png";
        // Creates the "Game" structure.
    UserInterface ui =
            {
                .window = window,
                .image = image,
                .change = change,
                .entry = entry,
                .path = chemin,
            };
    // Connects event handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(change, "clicked", G_CALLBACK(change_image), &ui);
    g_signal_connect(entry, "activate", G_CALLBACK(entry_changed), &ui);
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

        close(client_socket);
    }

    close(server_socket);
    exit(EXIT_SUCCESS);
}
