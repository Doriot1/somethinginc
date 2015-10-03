/*// zadanie1.c -- Zadanie 1 - Spravca pamati
// Daniel Novak, 27.9.2015 19:38:52
#include <stdio.h>

#define HEAD 24


// 24 bajtov
typedef struct mem_block {
    unsigned int isFree;
    unsigned int alloc_size;
    struct mem_block *next;
    struct mem_block *previous;
} MEM_BLOCK;


void *start;


void header_init(MEM_BLOCK *pom1, MEM_BLOCK *pom2) {
    pom1->next = pom2;
    pom2->previous = pom1;
    pom2->next = NULL;
    pom2->isFree = 1;
    pom2->alloc_size = 0;
}


void *memory_alloc(unsigned int size) {
    MEM_BLOCK *first = (MEM_BLOCK *) ((unsigned int *) start + 1);
    MEM_BLOCK *current = first;
    MEM_BLOCK *new = NULL;
    unsigned int *heap_size = (unsigned int *) start;
    char *end = ((char *)start) + *heap_size + HEAD + sizeof(unsigned int);


    // if we want to allocate 0 bytes
    if (size == 0)
        return NULL;

    if (first->isFree && (first->alloc_size == 0 || first->alloc_size >= size)) {
        first->isFree = 0;
        first->alloc_size = size;

        // ak je toto prva alokacia, tak vytvorime aj dalsi header
        if (first->next == NULL) {

            current = 1 + (MEM_BLOCK *) ((char *) current + size);
            header_init(first, current);
        }
        return first + 1;
    }

    while (current) {
        if (current->isFree && (current->alloc_size == 0 || current->alloc_size >= size)) {
            break;
        }
        current = current->next;
    }

    if (!current)
        return NULL;
    if ((end - (char *) current) < size)
        return NULL;

    current->isFree = 0;
    current->alloc_size = size;

    if (current->next == NULL) {
        new = 1 + (MEM_BLOCK *) ((char *) current + size);
        header_init(current, new);
    }

    return current + 1;
}


int memory_free(void *valid_ptr) {
    MEM_BLOCK *current = (MEM_BLOCK *) ((unsigned int *) start + 1);

    while (current) {
        if ((current + 1) == valid_ptr && current->isFree == 0) {
            current->isFree = 1;

            return 0;
        }
        current = current->next;
    }

    return 1;
}


int memory_check(void *ptr) {
    MEM_BLOCK *current = (MEM_BLOCK *) ((unsigned int *) start + 1);

    while (current) {
        if ((current + 1) == ptr) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}


void memory_init(void *ptr, unsigned int size) {
    MEM_BLOCK *first = (MEM_BLOCK *) ((int *) ptr + 1);
    unsigned int *heap_size = (unsigned int *) ptr;

    *heap_size = size;
    start = ptr;

    first->isFree = 1;
    first->next = NULL;
    first->alloc_size = 0;
}

// Tlacidlo 'Spustit' vyskusa vasho spravcu pamati na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.


int main() {
    char region[100];
    int i;
    char *pointer1, *pointer2, *pointer3;
    char *pointer4, *pointer5, *pointer6;

    memory_init(region, 100);

    if ((pointer1 = (char *) memory_alloc(8)))
        printf("Success\n");
    else
        printf("Fail\n");

    if ((pointer2 = (char *) memory_alloc(8)))
        printf("Success\n");
    else
        printf("Fail\n");

    if ((pointer3 = (char *) memory_alloc(8)))
        printf("Success\n");
    else
        printf("Fail\n");

    if ((pointer4 = (char *) memory_alloc(8)))
        printf("Success\n");
    else
        printf("Fail\n");

    if (memory_check(pointer3))
        memory_free(pointer3);
    else
        printf("Wrong pointer\n");

    if ((pointer3 = (char *) memory_alloc(8)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer5 = (char *) memory_alloc(8)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer6 = (char *) memory_alloc(8)))
        printf("Success\n");
    else
        printf("Fail\n");

    return 0;
}*/

// zadanie1.c -- Zadanie 1 - Spravca pamati
// Daniel Novak, 27.9.2015 19:38:52
#include <stdio.h>

#define HEAD 24


// 24 bajtov
typedef struct mem_block {
    unsigned int isFree;
    unsigned int alloc_size;
    struct mem_block *next;
    struct mem_block *previous;
} MEM_BLOCK;


void *start;


void header_init(MEM_BLOCK *pom1, MEM_BLOCK *pom2) {
    pom1->next = pom2;
    pom2->previous = pom1;
    pom2->next = NULL;
    pom2->isFree = 1;
    pom2->alloc_size = 0;
}


