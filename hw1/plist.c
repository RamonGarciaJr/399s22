// plist.c

// implements functions in plist.h

#include <_bsd_types.h>
#include "plist.h"

struct plist_struct {
    void *elements;
    uint64_t indices;
};

plist pl_new() {
    plist newList = malloc(sizeof(struct plist_struct));
    if (newList == NULL) {
        printf("%s", "Unable to allocate memory for new list");
        exit(1);
    }
    newList->indices = (uint64_t) malloc(sizeof(uint64_t));
    newList->elements = (void *) malloc(sizeof(void *));
    newList->indices = 0;
    return newList;
}

void pl_free(plist l) {
    l->indices = 0;
    free((uint64_t *) l->indices);
    free(l->elements);
    free(l);
    l = NULL;
}

void pl_print(plist l) {
    printf("[");
    uint64_t *array = (uint64_t *) l->elements;
    for (int i = 0; i < l->indices; ++i) {
        printf("%llu", array[i]);
        if (i + 1 < l->indices) {
            printf(", ");
        }
    }
    printf("]\n");
}

void pl_append(plist l, void *x) {
    uint64_t *array = l->elements;
    array[l->indices] = (uint64_t) x;
    uint64_t resize = l->indices + 1;
    l->elements = malloc(resize * sizeof(uint64_t));
    l->elements = (void *) array;
    l->indices = resize;
}

void pl_extend(plist l1, plist l2) {
    uint64_t *a2 = l2->elements;
    for (int i = 0; i < l2->indices; ++i) {
        pl_append(l1, (void *) a2[i]);
    }
}


void pl_insert(plist l, uint64_t i, void *x) {
    if (i > l->indices) {
        pl_append(l, x);
        return;
    }
    pl_append(l, x);
    uint64_t *array = l->elements;
    uint64_t min = i > 0 ? i : 0;
    uint64_t max = l->indices - 1;
    uint64_t tmp;
    for (int j = max; j > min; j--) {
        tmp = array[j];
        array[j] = array[j - 1];
        array[j - 1] = tmp;
    }
    l->elements = array;
}

bool pl_remove(plist l, void *x) {
    uint64_t *array = l->elements;
    for (int i = 0; i < l->indices; i++) {
        if (array[i] == (uint64_t) x) {
            uint64_t tmp;
            uint64_t max = l->indices - 1;
            for (int j = i; j < max; j++) {
                tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
            uint64_t resize = l->indices - 1;
            l->elements = array;
            l->indices = resize;
            return TRUE;
        }
    }
    return FALSE;
}

void *pl_pop(plist l, uint64_t i) {
    if (i >= l->indices) exit(1);
    uint64_t *array = l->elements;
    if (i == l->indices - 1) {
        void *pop = (void *) array[l->indices - 1];
        l->indices -= 1;
        return pop;
    }

    if (i < 0) return NULL;

    void *pop = (void *) array[i];
    uint64_t tmp;
    uint64_t max = l->indices - 1;
    for (int j = i; j < max; j++) {
        tmp = array[j];
        array[j] = array[j + 1];
        array[j + 1] = tmp;
    }
    uint64_t resize = l->indices - 1;
    l->elements = array;
    l->indices = resize;
    return pop;
}

void pl_clear(plist l) {
    l->indices = 0;
}

uint64_t pl_index(plist l, void *x) {
    uint64_t *array = l->elements;
    for (int i = 0; i < l->indices; i++) {
        if (array[i] == (uint64_t) x) return i;
    }
    return -1;
}

uint64_t pl_count(plist l, void *x) {
    uint64_t count = 0;
    uint64_t *array = l->elements;
    for (int i = 0; i < l->indices; ++i) {
        if (array[i] == (uint64_t) x) count += 1;
    }
    return count;
}

void pl_reverse(plist l) {
    uint64_t indices = l->indices;
    uint64_t *array = l->elements;
    for (int i = 0; i < indices / 2; ++i) {
        uint64_t temp = array[i];
        array[i] = array[indices - 1 - i];
        array[l->indices - 1 - i] = temp;
    }
    l->elements = array;
}

plist pl_copy(plist l) {
    plist copy = pl_new();
    copy->elements = l->elements;
    copy->indices = l->indices;
    return copy;
}
