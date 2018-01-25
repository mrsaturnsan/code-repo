/******************************************************************************/
/*
* @file   vector.h
* @author Aditya Harsh
* @brief  Smart vector functionality. Written in ANSI C.
*/
/******************************************************************************/

#pragma once

/* define boolean values */
typedef enum {false = 0, true = 1} bool;
/* opaque struct pointer */
typedef struct vector vector;
/* typedef for printing function */
typedef void (*PRINTFUNC)(const vector*);

/* allocates a vector */
vector* alloc_vector(unsigned capacity, unsigned data_size, const char* data_type, PRINTFUNC func);
/* frees a vector */
void free_vector(vector** vec);
/* pushes back into a vector */
void push_back(vector** vec, const void* data);
/* pushes onto the front of a vector */
void push_front(vector** vec, const void* data);
/* pops the back of a vector */
void pop_back(vector* vec);
/* pops the front of a vector */
void pop_front(vector* vec);
/* deep copies a vector */
void copy_vector(vector** destination, const vector* source);
/* clears a vector */
void clear_vector(vector* vec);
/* returns whether or not the vector is empty */
bool empty(const vector* vec);
/* returns the size of the vector */
unsigned size(const vector* vec);
/* returns the capacity of the vector */
unsigned capacity(const vector* vec);
/* matches the capacity of a vector to its size */
void shrink_to_fit(vector** vec);
/* removes an element at an index */
void remove_element(vector* vec, unsigned index);
/* gets an element */
void* get(const vector* vec, unsigned index);
/* prints the data type of a vector */
void print_data_type(const vector* vec);
/* dumps the contents of a vector */
void dump_vector(const vector* vec);
/* checks the type of the vector */
bool check_type(const vector* vec, const char* type);
