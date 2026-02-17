#include <stdlib.h>
#include <stdio.h>
#include <sys/msg.h>

#define MAX_TEXT 512

// Message structure for inter-process communication
struct my_msg_st {
    long int my_msg_type;      // Message type identifier
    char some_text[MAX_TEXT];  // Message payload
};

int main() {
    int msgid;
    struct my_msg_st some_data;

    // Create or access a message queue with key 1234
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    // Continuously receive messages from the queue
    while (1) {
        // Receive message from queue (0 = receive any message type)
        if (msgrcv(msgid, &some_data,
                   sizeof(some_data) - sizeof(long),
                   0, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }

        // Display message based on its type
        if (some_data.my_msg_type == 2) {
            printf("\nBinary: %s\n", some_data.some_text);
        }
        else if (some_data.my_msg_type == 8) {
            printf("Octal: %s\n", some_data.some_text);
        }
        else if (some_data.my_msg_type == 16) {
            printf("Hexadecimal: %s\n", some_data.some_text);
        }
    }

    return 0;
}
