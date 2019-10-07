/** @file
 * Interface for road used in the map of national roads
 */

#ifndef __ROAD_H__
#define __ROAD_H__

#include <stdlib.h>

/**
 * Struct which represents single road and contains basic informations about
 * that
 */
typedef struct Road
{
    unsigned length;  ///<  length of road
    int year;         ///<  year of last renovation
} Road;

/**
 * Struct which represents linked list of roads
 */
typedef struct LinkedRoad
{
    Road* road;               ///<  pointer to current road
    struct LinkedRoad* next;  ///<  next cell in linked list
} LinkedRoad;

/**
 * @brief deallocate roads
 * @param[in, out] roads - container with elements to delete
 */
void deleteRoads(LinkedRoad* roads);

/**
 * @brief initialize road
 * @param[in] length - length of considered road
 * @param[in] builtYear - year of creation
 * @return pointer to created road
 */
Road* initRoad(unsigned length, int builtYear);

/**
 * @brief takes linked list & road to delete and erase it from list
 * @param[in, out] roadList - container of roads
 * @param[in] destinedToDelete - road to delete
 */
void deleteRoadFromLinkedRoad(LinkedRoad* roadList, Road* destinedToDelete);

#endif /* __ROAD_H__ */