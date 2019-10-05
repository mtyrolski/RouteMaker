#include "map.h"

Map* newMap(void)
{
    Map* map = (Map*)malloc(sizeof(Map));
    if(map != NULL)
    {
        map->cities = NULL;
        map->roads = NULL;
        map->routes = NULL;
        map->ramp = treeCreate();
        map->citySize = 0;
    }
    return map;
}

void deleteMap(Map* map)
{
    if(map == NULL)
    {
        return;
    }

    deleteRoutes(map->routes);
    deleteCities(map->cities, map->citySize);
    deleteRoads(map->roads);
    deleteRamp(map->ramp);
    free(map->cities);
    free(map);
}

Road* adaptRoad(Map* map, unsigned length, int builtYear)
{
    Road* road = initRoad(length, builtYear);

    if(road == NULL)
    {
        return NULL;
    }

    LinkedRoad* updatedRoadList = malloc(sizeof(LinkedRoad));
    if(updatedRoadList == NULL)
    {
        return NULL;
    }

    updatedRoadList->road = road;
    updatedRoadList->next = map->roads;
    map->roads = updatedRoadList;
    return road;
}

void addCityToMap(Map* map, City* city)
{
    map->citySize++;
    map->cities = realloc(map->cities, map->citySize * sizeof(City*));
    map->cities[map->citySize - 1] = city;
}

bool tryInitCities(Map* map, const char* city1, const char* city2)
{
    if(city1 == NULL || city2 == NULL || strcmp(city1, city2) == 0 ||
       strcmp(city1, "") == 0 || strcmp(city2, "") == 0 ||
       !checkString(city1, 0, 31, ";", 1) || !checkString(city2, 0, 31, ";", 1))
    {
        return false;
    }

    City* first = getByKey(map->ramp, city1);
    City* second = getByKey(map->ramp, city2);

    if(first == NULL)
    {
        first = initCity(map->citySize, map->ramp, city1);
        if(first == NULL)
        {
            return false;
        }
        addCityToMap(map, first);
    }

    if(second == NULL)
    {
        second = initCity(map->citySize, map->ramp, city2);
        if(second == NULL)
        {
            return false;
        }
        addCityToMap(map, second);
    }

    return true;
}

bool updateRoads(Map* map, City* first, City* second, unsigned length,
                 int builtYear)
{
    first->roads =
        realloc(first->roads, first->numberOfNeighbours * sizeof(Road*));
    second->roads =
        realloc(second->roads, second->numberOfNeighbours * sizeof(Road*));

    if(first->roads == NULL || second->roads == NULL)
    {
        if(first->roads != NULL)
        {
            free(first->roads);
        }

        return false;
    }

    Road* road = adaptRoad(map, length, builtYear);
    first->roads[first->numberOfNeighbours - 1] = road;
    second->roads[second->numberOfNeighbours - 1] = road;

    return true;
}

bool updateInfrastructure(Map* map, const char* city1, const char* city2,
                          unsigned length, int builtYear)
{
    City* first = getByKey(map->ramp, city1);
    City* second = getByKey(map->ramp, city2);

    if(containsNeighbour(first, second))
    {
        return false;
    }

    addNeighbour(first, second);
    addNeighbour(second, first);

    return updateRoads(map, first, second, length, builtYear);
}

bool addRoad(Map* map, const char* city1, const char* city2, unsigned length,
             int builtYear)
{
    if(builtYear == 0 || length <= 0 || !tryInitCities(map, city1, city2))
    {
        return false;
    }

    return updateInfrastructure(map, city1, city2, length, builtYear);
}

bool updateRoad(Map* map, const char* city1, const char* city2, unsigned length,
                int builtYear)
{
    if(builtYear == 0 || length <= 0)
    {
        return false;
    }

    City* first = getByKey(map->ramp, city1);
    City* second = getByKey(map->ramp, city2);

    if(first == NULL || second == NULL ||
       getIdOfRoadBetween(first, second) == -1)
    {
        return addRoad(map, city1, city2, length, builtYear);
    }

    int idx = getIdOfRoadBetween(first, second);
    Road* considered = first->roads[idx];

    if(considered == NULL || considered->length != length ||
       considered->year > builtYear)
    {
        return false;
    }

    considered->year = builtYear;

    return true;
}

bool repairRoad(Map* map, const char* city1, const char* city2, int repairYear)
{
    return repairYear != 0 &&
           tryRepairRoad(getByKey(map->ramp, city1), getByKey(map->ramp, city2),
                         repairYear);
}

