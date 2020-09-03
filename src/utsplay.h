/*
Copyright (c) 2003-2018, Troy D. Hanson     http://troydhanson.github.com/uthash/
Copyright (c) 2020, Brice Videau, Swann Perarnau
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef UTSPLAY_H
#define UTSPLAY_H

#define UTSPLAY_VERSION 1.0.0

#include <string.h>   /* memcmp, memset, strlen */
#include <stddef.h>   /* ptrdiff_t */
#include <stdlib.h>   /* exit */

/* These macros use decltype or the earlier __typeof GNU extension.
   As decltype is only available in newer compilers (VS2010 or gcc 4.3+
   when compiling c++ source) this code uses whatever method is needed
   or, for VS2008 where neither is available, uses casting workarounds. */
#if !defined(SPDECLTYPE) && !defined(NO_DECLTYPE)
#if defined(_MSC_VER)   /* MS compiler */
#if _MSC_VER >= 1600 && defined(__cplusplus)  /* VS2010 or newer in C++ mode */
#define SPDECLTYPE(x) decltype(x)
#else                   /* VS2008 or older (or VS2010 in C mode) */
#define NO_DECLTYPE
#endif
#elif defined(__BORLANDC__) || defined(__ICCARM__) || defined(__LCC__) || defined(__WATCOMC__)
#define NO_DECLTYPE
#else                   /* GNU, Sun and other compilers */
#define SPDECLTYPE(x) __typeof(x)
#endif
#endif

#ifdef NO_DECLTYPE
#define SPDECLTYPE(x) char*
#define SPDECLTYPE_ASSIGN(dst,src)                                                 \
do {                                                                             \
  char **_da_dst = (char**)(&(dst));                                             \
  *_da_dst = (char*)(src);                                                       \
} while (0)
#else
#define SPDECLTYPE_ASSIGN(dst,src)                                                 \
do {                                                                             \
  (dst) = (SPDECLTYPE(dst))(src);                                                    \
} while (0)
#endif

#ifndef utsplay_malloc
#define utsplay_malloc(sz) malloc(sz)      /* malloc fcn                      */
#endif
#ifndef utsplay_realloc
#define utsplay_realloc(ptr, sz) realloc(ptr, sz) /* realloc fcn                      */
#endif
#ifndef utsplay_free
#define utsplay_free(ptr,sz) free(ptr)     /* free fcn                        */
#endif
#ifndef utsplay_bzero
#define utsplay_bzero(a,n) memset((char*)(a),'\0',n)
#endif
#ifndef utsplay_strlen
#define utsplay_strlen(s) strlen(s)
#endif
#ifndef utsplay_memcmp
#define utsplay_memcmp(a,b,n) memcmp((const char*)(a),(const char*)(b),n)
#endif
#ifndef utsplay_strcmp
#define utsplay_strcmp(a,b) strcmp((const char*)(a),(const char*)(b))
#endif

#ifndef utsplay_oom
#define utsplay_oom() exit(-1)
#endif

#ifndef SPLAY_KEYCMP
#define SPLAY_KEYCMP(a,b,n) utsplay_memcmp(a,b,n)
#endif

#ifndef SPLAY_STRCMP
#define SPLAY_STRCMP(a,b,n) utsplay_strcmp(a,b)
#endif

#ifndef SPLAY_INIT
#define SPLAY_INIT(e)
#endif

#ifndef SPLAY_PROCESS
#define SPLAY_PROCESS(e)
#endif

#ifndef SPLAY_DEINIT
#define SPLAY_DEINIT(e)
#endif

/*
 * This file contains macros to manipulate splaytrees.
 *
 * Your structure must have a "left", "right", and "parent" pointer.
 * Either way, the pointer to the head of the tree must be initialized to NULL.
 *
 * ----------------.EXAMPLE -------------------------
 * struct item {
 *      int key;
 *      struct item *left, *right, *parent;
 * }
 *
 * struct item *tree = NULL:
 *
 * int main() {
 *      struct item *item;
 *      ... allocate and populate item ...
 *      SPLAY_ADD(tree, item);
 * }
 * --------------------------------------------------
 *
 */

/* See https://en.wikipedia.org/wiki/Splay_tree */

#define SPLAY_ADD_KEYSTR(tree, node) \
    SPLAY_ADD_KEYSTR2(tree, key, 0, node)

