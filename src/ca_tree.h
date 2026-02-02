#ifndef __ca_tree_h
#define __ca_tree_h

typedef unsigned long size; //less includes
typedef size len; //disambiguates item length with memory size
typedef size type; //disambiguates type size with memory size
typedef unsigned char byte; //sometimes defined in windows
typedef struct { //access single bit
    unsigned val:1;
}bit;

typedef enum { //Status codes
    CA_ERROR_NG=-1, //negative value, something happened
    CA_ERROR    = 0, //false, function error
    CA_OK       = 1, //true, function succeeded
    CA_ENABLED  = 1, //sets flag bit to 1
    CA_EQUAL    = 0 // a-b==0?
}ca_tree_status;

typedef enum { //Flags for data types, makes programming easier
    CA_DATA_TYPE_MIN=0, //min value validation
    //
    CA_DATA_ERROR=CA_ERROR, //false, general error
    CA_DATA_NONE=CA_DATA_TYPE_MIN, //no data

    CA_DATA_IS_BLOCK=4,  //100
    CA_DATA_IS_ITEMS=2,  //010
    CA_DATA_IS_KEYVAL=1, //001

    CA_DATA_IS_EMPTY=    //111
        CA_DATA_IS_BLOCK + CA_DATA_IS_ITEMS + CA_DATA_IS_KEYVAL,
        //INIT empty: type is block, no data, semantic block data empty

    // type keyval cannot be init empty, contains struct ca_data_kv*

    CA_DATA_IS_EMPTY_BLOCK= //101
        CA_DATA_IS_BLOCK + CA_DATA_IS_KEYVAL,
        //type is block, no data

    CA_DATA_IS_EMPTY_ITEMS= //011
        CA_DATA_IS_ITEMS + CA_DATA_IS_KEYVAL,
        //type is items, no data

    CA_DATA_TYPE_MAX=CA_DATA_IS_EMPTY, //max value validation
} ca_data_type;

typedef enum {
    CA_TREE_TYPE_MIN=CA_ERROR,   // min value validation

    CA_TREE_NONE    =0,          //NO data
    CA_TREE_ERROR   =CA_ERROR,   //false

    CA_GRAPH    =8,     //10000
                        // One to many
    CA_DATA     =6,     //01000
    CA_BUFFER=CA_DATA,  //contigous memory
                        //sometimes called Vector
                        //CONV_BREAK: To me,
                        //Buffers are contigous blocks of memory
    CA_VECTOR   =4,     //00100
                        //Forward || Backward || Both,
                        //linked items 
                        //Sometimes called Linked List
                        //CONV_BREAK: To me,
                        //a Vector is a set of linked data points
    CA_BINTREE  =2,     //00010
                        //Left && Right Tree
    CA_TRITREE  =1,     //00001
                        //Left && Right && Forward Tree
    //
    CA_TREE_EMPTY=      //INIT Empty structure
        CA_GRAPH+CA_DATA+CA_VECTOR+CA_BINTREE+CA_TRITREE,

    CA_TREE_TYPE_MAX=CA_TREE_EMPTY // max value validation
} ca_tree_type;


// CA DATA: represents a piece of data.
// By some called Vecor or Buffer.
typedef struct {
    union{
        struct {
            unsigned mask:3;      //000
        };
        struct {
            unsigned is_block:1;  //100
            unsigned is_items:1;  //010
            unsigned is_keyval:1; //001
        };
    };
}ca_data_type_bflag;

typedef struct {
    void *bytes;
    struct {
        len items; //number of items
        size memory; //total memory size
    }len;
    ca_data_type_bflag type;
    unsigned is_empty:1;
} ca_data;

int ca_data_compare(ca_data *a, ca_data *b);
int ca_data_write(ca_data *a,void *data, size mlen);
int ca_data_read(ca_data *a,void *data, size mlen);

typedef enum {
    CA_DATA_DEF_TYPE= CA_DATA_IS_BLOCK,
    CA_DATA_DEF_ILEN= sizeof(byte),
    CA_DATA_DEF_MLEN= 32*CA_DATA_DEF_ILEN,
}ca_data_defaults;

typedef struct ca_data ca_buff; //if you want to call it a buffer

int ca_data_new(ca_data **data, len item_num, type item_len, size mlen, ca_data_type flag);
#define ca_buff_new(DATA,NUM,TYPE) ca_data_new(DATA,NUM,TYPE)
int ca_data_del(ca_data *data);
#define ca_buff_del(DATA) ca_data_del(DATA)

int ca_data_compare(ca_data *a, ca_data *b);

typedef struct {
    ca_data *key;
    ca_data *val;
}ca_data_kv;

int ca_data_kv_new(ca_data_kv **data, ca_data *key, ca_data *val);
int ca_data_kv_del(ca_data_kv *data);


// CA NODE
typedef struct { //6bits
    union {
        struct {
            unsigned mask:5;   //00000
        };
        struct {
            unsigned graph:1;  //10000:8
            unsigned buffer:1; //01000:6
            unsigned vector:1; //00100:4
            unsigned bintree:1;//00010:2
            unsigned tritree:1;//00001:1
        };
    };
} ca_tree_type_bflag;

