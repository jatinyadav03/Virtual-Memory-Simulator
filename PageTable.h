// PageTable.h
#ifndef PAGETABLE_H
#define PAGETABLE_H

#define MAX_PAGES 100

typedef struct {
    int pageFrame[MAX_PAGES];
    int frameCount;
    int diskAccesses;
} PageTable;

#endif