#define SPLAY_ADD_KEYSTR2(tree, key, sz, node) \
    SPLAY_ADD_KEYPTR3(tree, SPLAY_KEYPTR(node, key), sz, node, key, left, right, parent)

#define SPLAY_ADD_KEYSTR3(tree, key_val, sz, node, key, left, right, parent) \
    SPLAY_INSERT_ADD(SPLAY_ADD_CREATE, SPLAY_KEYPTR, SPLAY_STRCMP, tree, key_val, sz, node, key, left, right, parent)

#define SPLAY_ADD_KEYPTR(tree, sz, node) \
    SPLAY_ADD_KEYPTR2(tree, key, sz, node)

#define SPLAY_ADD_KEYPTR2(tree, key, sz, node) \
    SPLAY_ADD_KEYPTR3(tree, SPLAY_KEYPTR(node, key), sz, node, key, left, right, parent)

#define SPLAY_ADD_KEYPTR3(tree, key_val, sz, node, key, left, right, parent) \
    SPLAY_INSERT_ADD(SPLAY_ADD_CREATE, SPLAY_KEYPTR, SPLAY_KEYCMP, tree, key_val, sz, node, key, left, right, parent)

#define SPLAY_ADD(tree, node) \
    SPLAY_ADD2(tree, key, sizeof(SPLAY_FIELD(node, key)), node)

#define SPLAY_ADD2(tree, key,  sz, node) \
    SPLAY_ADD3(tree, SPLAY_KEY(node, key), sz, node, key, left, right, parent)

#define SPLAY_ADD3(tree, key_val, sz, node, key, left, right, parent) \
    SPLAY_INSERT_ADD(SPLAY_ADD_CREATE, SPLAY_KEY, SPLAY_KEYCMP, tree, key_val, sz, node, key, left, right, parent)

#define SPLAY_ADD_CREATE(key_macro, tree, n, key_val, sz, node, key, left, right, parent) \
do { \
    n = node; \
    SPLAY_RIGHT(n, right) = NULL; \
    SPLAY_LEFT(n, left) = NULL; \
    SPLAY_PARENT(n, parent) = NULL; \
    node = NULL; \
} while(0)

#define SPLAY_INSERT_KEYSTR(tree, key_val) \
    SPLAY_INSERT_KEYSTR2(tree, key_val, (utsplay_strlen(key_val)+1), key, left, right, parent)

#define SPLAY_INSERT_KEYSTR2(tree, key_val, sz, key, left, right, parent) \
    SPLAY_INSERT_ADD(SPLAY_INSERT_CREATE_KEYPTR, SPLAY_KEYPTR, SPLAY_STRCMP, tree, key_val, sz, NULL, key, left, right, parent)

#define SPLAY_INSERT_KEYPTR(tree, key_val, sz) \
    SPLAY_INSERT_KEYPTR2(tree, key_val, sz, key, left, right, parent)

#define SPLAY_INSERT_KEYPTR2(tree, key_val, sz, key, left, right, parent) \
    SPLAY_INSERT_ADD(SPLAY_INSERT_CREATE_KEYPTR, SPLAY_KEYPTR, SPLAY_KEYCMP, tree, key_val, sz, NULL, key, left, right, parent)

#define SPLAY_INSERT_CREATE_KEYPTR(key_macro, tree, n, key_val, sz, node, key, left, right, parent) \
do { \
    n = (SPDECLTYPE(tree))utsplay_malloc(sizeof(*tree) + sz); \
    if (!n) \
        utsplay_oom(); \
    utsplay_bzero(n, sizeof(*tree) + sz); \
    *((char **)&SPLAY_FIELD(n, key)) = (sizeof(*tree) + (char *)n); \
    memcpy(key_macro(n, key), key_val, sz); \
} while(0)

#define SPLAY_INSERT(tree, key_val, sz) \
    SPLAY_INSERT2(tree, key_val, sz, key, left, right, parent)

#define SPLAY_INSERT2(tree, key_val, sz, key, left, right, parent) \
    SPLAY_INSERT_ADD(SPLAY_INSERT_CREATE, SPLAY_KEY, SPLAY_KEYCMP, tree, key_val, sz, NULL, key, left, right, parent)

#define SPLAY_INSERT_CREATE(key_macro, tree, n, key_val, sz, node, key, left, right, parent) \
do { \
    n = (SPDECLTYPE(tree))utsplay_malloc(sizeof(*tree)); \
    if (!n) \
        utsplay_oom(); \
    utsplay_bzero(n, sizeof(*tree)); \
    memcpy(key_macro(n, key), key_val, sz); \
} while(0)