int ca_tree_type_set(ca_tree_type_bflag *flag, ca_tree_type type);

typedef struct {
     // Binary trees use l, r & p
     union { //naming disambiguation, same pointer
        struct ca_node *left;
        struct ca_node *up;
     };
     union { //naming disambiguation, same pointer
        struct ca_node *right;
        struct ca_node *down;
     };

     union { //naming disambiguation, same pointer
        struct ca_node *child;
        struct ca_node *forward;
     };

     union { //naming disambiguation, same pointer
        struct ca_node *parent;
        struct ca_node *back;
     };

     struct {
         union { //naming disambiguation, same value
                len child;
                len forward;
            };
         union { //naming disambiguation, same value
                len parent;
                len back;
            };
     } len;
} ca_node_mov;

typedef struct { //represents node in a tree or vector
  ca_data *val; //data
  ca_node_mov mov; //movement 
  ca_tree_type_bflag type; //pointer to parents tree type
} ca_node;

int ca_node_new(ca_node **node, ca_node *parent,
                void *data, size data_mlen, len data_len,
                ca_tree_type type);
int ca_node_del(ca_node *node);
int ca_node_compare(ca_node *a, ca_node *b);

// CA TREE
//Can be many data structured depending on mov
typedef struct { //we all become trees in the end
  ca_node *root; //start node, 
  len len;
  ca_tree_type_bflag type; //type of data structure
} ca_tree;

typedef ca_tree ca_vec;
typedef ca_vec ca_llist;
typedef ca_tree ca_tree_bin;
typedef ca_tree ca_tree_tri;

int ca_tree_new(ca_tree **tree, ca_tree_type type);
int ca_tree_destroy(ca_tree *tree);

typedef struct {
    ca_data val;
    size start;
    size end;
} ca_buff_ring;

typedef ca_buff_ring ca_data_ring;

int ca_buff_ring_new(ca_buff_ring **buff);
int ca_buff_ring_del(ca_buff_ring *buff);

typedef int (ca_tree_map)(ca_tree *node, void *user_data);

int ca_tree_traverse(ca_tree *tree, ca_tree_map handler);
void *ca_tree_del_node(ca_tree *tree, ca_data *key);

#endif

#if defined(CA_TREE_IMPL) || defined(DEBUG)
#include <stdlib.h>
#include <errno.h>

int ca_byte_compare(byte *a, byte *b, size size){
    int v = CA_EQUAL;
    for(int i=0;i<size;i++){
        if((v = (a[i] - b[i])) != CA_EQUAL){
            return v;
        }
    }
    return CA_EQUAL;
}

int ca_data_compare(ca_data *a, ca_data *b){
    if( //invalid argument
           a==NULL
           || b==NULL) goto invalid;

    if( //A is empty or smaller
        a->len.memory==CA_DATA_NONE
        || a->len.memory<b->len.memory) return b->len.memory;
    if( //B is empty or smaller
        b->len.memory==CA_DATA_NONE
        || b->len.memory<a->len.memory) return a->len.memory;

    if(//Comparing items that are not alike
        a->type.mask != b->type.mask) goto invalid;

    errno = CA_OK;
    int rc = CA_EQUAL;
    if((rc = ca_byte_compare(
        a->bytes,
        b->bytes,
        a->len.memory)
       ) == CA_ERROR_NG
       && errno != CA_OK) goto error;
    // Could be CA_ERROR_NG if comparing 1-2 (-1)
    // Real errors set errno.

    return CA_EQUAL;
invalid:
    errno=EINVAL;
error:
    return CA_ERROR_NG;
}

int ca_node_compare(ca_node *a, ca_node *b){
    if( //invalid argument
           a==NULL
        || b==NULL
      ) goto invalid;

    if(//Comparing unlike items
        a->type.mask!=b->type.mask) goto invalid;

    if( //no bytes to compare
           a->val->bytes==NULL
        || b->val->bytes==NULL
        || a->val->len.items==CA_DATA_NONE
        || b->val->len.memory==CA_DATA_NONE
      ) goto nobuff;

    if( //TODO: Compare keys and values
        a->val->type.is_keyval) goto noimpl;

    errno = CA_OK;
    int rc = CA_EQUAL;
    if((rc = ca_data_compare(
        a->val->bytes,
        b->val->bytes)
       ) == CA_ERROR_NG
       && errno != CA_OK) goto error;
    // Could be CA_ERROR_NG if comparing 1-2 (-1)
    // Real errors set errno.

    return CA_EQUAL;
noimpl:
    errno=ENOSYS;
    goto error;
invalid:
    errno=EINVAL;
    goto error;
nobuff:
    errno=ENOBUFS;
error:
    return CA_ERROR_NG;
}

