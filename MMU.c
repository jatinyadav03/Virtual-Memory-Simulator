// MMU.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include "PageTable.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: ./MMU <num_pages> <R/W sequence...> <shmid>\n");
        return 1;
    }

    int numPages = atoi(argv[1]);
    int shmid = atoi(argv[argc - 1]); // last argument is shared memory id

    // Attach to shared memory
    PageTable *pt = (PageTable *)shmat(shmid, NULL, 0);

    // ✅ Check for attach failure
    if (pt == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    printf("[MMU] Connected to OS shared memory.\n");

    int nextFrame = 0;
    for (int i = 2; i < argc - 1; i++) {
        char op = argv[i][0];
        int page = argv[i][1] - '0';

        if (page < 0 || page >= numPages) {
            printf("   [Error] Invalid page number %d\n", page);
            continue;
        }

        printf("[MMU] %c operation on Page %d\n", op, page);

        if (pt->pageFrame[page] == -1) {
            pt->diskAccesses++;
            pt->pageFrame[page] = nextFrame;
            nextFrame = (nextFrame + 1) % pt->frameCount;
            printf("   [MMU] Page %d caused page fault -> loaded into Frame %d\n",
                   page, pt->pageFrame[page]);
        } else {
            printf("   [MMU] Page %d is already in Frame %d\n",
                   page, pt->pageFrame[page]);
        }

        sleep(1);

        printf("   Current Page Table: ");
        for (int j = 0; j < numPages; j++) {
            printf("%d ", pt->pageFrame[j]);
        }
        printf("\n\n");
    }

    printf("[MMU] Tell OS that I'm finished.\n");

    shmdt(pt);
    return 0;
}
