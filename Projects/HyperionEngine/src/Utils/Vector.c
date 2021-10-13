#include "SatMalloc.h"
#include "Vector.h"

void exit(int);

#define Vector_Error(_) {};

Vector *Vector_Init(size_t dataSize, unsigned int capacity)
{
    Vector *v = malloc(sizeof(Vector));
    if (v == NULL)
        Vector_Error("Vector_Init: allocation of Vector failed.");

    if (capacity == 0)
        capacity = INITIAL_CAPACITY;

    v->data = malloc(dataSize * capacity);
    if (v->data == NULL)
        Vector_Error("Vector_Init: allocation of Vector data failed.");

    v->dataSize = dataSize;
    v->size = 0;
    v->capacity = capacity;
    return v;
}

void Vector_Free(void *v)
{
    free(((Vector *)v)->data);
    free(((Vector *)v));
}

void Vector_Resize(Vector *v)
{
    // int originalCapacity = v->capacity;
    v->capacity *= 2; /* double capacity */
    // Debug_PrintLine("Vector resized, %d to %d", originalCapacity, v->capacity);
    void *newAddress = realloc(v->data, v->capacity * v->dataSize);
    if (!newAddress)
        Vector_Error("Vector_Resize: resize failed.");

    v->data = newAddress;
}

unsigned int Vector_CheckBounds(Vector *v, unsigned int pos)
{
    return (pos < v->size);
}

unsigned int Vector_At(Vector *v, unsigned int pos)
{
    if (!Vector_CheckBounds((Vector *)v, pos))
        Vector_Error("Vector_At: out of bounds.");

    return pos;
}

void *Vector_GetPtrAt(Vector *v, unsigned int pos)
{
    if (!Vector_CheckBounds((Vector *)v, pos))
        Vector_Error("Vector_At: out of bounds.");

    return v->data + (pos * v->dataSize);
}

unsigned int Vector_Set(Vector *v, unsigned int pos)
{
    if (!Vector_CheckBounds((Vector *)v, pos))
        Vector_Error("Vector_Set: out of bounds.");

    return pos;
}

unsigned int Vector_Push(Vector *v)
{
    if (v->size == v->capacity)
        Vector_Resize(v);

    return v->size++;
}

void *Vector_GetPushPtr(Vector *v)
{
    return v->data + (Vector_Push(v) * v->dataSize);
}

unsigned int Vector_Pop(Vector *v)
{
    if (v->size == 0)
        Vector_Error("Vector_Pop: underflow.");

    return --v->size;
}

unsigned int Vector_Remove(Vector *v, unsigned int pos)
{
    if (!Vector_CheckBounds((Vector *)v, pos))
        Vector_Error("Vector_Remove: out of bounds.");

    char *dest = (char *)v->data + (v->dataSize * pos);
    char *src = dest + v->dataSize;
    size_t num_bytes = v->dataSize * (v->size - pos - 1);
    memmove(dest, src, num_bytes);
    v->size--;
    return pos;
}

unsigned int Vector_Insert(Vector *v, unsigned int pos)
{
    if (!Vector_CheckBounds((Vector *)v, pos))
        Vector_Error("Vector_Remove: out of bounds.");

    char *src = (char *)v->data + (v->dataSize * pos);
    char *dest = src + v->dataSize;
    size_t num_bytes = v->dataSize * (v->size - pos);
    memmove(dest, src, num_bytes);
    v->size++;
    return pos;
}

void *Vector_GetInsertPtr(Vector *v, unsigned int pos)
{
    return v->data + (Vector_Insert(v, pos) * v->dataSize);
}