#define SPLAY_COPY_KEY(node, key_val, sz, key) \

#define SPLAY_INSERT_ADD(create_macro, key_macro, cmp_macro, tree, key_val, sz, node, key, left, right, parent) \
do { \
    SPDECLTYPE(tree) _sp_z; \
    SPDECLTYPE(tree) _sp_p; \
    _sp_z = tree; \
    _sp_p = NULL; \
    int _sp_cmp = 0; \
    int _sp_found = 0; \
    while (_sp_z) { \
        _sp_p = _sp_z; \
        _sp_cmp = cmp_macro(key_macro(_sp_z, key), key_val, sz); \
	if (_sp_cmp < 0) \
            _sp_z = SPLAY_RIGHT(_sp_z, right); \
        else if (_sp_cmp > 0)  \
            _sp_z = SPLAY_LEFT(_sp_z, left); \
        else { \
            _sp_found = 1; \
            break; \
        } \
    } \
    if (!_sp_found) { \
        create_macro(key_macro, tree, _sp_z,  key_val, sz, node, key, left, right, parent); \
        SPLAY_PARENT(_sp_z, parent) = _sp_p; \
        if (!_sp_p) \
            tree = _sp_z; \
        else if (_sp_cmp < 0) \
            SPLAY_RIGHT(_sp_p, right) = _sp_z; \
        else \
            SPLAY_LEFT(_sp_p, left) = _sp_z; \
        SPLAY_INIT(_sp_z); \
    } \
    SPLAY_SPLAY2(tree, _sp_z, left, right, parent); \
} while (0)

#define SPLAY_SPLAY(tree, node) \
    SPLAY_SPLAY2(tree, node, left, right, parent)

#define SPLAY_SPLAY2(tree, node, left, right, parent) \
do { \
    SPDECLTYPE(tree) _spp_p = NULL; \
    SPDECLTYPE(tree) _spp_g = NULL; \
    while ((_spp_p = SPLAY_PARENT(node, parent))) { \
        _spp_g = SPLAY_PARENT(_spp_p, parent); \
        if (!_spp_g) { \
            if (SPLAY_LEFT(_spp_p, left)) \
                SPLAY_RIGHT_ROTATE2(tree, _spp_p, left, right, parent); \
            else \
                SPLAY_LEFT_ROTATE2(tree, _spp_p, left, right, parent); \
        } else if (SPLAY_LEFT(_spp_p, left) == node && SPLAY_LEFT(_spp_g, left) == _spp_p) { \
            SPLAY_RIGHT_ROTATE2(tree, _spp_g, left, right, parent); \
            SPLAY_RIGHT_ROTATE2(tree, _spp_p, left, right, parent); \
        } else if (SPLAY_RIGHT(_spp_p, right) == node && SPLAY_RIGHT(_spp_g, right) == _spp_p) { \
            SPLAY_LEFT_ROTATE2(tree, _spp_g, left, right, parent); \
            SPLAY_LEFT_ROTATE2(tree, _spp_p, left, right, parent); \
        } else if (SPLAY_LEFT(_spp_p, left) == node && SPLAY_RIGHT(_spp_g, right) == _spp_p) { \
            SPLAY_RIGHT_ROTATE2(tree, _spp_p, left, right, parent); \
            SPLAY_LEFT_ROTATE2(tree, _spp_g, left, right, parent); \
        } else { \
            SPLAY_LEFT_ROTATE2(tree, _spp_p, left, right, parent); \
            SPLAY_RIGHT_ROTATE2(tree, _spp_g, left, right, parent); \
        } \
    } \
} while(0)

#define SPLAY_LEFT_ROTATE(tree, node) \
    SPLAY_LEFT_ROTATE2(tree, node, left, right, parent)