LinkedRoute* fillRoutesWithRoad(City* firstCity, City* secondCity,
                                LinkedRoute* routes, IntList** first)
{
    LinkedRoute* routesWithRoad = NULL;

    while(routes != NULL)
    {
        routes->route->cityLists->city == firstCity
            ? addIfTargetIsOnBegin(first, secondCity, &routesWithRoad, routes)
            : addTargetInGeneralCase(first, firstCity, secondCity,
                                     &routesWithRoad, routes);

        routes = routes->next;
    }

    return routesWithRoad;
}

bool fillPotentialReplacementForRoad(
    Map* map, LinkedRoute** routesWithRoad, IntList** first, City* firstCity,
    City* secondCity, Road* forbiddenRoad, CityLists** newPaths,
    LinkedRoute** beginsOfRoutes, IntList** earliestBegin, CityLists* newRoutes)
{
    while((*routesWithRoad) != NULL)
    {
        CityLists* tempList =
            (*first)->value == 1
                ? dijkstra(map->citySize, map->cities, firstCity, secondCity,
                           (*routesWithRoad)->route->cityLists, NULL,
                           forbiddenRoad)
                : dijkstra(map->citySize, map->cities, secondCity, firstCity,
                           (*routesWithRoad)->route->cityLists, NULL,
                           forbiddenRoad);

        *first = (*first)->next;
        LinkedCity* temp = findRoute(tempList);
        if(temp == NULL)
        {
            freeList(*earliestBegin);
            freeCityLists(*newPaths);
            freeRouteList(*beginsOfRoutes);
            return false;
        }
        pinToEnd(&newRoutes, temp, newPaths);
        (*routesWithRoad) = (*routesWithRoad)->next;
    }

    return true;
}

void hoopRoutes(Map* map, LinkedRoute** routesWithRoad, IntList** first,
                City* firstCity, City* secondCity, CityLists** newPaths,
                LinkedRoute** beginsOfRoutes, IntList** earliestBegin,
                CityLists** newRoutes)
{
    *newRoutes = *newPaths;
    *first = *earliestBegin;
    *routesWithRoad = *beginsOfRoutes;

    while(*routesWithRoad != NULL)
    {
        if((*earliestBegin)->value == 1)
        {
            emplacePath(*routesWithRoad, firstCity, *newRoutes);
        }
        else
        {
            emplacePath(*routesWithRoad, secondCity, *newRoutes);
        }

        {
            IntList* temp = (*earliestBegin)->next;
            free(*earliestBegin);
            (*earliestBegin) = temp;
        }

        {
            CityLists* temp = (*newRoutes)->next;
            free(*newRoutes);
            *newRoutes = temp;
        }

        LinkedRoute* routes_add = (*routesWithRoad)->next;
        free(*routesWithRoad);
        *routesWithRoad = routes_add;
    }
    eraseNeighbour(&map->roads, firstCity, secondCity);
}

bool removeRoad(Map* map, const char* city1, const char* city2)
{
    City* firstCity = getByKey(map->ramp, city1);
    City* secondCity = getByKey(map->ramp, city2);

    if(firstCity == NULL || secondCity == NULL || firstCity == secondCity)
    {
        return false;
    }

    int i = getIdOfRoadBetween(firstCity, secondCity);

    if(i == -1)
    {
        return false;
    }

    Road* forbiddenRoad = firstCity->roads[i];
    LinkedRoute* routes = map->routes;
    IntList* first = NULL;
    LinkedRoute* routesWithRoad =
        fillRoutesWithRoad(firstCity, secondCity, routes, &first);
    CityLists* newRoutes = NULL;
    CityLists* newPaths = NULL;
    LinkedRoute* beginsOfRoutes = routesWithRoad;
    IntList* earliestBegin = first;

    if(!fillPotentialReplacementForRoad(
           map, &routesWithRoad, &first, firstCity, secondCity, forbiddenRoad,
           &newPaths, &beginsOfRoutes, &earliestBegin, newRoutes))
    {
        return false;
    }

    hoopRoutes(map, &routesWithRoad, &first, firstCity, secondCity, &newPaths,
               &beginsOfRoutes, &earliestBegin, &newRoutes);

    return true;
}

bool addRouteToBegin(Map* map, Route** tempRoute, LinkedRoute** tempRouteList,
                     LinkedCity* route, unsigned routeId)
{
    (*tempRoute)->cityLists = route;
    (*tempRoute)->routeNumber = routeId;

    (*tempRouteList)->route = *tempRoute;
    (*tempRouteList)->next = map->routes;
    map->routes = (*tempRouteList);

    return true;
}

