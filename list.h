/******************************************************************************/
/*
* @file   list.h
* @author Aditya Harsh
* @brief  Type-agnostic linked list. Written in ANSI-C.
*/
/******************************************************************************/

/* HOW TO USE
- Place init_list(some type) on top your file
- Use the uniform functions to on the bottom of the file

    EXAMPLE:
        #include "list.h"

        init_list(int)

        int main(void)
        {
            list(int) my_list = create_list(int);

            push_back_list(int, my_list, 1);
            push_back_list(int, my_list, 2);
            push_front_list(int, my_list, 100);
            
            clear_list(int, my_list);

            return 0;
        }
 */

#pragma once

#include <stdlib.h> /* malloc, free, NULL */
#include <string.h> /* memset             */

/* macro helper functions */
#define PASTE(x, y) x ## _ ## y
#define EVALUATE(x, y) PASTE(x, y)

/* define list and node */
#define list(type) EVALUATE(list,type)
#define l_node(type) EVALUATE(l_node,type)

/* define booleans */
typedef enum {false = 0, true = 1} bool;

/* call to setup the type */
#define init_list(type)                                                                     \
                                                                                            \
typedef struct l_node(type) l_node(type);                                                   \
typedef void (*EVALUATE(call_back,type)) (type*);                                           \
typedef bool (*compare)(const type*, const type*);                                          \
                                                                                            \
struct l_node(type)                                                                         \
{                                                                                           \
    type data_;                                                                             \
    l_node(type) * next_;                                                                   \
};                                                                                          \
                                                                                            \
typedef struct                                                                              \
{                                                                                           \
    l_node(type) * head_;                                                                   \
    l_node(type) * tail_;                                                                   \
    unsigned size_;                                                                         \
} list(type);                                                                               \
                                                                                            \
list(type) EVALUATE(create,list(type)) (void)                                               \
{                                                                                           \
    list(type) list = {NULL, NULL, 0};                                                      \
    return list;                                                                            \
}                                                                                           \
                                                                                            \
void EVALUATE(clear,list(type)) (list(type) * list)                                         \
{                                                                                           \
    l_node(type) * temp;                                                                    \
                                                                                            \
    if (!list) return;                                                                      \
                                                                                            \
    temp = list->head_;                                                                     \
                                                                                            \
    while (list->head_)                                                                     \
    {                                                                                       \
        temp = temp->next_;                                                                 \
        free(list->head_);                                                                  \
        list->head_ = temp;                                                                 \
    }                                                                                       \
                                                                                            \
    list->tail_ = NULL;                                                                     \
    list->size_ = 0;                                                                        \
}                                                                                           \
                                                                                            \
void EVALUATE(push_back,list(type)) (list(type) * list, type value)                         \
{                                                                                           \
    if (!list) return;                                                                      \
                                                                                            \
    if (list->tail_)                                                                        \
    {                                                                                       \
        l_node(type) * l_node = malloc(sizeof(l_node(type)));                               \
        if (!l_node) return;                                                                \
        l_node->next_ = NULL;                                                               \
        l_node->data_ = value;                                                              \
        list->tail_->next_ = l_node;                                                        \
        list->tail_ = l_node;                                                               \
    }                                                                                       \
    else                                                                                    \
    {                                                                                       \
        list->head_ = malloc(sizeof(list(type)));                                           \
        if (!list->head_) return;                                                           \
        list->head_->next_ = NULL;                                                          \
        list->head_->data_ = value;                                                         \
        list->tail_ = list->head_;                                                          \
    }                                                                                       \
    ++list->size_;                                                                          \
}                                                                                           \
                                                                                            \
void EVALUATE(push_front,list(type)) (list(type) * list, type value)                        \
{                                                                                           \
    if (!list) return;                                                                      \
                                                                                            \
    if (list->head_)                                                                        \
    {                                                                                       \
        l_node(type) * l_node = malloc(sizeof(l_node(type)));                               \
        if (!l_node) return;                                                                \
        l_node->next_ = list->head_;                                                        \
        l_node->data_ = value;                                                              \
        list->head_ = l_node;                                                               \
    }                                                                                       \
    else                                                                                    \
    {                                                                                       \
        list->head_ = malloc(sizeof(list(type)));                                           \
        if (!list->head_) return;                                                           \
        list->head_->next_ = NULL;                                                          \
        list->head_->data_ = value;                                                         \
        list->tail_ = list->head_;                                                          \
    }                                                                                       \
    ++list->size_;                                                                          \
}                                                                                           \
                                                                                            \
