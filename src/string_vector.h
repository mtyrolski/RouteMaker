/** @file
 * Interface of structure vector with string (char*) type
 *
 * @author Michał Tyrolski
 */

#ifndef __STRING_VECTOR_H__
#define __STRING_VECTOR_H__

#include "stdbool.h"
#include "stdlib.h"

/** Default capacity for vector */
#define DEFAULT_CAPACITY 64

/**
 * Struct which represents vector of string
 * @see std::vector from C++11 and newer
 */
typedef struct StringVector
{
    int bufferSize;  ///< size of buffer
    int size;        ///< size of vector
    char** content;  ///< content of vector
} StringVector;

/**
 * @brief allocate new vector
 * @return pointer to allocated vector
 */
StringVector* initVector(int size);

/**
 * @brief emplaces new element at end of the vector
 * @param[in, out] vector - to modify
 * @param[in] element - to emplace
 * @return true if element was added, false otherwise
 */
bool emplaceBack(StringVector* vector, char* element);

/**
 * @brief resets vector, free its content
 * @param[in, out] vector - to be cleared
 * @param[in] reinit - informs if vector should be recreated
 */
void resetVector(StringVector** vector, bool reinit);

/**
 * @brief double vector and move elements from old vector to new
 * @return pointer to new vector
 */
StringVector* doubleVector(StringVector** vector);

#endif /*__STRING_VECTOR_H__*/