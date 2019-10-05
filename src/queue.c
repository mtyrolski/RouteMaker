#include "queue.h"

void swapElements(Queue* queue, unsigned int i, unsigned int j)
{
    unsigned int helper = queue->invertedHeap[i];
    queue->invertedHeap[i] = queue->invertedHeap[j];
    queue->invertedHeap[j] = helper;
    queue->heap[queue->invertedHeap[i]] = i;
    queue->heap[queue->invertedHeap[j]] = j;
}

bool caterpillar(Queue* queue, unsigned int* begin, unsigned int* end, long* currentDistance)
{
    if(*end + 1 < queue->size && currentDistance[queue->invertedHeap[*end + 1]] < currentDistance[queue->invertedHeap[*end]])
    {
        *end = *end + 1;
    }

    if(currentDistance[queue->invertedHeap[*begin]] > currentDistance[queue->invertedHeap[*end]])
    {
        swapElements(queue, *begin, *end);
        *begin = *end;
        *end = 2*(*end) + 1;
    }
    else
    {
        return true;
    }

    return false;
}

int popMin(Queue* queue, long* currentDistance)
{
    queue->size--;
    swapElements(queue, queue->size, 0);
    unsigned int begin = 0;
    unsigned int end = 1;

    while(end < queue->size)
    {
        if(caterpillar(queue, &begin, &end, currentDistance))
        {
            return queue->invertedHeap[queue->size];
        }
    }
    return queue->invertedHeap[queue->size];
}

void balanceHeap(Queue* queue, long* currentDistance, unsigned int id)
{
    int i = id;
    int j = (unsigned int)((i - 1) / 2); //using ints because of looping value in case of i == 0

    while (j >= 0)
    {
        if (currentDistance[queue->invertedHeap[i]] >= currentDistance[queue->invertedHeap[j]])
        {
            return;
        }

        swapElements(queue, (unsigned int)i, (unsigned int)j);
        i = j;
        j = (i - 1) / 2;
    }
}