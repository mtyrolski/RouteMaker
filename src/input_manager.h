/** @file
 * Interface for managing standard input
 *
 * @author Michał Tyrolski
 */
#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "string_vector.h"
#include "string.h"
#include "stdbool.h"
#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "map.h"
#include "utils.h"

/**
 * Struct which represents input manager
 */
typedef struct InputManager
{
    unsigned long lineNr;   ///< number of current line
    StringVector* vector;   ///< vector of string
}InputManager;

/**
 * @brief allocate input manager and returns it
 * @return pointer to new input manager
 */
InputManager* initInputManager();

/**
 * @brief reads input to container
 * @param[in] inputManager - controls input
 * @param[in] buffer - container for input line
 * @param[in] bufferSize - current size of buffer
 * @param[in, out] edgeBeginCharacter - flag which informs if string is empty due to \0 on the beginning
 * @return false if buffer contains EOF, true otherwise
 */
bool readLine(InputManager* inputManager, char** buffer, size_t* bufferSize, bool* edgeBeginCharacter);

/**
 * @brief Takes manager, map and buffer and process program action
 * @param[in, out] inputManager - manager for std inp
 * @param[in, out] map - of routes, roads and cities
 * @param[in, out] buffer - for catch input
 * @return true if output of program is without problems, false otherwise
 */
bool takeAction(InputManager* inputManager, Map* map, char* buffer);



#endif /* __INPUT_MANAGER_H__ */