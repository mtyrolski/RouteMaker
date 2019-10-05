#include "city.h"

City* initCity(int currentMapSize, Tree* ramp, const char* cityName)
{
    City* city = malloc(sizeof(City));
    if(city == NULL)
    {
        return NULL;
    }

    char* newName = malloc((strlen(cityName) + 1) * sizeof(char));

    if(newName == NULL)
    {
        return NULL;
    }

    for(long long unsigned int i = 0; i < strlen(cityName) + 1; i++)
    {
        newName[i] = cityName[i];
    }
    city->name = newName;
    city->id = currentMapSize;
    city->numberOfNeighbours = 0;
    city->neighbours = NULL;
    city->roads = NULL;
    addToTree(ramp, cityName, city);
    return city;
}

void addNeighbour(City* current, City* new)
{
    current->numberOfNeighbours++;
    current->neighbours = realloc(current->neighbours, current->numberOfNeighbours*sizeof(City*));
    current->neighbours[current->numberOfNeighbours-1] = new;
}

bool containsNeighbour(City* first, City* second)
{
    for(int i = 0; i < first->numberOfNeighbours; i++)
    {
        if(first->neighbours[i] == second)
        {
            return true;
        }
    }
    return false;
}

Tree* treeCreate()
{
    Tree* root = malloc(sizeof(Tree));

    if(root == NULL)
    {
        return NULL;
    }

    root->key = "o"; //origin
    root->isRoot = true;
    root->left = NULL;
    root->right = NULL;
    return root;
}


