#include "string_vector.h"

StringVector* initVector(int size)
{
    StringVector* vector = malloc(sizeof(StringVector));
    if(vector == NULL)
    {
        return NULL;
    }
    vector->bufferSize = size;
    vector->size = 0;

    vector->content = malloc(size * sizeof(char*));
    if(vector->content == NULL)
    {
        free(vector);
        return NULL;
    }

    for(int i = 0; i < size; i++)
    {
        vector->content[i] = NULL;
    }
    return vector;
}

StringVector* doubleVector(StringVector** vector)
{
    if(*vector == NULL)
    {
        return NULL;
    }

    StringVector* newVector = initVector((*vector)->bufferSize * 2);

    if(newVector == NULL)
    {
        return NULL;
    }

    for(int i = 0; i < (*vector)->size; i++)
    {
        newVector->content[i] = (*vector)->content[i];
        newVector->size++;
    }

    free((*vector)->content);
    free(*vector);
    return newVector;
}

bool emplaceBack(StringVector* vector, char* element)
{
    vector->content[vector->size] = element;
    vector->size++;
    return true;
}

void resetVector(StringVector** vector, bool reinit)
{
    if(*vector == NULL)
    {
        return;
    }

    for(int i = 0; i < (*vector)->size; i++)
    {
        free((*vector)->content[i]);
        (*vector)->content[i] = NULL;
    }

    free((*vector)->content);
    free((*vector));
    if(reinit)
    {
        *vector = initVector(DEFAULT_CAPACITY);
    }
}
