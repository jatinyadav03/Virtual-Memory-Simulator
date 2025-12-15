// OS.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include "PageTable.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./OS <num_pages> <num_frames>\n");
        return 1;
    }

    int numPages = atoi(argv[1]);
    int numFrames = atoi(argv[2]);

    // Create shared memory
    key_t key = ftok("PageTable.h", 65);
    int shmid = shmget(key, sizeof(PageTable), 0666 | IPC_CREAT);
    PageTable *pt = (PageTable *)shmat(shmid, NULL, 0);

    // Initialize page table
    pt->pageCount = numPages;
    pt->frameCount = numFrames;
    pt->diskAccesses = 0;
    for (int i = 0; i < numPages; i++)
        pt->pageFrame[i] = -1; // not loaded

    printf("[OS] Initialized Page Table with %d pages and %d frames\n", numPages, numFrames);
    printf("[OS] Shared memory ID (XXXX) = %d\n", shmid);
    printf("[OS] Waiting for MMU...\n");

    // Wait for MMU to finish (simply wait for user input or signal)
    getchar();

    printf("[OS] Simulation finished\n");
    printf("[OS] Total Disk Accesses (page faults): %d\n", pt->diskAccesses);

    // Cleanup
    shmdt(pt);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
