#include "dynamic_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct list{
    void *element;
    unsigned int size_element;
    unsigned int size_list;
    struct list *next;
    unsigned int (*comparison_function) (void *,void *);
};

//private
void insert_element_full(struct list *this, unsigned int size_list, void *element, unsigned int (*comparison_function) (void *,void *)){
    if(!this){
        this=malloc(sizeof(*this));
        this->next=NULL;
        this->comparison_function=comparison_function;
    }
    if(!this->element){
        this->element=element;
        this->size_list=size_list;
        return;
    }
    
    if(!this->next){
        this->next=malloc(sizeof(*this));
        this->next->next=NULL;
        this->next->comparison_function=comparison_function;
    }
    
    return insert_element_full(this->next,size_list, element, comparison_function);
}

//public

struct list *new_list(unsigned int size, unsigned int size_element, unsigned int (*comparison_function) (void *,void *))
{
    struct list *new_list=malloc(sizeof(*new_list));
    struct list *next=new_list;
    new_list->element=NULL;
    new_list->size_element=size_element;
    new_list->comparison_function=comparison_function;
    while (size > 1){
        next->next=malloc(sizeof(*next->next));
        next->next->element=NULL;
        next->next->size_element=size_element;
        next->next->comparison_function=comparison_function;
        next->next->next=NULL;
        next=next->next;
        size--;
    }

    return new_list;
}
void list_add_element(struct list *this,void *element)
{
    void *copy_element = malloc(this->size_element);
    memcpy(copy_element, element, this->size_element);
    return insert_element_full(this,this->size_list+1, copy_element, this->comparison_function);
    
}
void *search_element(struct list *this,void *element)
{
    if(this){
        if(this->comparison_function(this->element,element)){
            return this->element;
        }
        return search_element(this->next, element);
    }
    return NULL;
}

void print_list(struct list *this)
{
    if(this){
        printf("%d\t",*(int *)this->element);
        print_list(this->next);
    }
}

//test
// void print_list(struct list *this)
// {
//     if(this){
//         printf("%d\t",*(int *)this->element);
//         return print_list(this->next);
//     }
//     printf("\n");
// }
//
// unsigned int comp_int(void *a,void *b){
//     int a_int = *(int *)a;
//     int b_int = *(int *)b;
//     return a<b;
// }
//
// int main()
// {
//     // struct list * this= new_list(5, sizeof(int),comp_int);
//     struct list *this=NULL;
//     for (int i=0; i<5; i++) {
//         list_add_element(this,&i);
//     }
//     print_list(this);
//     return 0;
// }