bool tryAddRoute(Map* map, City* first, City* second, unsigned routeId)
{
    CityLists* routes =
        dijkstra(map->citySize, map->cities, first, second, NULL, NULL, NULL);
    LinkedCity* route = findRoute(routes);

    if(route == NULL)
    {
        freeLinkedCity(route);
        return false;
    }

    Route* tempRoute = malloc(sizeof(Route));

    if(tempRoute == NULL)
    {
        return false;
    }

    LinkedRoute* tempRouteList = malloc(sizeof(LinkedRoute));
    if(tempRouteList == NULL)
    {
        return false;
    }

    return addRouteToBegin(map, &tempRoute, &tempRouteList, route, routeId);
}

bool newRoute(Map* map, unsigned routeId, const char* city1, const char* city2)
{
    if(!(routeId >= 1 && routeId <= 999))
    {
        return false;
    }

    City* first = getByKey(map->ramp, city1);
    City* second = getByKey(map->ramp, city2);

    if((first == NULL || second == NULL) || (first == second))
    {
        return false;
    }

    LinkedRoute* itr = map->routes;
    while(itr != NULL)
    {
        if(itr->route->routeNumber == routeId)
        {
            return false;
        }
        itr = itr->next;
    }

    return tryAddRoute(map, first, second, routeId);
}

bool getEndOfRoute(LinkedCity* cityList, LinkedCity** finish, City* town)
{
    LinkedCity* itr = cityList;

    while(itr != NULL)
    {
        if(itr->city == town)
        {
            return false;
        }

        itr = itr->next;

        if(itr != NULL)
        {
            *finish = itr;
        }
    }

    return true;
}

bool extendRoute(Map* map, unsigned routeId, const char* city)
{
    if(!(routeId >= 1 && routeId <= 999))
    {
        return false;
    }

    City* town = getByKey(map->ramp, city);

    if(town == NULL)
    {
        return false;
    }

    LinkedRoute* itr = map->routes;
    LinkedCity* cityList = NULL;
    Route* route = NULL;
    LinkedRoute* routeListElement = getRouteWithID(itr, routeId);

    if(routeListElement == NULL)
    {
        return false;
    }

    route = routeListElement->route;
    cityList = routeListElement->route->cityLists;

    if(route == NULL)
    {
        return false;
    }

    LinkedCity* finish = NULL;

    if(!getEndOfRoute(route->cityLists, &finish, town) || finish == NULL)
    {
        return false;
    }

    long beginDestination = 0;
    long endDestination = 0;

    CityLists* fromBeginToTown =
        dijkstra(map->citySize, map->cities, town, cityList->city, cityList,
                 &beginDestination, NULL);
    CityLists* fromEndToTown =
        dijkstra(map->citySize, map->cities, finish->city, town, cityList,
                 &endDestination, NULL);

    beginDestination = fromBeginToTown == NULL ? INT_MAX : beginDestination;
    endDestination = fromEndToTown == NULL ? INT_MAX : endDestination;

    return fromBeginToTown == NULL && fromEndToTown == NULL
               ? false
               : considerDestinations(beginDestination, endDestination,
                                      &fromBeginToTown, &fromEndToTown, &finish,
                                      &cityList, &route);
}

char const* getRouteDescription(Map* map, unsigned routeId)
{
    if(!(routeId >= 1 && routeId <= 999))
    {
        return (char const*)strdupl("");
    }

    LinkedRoute* consideredRoute = getRouteWithID(map->routes, routeId);

    if(consideredRoute == NULL)
    {
        return (char const*)strdupl("");
    }

    char* text = intToString(routeId, "%d");

    if(text == NULL)
    {
        return NULL;
    }

    text = mergeStrings(text, ";");

    LinkedCity* currentCityList = consideredRoute->route->cityLists;

    while(currentCityList->next != NULL)
    {
        char* cityName = currentCityList->city->name;
        text = mergeStrings(text, cityName);
        text = mergeStrings(text, ";");

        int pos = getIdOfRoadBetween(currentCityList->city,
                                     currentCityList->next->city);

        char* length =
            intToString(currentCityList->city->roads[pos]->length, "%u");
        text = mergeStrings(text, length);
        text = mergeStrings(text, ";");
        free(length);

        char* year = intToString(currentCityList->city->roads[pos]->year, "%d");
        text = mergeStrings(text, year);
        free(year);
        text = mergeStrings(text, ";");
        currentCityList = currentCityList->next;
    }

    char* city_name = currentCityList->city->name;
    text = mergeStrings(text, city_name);
    return (char const*)text;
}

