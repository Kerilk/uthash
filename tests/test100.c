#include "utsplay.h"
#include <stdlib.h>   /* malloc, atoi */
#include <errno.h>    /* perror */
#include <stdio.h>    /* printf */
#include <assert.h>

#define BUFLEN 20

struct splay_elem_s;
typedef struct splay_elem_s {
    struct splay_elem_s *left, *right, *parent;
    char *key;
} splay_elem_t;

void depth_first_print(splay_elem_t *elt) {
    if (elt->left)
        depth_first_print(elt->left);
    if (elt->right)
        depth_first_print(elt->right);
    printf("%s\n", elt->key);
}

int main()
{
    splay_elem_t *tree = NULL;
    char linebuf[BUFLEN];
    FILE *file;

    file = fopen( "test100.dat", "r" );
    if (file == NULL) {
        perror("can't open: ");
        exit(-1);
    }

    while (fgets(linebuf, BUFLEN, file) != NULL) {
        *strchr(linebuf, '\n') = '\0';
        SPLAY_INSERT_KEYSTR(tree,  linebuf);
    }
    depth_first_print(tree);
    printf("---\n");
    fseek(file, SEEK_SET, 0);
    while (fgets(linebuf, BUFLEN, file) != NULL) {
        *strchr(linebuf, '\n') = '\0';
        splay_elem_t *e;
        SPLAY_FIND_KEYSTR(tree, linebuf, e);
        assert(tree == e);
        assert(!strcmp(e->key, linebuf));
    }
    depth_first_print(tree);
    printf("---\n");
    fseek(file, SEEK_SET, 0);
    int i = 0;
    while (fgets(linebuf, BUFLEN, file) != NULL) {
        if (0 == i % 4) {
            *strchr(linebuf, '\n') = '\0';
            SPLAY_ERASE_KEYSTR(tree, linebuf);
        }
        i++;
    }
    depth_first_print(tree);
    SPLAY_CLEAR(tree);
    fclose(file);
    return 0;
}
