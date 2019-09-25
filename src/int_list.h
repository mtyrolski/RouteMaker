/** @file
 * Interface for IntList which represents linked list of ints
 *
 * @author Michał Tyrolski
 */


#ifndef __INT_LIST_H__
#define __INT_LIST_H__

#include "stdlib.h"

/**
 * Struct which represents linked list of ints
 */
typedef struct IntList
{
    int value;              ///< value of node
    struct IntList* next;   ///< pointer to next element
}IntList;

/**
 * @brief frees list of integer
 * @param[in, out] list - source to free
 */
void freeList(IntList* list);


#endif /* __INT_LIST_H__ */
