/* vector.h */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stddef.h>
#include <Arduino.h>

#include "wifi_data.h"

typedef struct {
  WifiCredentials* data;
  size_t capacity;
  size_t size;
} Vector;

Vector* vector_create();
bool vector_is_empty(Vector* vector);
void vector_push_back(Vector* vector, WifiCredentials value);
WifiCredentials vector_pop_front(Vector* vector);
void vector_clear(Vector* vector);


#endif // VECTOR_H