bool pinRouteToRouteList(Map* map, LinkedCity* citiesOfRoute,
                         unsigned int number)
{
    LinkedRoute* tempRouteList = malloc(sizeof(LinkedRoute));
    if(tempRouteList == NULL)
    {
        return false;
    }

    Route* tempRoute = malloc(sizeof(Route));
    if(tempRoute == NULL)
    {
        return false;
    }

    return addRouteToBegin(map, &tempRoute, &tempRouteList, citiesOfRoute,
                           number);
}

bool checkCorectnessOfArgs(Map* map, char** cities, unsigned int* lengths,
                           int* years, int roadsSize, char* finishRoad)
{
    if(finishRoad != NULL || map == NULL)
    {
        return false;
    }

    for(int i = 0; i < roadsSize; i++)
    {
        if(years[i] == 0 || lengths[i] == 0)
        {
            return false;
        }

        char* city1 = cities[i];
        char* city2 = cities[i + 1];
        if(city1 == NULL || city2 == NULL || strcmp(city1, city2) == 0 ||
           strcmp(city1, "") == 0 || strcmp(city2, "") == 0 ||
           !checkString(city1, 0, 31, ";", 1) ||
           !checkString(city2, 0, 31, ";", 1))
        {
            return false;
        }

        City* first = getByKey(map->ramp, cities[i]);
        City* second = getByKey(map->ramp, cities[i + 1]);
        if(first == NULL || second == NULL)
        {
            continue;
        }

        int idx = getIdOfRoadBetween(first, second);
        if(idx == -1)
        {
            continue;
        }

        Road* road = first->roads[idx];

        if(road->length != lengths[i] || road->year > years[i])
        {
            return false;
        }
    }

    return true;
}

bool constructNewRoute(Map* map, char** cities, unsigned int* lengths,
                       int* years, int roadsSize, unsigned int routeNumber,
                       char* finishRoad)
{
    if(!(routeNumber >= 1 && routeNumber <= 999) || roadsSize == 0 ||
       !checkCorectnessOfArgs(map, cities, lengths, years, roadsSize,
                              finishRoad))
    {
        return false;
    }

    if(getRouteWithID(map->routes, routeNumber) != NULL || cities == NULL ||
       cities[0] == NULL)
    {
        return false;
    }

    LinkedCity* citiesOfRoute = (LinkedCity*)malloc(sizeof(LinkedCity));
    LinkedCity* itr = citiesOfRoute;
    if(citiesOfRoute == NULL)
    {
        return false;
    }

    City* first = getByKey(map->ramp, cities[0]);
    if(first == NULL)
    {
        citiesOfRoute->city = initCity(map->citySize, map->ramp, cities[0]);
        addCityToMap(map, citiesOfRoute->city);
    }
    else
    {
        citiesOfRoute->city = first;
    }
    citiesOfRoute->next = NULL;
    for(int i = 0; i < roadsSize; i++)
    {
        if(finishRoad != NULL && strcmp(finishRoad, cities[i]) == 0)
        {
            break;
        }

        if(!updateRoad(map, cities[i], cities[i + 1], lengths[i], years[i]))
        {
            freeLinkedCity(citiesOfRoute);
            return false;
        }
        itr->next = malloc(sizeof(LinkedCity));
        if(itr->next == NULL)
        {
            freeLinkedCity(citiesOfRoute);
            return false;
        }
        itr->next->city = getByKey(map->ramp, cities[i + 1]);
        itr->next->next = NULL;
        itr = itr->next;
    }

    pinRouteToRouteList(map, citiesOfRoute, routeNumber);
    return finishRoad == NULL;
}

bool removeRoute(Map* map, unsigned routeId)
{
    LinkedRoute* itr = map->routes;
    if(itr == false)
    {
        return false;
    }

    if(itr->route->routeNumber == routeId)
    {
        map->routes = itr->next;
        freeLinkedCity(itr->route->cityLists);
        free(itr->route);
        free(itr);

        return true;
    }

    LinkedRoute* prev = map->routes;
    itr = itr->next;

    while(itr != NULL && itr->route->routeNumber != routeId)
    {
        prev = itr;
        itr = itr->next;
    }

    if(itr == NULL)
    {
        return false;
    }
    prev->next = itr->next;

    freeLinkedCity(itr->route->cityLists);
    free(itr->route);
    free(itr);
    return true;
}
