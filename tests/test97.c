#include "utsplay.h"
#include <stdlib.h>   /* malloc, atoi */
#include <errno.h>    /* perror */
#include <stdio.h>    /* printf */
#include <assert.h>

#define BUFLEN 20

struct splay_elem_s;
typedef struct splay_elem_s {
    struct splay_elem_s *left, *right, *parent;
    int key;
    int weight;
} splay_elem_t;

#undef SPLAY_INIT
#define SPLAY_INIT(e) \
do { \
    e->weight = 1; \
} while(0)

#undef SPLAY_PROCESS
#define SPLAY_PROCESS(e) \
do { \
    e->weight = 1; \
    if (e->right) e->weight += e->right->weight; \
    if (e->left) e->weight += e->left->weight; \
} while (0)

void depth_first_print(splay_elem_t *elt) {
    if (elt->left)
        depth_first_print(elt->left);
    if (elt->right)
        depth_first_print(elt->right);
    printf("%d\n", elt->key);
    printf("%d\n", elt->weight);
}

int main()
{
    splay_elem_t *tree = NULL;
    char linebuf[BUFLEN];
    FILE *file;

    file = fopen( "test97.dat", "r" );
    if (file == NULL) {
        perror("can't open: ");
        exit(-1);
    }

    while (fgets(linebuf, BUFLEN, file) != NULL) {
        int v = atoi(linebuf);
        SPLAY_INSERT(tree, &v, sizeof(v));
    }
    depth_first_print(tree);
    printf("---\n");
    fseek(file, SEEK_SET, 0);
    while (fgets(linebuf, BUFLEN, file) != NULL) {
        int v = atoi(linebuf);
        splay_elem_t *e;
        SPLAY_FIND(tree, &v, sizeof(v), e);
        assert(tree == e);
        assert(e->key == v);
    }
    depth_first_print(tree);
    printf("---\n");
    fseek(file, SEEK_SET, 0);
    int i = 0;
    while (fgets(linebuf, BUFLEN, file) != NULL) {
        if (0 == i % 4) {
            int v = atoi(linebuf);
            SPLAY_ERASE(tree, &v, sizeof(v));
        }
        i++;
    }
    depth_first_print(tree);
    SPLAY_CLEAR(tree);
    fclose(file);
    return 0;
}
