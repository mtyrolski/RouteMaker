#include "road.h"

void deleteRoads(LinkedRoad* roads)
{
    while(roads != NULL)
    {
        LinkedRoad* temp = roads->next;
        free(roads->road);
        free(roads);
        roads = temp;
    }
}

void deleteRoadFromLinkedRoad(LinkedRoad* roadList, Road* destinedToDelete)
{
    while(roadList->next->road != destinedToDelete)
    {
        roadList = roadList->next;
    }
    LinkedRoad* temp = roadList->next;
    roadList->next = roadList->next->next;
    free(temp->road);
    free(temp);
}

Road* initRoad(unsigned length, int builtYear)
{
    Road* road = malloc(sizeof(Road));

    if(road == NULL)
    {
        return NULL;
    }

    road->length = length;
    road->year = builtYear;

    return road;
}