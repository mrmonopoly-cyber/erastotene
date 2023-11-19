#include "dynamic_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCREASE_SIZE_FACTOR 2

struct list{
    unsigned int (*comparison_function) (void *,void *);
    void (*free_element) (void *);
    void (*print_ele) (void *);
    struct list *root;
    void *elements;
    unsigned int capacity;
    unsigned int num_eles;
    unsigned int ele_size;
};

//public
struct list *new_list(unsigned int size,unsigned int size_element, 
        unsigned int (*comparison_function) (void *,void *),
        void (*free_element) (void *),
        void (*print_element) (void *))
{
    struct list *new_list;

    new_list = malloc(sizeof(*new_list));
    if(!new_list){
        fprintf(stderr, "dynamic_buffer,error malloc at line: %d\n",__LINE__);
        goto error_malloc_new_list;
    }
    new_list->root=new_list;
    new_list->capacity=size;
    new_list->num_eles=0;
    new_list->ele_size=size_element;
    new_list->free_element=free_element;
    new_list->comparison_function=comparison_function;
    new_list->print_ele=print_element;
    new_list->elements=malloc(size * size_element);
    if(!new_list->elements){
        fprintf(stderr, "dynamic_buffer,error malloc at line: %d\n",__LINE__);
        goto error_malloc_elements;
    }
    return new_list;

error_malloc_elements:
        free(new_list);
error_malloc_new_list:
        return NULL;
}

void *list_add_element(struct list *this,void *element)
{
    if(!this || !element){
        fprintf(stderr, "invalid list or element\n");
        return NULL;
    }

    void *buffer = this->elements;
    unsigned int ele_size = this->ele_size;
    unsigned int ele_posi = this->num_eles;

    if(!this | !element){
        return NULL;
    }

    if(this->num_eles==this->capacity){
        this->capacity*=INCREASE_SIZE_FACTOR;
        this->elements=realloc(this->elements, this->capacity);
    }
    memcpy(buffer+(ele_posi*ele_size), element, ele_size);
    this->num_eles++;
    return buffer + (ele_posi*ele_size);
}

void *search_element(struct list *this,void *element)
{
    unsigned int i=0;
    void *ele_l=NULL;

    if(!this || !element){
        return NULL;
    }

    for(i=0;i<this->capacity;i++){
        ele_l=this->elements + i*this->ele_size;
        if(this->comparison_function(ele_l,element)){
            return ele_l;
        }
    }
    return NULL;
}

void *list_get_at_index(struct list *this, unsigned int i)
{
    if(!this || (i>=this->capacity)){
        return NULL;
    }

    return this->elements + (this->ele_size*i);
}

void list_remove_last_element(struct list *this)
{
    void *ele_l=NULL;
    unsigned int size_ele;
    unsigned int index_last;
    
    if(!this){
        return;
    }

    size_ele=this->ele_size;
    index_last=this->num_eles-1;
    ele_l = this->elements + (size_ele*index_last);
    if(this->free_element){
        this->free_element(ele_l);
    }
    memset(ele_l, 0x0, size_ele);
}
void list_clear_all(struct list *this){
    void *ele;
    unsigned int size_ele = this->ele_size;
    unsigned int i=0;

    if(!this){
        return;
    }

    if(this->free_element){
        for(i=0;i<this->num_eles;i++){
            ele=this->elements + (i*size_ele);
            this->free_element(ele);
        }
    }
    memset(this->elements, 0x0, size_ele*this->num_eles);
    this->num_eles=0;
}

void print_list(struct list *this)
{
    if(!this->print_ele){
        return;
    }
    void *ele;
    unsigned int size_ele = this->ele_size;
    unsigned int i=0;

    if(!this){
        return;
    }

    for(i=0;i<this->num_eles;i++){
        ele=this->elements + (i*size_ele);
        this->print_ele(ele);
    }   
}
