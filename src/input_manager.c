#include "input_manager.h"

bool readLine(InputManager* inputManager, char** buffer, size_t* bufferSize,
              bool* edgeBeginCharacter)
{
    *edgeBeginCharacter = false;
    int current;
    unsigned int position = 0;
    while((current = fgetc(stdin)) != '\n' && current != EOF)
    {
        if(current == 0 && position == 0)
        {
            *edgeBeginCharacter = true;
        }
        if(position + 1 >= *bufferSize)
        {
            *bufferSize *= 2;
            *buffer = realloc(*buffer, *bufferSize);
            if(*buffer == NULL)
            {
                return false;
            }
        }
        (*buffer)[position] = (char)current;
        position++;
    }
    (*buffer)[position] = '\0';
    inputManager->lineNr++;

    return current != EOF;
}

InputManager* initInputManager()
{
    InputManager* inputManager = malloc(sizeof(InputManager));
    inputManager->vector = initVector(DEFAULT_CAPACITY);
    inputManager->lineNr = 0;
    return inputManager;
}

bool validateAddRoad(InputManager* inputManager, Map* map)
{
    if(inputManager->vector->size != 4)
    {
        return false;
    }
    char* city1 = inputManager->vector->content[0];
    char* city2 = inputManager->vector->content[1];
    char* length = inputManager->vector->content[2];
    char* builtYear = inputManager->vector->content[3];
    bool flagLength;
    bool flagBuiltYear;

    if(city1 == NULL || city2 == NULL || length == NULL || builtYear == NULL ||
       inputManager->vector->size > 4 || strcmp(city1, "") == 0 ||
       strcmp(city1, "") == 0)
    {
        return false;
    }

    unsigned int lengthVal =
        (unsigned)str2int(length, 10, &flagLength, 1, UINT_MAX);
    int builtYearVal =
        (int)str2int(builtYear, 10, &flagBuiltYear, INT_MIN, INT_MAX);

    return !flagLength || !flagBuiltYear
               ? false
               : addRoad(map, city1, city2, lengthVal, builtYearVal);
}

bool validateRepairRoad(InputManager* inputManager, Map* map)
{
    if(inputManager->vector->size != 3)
    {
        return false;
    }

    char* city1 = inputManager->vector->content[0];
    char* city2 = inputManager->vector->content[1];
    char* builtYear = inputManager->vector->content[2];

    if(city1 == NULL || city2 == NULL || builtYear == NULL ||
       inputManager->vector->size > 3 || strcmp(city1, "") == 0 ||
       strcmp(city1, "") == 0)
    {
        return false;
    }

    bool flagBuiltYear;
    int builtYearVal =
        (int)str2int(builtYear, 10, &flagBuiltYear, INT_MIN, INT_MAX);

    return !flagBuiltYear ? false : repairRoad(map, city1, city2, builtYearVal);
}

bool validateGetRouteDescription(InputManager* inputManager, Map* map)
{
    if(inputManager->vector->size != 1)
    {
        return false;
    }

    char* id = inputManager->vector->content[0];

    if(id == NULL || strcmp(id, "") == 0)
    {
        return false;
    }

    bool flagId;
    unsigned int idVal = (unsigned)str2int(id, 10, &flagId, 0, UINT_MAX);

    if(!flagId)
    {
        return false;
    }

    const char* result = getRouteDescription(map, idVal);

    if(result == NULL)
    {
        return false;
    }

    printf("%s\n", result);
    free((void*)result);
    return true;
}

bool divideNewRouteData(char*** cities, unsigned int** lengths, int** years,
                        int* roadsSize, InputManager* inputManager,
                        AVLNode** root, char** finishCity)
{
    bool flagLength;
    bool flagYears;
    bool repeatFlag = false;

    (*cities)[0] = inputManager->vector->content[0];
    *root = insert(*root, (*cities)[0], &repeatFlag);
    for(int i = 1; i <= *roadsSize; i++)
    {
        flagLength = true;
        flagYears = true;
        repeatFlag = false;
        (*cities)[i] = inputManager->vector->content[3 * i];

        *root = insert(*root, (*cities)[i], &repeatFlag);
        if(!repeatFlag)
        {
            (*lengths)[i - 1] = (unsigned)str2int(
                inputManager->vector->content[3 * (i - 1) + 1], 10, &flagLength,
                0, UINT_MAX);
            (*years)[i - 1] =
                (int)str2int(inputManager->vector->content[3 * (i - 1) + 2], 10,
                             &flagYears, INT_MIN, INT_MAX);
        }
        else
        {
            *roadsSize = i;
            if(i > 0)
            {
                (*finishCity) = (*cities)[i - 1];
                free(*cities);
                free(*lengths);
                free(*years);
                freeNode(*root);
                return false;
            }
        }
        if(!flagLength || !flagYears)
        {
            free(*cities);
            free(*lengths);
            free(*years);
            freeNode(*root);
            return false;
        }
    }
    return true;
}

