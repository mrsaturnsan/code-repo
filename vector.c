/******************************************************************************/
/*
* @file   vector.h
* @author Aditya Harsh
* @brief  Smart vector functionality. Written in ANSI C.
*/
/******************************************************************************/

#include "vector.h"   /* vector interface */
#include <stdlib.h>   /* malloc, calloc, free */
#include <stdio.h>    /* printf */
#include <string.h>   /* memcpy, strcpy, strcmp */

/* default capacity is set to two */
#define DEFAULT_CAPACITY 2

/**
 * @brief Vector struct.
 * 
 */
struct vector
{
    /* the number of elements */
    unsigned size_;
    /* the max number of elements */
    unsigned capacity_;
    /* the size of the data */
    unsigned data_size_;

    /* holds the type of data */
    char* data_type_;
    PRINTFUNC pf_;

    /* the array of data stored inside the vector */
    void** data_;
};

/* forward declare */
static void copy_vector_shallow(vector** destination, const vector* source);

/**
 * @brief Allocates a vector of any type of elements.
 * 
 * @param capacity
 * @param data_size
 * @param data_type
 * @param func
 */
vector* alloc_vector(unsigned capacity, unsigned data_size, const char* data_type, PRINTFUNC func)
{
    /* allocate the data */
    vector* vec = malloc(sizeof(vector));

    /* check if allocation succeeded */
    if (vec)
    {
        /* allocate memory (0 = default capacity) */
        if (capacity)
        {
            vec->data_ = calloc(capacity, sizeof(void*));
            vec->capacity_ = capacity;
        }
        else
        {
            vec->data_ = calloc(DEFAULT_CAPACITY, sizeof(void*));
            vec->capacity_ = DEFAULT_CAPACITY;
        }

        /* check for successful allocation */
        if (!vec->data_)
        {
            free(vec);
            return NULL;
        }

        /* store the name of the type */
        vec->data_type_ = malloc(sizeof(char) * (strlen(data_type) + 1));

        /* free allocated memory */
        if (!vec->data_type_)
        {
            free(vec->data_);
            free(vec);
            return NULL;
        }

        /* set values */
        strcpy(vec->data_type_, data_type);
        vec->size_ = 0;
        vec->data_size_ = data_size;
        vec->pf_ = func;

        return vec;
    }

    /* failure */
    return NULL;
}

/**
 * @brief Frees allocated memory.
 * 
 * @param vec
 * @param preserve_data
 */
void free_vector(vector** vec)
{
    /* iterator */
    unsigned i;

    if (vec && *vec)
    {
        /* free memory */
        for (i = 0; i < (*vec)->capacity_; ++i)
            if ((*vec)->data_[i])
                free((*vec)->data_[i]);

        free((*vec)->data_);
        free((*vec)->data_type_);
        free(*vec);
        *vec = NULL;
    }
}

/**
 * @brief Frees a vector, but preserves data.
 * 
 * @param vec 
 */
static void free_vector_preserve(vector** vec)
{
    if (vec && *vec)
    {
        free((*vec)->data_);
        free((*vec)->data_type_);
        free(*vec);
        *vec = NULL;
    }
}

/**
 * @brief Pushes back.
 * 
 * @param vec 
 * @param data 
 */
void push_back(vector** vec, const void* data)
{
    /* used for allocating additional memory if necessary */
    vector* temp = NULL;
    void* dat = NULL;

    if (!vec || !(*vec) || !data) return;

    dat = malloc((*vec)->data_size_);
    if (!dat) return;
    memcpy(dat, data, (*vec)->data_size_); 
    
    if ((*vec)->size_ == (*vec)->capacity_)
    {
        temp = alloc_vector((*vec)->capacity_ * 2, (*vec)->data_size_, (*vec)->data_type_, (*vec)->pf_);

        if (temp)
        {
            /* copy values */
            copy_vector_shallow(&temp, *vec);

            /* set */
            temp->data_[(*vec)->size_] = dat;
            ++temp->size_;

            /* free the old vector */
            free_vector_preserve(vec);

            /* set new */
            *vec = temp;
        }
        else
        {
            /* free allocated memory */
            free(dat);
        }
    }
    else
    {
        /* add in data */
        (*vec)->data_[(*vec)->size_++] = dat;
    }
}

/**
 * @brief Pushes front.
 * 
 * @param vec 
 * @param data 
 */
void push_front(vector** vec, const void* data)
{
    /* iter */
    int i;
    /* used for allocating additional memory if necessary */
    vector* temp = NULL;
    void* dat = NULL;

    if (!vec || !(*vec) || !data) return;

    dat = malloc((*vec)->data_size_);
    if (!dat) return;
    memcpy(dat, data, (*vec)->data_size_); 

    if ((*vec)->size_ == (*vec)->capacity_)
    {
        temp = alloc_vector((*vec)->capacity_ * 2, (*vec)->data_size_, (*vec)->data_type_, (*vec)->pf_);

        if (temp)
        {
            /* copy values */
            copy_vector_shallow(&temp, *vec);

            for (i = (*vec)->size_; i >= 0; --i)
                temp->data_[i + 1] = temp->data_[i];          

            /* set */
            temp->data_[0] = dat;
            ++temp->size_;

            /* free the old vector */
            free_vector_preserve(vec);

            /* set new */
            *vec = temp;
        }
        else
        {
            /* free allocated memory */
            free(dat);
        }
    }
    else
    {
        for (i = (*vec)->size_; i >= 0; --i)
            (*vec)->data_[i + 1] = (*vec)->data_[i];
        (*vec)->data_[0] = dat;
        ++(*vec)->size_;
    }
}

