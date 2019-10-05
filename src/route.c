#include "route.h"

void freeRouteList(LinkedRoute* routeList)
{
    LinkedRoute* temp = NULL;
    while(routeList != NULL)
    {
        temp = routeList->next;
        free(routeList);
        routeList = temp;
    }
}

LinkedRoute* getRouteWithID(LinkedRoute* routeList, unsigned id)
{
    while(routeList != NULL && routeList->route->routeNumber != id)
    {
        routeList = routeList->next;
    }

    return routeList;
}

void deleteRoutes(LinkedRoute* routeList)
{
    while(routeList != NULL)
    {
        LinkedRoute* temp = routeList->next;
        freeLinkedCity(routeList->route->cityLists);
        free(routeList->route);
        free(routeList);
        routeList = temp;
    }
}

void generateAllPossiblePaths(LinkedCity** previous, City* first, City* second,
                              CityLists** routes, LinkedCity** route)
{
    if(first == second)
    {
        CityLists* temp = initLinkedCities(*routes, *route);
        (*routes) = temp;
        return;
    }

    LinkedCity* nextRoute = malloc(sizeof(LinkedCity));

    if(nextRoute == NULL)
    {
        return;
    }

    nextRoute->city = previous[second->id]->city;
    nextRoute->next = (*route);
    LinkedCity* prev = previous[second->id]->next;

    while(prev != NULL)
    {
        processCurrentPrev(&prev, previous, first, routes, route);
    }

    (*route) = nextRoute;
    generateAllPossiblePaths(previous, first, previous[second->id]->city,
                             routes, route);
}

void processCurrentPrev(LinkedCity** prev, LinkedCity** previous, City* city,
                        CityLists** routes, LinkedCity** route)
{
    LinkedCity* tempNewRoute = deepCopyLinkedList(*route);
    LinkedCity* temp = malloc(sizeof(LinkedCity));

    if(temp == NULL)
    {
        return;
    }

    temp->city = (*prev)->city;
    temp->next = tempNewRoute;
    tempNewRoute = temp;
    generateAllPossiblePaths(previous, city, (*prev)->city, routes,
                             &tempNewRoute);
    *prev = (*prev)->next;
}

CityLists* getRoutesWithTheSmallestLength(LinkedCity** previous, City* first,
                                          City* second)
{
    CityLists* routes = NULL;
    LinkedCity* route = malloc(sizeof(LinkedCity));
    route->city = second;
    route->next = NULL;
    generateAllPossiblePaths(previous, first, second, &routes, &route);
    return routes;
}

bool prepareVariablesForDijkstra(long** destination, bool** choosenTab,
                                 LinkedCity*** previous, Queue** queue,
                                 unsigned citySize)
{
    *destination = malloc(citySize * sizeof(long));
    if(*destination == NULL)
    {
        return false;
    }

    *choosenTab = malloc(citySize * sizeof(bool));
    if(*choosenTab == NULL)
    {
        free(*destination);
        return false;
    }

    *previous = malloc(citySize * sizeof(LinkedCity*));
    if(*previous == NULL)
    {
        free(*destination);
        free(*choosenTab);
        return false;
    }

    *queue = malloc(sizeof(Queue));
    if(*queue == NULL)
    {
        free(*destination);
        free(*choosenTab);
        free(*previous);
        return false;
    }
    (*queue)->size = citySize;
    (*queue)->invertedHeap = malloc((*queue)->size * sizeof(int));
    if((*queue)->invertedHeap == NULL)
    {
        free(*destination);
        free(*choosenTab);
        free(*previous);
        free(*queue);
        return false;
    }

    (*queue)->heap = malloc((*queue)->size * sizeof(int));
    if((*queue)->heap == NULL)
    {
        free(*destination);
        free(*choosenTab);
        free(*previous);
        free((*queue)->invertedHeap);
        free(*queue);
        return false;
    }

    return true;
}

void prepareValues(Queue** queue, long** destination, bool** choosenTab,
                   LinkedCity*** previous)
{
    for(unsigned int i = 0; i < (*queue)->size; i++)
    {
        (*queue)->invertedHeap[i] = i;
        (*queue)->heap[i] = i;
        (*destination)[i] = LONG_MAX;
        (*choosenTab)[i] = false;
        (*previous)[i] = NULL;
    }
}

void ignoreForbidden(bool** choosenTab, Queue** queue, City* first,
                     City* second, LinkedCity* route)
{
    LinkedCity* forbiddenInDijkstraIterator = route;

    while(forbiddenInDijkstraIterator != NULL)
    {
        if(forbiddenInDijkstraIterator->city != first &&
           forbiddenInDijkstraIterator->city != second)
        {
            (*choosenTab)[forbiddenInDijkstraIterator->city->id] = true;
            swapElements(*queue,
                         (*queue)->heap[forbiddenInDijkstraIterator->city->id],
                         (*queue)->size - 1);
            (*queue)->size--;
        }
        forbiddenInDijkstraIterator = forbiddenInDijkstraIterator->next;
    }
}

