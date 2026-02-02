#ifndef __ca_bintree_h
#define __ca_bintree_h

typedef enum {
    CA_GRAPH=5,
    CA_BUFFER=1,
    CA_VECTOR=2,
    CA_BINTREE=3,
    CA_TRITREE=4,
} ca_tree_type;

typedef enum {
    CA_MOV_NEXT=0,
    CA_MOV_LEFT=0,
    CA_MOV_RIGHT=2,
    CA_MOV_PARENT=1,
    CA_MOV_FORWARD=3
}ca_tree_mov;

typedef struct {
    void *key;
    void *val;
    struct {
        unsigned long key;
        unsigned long value;
    } len;
} ca_data_kv;

typedef struct {
  void *data;
  struct ca_node *mov;
  struct {
    unsigned long data;
    unsigned long dir;
  } len;
  struct {
    unsigned long start;
    unsigned long end;
  } rw;
} ca_node;

typedef struct {
  ca_node *root;
  ca_tree_type type;
  unsigned long len;
} ca_tree;

typedef ca_node ca_buffer;
typedef ca_tree ca_vector;
typedef ca_tree ca_bintree;
typedef ca_tree ca_tritree;
typedef ca_node ca_ringbuffer;

typedef int (*ca_tree_map)(ca_tree *node, void *user_data);
typedef int (*ca_node_compare)(ca_node *n1, ca_node *n2);

int ca_tree_create(ca_tree **tree, ca_tree_type type);
int ca_tree_destroy(ca_tree *tree);

int ca_node_create(ca_node **node);
int ca_node_delete(ca_node *node);

int ca_tree_set(ca_tree *tree, ca_data_kv *key, unsigned long key_len, void *val, unsigned long val_len);
int ca_tree_get(ca_tree *tree, void *key, unsigned long key_len, void *out);

int ca_tree_traverse(ca_tree *tree, ca_tree_map handler);
void *ca_tree_delete_node(ca_tree *tree, void *key, unsigned long key_len);

int ca_node_swap(ca_node *a, ca_node *b);

#endif

#define CA_TREE_IMPL //For LSP: remember to comment
#define CA_BINTREE_IMPL //For LSP: remember to comment
#define CA_TRITREE_IMPL //For LSP: remember to comment
#define CA_GRAPH_IMPL //For LSP: remember to comment

#ifdef CA_TREE_IMPL
#include <stdbool.h>
#include <errno.h>

int ca_bintree_compare(void *a, void *b, unsigned long len){
    if(a==0
        || b==0){
        errno=EINVAL;
        return -1;
    }

    int v = 0;
    for(int i=0;i<len;i++){
        v = ((unsigned char *)a)[i] - ((unsigned char *)b)[i];
        if(v!=0){
            return v;
        }
    }
    return 0;
}

int ca_bintree_compare_node(ca_bintree_node *a, ca_bintree_node *b){
    if(a==0
        || b==0){
        errno=EINVAL;
        return -1;
    }
    if(a->key==0
        || b->key==0
        || a->key_len==0
        || b->key_len==0
    ){
        errno=ENOBUFS;
        return -1;
    }

    if(a->key_len != b->key_len){
        return a->key_len - b->key_len;
    }

    return ca_bintree_compare(a->key,b->key,a->key_len);
}

#endif