/**
 * @brief "Removes" an element from the back.
 * 
 * @param vec 
 */
void pop_back(vector* vec)
{
    if (!vec) return;

    if (vec->size_)
       remove_element(vec, vec->size_ - 1);      
}

/**
 * @brief Removes the first element, and shifts the vector.
 * 
 * @param vec 
 */
void pop_front(vector* vec)
{
    if (!vec) return;

    /* remove an element and shift everything */
    remove_element(vec, 0);
}

/**
 * @brief Copies from one vector into another.
 * 
 * @param destination 
 * @param source 
 */
void copy_vector(vector** destination, const vector* source)
{
    /* loop iter */
    unsigned i;
    int j;

    /* safety check */
    if (!destination || !source) return;

    /* allocate additional memory if necessary */
    if (!(*destination) || (*destination)->capacity_ < source->size_)
    {
        free_vector(destination);
        *destination = alloc_vector(source->size_, source->data_size_, source->data_type_, source->pf_);
        if (!(*destination)) return;
    }

    /* copy data */
    for (i = 0; i < source->size_; ++i)
    {
        /* allocate memory */
        (*destination)->data_[i] = malloc(source->data_size_);

        if (!(*destination)->data_[i])
        {
            if (i)
            {
                j = i - 1;

                /* malloc failed, free all previously allocated memory, and return */
                for (; j >= 0; --j)
                    free((*destination)->data_[j]);
            }

            /* free the vector that was allocated (data should have been deallocated) */
            free_vector_preserve(destination);
            return;
        }
        else
        {
            memcpy((*destination)->data_[i], source->data_[i], source->data_size_); 
        }     
    }
    
    /* set the size */
    (*destination)->size_ = source->size_;
}

/**
 * @brief Copies from one vector into another.
 * 
 * @param destination 
 * @param source 
 */
static void copy_vector_shallow(vector** destination, const vector* source)
{
    /* loop iter */
    unsigned i;

    /* safety check */
    if (!destination || !source) return;

    /* allocate additional memory if necessary */
    if (!(*destination) || (*destination)->capacity_ < source->size_)
    {
        free_vector(destination);
        *destination = alloc_vector(source->size_, source->data_size_, source->data_type_, source->pf_);
        if (!(*destination)) return;
    }

    /* copy data */
    for (i = 0; i < source->size_; ++i)
        (*destination)->data_[i] = source->data_[i];
    
    /* set the size */
    (*destination)->size_ = source->size_;
}

/**
 * @brief "Clears" a vector.
 * 
 * @param vec 
 */
void clear_vector(vector* vec)
{
    /* iterator */
    unsigned i;

    if (!vec) return;

    for (i = 0; i < vec->size_; ++i)
    {
        if (vec->data_[i])
        {
            free(vec->data_[i]);
            vec->data_[i] = NULL;
        }
    }

    /* set the size to 0 */
    vec->size_ = 0;
}

/**
 * @brief Returns whether or not a vector is empty.
 * 
 * @param vec 
 * @return true 
 * @return false 
 */
bool empty(const vector* vec)
{
    if (!vec) return true;

    return vec->size_ ? false : true;
}

/**
 * @brief Returns the size of a vector.
 * 
 * @param vec 
 * @return unsigned 
 */
unsigned size(const vector* vec)
{
    if (!vec) return 0;

    return vec->size_;
}

/**
 * @brief Returns the capacity of a vector.
 * 
 * @param vec 
 * @return unsigned 
 */
unsigned capacity(const vector* vec)
{
    if (!vec) return 0;

    return vec->capacity_;
}

/**
 * @brief Resizes the vector to not waste memory.
 * 
 * @param vec 
 */
void shrink_to_fit(vector** vec)
{
    vector* temp = NULL;

    if (!vec || !(*vec)) return;

    if ((*vec)->capacity_ > (*vec)->size_)
    {
        temp = alloc_vector((*vec)->size_, (*vec)->data_size_, (*vec)->data_type_, (*vec)->pf_);
        if (temp)
        {
            copy_vector_shallow(&temp, *vec);
            free_vector_preserve(vec);
            *vec = temp;
        }
    }
}

/**
 * @brief Removes an element from the vector.
 * 
 * @param vec 
 */
void remove_element(vector* vec, unsigned index)
{
    /* iterator variable */
    unsigned i;

    /* safety checking */
    if (!vec || index >= vec->size_) return;

    /* free memory */
    free(vec->data_[index]);
    vec->data_[index] = NULL;

    /* shift elements */
    for (i = index; i < (vec->size_ - 1); ++i)
        vec->data_[i] = vec->data_[i + 1];

    --vec->size_;
}

/**
 * @brief Gets a requested value.
 * 
 * @param vec 
 * @param index 
 * @return void* 
 */
void* get(const vector* vec, unsigned index)
{
    if (!vec || index >= vec->size_) return NULL;

    return vec->data_[index];
}

/**
 * @brief Prints the type of data held within the vector.
 * 
 * @param vec 
 */
void print_data_type(const vector* vec)
{
    if (!vec) return;
    printf("%s\n", vec->data_type_);
}

/**
 * @brief Dumps the contents of a vector.
 * 
 * @param vec 
 */
void dump_vector(const vector* vec)
{
    if (!vec || !vec->pf_) return;

    /* execute print function */
    vec->pf_(vec);
}

/**
 * @brief Checks a type of the vector.
 * 
 * @param vec 
 * @param type 
 * @return true 
 * @return false 
 */
bool check_type(const vector* vec, const char* type)
{
    if (!vec || !type) return false;

    return (!strcmp(vec->data_type_, type));
}
