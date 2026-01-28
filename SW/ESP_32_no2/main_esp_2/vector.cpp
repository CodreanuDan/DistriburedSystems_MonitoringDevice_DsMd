/* vector.cpp */

#include "vector.h"

Vector* vector_create()
{
    /* Allocate memory */
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    /* Init vals */
    vector->data = NULL;
    vector->capacity = 0;
    vector->size = 0;
    /* Return new vector */
    return vector;
}

bool vector_is_empty(Vector* vector)
{
    return vector->size == 0;
}

void vector_push_back(Vector* vector, WifiCredentials value)
{
    /* Allocate memory if the vector data is NULL */
    if (vector->data == NULL)
    {
        vector->data = (WifiCredentials*)malloc(sizeof(WifiCredentials));
        vector->capacity = 1;
    }
    /* Realloc memory if the size is bigger than capacity*/
    else if (vector->size >= vector->capacity)
    {
        vector->capacity *= 2;
        vector->data = (WifiCredentials*)realloc(vector->data, sizeof(WifiCredentials) * vector->capacity);
    }

    /* Insert new element */
    vector->data[vector->size] = value;
    /* Increase capacity */
    vector->size++;
}

WifiCredentials vector_pop_front(Vector* vector)
{
    WifiCredentials empty = {0};

    if (vector == NULL || vector->size == 0)
    {
        return empty;
    }

    WifiCredentials value = vector->data[0];

    for (size_t i = 1; i < vector->size; i++)
    {
        vector->data[i-1] = vector->data[i];  
    }

    vector->size--;

    return value;
}

void vector_clear(Vector* vector)
{
    if (vector->data != NULL)
    {
        free(vector->data);
    }

    vector->data = NULL;
    vector->size = 0;
    vector->capacity=0;
}