void updateRoutesWithRoad(int value, IntList** first,
                          LinkedRoute** routesWithRoad, LinkedRoute* routes)
{
    IntList* tempIntList = malloc(sizeof(IntList));
    tempIntList->value = value;
    tempIntList->next = (*first);
    (*first) = tempIntList;
    LinkedRoute* tempRouteList = malloc(sizeof(LinkedRoute));
    tempRouteList->route = routes->route;
    tempRouteList->next = (*routesWithRoad);
    (*routesWithRoad) = tempRouteList;
}

void addIfTargetIsOnBegin(IntList** first, City* townSecond,
                          LinkedRoute** routesWithRoad, LinkedRoute* routes)
{
    if(routes->route->cityLists->next->city != townSecond)
    {
        return;
    }

    updateRoutesWithRoad(1, first, routesWithRoad, routes);
}

void addTargetInGeneralCase(IntList** first, City* townFirst, City* townSecond,
                            LinkedRoute** routesWithRoad, LinkedRoute* routes)
{
    LinkedCity* cityList = routes->route->cityLists;

    while(cityList->next != NULL && cityList->next->city != townFirst)
    {
        cityList = cityList->next;
    }

    if(cityList->next == NULL || !(cityList->city == townSecond ||
                                   (cityList->next->next != NULL &&
                                    cityList->next->next->city == townSecond)))
    {
        return;
    }
    updateRoutesWithRoad(cityList->city == townSecond ? 2 : 1, first,
                         routesWithRoad, routes);
}

bool initDijkstra(long** destination, bool** choosenTab, LinkedCity*** previous,
                  Queue** queue, unsigned citySize)
{
    if(!prepareVariablesForDijkstra(destination, choosenTab, previous, queue,
                                    citySize))
    {
        return false;
    }
    prepareValues(queue, destination, choosenTab, previous);
    return true;
}

void finishDijkstra(unsigned citySize, long* destination, Queue* queue,
                    LinkedCity** previous, bool* choosenTab)
{
    free(destination);
    for(unsigned i = 0; i < citySize; i++)
    {
        freeLinkedCity(previous[i]);
    }
    free(previous);
    free(choosenTab);
    free(queue->invertedHeap);
    free(queue->heap);
    free(queue);
}

bool updateDestination(long* destination, LinkedCity** previous, Queue* queue,
                       City* currentCity, Road* roadToNeighbour,
                       City* neighbour, int idNeighbour)
{
    destination[neighbour->id] =
        destination[currentCity->id] + roadToNeighbour->length;
    balanceHeap(queue, destination, queue->heap[idNeighbour]);

    freeLinkedCity(previous[idNeighbour]);

    previous[idNeighbour] = malloc(sizeof(LinkedCity));

    if(previous[idNeighbour] == NULL)
    {
        return false;
    }

    previous[idNeighbour]->city = currentCity;
    previous[idNeighbour]->next = NULL;

    return true;
}

void considerNeighbour(long* destination, bool* choosenTab,
                       LinkedCity** previous, Queue* queue, City* currentCity,
                       Road* roadToNeighbour, City* neighbour,
                       Road* forbiddenRoad, unsigned citySize)
{
    int idNeighbour = neighbour->id;
    if(!choosenTab[idNeighbour] && (roadToNeighbour != forbiddenRoad))
    {
        if(destination[currentCity->id] + roadToNeighbour->length <
           destination[idNeighbour])
        {
            if(!updateDestination(destination, previous, queue, currentCity,
                                  roadToNeighbour, neighbour, idNeighbour))
            {
                finishDijkstra(citySize, destination, queue, previous,
                               choosenTab);
                return;
            }
        }
        else
        {
            if(destination[currentCity->id] + roadToNeighbour->length ==
               destination[idNeighbour])
            {
                previous[idNeighbour] =
                    initLinkedCity(currentCity, previous[idNeighbour]);
            }
        }
    }
}

bool dijkstraLoopProcess(long* destination, Queue* queue, bool* choosenTab,
                         LinkedCity** previous, unsigned citySize,
                         City** cities, Road* forbiddenRoad)
{
    if(destination[queue->invertedHeap[0]] == LONG_MAX)
    {
        finishDijkstra(citySize, destination, queue, previous, choosenTab);
        return false;
    }

    City* currentCity = cities[popMin(queue, destination)];
    for(int i = 0; i < currentCity->numberOfNeighbours; i++)
    {
        considerNeighbour(destination, choosenTab, previous, queue, currentCity,
                          currentCity->roads[i], currentCity->neighbours[i],
                          forbiddenRoad, citySize);
    }
    choosenTab[currentCity->id] = true;
    return true;
}

