#include <stdio.h>

#define HEAD 32

// 32 bajtov
typedef struct mem_block {
    unsigned int isFree;
    unsigned int heap_size;
    unsigned int alloc_size;
    struct mem_block *next;
    struct mem_block *previous;
} MEM_BLOCK;

void *start;

/*
 * explicitny zoznam, implicitny zoznam s first fit, best fit, next fit
 * spravis si
 */

void *memory_alloc(unsigned int size) {
    MEM_BLOCK *first = (MEM_BLOCK *) start;
    MEM_BLOCK *current = first;
    MEM_BLOCK *new = NULL;

    if (first->isFree) {
        if (first->heap_size < HEAD + size)
            return NULL;
        first->heap_size -= HEAD + size;
        first->isFree = 0;
        first->alloc_size = size;

        // current bude ukazovat na dalsie volne miesto v pamati
        current = 1 + (MEM_BLOCK *) ((char *) current + size);
        first->next = current;
        current->previous = first;
        current->next = NULL;
        current->heap_size = first->heap_size;
        current->isFree = 1;
        current->alloc_size = 0;
        return first + 1;
    }

    while (!current->isFree && (current->alloc_size != 0 && current->alloc_size < size))
        current = current->next;

    if (current->heap_size < HEAD + size)
        return NULL;

    current->heap_size -= HEAD + size;
    current->isFree = 0;
    current->alloc_size = size;

    new = 1 + (MEM_BLOCK *) ((char *) current + size);
    current->next = new;
    new->previous = current;
    new->previous = NULL;
    new->heap_size = current->heap_size;
    new->isFree = 0;
    new->alloc_size = 0;


    return current + 1;
}

/*
 * 1.st checkne sa pointer v memory check
 *
 */

int memory_free(void *valid_ptr) {
    return 0;
}

/*
 *
 */

int memory_check(void *ptr) {
    return 0;
}

void memory_init(void *ptr, unsigned int size) {
    start = ptr;
    MEM_BLOCK *first = (MEM_BLOCK *) start;
    first->heap_size = size;
    first->isFree = 1;

}

int main() {
    char region[100];
    int i;
    char *pointer, *pointer2;
    memory_init(region, 100);

    pointer = (char *) memory_alloc(5);
    pointer[0] = 'a';
    pointer[1] = 'b';
    pointer[2] = 'c';
    pointer[3] = 'd';
    pointer[4] = 'e';
    for (i = 0; i < 5; i++)
        printf("%c", pointer[i]);
    pointer2 = (char *) memory_alloc(5);
    pointer2[0] = 'z';
    printf("\n%c", pointer2[0]);

    return 0;
}