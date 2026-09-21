
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message
{
    long msg_type;
    char msg_text[100];
};

int main(int argc, char *argv[])
{
    key_t key;
    int msgid;
    struct message msg;

    key = ftok(".", 65);

    if (key == -1)
    {
        perror("ftok");
        exit(1);
    }

    msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }

    if (argc > 1 && strcmp(argv[1], "writer") == 0)
    {
        msg.msg_type = 1;

        printf("Enter message: ");
        fgets(msg.msg_text, sizeof(msg.msg_text), stdin);

        msgsnd(msgid, &msg, sizeof(msg.msg_text), 0);

        printf("Message sent successfully\n");
    }
    else if (argc > 1 && strcmp(argv[1], "reader") == 0)
    {
        msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0);

        printf("Received message: %s", msg.msg_text);
    }
    else
    {
        printf("Usage:\n");
        printf("./msqueue writer\n");
        printf("./msqueue reader\n");
    }

    return 0;
}
