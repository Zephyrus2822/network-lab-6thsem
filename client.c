#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define SOCKET_PATH "sort_socket"
#define MAX 100

int main() {

    int sock_fd;
    struct sockaddr_un server_addr;
    int choice;

    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCKET_PATH);

    connect(sock_fd, (struct sockaddr*)&server_addr,
            sizeof(server_addr));

    while(1) {

        printf("\n1. Sort Numbers\n");
        printf("2. Addition\n");
        printf("3. Subtraction\n");
        printf("4. Multiplication\n");
        printf("5. Division\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        write(sock_fd, &choice, sizeof(int));

        if(choice == 1) {
            int n, arr[MAX];

            printf("Enter number of integers: ");
            scanf("%d", &n);

            printf("Enter numbers:\n");
            for(int i=0;i<n;i++)
                scanf("%d", &arr[i]);

            write(sock_fd, &n, sizeof(int));
            write(sock_fd, arr, n * sizeof(int));

            read(sock_fd, arr, n * sizeof(int));

            printf("Sorted numbers:\n");
            for(int i=0;i<n;i++)
                printf("%d ", arr[i]);
            printf("\n");
        }
        else if(choice >= 2 && choice <= 5) {

            int a, b;

            printf("Enter two integers: ");
            scanf("%d %d", &a, &b);

            write(sock_fd, &a, sizeof(int));
            write(sock_fd, &b, sizeof(int));

            if(choice == 5) {
                float result;
                read(sock_fd, &result, sizeof(float));
                printf("Result: %.2f\n", result);
            } else {
                int result;
                read(sock_fd, &result, sizeof(int));
                printf("Result: %d\n", result);
            }
        }
    }

    close(sock_fd);
    return 0;
}