#define SPLAY_LEFT_ROTATE2(tree, node, left, right, parent) \
do { \
    SPDECLTYPE(tree) _splr_r = NULL; \
    SPDECLTYPE(tree) _splr_p = NULL; \
    _splr_r = SPLAY_RIGHT(node, right); \
    _splr_p = SPLAY_PARENT(node, parent); \
    SPLAY_RIGHT(node, right) = SPLAY_LEFT(_splr_r, left); \
    if (SPLAY_LEFT(_splr_r, left)) \
       SPLAY_PARENT(SPLAY_LEFT(_splr_r, left), parent) = node; \
    SPLAY_PARENT(_splr_r, parent) = _splr_p; \
    if (!_splr_p) \
        tree = _splr_r; \
    else if (SPLAY_LEFT(_splr_p, left) == node) \
        SPLAY_LEFT(_splr_p, left) = _splr_r; \
    else \
        SPLAY_RIGHT(_splr_p, right) = _splr_r; \
    SPLAY_LEFT(_splr_r, left) = node; \
    SPLAY_PARENT(node, parent) = _splr_r; \
    SPLAY_PROCESS(node); \
    SPLAY_PROCESS(_splr_r); \
} while (0)

#define SPLAY_RIGHT_ROTATE(tree, node) \
    SPLAY_RIGHT_ROTATE2(tree, node, left, right, parent)

#define SPLAY_RIGHT_ROTATE2(tree, node, left, right, parent) \
do { \
    SPDECLTYPE(tree) _sprr_l = NULL; \
    SPDECLTYPE(tree) _sprr_p = NULL; \
    _sprr_l = SPLAY_LEFT(node, left); \
    _sprr_p = SPLAY_PARENT(node, parent); \
    SPLAY_LEFT(node, left) = SPLAY_RIGHT(_sprr_l, right); \
    if (SPLAY_RIGHT(_sprr_l, right)) \
        SPLAY_PARENT(SPLAY_RIGHT(_sprr_l, right), parent) = node; \
    SPLAY_PARENT(_sprr_l, parent) = _sprr_p; \
    if (!_sprr_p) \
        tree = _sprr_l; \
    else if (SPLAY_LEFT(_sprr_p, left) == node) \
        SPLAY_LEFT(_sprr_p, left) = _sprr_l; \
    else \
        SPLAY_RIGHT(_sprr_p, right) = _sprr_l; \
    SPLAY_RIGHT(_sprr_l, right) = node; \
    SPLAY_PARENT(node, parent) = _sprr_l; \
    SPLAY_PROCESS(node); \
    SPLAY_PROCESS(_sprr_l); \
} while (0)

#define SPLAY_FIND_KEYSTR(tree, key_val, out) \
    SPLAY_FIND_KEYSTR2(tree, key_val, (utsplay_strlen(key_val)+1), key, left, right, parent, out)

#define SPLAY_FIND_KEYSTR2(tree, key_val, sz, key, left, right, parent, out) \
    SPLAY_FIND3(SPLAY_KEYPTR, SPLAY_STRCMP, tree, key_val, sz, key, left, right, parent, out)

#define SPLAY_FIND_KEYPTR(tree, key_val, sz, out) \
    SPLAY_FIND_KEYPTR2(tree, key_val, sz, key, left, right, parent, out)

#define SPLAY_FIND_KEYPTR2(tree, key_val, sz, key, left, right, parent, out) \
    SPLAY_FIND3(SPLAY_KEYPTR, SPLAY_KEYCMP, tree, key_val, sz, key, left, right, parent, out)

#define SPLAY_FIND(tree, key_val, sz, out) \
    SPLAY_FIND2(tree, key_val, sz, key, left, right, parent, out)

#define SPLAY_FIND2(tree, key_val, sz, key, left, right, parent, out) \
    SPLAY_FIND3(SPLAY_KEY, SPLAY_KEYCMP, tree, key_val, sz, key, left, right, parent, out)

#define SPLAY_FIND3(key_macro, cmp_macro, tree, key_val, sz, key, left, right, parent, out) \
do { \
    SPDECLTYPE(tree) _spf_z = tree; \
    out = NULL; \
    int _spf_cmp = 0; \
    while (_spf_z) { \
        _spf_cmp = cmp_macro(key_macro(_spf_z, key), key_val, sz); \
        if (_spf_cmp < 0) \
            _spf_z = SPLAY_RIGHT(_spf_z, right); \
        else if (_spf_cmp > 0) \
            _spf_z = SPLAY_LEFT(_spf_z, left); \
        else {\
            out = _spf_z; \
            SPLAY_SPLAY2(tree, out, left, right, parent); \
            break; \
        } \
    } \
} while(0)

#define SPLAY_REMOVE_KEYSTR(tree, key_val, node) \
    SPLAY_REMOVE_KEYSTR2(tree, key_val, (utsplay_strlen(key_val)+1), node, key, left, right, parent)