type EVALUATE(get,list(type)) (const list(type) * list, unsigned index)                     \
{                                                                                           \
    type garbage;                                                                           \
    const l_node(type) * temp;                                                              \
    unsigned i;                                                                             \
                                                                                            \
    memset(&garbage, 0, sizeof(type));                                                      \
                                                                                            \
    if (!list) return garbage;                                                              \
                                                                                            \
    temp = list->head_;                                                                     \
                                                                                            \
    if (!list || index >= list->size_) return garbage;                                      \
                                                                                            \
    for (i = 0; i < index; ++i)                                                             \
        temp = temp->next_;                                                                 \
                                                                                            \
    return temp->data_;                                                                     \
}                                                                                           \
                                                                                            \
unsigned EVALUATE(size,list(type)) (const list(type) * list)                                \
{                                                                                           \
    if (!list) return 0;                                                                    \
                                                                                            \
    return list->size_;                                                                     \
}                                                                                           \
                                                                                            \
void EVALUATE(pop_back,list(type)) (list(type) * list)                                      \
{                                                                                           \
    unsigned i;                                                                             \
                                                                                            \
    if (!list) return;                                                                      \
                                                                                            \
    if (list->tail_)                                                                        \
    {                                                                                       \
        free(list->tail_);                                                                  \
                                                                                            \
        --list->size_;                                                                      \
                                                                                            \
        if (!list->size_)                                                                   \
        {                                                                                   \
            list->head_ = NULL;                                                             \
            list->tail_ = NULL;                                                             \
            return;                                                                         \
        }                                                                                   \
                                                                                            \
        list->tail_ = list->head_;                                                          \
                                                                                            \
        for (i = 0; i < (list->size_ - 1); ++i)                                             \
            list->tail_ = list->tail_->next_;                                               \
    }                                                                                       \
}                                                                                           \
                                                                                            \
void EVALUATE(pop_front,list(type)) (list(type) * list)                                     \
{                                                                                           \
    if (!list) return;                                                                      \
                                                                                            \
    if (list->head_)                                                                        \
    {                                                                                       \
        l_node(type) * temp = list->head_->next_;                                           \
                                                                                            \
        free(list->head_);                                                                  \
                                                                                            \
        --list->size_;                                                                      \
                                                                                            \
        if (!list->size_)                                                                   \
        {                                                                                   \
            list->head_ = NULL;                                                             \
            list->tail_ = NULL;                                                             \
            return;                                                                         \
        }                                                                                   \
                                                                                            \
        list->head_ = temp;                                                                 \
    }                                                                                       \
}                                                                                           \
                                                                                            \
type EVALUATE(front,list(type)) (const list(type) * list)                                   \
{                                                                                           \
    type garbage;                                                                           \
    memset(&garbage, 0, sizeof(type));                                                      \
    if (!list || !list->head_) return garbage;                                              \
                                                                                            \
    return list->head_->data_;                                                              \
}                                                                                           \
                                                                                            \
type EVALUATE(back,list(type)) (const list(type) * list)                                    \
{                                                                                           \
    type garbage;                                                                           \
    memset(&garbage, 0, sizeof(type));                                                      \
    if (!list || !list->tail_) return garbage;                                              \
                                                                                            \
    return list->tail_->data_;                                                              \
}                                                                                           \
                                                                                            \
void EVALUATE(copy,list(type)) (list(type) * dest, const list(type) * source)               \
{                                                                                           \
    unsigned i;                                                                             \
                                                                                            \
    if (!dest || !source) return;                                                           \
                                                                                            \
    if (dest->size_)                                                                        \
        EVALUATE(clear, list(type))(dest);                                                  \
                                                                                            \
    for (i = 0; i < source->size_; ++i)                                                     \
         EVALUATE(push_back, list(type))(dest,  EVALUATE(get, list(type))(source, i));      \
}                                                                                           \
                                                                                            \
void EVALUATE(foreach,list(type)) (list(type) * list, EVALUATE(call_back,type) cb)          \
{                                                                                           \
    l_node(type) * temp;                                                                    \
                                                                                            \
    if (!list) return;                                                                      \
                                                                                            \
    temp = list->head_;                                                                     \
                                                                                            \
    while (temp)                                                                            \
    {                                                                                       \
        cb(&temp->data_);                                                                   \
        temp = temp->next_;                                                                 \
    }                                                                                       \
}                                                                                           \
                                                                                            \
