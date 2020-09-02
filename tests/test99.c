#include "utsplay.h"
#include <stdlib.h>   /* malloc, atoi */
#include <errno.h>    /* perror */
#include <stdio.h>    /* printf */
#include <assert.h>

#define BUFLEN 20

struct splay_elem_s;
typedef struct splay_elem_s {
    struct splay_elem_s *left, *right, *parent;
    int *key;
} splay_elem_t;


void depth_first_print(splay_elem_t *elt) {
    if (elt->left)
        depth_first_print(elt->left);
    if (elt->right)
        depth_first_print(elt->right);
    printf("%d\n", *(elt->key));
}

int main()
{
    splay_elem_t *tree = NULL;
    splay_elem_t *nodes = NULL;
    splay_elem_t *pNode = NULL;
    int *keys = NULL;
    char linebuf[BUFLEN];
    FILE *file;

    file = fopen( "test99.dat", "r" );
    if (file == NULL) {
        perror("can't open: ");
        exit(-1);
    }

    int count = 0;
    while (fgets(linebuf, BUFLEN, file) != NULL)
        count++;
    nodes = (splay_elem_t *)malloc(count*sizeof(splay_elem_t));
    keys = (int *)malloc(count*sizeof(int));

    fseek(file, SEEK_SET, 0);
    int i = 0;
    while (fgets(linebuf, BUFLEN, file) != NULL) {
        keys[i] = atoi(linebuf);
        pNode = nodes + i;
        pNode->key = keys + i;
        i++;
        SPLAY_ADD_KEYPTR(tree, sizeof(int), pNode);
        assert(pNode == NULL);
    }
    fclose(file);
    depth_first_print(tree);
    printf("---\n");
    for(i = 0; i < count; i++) {
        splay_elem_t *e;
        SPLAY_FIND_KEYPTR(tree, keys + i, sizeof(int), e);
        assert(tree == e);
        assert(*(e->key) == keys[i]);
    }
    depth_first_print(tree);
    printf("---\n");
    fseek(file, SEEK_SET, 0);
    for(i = 0; i < count; i += 4) {
        pNode = NULL;
        SPLAY_REMOVE_KEYPTR(tree, keys + i, sizeof(int), pNode);
        assert(pNode);
    }
    depth_first_print(tree);
    free(nodes);
    free(keys);
    return 0;
}
