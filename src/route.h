/** @file
 * Interface for routes used in the map of national roads
 */

#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "city.h"
#include "queue.h"

/**
 * Struct which represents single route
 */
typedef struct Route
{
    unsigned int routeNumber;  ///< number of route
    LinkedCity* cityLists;     ///< list with cities in route
} Route;

/**
 *  Struct which represents linked list of routes
 */
typedef struct LinkedRoute
{
    Route* route;              ///< current route
    struct LinkedRoute* next;  ///< next route
} LinkedRoute;

/**
 * @brief frees route linked list
 * @param[in, out] routeList - list to be freed
 */
void freeRouteList(LinkedRoute* routeList);

/**
 * @brief Finds pointer to route with given id
 * @param[in] routeList - container of all routes
 * @param[in] id - of seeked route
 * @return pointer to route with given id
 */
LinkedRoute* getRouteWithID(LinkedRoute* routeList, unsigned id);

/**
 * @brief Deletes routes in given list
 * @param[in, out] routeList - destined to delete
 */
void deleteRoutes(LinkedRoute* routeList);

/**
 * @brief generates all possible paths from first to second
 * @param[in, out] previous - previous [cityx-> id] is a list of such cities
 * that the shortest way to cityx from a given initial city leads through any of
 * these cities
 * @param[in] first - first considered city
 * @param[in] second - second considered city
 * @param[in] routes - routes stored in cityLists
 * @param[in] route - route represented as linked list (without number of route)
 */
void generateAllPossiblePaths(LinkedCity** previous, City* first, City* second,
                              CityLists** routes, LinkedCity** route);

/**
 * @brief processes next prev list and call generateAllPossiblePaths
 * @param[in, out] prev - current considered linked list
 * @param[in, out] previous - previous list generated by dijkstra algorithm
 * @param[in] city - current city
 * @param[in] routes - routes stored in cityLists
 * @param[in] route - route represented as linked list (without number of route)
 */
void processCurrentPrev(LinkedCity** prev, LinkedCity** previous, City* city,
                        CityLists** routes, LinkedCity** route);

/**
 * @brief looking for routes with the smallest destination
 * @param[in, out] previous - previous list generated by dijkstra algorithm
 * @param[in] first - first considered city
 * @param[in] second - second considered city
 * @return returns one or more routes with the smallest destination
 */
CityLists* getRoutesWithTheSmallestLength(LinkedCity** previous, City* first,
                                          City* second);

/**
 * @brief dijkstra algorithm
 * @param[in] citySize - number of cities
 * @param[in] cities - cities tab from map
 * @param[in] first - first considered city
 * @param[in] second - second considered city
 * @param[in] route - route of cities represented as linked list (from without
 * number)
 * @param[in] length - potential destination for fill
 * @param[in] forbiddenRoad - potential road which can be excluded from seeking
 * path
 * @return paths represented as city lists
 */
CityLists* dijkstra(unsigned citySize, City** cities, City* first, City* second,
                    LinkedCity* route, long* length, Road* forbiddenRoad);

/**
 * @brief adds the route to routes with road when the first element of the rout
 * is town1
 * @param[in, out] first - linked list of ints (for dijkstra)
 * @param[in] townSecond - second considered city (town)
 * @param[in] routesWithRoad - linked list of routes with given road
 * @param[in] routes - list of routes represented as linked route (with the
 * whole data)
 */
void addIfTargetIsOnBegin(IntList** first, City* townSecond,
                          LinkedRoute** routesWithRoad, LinkedRoute* routes);

/**
 * @brief goes through the rout until it finds town1 and also adds it to
 * routeswithroad
 * @param[in, out] first - linked list of ints (for dijkstra)
 * @param[in] townFirst - first considered city (town)
 * @param[in] townSecond - second considered city (town)
 * @param[in] routesWithRoad - linked list of routes with given road
 * @param routes - list of routes represented as linked route (with the whole
 * data)
 */
void addTargetInGeneralCase(IntList** first, City* townFirst, City* townSecond,
                            LinkedRoute** routesWithRoad, LinkedRoute* routes);

/**
 * @brief add new path to route
 * @param[in, out] routes - current routes
 * @param[in] city - considered town
 * @param paths - path to add
 */
void emplacePath(LinkedRoute* routes, City* city, CityLists* paths);

/**
 * @brief consider destination from begin to town and from end to town
 * @param[in] beginDestination - destination from begin to town
 * @param[in] endDestination - end from begin to town
 * @param[in, out] fromBeginToTown - path from begin to town
 * @param[in, out] fromEndToTown - path from end to town
 * @param[in, out] finish - final linked city
 * @param[in, out] cityList - considered city list (acc)
 * @param[in] route - current route
 * @return true if considering succeeded, false otherwise
 */
bool considerDestinations(long beginDestination, long endDestination,
                          CityLists** fromBeginToTown,
                          CityLists** fromEndToTown, LinkedCity** finish,
                          LinkedCity** cityList, Route** route);
#endif /*__ROUTE_H__*/