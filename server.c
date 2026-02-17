#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_PATH "sort_socket"
#define MAX 100

// Sorting function
void sort(int a[], int n) {
    for(int i = 0; i < n - 1; i++)
        for(int j = i + 1; j < n; j++)
            if(a[i] > a[j]) {
                int t = a[i];
                a[i] = a[j];
                a[j] = t;
            }
}

int main() {

    int server_fd, client_fd;
    struct sockaddr_un server_addr;
    int choice;

    // Remove old socket file if exists
    unlink(SOCKET_PATH);

    // Create UNIX domain socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    // Bind socket to file path
    if(bind(server_fd, (struct sockaddr*)&server_addr,
            sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for client connections
    listen(server_fd, 5);
    printf("Server waiting...\n");

    // Accept client
    client_fd = accept(server_fd, NULL, NULL);
    if(client_fd < 0) {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected.\n");

    // Continuous service loop
    while(1) {

        // Read operation choice
        if(read(client_fd, &choice, sizeof(int)) <= 0)
            break;

        switch(choice) {

            case 1: {  // SORTING
                int n, arr[MAX];

                read(client_fd, &n, sizeof(int));
                read(client_fd, arr, n * sizeof(int));

                sort(arr, n);

                write(client_fd, arr, n * sizeof(int));
                break;
            }

            case 2: {  // ADDITION
                int a, b, result;

                read(client_fd, &a, sizeof(int));
                read(client_fd, &b, sizeof(int));

                result = a + b;

                write(client_fd, &result, sizeof(int));
                break;
            }

            case 3: {  // SUBTRACTION
                int a, b, result;

                read(client_fd, &a, sizeof(int));
                read(client_fd, &b, sizeof(int));

                result = a - b;

                write(client_fd, &result, sizeof(int));
                break;
            }

            case 4: {  // MULTIPLICATION
                int a, b, result;

                read(client_fd, &a, sizeof(int));
                read(client_fd, &b, sizeof(int));

                result = a * b;

                write(client_fd, &result, sizeof(int));
                break;
            }

            case 5: {  // DIVISION
                int a, b;
                float result;

                read(client_fd, &a, sizeof(int));
                read(client_fd, &b, sizeof(int));

                if(b == 0) {
                    result = 0;
                } else {
                    result = (float)a / b;
                }

                write(client_fd, &result, sizeof(float));
                break;
            }

            default:
                printf("Invalid choice received.\n");
        }
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}
