#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>

#define MAX_TEXT 512

struct my_msg_st {
    long int my_msg_type;
    char some_text[MAX_TEXT];
};

void toBinary(int num, char *buffer) {
    int i = 0;
    char temp[64];

    if (num == 0) {
        strcpy(buffer, "0");
        return;
    }

    while (num > 0) {
        temp[i++] = (num % 2) + '0';
        num /= 2;
    }

    for (int j = 0; j < i; j++)
        buffer[j] = temp[i - j - 1];

    buffer[i] = '\0';
}

int main() {
    int msgid;
    struct my_msg_st some_data;
    int decimal;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    while (1) {

        printf("\nEnter a decimal number (-1 to exit): ");
        scanf("%d", &decimal);

        if (decimal == -1)
            break;

        /* ---- Send Binary ---- */
        some_data.my_msg_type = 2;
        toBinary(decimal, some_data.some_text);

        msgsnd(msgid, &some_data,
               sizeof(some_data) - sizeof(long), 0);

        /* ---- Send Octal ---- */
        some_data.my_msg_type = 8;
        sprintf(some_data.some_text, "%o", decimal);

        msgsnd(msgid, &some_data,
               sizeof(some_data) - sizeof(long), 0);

        /* ---- Send Hex ---- */
        some_data.my_msg_type = 16;
        sprintf(some_data.some_text, "%X", decimal);

        msgsnd(msgid, &some_data,
               sizeof(some_data) - sizeof(long), 0);

        printf("Sent Binary, Octal, Hex messages.\n");
    }

    return 0;
}
