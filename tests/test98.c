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
} splay_elem_t;


void depth_first_print(splay_elem_t *elt) {
    if (elt->left)
        depth_first_print(elt->left);
    if (elt->right)
        depth_first_print(elt->right);
    printf("%d\n", elt->key);
}

int main()
{
    splay_elem_t *tree = NULL;
    splay_elem_t *nodes = NULL;
    splay_elem_t *pNode = NULL;
    char linebuf[BUFLEN];
    FILE *file;

    file = fopen( "test98.dat", "r" );
    if (file == NULL) {
        perror("can't open: ");
        exit(-1);
    }

    int count = 0;
    while (fgets(linebuf, BUFLEN, file) != NULL)
        count++;
    nodes = (splay_elem_t *)malloc(count*sizeof(splay_elem_t));
    fseek(file, SEEK_SET, 0);
    int i = 0;
    while (fgets(linebuf, BUFLEN, file) != NULL) {
        int v = atoi(linebuf);
        pNode = nodes + i++;
        SPLAY_ADD(tree, &v, sizeof(v), pNode);
        assert(pNode == NULL);
    }
    fseek(file, SEEK_SET, 0);
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
    i = 0;
    while (fgets(linebuf, BUFLEN, file) != NULL) {
        if (0 == i % 4) {
            int v = atoi(linebuf);
            pNode = NULL;
            SPLAY_REMOVE(tree, &v, sizeof(v), pNode);
            assert(pNode);
        }
        i++;
    }
    depth_first_print(tree);
    free(nodes);
    fclose(file);
    return 0;
}