#define SPLAY_REMOVE_KEYSTR2(tree, key_val, sz, node, key, left, right, parent) \
    SPLAY_ERASE_REMOVE(SPLAY_REMOVE_DELETE, SPLAY_KEYPTR, SPLAY_STRCMP, tree, key_val, sz, node, key, left, right, parent)

#define SPLAY_REMOVE_KEYPTR(tree, key_val, sz, node) \
    SPLAY_REMOVE_KEYPTR2(tree, key_val, sz, node, key, left, right, parent)

#define SPLAY_REMOVE_KEYPTR2(tree, key_val, sz, node, key, left, right, parent) \
    SPLAY_ERASE_REMOVE(SPLAY_REMOVE_DELETE, SPLAY_KEYPTR, SPLAY_KEYCMP, tree, key_val, sz, node, key, left, right, parent)

#define SPLAY_REMOVE(tree, key_val, sz, node) \
    SPLAY_REMOVE2(tree, key_val, sz, node, key, left, right, parent)

#define SPLAY_REMOVE2(tree, key_val, sz, node, key, left, right, parent) \
    SPLAY_ERASE_REMOVE(SPLAY_REMOVE_DELETE, SPLAY_KEY, SPLAY_KEYCMP, tree, key_val, sz, node, key, left, right, parent)

#define SPLAY_REMOVE_DELETE(tree, n, node, left, right, parent) \
do { \
    SPLAY_PARENT(n, parent) = NULL; \
    SPLAY_LEFT(n, left) = NULL; \
    SPLAY_RIGHT(n, right) = NULL; \
    node = n; \
} while (0)

#define SPLAY_ERASE_KEYSTR(tree, key_val) \
    SPLAY_ERASE_KEYSTR2(tree, key_val, (utsplay_strlen(key_val)+1), key, left, right, parent)

#define SPLAY_ERASE_KEYSTR2(tree, key_val, sz, key, left, right, parent) \
    SPLAY_ERASE_REMOVE(SPLAY_ERASE_DELETE, SPLAY_KEYPTR, SPLAY_STRCMP, tree, key_val, sz, NULL, key, left, right, parent)

#define SPLAY_ERASE_KEYPTR(tree, key_val, sz) \
    SPLAY_ERASE_KEYPTR2(tree, key_val, sz, key, left, right, parent)

#define SPLAY_ERASE_KEYPTR2(tree, key_val, sz, key, left, right, parent) \
    SPLAY_ERASE_REMOVE(SPLAY_ERASE_DELETE, SPLAY_KEYPTR, SPLAY_KEYCMP, tree, key_val, sz, NULL, key, left, right, parent)

#define SPLAY_ERASE(tree, key_val, sz) \
    SPLAY_ERASE2(tree, key_val, sz, key, left, right, parent)

#define SPLAY_ERASE2(tree, key_val, sz, key, left, right, parent) \
    SPLAY_ERASE_REMOVE(SPLAY_ERASE_DELETE, SPLAY_KEY, SPLAY_KEYCMP, tree, key_val, sz, NULL, key, left, right, parent)

#define SPLAY_ERASE_DELETE(tree, n, node, left, right, parent) \
do { \
    utsplay_free(_spe_z, sizeof(*tree)); \
} while (0)

