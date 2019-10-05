#include "input_manager.h"
#include "map.h"

int main()
{
    Map* m = newMap();
    if(m == NULL)
    {
        return 0;
    }

    char* buffer;
    size_t bufferSize = 64;
    buffer = malloc(bufferSize * sizeof(char));

    if(buffer == NULL)
    {
        deleteMap(m);
        return 0;
    }

    InputManager* inputManager = initInputManager();

    bool edgeBeginFlag = false;
    while(readLine(inputManager, &buffer, &bufferSize, &edgeBeginFlag))
    {
        if(edgeBeginFlag || (strcmp(buffer, "") != 0 && buffer[0] != '#' && !takeAction(inputManager, m, buffer)))
        {
            fprintf(stderr, "ERROR %ld\n", inputManager->lineNr);
        }
        resetVector(&inputManager->vector, true);
        edgeBeginFlag = false;
    }

    if(strcmp(buffer, "") != 0)
    {
        fprintf(stderr, "ERROR %ld\n", inputManager->lineNr);
    }

    resetVector(&inputManager->vector, false);
    free(inputManager);
    free(buffer);
    deleteMap(m);
    return 0;
}