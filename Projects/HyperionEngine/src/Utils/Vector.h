#pragma once

/*
 * Type-safe generic dynamic array adapted from sixthgear
 * https://gist.github.com/sixthgear/1507970
 */

#include <stdlib.h>

#define INITIAL_CAPACITY 16

typedef struct
{
    int size;
    int capacity;
    size_t dataSize;
    void *data;
} Vector;

/* vector type for use in type-unsafe functions */
/* macros will generate type-safe functions for each type called in */
/* VECT_GENERATE_TYPE */

Vector *Vector_Init(size_t dataSize, unsigned int capacity);

unsigned int Vector_At(Vector *v, unsigned int pos);

unsigned int Vector_Set(Vector *v, unsigned int pos);

unsigned int Vector_Push(Vector *v);

unsigned int Vector_Pop(Vector *v);

unsigned int Vector_Remove(Vector *v, unsigned int pos);

unsigned int Vector_Insert(Vector *v, unsigned int pos);

void *Vector_GetPtrAt(Vector *v, unsigned int pos);

void *Vector_GetPushPtr(Vector *v);

void *Vector_GetInsertPtr(Vector *v, unsigned int pos);

#define VECT_GENERATE_TYPE(TYPE) VECT_GENERATE_NAME(TYPE, TYPE)
#define VECT_GENERATE_NAME(TYPE, NAME)                                      \
    typedef struct                                                          \
    {                                                                       \
        int size;                                                           \
        int capacity;                                                       \
        size_t dataSize;                                                    \
        TYPE *data;                                                         \
    } NAME##Vector;                                                         \
    NAME##Vector *NAME##Vector_Init(unsigned int capacity)                  \
    {                                                                       \
        return (NAME##Vector *)Vector_Init(sizeof(TYPE), capacity);         \
    }                                                                       \
    TYPE *NAME##Vector_Pointer(NAME##Vector *v, unsigned int pos)           \
    {                                                                       \
        return v->data + Vector_At((Vector *)v, pos);                       \
    }                                                                       \
    TYPE NAME##Vector_At(NAME##Vector *v, unsigned int pos)                 \
    {                                                                       \
        return v->data[Vector_At((Vector *)v, pos)];                        \
    }                                                                       \
    void NAME##Vector_Push(NAME##Vector *v, TYPE value)                     \
    {                                                                       \
        v->data[Vector_Push((Vector *)v)] = value;                          \
    }                                                                       \
    TYPE *NAME##Vector_PushAndGetPointer(NAME##Vector *v)                   \
    {                                                                       \
        return &v->data[Vector_Push((Vector *)v)];                          \
    }                                                                       \
    void NAME##Vector_Set(NAME##Vector *v, unsigned int pos, TYPE value)    \
    {                                                                       \
        v->data[Vector_Set((Vector *)v, pos)] = value;                      \
    }                                                                       \
    TYPE NAME##Vector_Pop(NAME##Vector *v)                                  \
    {                                                                       \
        return v->data[Vector_Pop((Vector *)v)];                            \
    }                                                                       \
    void NAME##Vector_Remove(NAME##Vector *v, unsigned int pos)             \
    {                                                                       \
        Vector_Remove((Vector *)v, pos);                                    \
    }                                                                       \
    void NAME##Vector_Insert(NAME##Vector *v, unsigned int pos, TYPE value) \
    {                                                                       \
        v->data[Vector_Insert((Vector *)v, pos)] = value;                   \
    }