int ca_data_new(ca_data **data, len item_num, type ilen, size mlen, ca_data_type type){
    if( //invalid argument
        data==NULL) goto invalid;
    if(*data!=NULL) goto invalid;
    if(type<=CA_DATA_TYPE_MIN
        || type>CA_DATA_TYPE_MAX) goto invalid;

    *data = (ca_data *)calloc(1,sizeof(ca_data));
    if(data==NULL) goto error;

    switch(type){
        case CA_DATA_IS_BLOCK:
            if(mlen<=CA_DATA_NONE
                || ilen!=CA_DATA_NONE
                || item_num!=CA_DATA_NONE) goto invalid;

            (*data)->type.is_block=CA_ENABLED;
            (*data)->len.memory=mlen;
            (*data)->len.items=mlen/CA_DATA_DEF_ILEN;
        break;

        case CA_DATA_IS_ITEMS:
            if(mlen!=CA_DATA_NONE
                || ilen<=CA_DATA_NONE
                || item_num<=CA_DATA_NONE) goto invalid;

            (*data)->type.is_items=CA_ENABLED;
            (*data)->len.memory=item_num*ilen;
            (*data)->len.items=ilen;
        break;

        case CA_DATA_IS_EMPTY_ITEMS:
            (*data)->type.is_items=CA_ENABLED;
            if(ilen<=CA_DATA_NONE) goto invalid;

        case CA_DATA_IS_EMPTY_BLOCK:
            (*data)->type.is_block=CA_ENABLED;
            if(ilen!=CA_DATA_NONE) goto invalid;

        case CA_DATA_NONE:
        case CA_DATA_IS_EMPTY:
        default:
            if(mlen!=CA_DATA_NONE
                || item_num!=CA_DATA_NONE) goto invalid;

            (*data)->is_empty=CA_ENABLED;
            (*data)->len.memory=CA_DATA_NONE;
            (*data)->len.items=CA_DATA_NONE;
        break;
    }

    if((*data)->len.memory<0) goto nobuff;

    (*data)->bytes=calloc(1,(*data)->len.memory);
    if((*data)->bytes==NULL) goto error;

    return CA_OK;
nobuff:
    errno=ENOBUFS;
    goto error;
invalid:
    errno=EINVAL;
    if((*data)!=NULL) free(*data);
error:
    return CA_ERROR;
}

int ca_data_del(ca_data *data){
    if(data==NULL) goto invalid;

    if(data->bytes!=NULL) free(data->bytes);
    free(data);
    return CA_OK;
invalid:
    errno=EINVAL;
    return CA_ERROR;
}

int ca_data_kv_new(ca_data_kv **data, ca_data *key, ca_data *val){
    if(data==NULL
        || key==NULL
        || val==NULL) goto invalid;
    if(*data!=NULL) goto invalid;

    *data = calloc(1,sizeof(ca_data_kv));
    if(data==NULL) goto error;

    (*data)->key = key;
    (*data)->val = val;

    return CA_OK;
invalid:
    errno = EINVAL;
error:
    return CA_ERROR;
}

int ca_data_kv_del(ca_data_kv *data){
    if(data==NULL) goto invalid;

    if(data->key!=NULL)
        if(!ca_data_del(data->key))
            goto error;
    if(data->val!=NULL)
        if(!ca_data_del(data->key))
            goto error;

    free(data);

    return CA_OK;
invalid:
    errno = EINVAL;
error:
    return CA_ERROR;
}

int ca_tree_type_set(ca_tree_type_bflag *flag, ca_tree_type type){
    if(flag==NULL
        || type <= CA_TREE_ERROR
        || type > CA_TREE_TYPE_MAX) goto invalid;

    switch(type){
        case CA_GRAPH:
            flag->graph=1;
            break;
        case CA_BUFFER:
            flag->buffer=1;
            break;
        case CA_VECTOR:
            flag->vector=1;
            break;
        case CA_BINTREE:
            flag->bintree=1;
            break;
        case CA_TRITREE:
            flag->tritree=1;
            break;
        default:
            goto invalid;
        break;
    }
    return CA_OK;
invalid:
    errno=EINVAL;
    return CA_ERROR;
}

int ca_node_new(ca_node **node, ca_node *parent, void *data, size data_mlen, len data_len, ca_tree_type type){
    if(node == NULL
        || parent == NULL
        || data == NULL) goto invalid;
    if(*node!=NULL
        || data_mlen <= 0
        || data_len >= data_mlen //e.g data_len = data_mlen/(siseof(byte)) | data_mlen = 16/8
        || type>CA_TREE_TYPE_MAX
        || type<CA_TREE_TYPE_MIN) goto invalid;

    (*node) = calloc(1,sizeof(ca_node));
    if(*node==NULL) goto error;

    if(!ca_tree_type_set(&(*node)->type,type)){
        (*node)->type = parent->type;
    }

    (*node)->mov.parent = (struct ca_node *)parent;

    (*node)->val->bytes = data;
    (*node)->val->len.memory = data_mlen;
    (*node)->val->len.items =
        data_len == 0 ? data_mlen/sizeof(byte) : data_len;

    return CA_OK;
invalid:
    errno=EINVAL;
error:
    return CA_ERROR;
}

int ca_node_del(ca_node *node){
    if(node==NULL) goto invalid;

    return CA_OK;
invalid:
    errno=EINVAL;
error:
    return CA_ERROR;
}
#endif
