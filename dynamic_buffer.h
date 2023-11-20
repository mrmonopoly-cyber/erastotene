#pragma once

#include <stdint.h>

struct list;

struct list *new_list(unsigned int size, unsigned int size_element, 
        unsigned int (*comparison_function) (void *,void *),
        void (*free_element) (void *),
        void (*print_element) (void *));
void *list_add_element(struct list *this,void *element);
void *list_search_element(struct list *this,void *key);
void *list_get_at_index(struct list *this, unsigned int index);
uint8_t list_get_index_element(struct list *this, void *key);
void list_remove_last_element(struct list *this);
void list_clear_all(struct list *this);
uint8_t list_clear_element(struct list *this, void *key);
void list_print(struct list *this);
unsigned int list_get_num_ele(struct list *this);