void *memory_alloc(unsigned int size) {
    MEM_BLOCK *first = (MEM_BLOCK *) ((unsigned int *) start + 1);
    MEM_BLOCK *current = first;
    MEM_BLOCK *new = NULL;
    unsigned int *heap_size = (unsigned int *) start;
    char *end = ((char *)start) + *heap_size + HEAD + sizeof(unsigned int);


    // if we want to allocate 0 bytes
    if (size == 0)
        return NULL;

    if (first->isFree && (first->alloc_size == 0 || first->alloc_size >= size)) {
        if (*heap_size < size)
            return NULL;
        first->isFree = 0;
        first->alloc_size = size;
        *heap_size -= size;

        // ak je toto prva alokacia, tak vytvorime aj dalsi header
        if (first->next == NULL) {
            *heap_size -= HEAD;
            current = 1 + (MEM_BLOCK *) ((char *) current + size);
            header_init(first, current);
        }
        return first + 1;
    }

    while (current) {
        if (current->isFree && (current->alloc_size == 0 || current->alloc_size >= size)) {
            break;
        }
        current = current->next;
    }

    if (*heap_size < size || !current)
        return NULL;
    if((end - (char *)current) < size)
        return NULL;

    *heap_size -= size;
    current->isFree = 0;
    current->alloc_size = size;

    if (current->next == NULL && *heap_size > HEAD + 1) {
        *heap_size -= HEAD;
        new = 1 + (MEM_BLOCK *) ((char *) current + size);
        header_init(current, new);
    }

    return current + 1;
}


int memory_free(void *valid_ptr) {
    MEM_BLOCK *current = (MEM_BLOCK *) ((unsigned int *) start + 1);
    int *heap_size = (int *) start;

    while (current) {
        if ((current + 1) == valid_ptr && current->isFree == 0) {
            current->isFree = 1;
            *heap_size += current->alloc_size;
            return 0;
        }
        current = current->next;
    }

    return 1;
}


int memory_check(void *ptr) {
    MEM_BLOCK *current = (MEM_BLOCK *) ((unsigned int *) start + 1);

    while (current) {
        if ((current + 1) == ptr) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}


void memory_init(void *ptr, unsigned int size) {
    MEM_BLOCK *first = (MEM_BLOCK *) ((int *) ptr + 1);
    unsigned int *heap_size = (unsigned int *) ptr;

    *heap_size = size - 4 - HEAD;
    start = ptr;

    first->isFree = 1;
    first->next = NULL;
    first->alloc_size = 0;
}

// Tlacidlo 'Spustit' vyskusa vasho spravcu pamati na testovacich scenaroch.
// Vlastna funkcia main() nie je potrebna, a nebude sa spustat.


int main() {
    char region[500];
    int i;
    char *pointer1, *pointer2, *pointer3;
    char *pointer4, *pointer5, *pointer6;
    char *pointer7, *pointer8, *pointer9;
    char *pointer10, *pointer11, *pointer12, *pointer13;

    memory_init(region, 500);

    if ((pointer1 = (char *) memory_alloc(23)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer2 = (char *) memory_alloc(23)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer3 = (char *) memory_alloc(10)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer4 = (char *) memory_alloc(21)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer5 = (char *) memory_alloc(19)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer6 = (char *) memory_alloc(11)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer7 = (char *) memory_alloc(13)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer8 = (char *) memory_alloc(17)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer9 = (char *) memory_alloc(13)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer10 = (char *) memory_alloc(18)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer11 = (char *) memory_alloc(12)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer12 = (char *) memory_alloc(19)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer13 = (char *) memory_alloc(23)))
        printf("Success\n");
    else
        printf("Fail\n");


    if (memory_check(pointer3))
        memory_free(pointer3);
    if (memory_check(pointer4))
        memory_free(pointer4);
    if (memory_check(pointer7))
        memory_free(pointer7);
    if (memory_check(pointer10))
        memory_free(pointer10);
    if (memory_check(pointer6))
        memory_free(pointer6);
    if (memory_check(pointer1))
        memory_free(pointer1);
    if (memory_check(pointer5))
        memory_free(pointer5);
    if (memory_check(pointer12))
        memory_free(pointer12);
    if (memory_check(pointer13))
        memory_free(pointer13);

    if ((pointer3 = (char *) memory_alloc(12)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer4 = (char *) memory_alloc(17)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer7 = (char *) memory_alloc(21)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer10 = (char *) memory_alloc(8)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer6 = (char *) memory_alloc(12)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer1 = (char *) memory_alloc(12)))
        printf("Success\n");
    else
        printf("Fail\n");
    if ((pointer5 = (char *) memory_alloc(10)))
        printf("Success\n");
    else
        printf("Fail\n");

    return 0;
}

