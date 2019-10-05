/** @file
 * Interface for City and other struct which are strongly connected with cities.
 *
 * @author Michał Tyrolski
 */

#ifndef __CITY_H__
#define __CITY_H__

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#include "int_list.h"
#include "road.h"
#include "utils.h"

/**
 * Struct which represents single city. Contains basic informations such name, roads, neighbours and id
 */
typedef struct City
{
    char* name;                 ///< name of city
    Road** roads;               ///< roads around the city
    struct City** neighbours;   ///< neighbours of city
    int numberOfNeighbours;     ///< number of neigbours
    int id;                     ///< unique id of city
}City;

/**
 * Struct which represents linked list of citiess
 */
typedef struct LinkedCity
{
    City* city;                 ///< pointer to current city
    struct LinkedCity* next;    ///< next cell of linked list
}LinkedCity;

/**
 * Struct which represents linked list of city linked list
 */
typedef struct CityLists
{
    LinkedCity* cityList;       ///< pointer to current city linked list
    struct CityLists* next;     ///<  next cell of linked list
}CityLists;

/**
 * Struct which represents tree representation of cities in graph. Used mainly for performance
 */
typedef struct Tree
{
    char* key;          ///<  key (there name) of node
    City* value;        ///<  value (there pointer to city) of node
    struct Tree* left;  ///<  left son
    struct Tree* right; ///<  right son
    bool isRoot;        ///<  flag for total root of tree
}Tree;

/**
 * @brief adds neghbour new for current city
 * @param[in, out] current - current considered
 * @param[in] new - new neighbour for current
 */
void addNeighbour(City* current, City* new);

/**
 * @brief checks if first contains second as neighbour
 * @param[in] first - considered city
 * @param[in] second - potential neighbour
 * @return true if contains, false otherwise
 */
bool containsNeighbour(City* first, City* second);

/**
 * @brief returns id of road between cities
 * @param[in] first - first city of road
 * @param[in] second - second city of road
 * @return id of found road: -1 if not found
 */
int getIdOfRoadBetween(City* first, City* second);

/**
 * @brief creates tree for storing cities
 * @return root of tree
 */
Tree* treeCreate();

/**
 * @brief returns pointer to city with given name
 * @param[in] tree - root of city tree
 * @param[in] key - name of city
 * @return pointer to city
 */
City* getByKey(Tree* tree, const char* key);

/**
 * @brief adds node to city tree
 * @param[in, out] tree - root of tree
 * @param[in] key - name of city
 * @param[in] value city
 * @return pointer to updated tree
 * @warning key is stored separately to faster access.
 */
Tree* addToTree(Tree* tree, const char *key, City *value);

/**
 * @brief initialize city with starting data
 * @param[in] currentMapSize - size of map
 * @param[in, out] root - root of city tree
 * @param[in] cityName - name of city
 * @return pointer to created city
 */
City* initCity(int currentMapSize, Tree* root, const char* cityName);

/**
 * @brief deallocate list of cities
 * @param[in, out] cityList - list of cities
 */
void freeLinkedCity(LinkedCity* cityList);

/**
 * @brief deallocate list of city lists
 * @param[in, out] cityLists - list of city lists
 */
void freeCityLists(CityLists* cityLists);

/**
 * @brief makes deep copy of linked list
 * @param[in] cityList - list to copy
 * @return pointer to deep copy of linked list
 */
LinkedCity* deepCopyLinkedList(LinkedCity* cityList);

/**
 * @brief finds route
 * @param[in] routes - structure which can contains files
 * @return pointer to found (or not, then NULL) route
 */
LinkedCity* findRoute(CityLists* routes);

/**
 * @brief deallocate all cities
 * @param[in, out] cities - container for cities
 * @param[in] size - length of container
 */
void deleteCities(City** cities, unsigned size);

/**
 * @brief deallocate tree - ramp for cities
 * @param[in, out] root - total root of tree
 */
void deleteRamp(Tree* root);

/**
 * @brief initialize linked list of city list
 * @param[in, out] routes - city lists to fill
 * @param[in, out] route - initial route to fill
 * @return pointer to cityLists
 */
CityLists* initLinkedCities(CityLists* routes, LinkedCity* route);

/**
 * @brief initialize linked list of city
 * @param[in] city - initial city to linked city
 * @param[in] next - next element
 * @return pointer to new linked city
 */
LinkedCity* initLinkedCity(City* city, LinkedCity* next);

/**
 * @brief tries to repair road
 * @param[in] first - first city of road
 * @param[in] second - second city of road
 * @param[in] repairYear - potential new year
 * @return true if road was repaired, false otherwise
 */
bool tryRepairRoad(City* first, City* second, int repairYear);

/**
 * @brief erases neighbour (second) of first
 * @param[in] roads - roads from the map
 * @param[in] first - first city erase
 * @param[in] second - second city and neighbour
 */
void eraseNeighbour(LinkedRoad** roads, City* first, City* second);

/**
 * @brief pins element to end of list
 * @param[in] end - end city to pin
 * @param[in] element - element to pin
 * @param[in] start - start city to pin
 */
void pinToEnd(CityLists** end, LinkedCity* element, CityLists** start);

#endif /*__CITY_H__*/