Tree* addToTree(Tree* tree, const char *key, City *value)
{
    if(tree == NULL)
    {
        Tree* newNode = malloc(sizeof(Tree));
        if(newNode == NULL)
        {
            return NULL;
        }

        newNode->key = value->name;
        newNode->value = value;
        newNode->isRoot = false;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if(strcmp(tree->key, key) == 0)
    {
        if(tree->isRoot)
        {
            tree->left = addToTree(tree->left, key, value);
        }else
        {
            return NULL;
        }
    }


    if(strcmp(tree->key, key) < 0)
    {
        tree->left = addToTree(tree->left, key, value);
    }else
    {
        tree->right = addToTree(tree->right, key, value);
    }
    return tree;
}

void freeLinkedCity(LinkedCity* cityList)
{
    LinkedCity* temp = NULL;

    while(cityList != NULL)
    {
        temp = cityList->next;
        free(cityList);
        cityList = temp;
    }
}


City* getByKey(Tree* tree, const char* key)
{
    if(tree == NULL)
    {
        return NULL;
    }

    if(strcmp(tree->key, key) == 0 && !tree->isRoot)
    {
        return tree->value;
    }

    return strcmp(tree->key, key) >= 0 ?
        getByKey(tree->right, key) :
        getByKey(tree->left, key);
}

Tree* minValueNode(Tree* node)
{
    Tree* current = node;

    while(current->left != NULL)
        current = current->left;

    return current;
}

void freeCityLists(CityLists* cityLists)
{
    CityLists* temp = NULL;

    while(cityLists != NULL)
    {
        temp = cityLists->next;
        freeLinkedCity(cityLists->cityList);
        free(cityLists);
        cityLists = temp;
    }
}

Tree* deleteNode(Tree* root, const char* key)
{
    if(root == NULL)
    {
        return root;
    }

    if(strcmp(key, root->key) < 0)
    {
        root->left = deleteNode(root->left, key);
    }
    else if (strcmp(key, root->key) > 0)
    {
        root->right = deleteNode(root->right, key);
    }
    else
    {
        if (root->left == NULL)
        {
            Tree *temp = root->right;
            free(root);
            return temp;
        }
        else if(root->right == NULL)
        {
            Tree* temp = root->left;
            free(root);
            return temp;
        }

        Tree* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

int getIdOfRoadBetween(City* first, City* second)
{
    int itr = 0;
    while(itr < first->numberOfNeighbours && first->neighbours[itr] != second)
    {
        itr++;
    }
    return itr == first->numberOfNeighbours ? -1 : itr;
}

bool repinElements(LinkedCity** firstListIter, LinkedCity** temp, LinkedCity** output)
{
    LinkedCity* element = malloc(sizeof(LinkedCity));

    if(element == NULL)
    {
        return false;
    }

    element->city = (*firstListIter)->city;
    element->next = NULL;

    if(*temp == NULL)
    {
        *temp = element;
        *output = element;
    }else
    {
        (*temp)->next = element;
        (*temp) = (*temp)->next;
    }
    *firstListIter = (*firstListIter)->next;

    return true;
}

LinkedCity* deepCopyLinkedList(LinkedCity* cityList)
{
    LinkedCity* firstListIter = cityList;
    LinkedCity* temp = NULL;
    LinkedCity* output = NULL;
    while(firstListIter != NULL)
    {
        if(!repinElements(&firstListIter, &temp, &output))
        {
            return false;
        }
    }
    return output;
}
void updateMaxYear(int* maxYear, CityLists** itrFirst)
{
    LinkedCity* temp = (*itrFirst)->cityList;
    while(temp->next != NULL)
    {
        int index =0;
        while(temp->city->neighbours[index] != temp->next->city)
        {
            index++;
        }
        if(temp->city->roads[index]->year < *maxYear)
        {
            *maxYear = temp->city->roads[index]->year;
        }
        temp = temp->next;
    }
}

void updateListsInFindingRoute(int* maxYear, IntList** begin, IntList** max, CityLists** itrFirst)
{
    IntList* temp = malloc(sizeof(IntList));
    temp->value = *maxYear;
    temp->next = NULL;
    if(*max == NULL)
    {
        *max = temp;
        *begin = *max;
    }
    else
    {
        (*max)->next = temp;
        (*max) = (*max)->next;
    }
    *itrFirst = (*itrFirst)->next;
}

void findOldestRoad(int* maxYear, IntList** begin, IntList** max, CityLists** itrFirst)
{
    while(*itrFirst != NULL)
    {
        *maxYear = INT_MAX;
        updateMaxYear(maxYear, itrFirst);
        updateListsInFindingRoute(maxYear, begin, max, itrFirst);
    }
}

int howManySimilarValues(IntList** begin, int* index)
{
    int j = 0, equalities = 0, min = INT_MIN;

    IntList* temp = *begin;
    while(*begin != NULL)
    {
        if((*begin)->value > min)
        {
            min = (*begin)->value;
            equalities = 1;
            *index = j;
        }else if((*begin)->value == min)
        {
            equalities++;
        }
        temp = (*begin)->next;
        free(*begin);
        (*begin) = temp;
        j++;
    }

    return equalities;
}

LinkedCity* chooseRoute(CityLists* routes, int index)
{
    CityLists* itr = routes;
    CityLists* temp = NULL;
    for(int i = 0; i < index; i++)
    {
        freeLinkedCity(itr->cityList);
        temp = itr->next;
        free(itr);
        itr = temp;
    }
    LinkedCity* result = itr->cityList;
    freeCityLists(itr->next);
    free(itr);
    return result;
}

LinkedCity* findRoute(CityLists* routes)
{
    CityLists* itrFirst = routes;
    int maxYear;
    IntList* begin = NULL;
    IntList* max = NULL;

    findOldestRoad(&maxYear, &begin, &max, &itrFirst);
    int index = 0;
    if(howManySimilarValues(&begin, &index) == 1)
    {
        return chooseRoute(routes, index);
    }
    freeCityLists(routes);
    return NULL;
}



void deleteCities(City** cities, unsigned size)
{
    for(unsigned i = 0; i < size; i++)
    {
        free(cities[i]->neighbours);
        free(cities[i]->roads);
        free(cities[i]->name);
        free(cities[i]);
    }
}

void deleteRamp(Tree* root)
{
    if(root == NULL)
    {
        return;
    }

    if(root->right != NULL)
    {
        deleteRamp(root->right);
    }

    if(root->left != NULL)
    {
        deleteRamp(root->left);
    }

    free(root);
}

CityLists* initLinkedCities(CityLists* routes, LinkedCity* route)
{
    CityLists* temp = (CityLists*)malloc(sizeof(CityLists));

    if(temp == NULL)
    {
        return NULL;
    }

    temp->cityList = route;
    temp->next = routes;
    return temp;
}

bool tryRepairRoad(City* first, City* second, int repairYear)
{
    if(repairYear == 0)
    {
        return false;
    }

    if(first == NULL || second == NULL)
    {
        return false;
    }

    int indexOfRoad = getIdOfRoadBetween(first, second);

    if(indexOfRoad == -1 || first->roads[indexOfRoad]->year > repairYear)
    {
        return false;
    }

    first->roads[indexOfRoad]->year = repairYear;
    return true;
}

LinkedCity *initLinkedCity(City *city, LinkedCity *next)
{
    LinkedCity* temp = malloc(sizeof(LinkedCity));
    temp->city = city;
    temp->next = next;
    return temp;
}

void repinNeighboursWithout(City* city, int* itr)
{
    city->numberOfNeighbours--;
    while(*itr < city->numberOfNeighbours)
    {
        city->neighbours[*itr] = city->neighbours[*itr+1];
        city->roads[*itr] = city->roads[*itr+1];
        (*itr)++;
    }
}

void eraseNeighbour(LinkedRoad** roads, City* first, City* second)
{
    int i = getIdOfRoadBetween(first, second);
    int j = getIdOfRoadBetween(second, first);

    Road* destinedToDelete = first->roads[i];
    repinNeighboursWithout(first, &i);
    repinNeighboursWithout(second, &j);

    first->neighbours = realloc(first->neighbours, first->numberOfNeighbours * sizeof(City*));
    second->neighbours = realloc(second->neighbours, second->numberOfNeighbours * sizeof(City*));
    first->roads = realloc(first->roads, first->numberOfNeighbours * sizeof(Road*));
    second->roads = realloc(second->roads, second->numberOfNeighbours * sizeof(Road*));

    LinkedRoad* roadList = *roads;
    if(roadList->road == destinedToDelete)
    {
        *roads = (*roads)->next;
        free (roadList->road);
        free (roadList);
    }else
    {
        deleteRoadFromLinkedRoad(roadList, destinedToDelete);
    }
}

void pinToEnd(CityLists** end, LinkedCity* element, CityLists** start)
{
    CityLists* temp = malloc(sizeof(LinkedCity));
    temp->cityList = element;
    temp->next = NULL;

    if((*end) == NULL)
    {
        (*end) = temp;
        (*start) = (*end);
    }
    else
    {
        (*end)->next = temp;
        (*end) = (*end)->next;
    }
}
