/** @file
 * Interface for queue prepared for dijkstra algorithm
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "stdbool.h"

/**
 * Struct which represents queue used for dijkstra algorithm
 */
typedef struct Queue
{
    unsigned int*
        heap;  ///< heap with indexes: heap[i] == x <=> invertedHeap[x] == i
    unsigned int* invertedHeap;  ///< shows us which city occupies which
                                 ///< position in the real mound
    unsigned int size;           ///< size of heap (queue)
} Queue;

/**
 * @brief Pop minimal element from queue
 * @param[in, out] queue - pointer to queue obj
 * @param[in, out] currentDistance - array of current distances
 * @return index of city removed from queue
 */
int popMin(Queue* queue, long* currentDistance);

/**
 * @brief balances given queue to heap condition
 * @param[in, out] queue - pointer to queue obj
 * @param[in, out] currentDistance - array of current distances
 * @param[in] id - id of city where currentDistance[id] was changed
 * so we have to balance queue to guarantee heap condition
 */
void balanceHeap(Queue* queue, long* currentDistance, unsigned id);

/**
 * @brief swaps elements in queue
 * @param[in, out] queue - pointer to queue obj
 * @param[in] i - first iterator
 * @param[in] j - second iterator
 */
void swapElements(Queue* queue, unsigned int i, unsigned int j);
#endif /*__QUEUE_H__*/