CityLists* dijkstra(unsigned citySize, City** cities, City* first, City* second,
                    LinkedCity* route, long* length, Road* forbiddenRoad)
{
    long* destination = NULL;
    bool* choosenTab = NULL;
    LinkedCity** previous = NULL;
    Queue* queue = NULL;

    if(!initDijkstra(&destination, &choosenTab, &previous, &queue, citySize))
    {
        return NULL;
    }
    ignoreForbidden(&choosenTab, &queue, first, second, route);

    destination[first->id] = 0;
    swapElements(queue, queue->heap[first->id], 0);

    while(queue->size != 0 && (cities[queue->invertedHeap[0]] != second ||
                               destination[queue->invertedHeap[0]] == LONG_MAX))
    {
        if(!dijkstraLoopProcess(destination, queue, choosenTab, previous,
                                citySize, cities, forbiddenRoad))
        {
            return NULL;
        }
    }

    if(length != NULL)
    {
        (*length) = destination[second->id];
    }

    CityLists* routes = getRoutesWithTheSmallestLength(previous, first, second);
    finishDijkstra(citySize, destination, queue, previous, choosenTab);
    return routes;
}

void emplacePath(LinkedRoute* routes, City* city, CityLists* paths)
{
    LinkedCity* itr = routes->route->cityLists;
    LinkedCity* saver = itr;
    while(itr->city != city)
    {
        itr = itr->next;
    }
    LinkedCity* cityElementPointer = itr->next;
    itr->next = paths->cityList->next;
    free(paths->cityList);
    while(itr->next != NULL)
    {
        itr = itr->next;
    }

    itr->next = cityElementPointer->next;
    free(cityElementPointer);
    routes->route->cityLists = saver;
}

bool finishConsideringWithoutAction(CityLists** fromBeginToTown,
                                    CityLists** fromEndToTown,
                                    LinkedCity** finish, LinkedCity** itr)
{
    *itr = findRoute(*fromEndToTown);

    if(*itr == NULL)
    {
        freeCityLists(*fromBeginToTown);
        return false;
    }

    (*finish)->next = (*itr)->next;
    free(*itr);
    freeCityLists(*fromBeginToTown);

    return true;
}

bool finishConsideringWithRepinInequality(CityLists** fromBeginToTown,
                                          CityLists** fromEndToTown,
                                          LinkedCity** itr,
                                          LinkedCity** cityList)
{
    *itr = findRoute(*fromBeginToTown);

    if(*itr == NULL)
    {
        freeCityLists(*fromEndToTown);
        return false;
    }
    LinkedCity* temp = *itr;
    while((*itr)->next->city != (*cityList)->city)
    {
        *itr = (*itr)->next;
    }
    free((*itr)->next);
    (*itr)->next = (*cityList);
    (*cityList) = temp;

    freeCityLists(*fromEndToTown);

    return true;
}

void repinPaths(CityLists** fromBeginToTown, CityLists** fromEndToTown,
                LinkedCity** itr)
{
    CityLists* routes = *fromBeginToTown;

    while((*fromBeginToTown)->next != NULL)
    {
        (*fromBeginToTown) = (*fromBeginToTown)->next;
    }

    (*fromBeginToTown)->next = *fromEndToTown;
    *itr = findRoute(routes);
}

void repinCityLists(LinkedCity** finish, LinkedCity** cityList,
                    LinkedCity** itr)
{
    if((*itr)->city == (*finish)->city)
    {
        (*finish)->next = (*itr)->next;
        free(*itr);
    }
    else
    {
        LinkedCity* temp = *itr;
        while((*itr)->next->city != (*cityList)->city)
        {
            *itr = (*itr)->next;
        }
        free((*itr)->next);
        (*itr)->next = *cityList;
        *cityList = temp;
    }
}

bool finishConsideringWithRepinEquality(CityLists** fromBeginToTown,
                                        CityLists** fromEndToTown,
                                        LinkedCity** finish,
                                        LinkedCity** cityList, LinkedCity** itr)
{
    repinPaths(fromBeginToTown, fromEndToTown, itr);
    if(*itr == NULL)
    {
        return false;
    }
    repinCityLists(finish, cityList, itr);
    return true;
}

bool considerDestinations(long beginDestination, long endDestination,
                          CityLists** fromBeginToTown,
                          CityLists** fromEndToTown, LinkedCity** finish,
                          LinkedCity** cityList, Route** route)
{
    LinkedCity* itr = NULL;

    if((beginDestination > endDestination &&
        !finishConsideringWithoutAction(fromBeginToTown, fromEndToTown, finish,
                                        &itr)) ||
       (beginDestination < endDestination &&
        !finishConsideringWithRepinInequality(fromBeginToTown, fromEndToTown,
                                              &itr, cityList)) ||
       (beginDestination == endDestination &&
        !finishConsideringWithRepinEquality(fromBeginToTown, fromEndToTown,
                                            finish, cityList, &itr)))
    {
        return false;
    }

    (*route)->cityLists = *cityList;
    return true;
}