#define SPLAY_ERASE_REMOVE(delete_macro, key_macro, cmp_macro, tree, key_val, sz, node, key, left, right, parent) \
do { \
    SPDECLTYPE(tree) _spe_z; \
    SPDECLTYPE(tree) _spe_p; \
    SPLAY_FIND3(key_macro, cmp_macro, tree, key_val, sz, key, left, right, parent, _spe_z); \
    if (_spe_z) { \
        if (!SPLAY_LEFT(_spe_z, left)) \
            SPLAY_REPLACE2(tree, _spe_z, SPLAY_RIGHT(_spe_z, right), left, right, parent); \
        else if (!SPLAY_RIGHT(_spe_z, right)) \
            SPLAY_REPLACE2(tree, _spe_z, SPLAY_LEFT(_spe_z, left), left, right, parent); \
        else  { \
            SPDECLTYPE(tree) _spe_l; \
            _spe_l = SPLAY_RIGHT(_spe_z, right); \
            while (SPLAY_LEFT(_spe_l, left)) \
                _spe_l = SPLAY_LEFT(_spe_l, left); \
            _spe_p = SPLAY_PARENT(_spe_l, parent); \
            if (_spe_p != _spe_z) { \
                SPLAY_REPLACE2(tree, _spe_l, SPLAY_RIGHT(_spe_l, right), left, right, parent); \
                SPLAY_RIGHT(_spe_l, right) = SPLAY_RIGHT(_spe_z, right); \
                SPLAY_PARENT(SPLAY_RIGHT(_spe_l, right), parent) = _spe_l; \
                SPLAY_PROCESS(_spe_p); \
            	while((_spe_p = SPLAY_PARENT(_spe_p, parent)) != _spe_l) \
                	SPLAY_PROCESS(_spe_p); \
            } \
            SPLAY_REPLACE2(tree, _spe_z, _spe_l, left, right, parent); \
            SPLAY_LEFT(_spe_l, left) = SPLAY_LEFT(_spe_z, left); \
            SPLAY_PARENT(SPLAY_LEFT(_spe_l, left), parent) = _spe_l; \
            SPLAY_PROCESS(_spe_l); \
        } \
        delete_macro(tree, _spe_z, node, left, right, parent); \
    } \
} while (0)

#define SPLAY_REPLACE(tree, u, v) \
    SPLAY_REPLACE2(tree, u, v, left, right, parent)

#define SPLAY_REPLACE2(tree, u, v, left, right, parent) \
do { \
    if (tree) { \
        SPDECLTYPE(tree) _spr_p; \
        _spr_p = SPLAY_PARENT(u, parent); \
        if (!_spr_p) \
            tree = v; \
        else if (SPLAY_LEFT(_spr_p, left) == u) {\
            SPLAY_LEFT(_spr_p, left) = v; \
            SPLAY_PROCESS(_spr_p); \
        } else { \
            SPLAY_RIGHT(_spr_p, right) = v; \
            SPLAY_PROCESS(_spr_p); \
        } \
        if (v) \
            SPLAY_PARENT(v, parent) = _spr_p; \
    } \
} while(0)

#define SPLAY_CLEAR(tree) \
    SPLAY_CLEAR2(tree, left, right, parent)

#define SPLAY_CLEAR2(tree, left, right, parent) \
do { \
    SPDECLTYPE(tree) _spc_z; \
    SPDECLTYPE(tree) _spc_p; \
    _spc_z = tree; \
    while (_spc_z) { \
        if (SPLAY_LEFT(_spc_z, left)) \
            _spc_z = SPLAY_LEFT(_spc_z, left); \
        else if (SPLAY_RIGHT(_spc_z, right)) \
            _spc_z = SPLAY_RIGHT(_spc_z, right); \
        else { \
            _spc_p = SPLAY_PARENT(_spc_z, parent); \
            if (_spc_p) { \
                if (SPLAY_LEFT(_spc_p, left) == _spc_z) \
                    SPLAY_LEFT(_spc_p, left) = NULL; \
                else \
                    SPLAY_RIGHT(_spc_p, right) = NULL; \
            } \
            SPLAY_DEINIT(_spc_z); \
            utsplay_free(_spc_z, sizeof(*tree)); \
            _spc_z = _spc_p; \
        } \
    } \
} while(0)

static inline
void * _splay_buffalloc(void *pe) {
    if (!pe)
        return NULL;
    void **mem = (void **)utsplay_malloc(10 * sizeof(void *));
    if (!mem)
        utsplay_oom();
    *mem = pe;
    return (void *)mem;
}

static inline
void * _splay_buffalloc2(void *pe) {
    if (!pe)
        return NULL;
    void **mem = (void **)utsplay_malloc(10 * sizeof(void *));
    if (!mem)
        utsplay_oom();
    return (void *)mem;
}

#define splay_define_stack(tree) struct { \
  SPDECLTYPE(tree) *arr; \
  size_t c; \
  size_t sz; }

#define splay_init_stack(tree) { \
    (SPDECLTYPE(&tree))_splay_buffalloc((void *)tree), \
    (tree ? 1 : 0), (tree ? 10 : 0) }

#define splay_deinit_stack(stack) ( \
    utsplay_free(stack.arr, stack.sz * sizeof(void*)) )

