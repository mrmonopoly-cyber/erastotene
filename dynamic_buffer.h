#pragma once

struct list;

struct list *new_list(unsigned int size, unsigned int size_element, unsigned int (*comparison_function) (void *,void *));
void list_add_element(struct list *this,void *element);
void *search_element(struct list *this,void *element);
