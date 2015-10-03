#include <stdio.h>

#define HEAD 32
#define true 1

// 32 bajtov
typedef struct mem_block {
    unsigned int isFree;
    unsigned int heap_size;
    unsigned int alloc_size;
    struct mem_block *next;
    struct mem_block *previous;
} MEM_BLOCK;


void *start;


void header_init(MEM_BLOCK *pom1, MEM_BLOCK *pom2){
    pom1->next = pom2;
    pom2->previous = pom1;
    pom2->next = NULL;
    pom2->heap_size = pom1->heap_size;
    pom2->isFree = 1;
    pom2->alloc_size = 0;
}


void *memory_alloc(unsigned int size) {
    MEM_BLOCK *first = (MEM_BLOCK *) start;
    MEM_BLOCK *current = first;
    MEM_BLOCK *new = NULL;
    short static isFirst = 0;

    // if we want to allocate 0 bytes
    if (size == 0)
        return NULL;

    // spusti sa prave raz pri prvej alokacii
    if (first->isFree && !isFirst) {
        if (first->heap_size < HEAD + size)
            return NULL;
        first->heap_size -= HEAD + size;
        first->isFree = 0;
        first->alloc_size = size;
        isFirst = 1;

        // current bude ukazovat na dalsie volne miesto v pamati
        current = 1 + (MEM_BLOCK *) ((char *) current + size);
        header_init(first, current);
        return first + 1;
    }

    while (true) {
        current = current->next;
        if (current->isFree && (current->alloc_size == 0 || current->alloc_size >= size))
            break;
    }

    if (current->heap_size < HEAD + size)
        return NULL;

    current->heap_size -= HEAD + size;
    current->isFree = 0;
    current->alloc_size = size;

    if (current->heap_size > HEAD && current->next == NULL) {
        // explain this
        new = 1 + (MEM_BLOCK *) ((char *) current + size);
        header_init(current, new);
    }

    return current + 1;
}


int memory_free(void *valid_ptr) {
    MEM_BLOCK *current = start;

    while (current) {
        if ((current + 1) == valid_ptr) {
            current->isFree = 1;
            current->heap_size += current->alloc_size;
            if (current->next->isFree) {

            }
            return 0;
        }
        current = current->next;
    }

    return 1;
}


int memory_check(void *ptr) {
    MEM_BLOCK *first = start, *current = start;

    while (current) {
        if ((current + 1) == ptr)
            return 1;
        current = current->next;
    }

    return 0;
}


void memory_init(void *ptr, unsigned int size) {
    start = ptr;
    MEM_BLOCK *first = (MEM_BLOCK *) start;
    first->heap_size = size;
    first->isFree = 1;
}


int main() {
    char region[300];
    int i;
    char *pointer1, *pointer2, *pointer3;
    int *pointer4;
    memory_init(region, 300);

    pointer1 = (char *) memory_alloc(5);

    /*if (memory_check(pointer1))
        memory_free(pointer1);
    else
        printf("Not correct-o! :(");*/
      for (i = 0; i < 5; i++) {
          pointer1[i] = (char) (97 + i);
          printf("%c", pointer1[i]);
      }

      pointer2 = (char *) memory_alloc(5);
      pointer2[0] = 'z';
      printf("\n%c\n", pointer2[0]);

      pointer3 = (char *) memory_alloc(30);
      pointer4 = (int *) memory_alloc(sizeof(int)*5);

      fgets(pointer3, 30, stdin);
      printf("\n%s\n", pointer3);

      for(i = 0; i < 5; i++){
          pointer4[i] = i * i;
          printf("%d\n", pointer4[i]);
      }

    return 0;
}