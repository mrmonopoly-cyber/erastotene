#pragma once

struct list;

struct list *new_list(unsigned int size, unsigned int size_element, 
        unsigned int (*comparison_function) (void *,void *),
        void (*free_element) (void *element));
void list_add_element(struct list *this,void *element);
void *search_element(struct list *this,void *element);
void *list_get_at_index(struct list *this, unsigned int index);
void list_remove_last_element(struct list *this);
void list_clear_all(struct list *this);
void print_list(struct list *this);
