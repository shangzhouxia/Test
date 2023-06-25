#include <stdio.h>
#include "_queue.h"

int main(void)
{
    Queue line;
    Item temp;
    char ch;


    InitializeQueue(&line);
    puts("Type \'a\' to add a value,");
    puts("Type \'d\' to delete a value, and \'q\' to quit.");

    while ((ch = getchar()) != 'q') {
        if (ch != 'a' && ch != 'd') {
            continue;
        }

        if (ch == 'a') {
            printf("int to add: ");
            scanf("%d", &temp);

            if (!QueueIsFull(&line)) {
                printf("add %d into queue.\n", temp);
                AddQueue(temp, &line);
            } else {
                puts("quque is full!");
            }
        } else {
            if (QueueIsEmpty(&line)) {
                puts("Nothing to delete!");
            } else {
                DelQueue(&temp, &line);
                printf("delete %d from queue.\n", temp);
            }
        }

        printf("%d items in queue\n", QueueItemCount(&line));

        puts("Type \'a\' to add a value,");
        puts("Type \'d\' to delete a value, and \'q\' to quit.");
    }

    EmptyQueue(&line);
    puts("Bye!");

    return 0;
}