void EVALUATE(reverse,list(type)) (list(type) * list)                                       \
{                                                                                           \
    l_node(type) * prev = NULL;                                                             \
    l_node(type) * next = NULL;                                                             \
    l_node(type) * curr;                                                                    \
                                                                                            \
    if (!list || !list->head_) return;                                                      \
                                                                                            \
    curr = list->head_;                                                                     \
                                                                                            \
    while (curr)                                                                            \
    {                                                                                       \
        next = curr->next_;                                                                 \
        curr->next_ = prev;                                                                 \
        prev = curr;                                                                        \
        curr = next;                                                                        \
    }                                                                                       \
                                                                                            \
    list->head_ = prev;                                                                     \
}                                                                                           \
                                                                                            \
void EVALUATE(erase_element,list(type)) (list(type) * list, type value)                     \
{                                                                                           \
    l_node(type) * prev = NULL;                                                             \
    l_node(type) * next = NULL;                                                             \
    l_node(type) * curr;                                                                    \
                                                                                            \
    if (!list || !list->head_) return;                                                      \
                                                                                            \
    curr = list->head_;                                                                     \
                                                                                            \
    while (curr)                                                                            \
    {                                                                                       \
        next = curr->next_;                                                                 \
        if (curr->data_ == value)                                                           \
        {                                                                                   \
            if (curr == list->head_)                                                        \
                list->head_ = next;                                                         \
            else if (curr == list->tail_)                                                   \
                list->tail_ = NULL;                                                         \
            free(curr);                                                                     \
            if (prev)                                                                       \
                prev->next_ = next;                                                         \
            break;                                                                          \
        }                                                                                   \
        prev = curr;                                                                        \
        curr = next;                                                                        \
    }                                                                                       \
}                                                                                           \
                                                                                            \
void EVALUATE(erase_element_custom,list(type)) (list(type) * list, const type * value,      \
             compare comp)                                                                  \
{                                                                                           \
    l_node(type) * prev = NULL;                                                             \
    l_node(type) * next = NULL;                                                             \
    l_node(type) * curr;                                                                    \
                                                                                            \
    if (!list || !list->head_ || !value) return;                                            \
                                                                                            \
    curr = list->head_;                                                                     \
                                                                                            \
    while (curr)                                                                            \
    {                                                                                       \
        next = curr->next_;                                                                 \
        if (comp(&curr->data_, value))                                                      \
        {                                                                                   \
            if (curr == list->head_)                                                        \
                list->head_ = next;                                                         \
            else if (curr == list->tail_)                                                   \
                list->tail_ = NULL;                                                         \
            free(curr);                                                                     \
            if (prev)                                                                       \
                prev->next_ = next;                                                         \
            break;                                                                          \
        }                                                                                   \
        prev = curr;                                                                        \
        curr = next;                                                                        \
    }                                                                                       \
}                                                                                           \

/* Uniform function call syntax for all lists */
#define create_list(type) EVALUATE(create,list(type)) ()
/* clears the list */
#define clear_list(type, _list) EVALUATE(clear,list(type)) (&_list)
/* pushes to the back of the list */
#define push_back_list(type, _list, _value) EVALUATE(push_back,list(type)) (&_list, _value)
/* pushes to the front of the list */
#define push_front_list(type, _list, _value) EVALUATE(push_front,list(type)) (&_list, _value)
/* gets an element in an index */
#define get_element_list(type, _list, _index) EVALUATE(get,list(type)) (&_list, _index)
/* gets the size of the list (unsigned) */
#define size_list(type, _list) EVALUATE(size,list(type)) (&_list)
/* pops the back of the list */
#define pop_back_list(type, _list) EVALUATE(pop_back,list(type)) (&_list)
/* pops the first element in the list */
#define pop_front_list(type, _list) EVALUATE(pop_front,list(type)) (&_list)
/* gets the first element of a list */
#define front_list(type, _list) EVALUATE(front,list(type)) (&_list)
/* gets the last element of a list */
#define back_list(type, _list) EVALUATE(back,list(type)) (&_list)
/* copies lists */
#define copy_list(type, _destination, _source) EVALUATE(copy,list(type)) (&_destination, &_source)
/* runs a foreach on all elements within a list */
#define foreach_list(type, _list, _func) EVALUATE(foreach,list(type)) (&_list, _func)
/* reverses a list */
#define reverse_list(type, _list) EVALUATE(reverse,list(type)) (&_list)
/* searches for and removes an element in the list */
#define erase_element_list(type, _list, _value) EVALUATE(erase_element,list(type)) (&_list, _value)
/* searches for and removes an element in the list (custom type) */
#define erase_element_custom_list(type, _list, _value, _func) EVALUATE(erase_element_custom,list(type)) (&_list, &_value, _func)