#define splay_maybe_realloc_stack(tree, stack) ( \
    stack.c == stack.sz ? ( \
        stack.arr = (SPDECLTYPE(&tree)) \
            utsplay_realloc(stack.arr, stack.sz * 2 * sizeof(void*)), \
        (stack.arr ? 0 : (utsplay_oom(), 0)), \
        stack.sz = stack.sz * 2) : 1 )

#define splay_push_back_field(tree, stack, el, field) ( \
  SPLAY_FIELD( el, field ) ? splay_push_back(stack, SPLAY_FIELD( el, field )) : 0 )

#define splay_push_back(stack, el) (\
    splay_maybe_realloc_stack(tree,stack), \
    stack.arr[stack.c++] = el )

#define splay_pop_back(stack) stack.arr[--(stack.c)]

#define splay_peek_back(stack) stack.arr[stack.c - 1]

#define SPLAY_FOREACH_DFSPRE(tree, el) \
    SPLAY_FOREACH_DFSPRE2(tree, el, left, right, parent)

#define SPLAY_FOREACH_DFSPRE2(tree, el, left, right, parent) \
for ( splay_define_stack(tree) _sp_stack = splay_init_stack(tree); \
     (_sp_stack.c ? 1 : (splay_deinit_stack(_sp_stack), 0)) && \
         ( el = splay_pop_back(_sp_stack), 1 ); \
     splay_push_back_field(tree, _sp_stack, el, right), \
     splay_push_back_field(tree, _sp_stack, el, left))

#define splay_define_stack2(tree) struct { \
  SPDECLTYPE(tree) *arr; \
  size_t c; \
  size_t sz; \
  int i; }

#define splay_init_stack2(tree, el) { \
    (el = tree, (SPDECLTYPE(&tree))_splay_buffalloc2((void *)tree)), \
    0, tree ? 10 : 0, 0 }

#define SPLAY_FOREACH_DFSIN(tree, el) \
    SPLAY_FOREACH_DFSIN2(tree, el, left, right, parent)

#define SPLAY_FOREACH_DFSIN2(tree, el, left, right, parent) \
for ( splay_define_stack2(tree) _sp2_stack = splay_init_stack2(tree, el); \
    ((_sp2_stack.c || el) ? 1 : (splay_deinit_stack(_sp2_stack), 0)); ) if (el) { \
        splay_push_back(_sp2_stack, el); el = SPLAY_LEFT(el, left); } \
    else for (_sp2_stack.i = 0; _sp2_stack.i < 1 && (el = splay_pop_back(_sp2_stack), 1); _sp2_stack.i++, el = SPLAY_RIGHT(el, right))

#define splay_define_stack3(tree) struct { \
  SPDECLTYPE(tree) *arr; \
  size_t c; \
  size_t sz; \
  SPDECLTYPE(tree) last; \
  int i; }

#define splay_init_stack3(tree, el) { \
    (el = tree, (SPDECLTYPE(&tree))_splay_buffalloc2((void *)tree)), \
    0, tree ? 10 : 0, NULL, 0 }

#define SPLAY_FOREACH_DFSPOST(tree, el) \
    SPLAY_FOREACH_DFSPOST2(tree, el, left, right, parent)

#define SPLAY_FOREACH_DFSPOST2(tree, el, left, right, parent) \
for ( splay_define_stack3(tree) _sp3_stack = splay_init_stack3(tree, el); \
    ((_sp3_stack.c || el) ? 1 : (splay_deinit_stack(_sp3_stack), 0)); ) if (el) { \
        splay_push_back(_sp3_stack, el); el = SPLAY_LEFT(el, left);} \
    else if ((el = splay_peek_back(_sp3_stack), SPLAY_RIGHT(el, right)) && SPLAY_RIGHT(el, right) != _sp3_stack.last) { el = SPLAY_RIGHT(el, right); } \
         else for (_sp3_stack.i = 0; _sp3_stack.i < 1; _sp3_stack.i++, _sp3_stack.last = splay_pop_back(_sp3_stack), el = NULL)

#define SPLAY_FIELD(elt, name) ((elt)->name)
#define SPLAY_KEY(elt, key) (&((elt)->key))
#define SPLAY_KEYPTR(elt, key) ((elt)->key)
#define SPLAY_LEFT(elt, left) ((elt)->left)
#define SPLAY_RIGHT(elt, right) ((elt)->right)
#define SPLAY_PARENT(elt, parent) ((elt)->parent)

#endif /*UTSPLAY_H*/
