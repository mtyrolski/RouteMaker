#include "int_list.h"

void freeList(IntList* list)
{
    IntList* temp = NULL;
    while(list != NULL)
    {
        temp = list->next;
        free(list);
        list = temp;
    }
}