bool validateConstructNewRoute(char* routeNumberString,
                               InputManager* inputManager, Map* map)
{
    char** cities = NULL;
    unsigned int* lengths = NULL;
    int* years = NULL;
    AVLNode* root = NULL;
    char* finishCity = NULL;

    if(inputManager->vector->size % 3 != 1)
    {
        return false;
    }

    int roadsSize = inputManager->vector->size / 3;
    bool flagRouteNumber;
    int numberOfRoute =
        (int)str2int(routeNumberString, 10, &flagRouteNumber, INT_MIN, INT_MAX);

    if(!flagRouteNumber)
    {
        return false;
    }

    cities = malloc((roadsSize + 1) * sizeof(char*));
    if(cities == NULL)
    {
        return false;
    }
    lengths = malloc((roadsSize) * sizeof(int));
    if(lengths == NULL)
    {
        free(cities);
        return false;
    }
    years = malloc((roadsSize) * sizeof(int));
    if(years == NULL)
    {
        free(cities);
        free(lengths);
        return false;
    }

    if(!divideNewRouteData(&cities, &lengths, &years, &roadsSize, inputManager,
                           &root, &finishCity))
    {
        return false;
    }

    cities[roadsSize] =
        inputManager->vector->content[inputManager->vector->size - 1];

    bool result = constructNewRoute(map, cities, lengths, years, roadsSize,
                                    (unsigned)numberOfRoute, finishCity);

    free(cities);
    free(lengths);
    free(years);
    freeNode(root);

    return result;
}

bool validateNewRoute(InputManager* inputManager, Map* map)
{
    if(inputManager->vector->size != 3)
    {
        return false;
    }

    char* city1 = inputManager->vector->content[1];
    char* city2 = inputManager->vector->content[2];
    char* routeId = inputManager->vector->content[0];

    if(city1 == NULL || city2 == NULL || routeId == NULL ||
       strcmp(city1, "") == 0 || strcmp(city1, "") == 0)
    {
        return false;
    }

    bool flagRouteId;

    unsigned routeIdVal = (unsigned)str2int(routeId, 10, &flagRouteId, MIN_ROUTE_ID, MAX_ROUTE_ID);

    return !routeIdVal ? false : newRoute(map, routeIdVal, city1, city2);
}

bool validateExtendRoute(InputManager* inputManager, Map* map)
{
    if(inputManager->vector->size != 2)
    {
        return false;
    }

    char* city = inputManager->vector->content[1];
    char* routeId = inputManager->vector->content[0];

    if(city == NULL || routeId == NULL || strcmp(city, "") == 0)
    {
        return false;
    }

    bool flagRouteId;
    unsigned routeIdVal = (unsigned)str2int(routeId, 10, &flagRouteId, MIN_ROUTE_ID, MAX_ROUTE_ID);

    return !routeIdVal ? false : extendRoute(map, routeIdVal, city);
}

bool validateRemoveRoad(InputManager* inputManager, Map* map)
{
    if(inputManager->vector->size != 2)
    {
        return false;
    }

    char* city1 = inputManager->vector->content[0];
    char* city2 = inputManager->vector->content[1];

    if(city1 == NULL || city2 == NULL || strcmp(city1, "") == 0 ||
       strcmp(city1, "") == 0)
    {
        return false;
    }

    return removeRoad(map, city1, city2);
}

bool validateRemoveRoute(InputManager* inputManager, Map* map)
{
    if(inputManager->vector->size != 1)
    {
        return false;
    }

    char* id = inputManager->vector->content[0];

    bool flagId;
    unsigned int idVal = (unsigned)str2int(id, 10, &flagId, 0, UINT_MAX);

    return !flagId ? false : removeRoute(map, idVal);
}

bool takeAction(InputManager* inputManager, Map* map, char* buffer)
{
    int semicolonCounter = 0;
    int itr = 0;
    if(buffer == NULL || buffer[0] == ';')
    {
        return false;
    }

    while(buffer[itr] != '\0')
    {
        if(itr > 0 && buffer[itr] == ';' && buffer[itr - 1] == ';')
        {
            return false;
        }
        if(buffer[itr] == ';')
        {
            semicolonCounter++;
        }
        itr++;
    }

    char* command = strdupl(strtok(buffer, ";\n"));

    if(command == NULL)
    {
        return false;
    }

    char* arg = strtok(NULL, ";\n");
    while(arg != NULL)
    {
        char* dupArg = strdupl(arg);
        if(dupArg == NULL)
        {
            free(command);
            return false;
        }
        if(inputManager->vector->size == inputManager->vector->bufferSize)
        {
            inputManager->vector = doubleVector(&inputManager->vector);
        }
        emplaceBack(inputManager->vector, dupArg);
        arg = strtok(NULL, ";\n");
    }
    if(strcmp(command, "addRoad") == 0 && semicolonCounter == ADD_ROAD_SEMICOLONS)
    {
        free(command);
        return validateAddRoad(inputManager, map);
    }
    else if(strcmp(command, "repairRoad") == 0 && semicolonCounter == REPAIR_ROAD_SEMICOLONS)
    {
        free(command);
        return validateRepairRoad(inputManager, map);
    }
    else if(strcmp(command, "getRouteDescription") == 0 &&
            semicolonCounter == GET_DESCR_SEMICOLONS)
    {
        free(command);
        return validateGetRouteDescription(inputManager, map);
    }
    else if(strcmp(command, "newRoute") == 0 && semicolonCounter == NEW_ROUTE_SEMICOLONS)
    {
        free(command);
        return validateNewRoute(inputManager, map);
    }
    else if(strcmp(command, "extendRoute") == 0 && semicolonCounter == EXTEND_ROUTE_SEMICOLONS)
    {
        free(command);
        return validateExtendRoute(inputManager, map);
    }
    else if(strcmp(command, "removeRoad") == 0 && semicolonCounter == REMOVE_ROAD_SEMICOLONS)
    {
        free(command);
        return validateRemoveRoad(inputManager, map);
    }
    else if(strcmp(command, "removeRoute") == 0 && semicolonCounter == REMOVE_ROUTE_SEMICOLONS)
    {
        free(command);
        return validateRemoveRoute(inputManager, map);
    }
    bool result = semicolonCounter % 3 == 1 && semicolonCounter > 1 &&
                  validateConstructNewRoute(command, inputManager, map);
    free(command);
    return result;
}
