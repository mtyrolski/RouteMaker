/** @file
 * Interface for AVL Trees which improve performance of checking input
 *
 * @author Michał Tyrolski
 */
#ifndef __AVL_TREE_H__
#define __AVL_TREE_H__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/**
 * Struct which represents single node of tree
 */
typedef struct AVLNode
{
    char* key;              ///< key in node
    struct AVLNode* left;   ///< left son of current node
    struct AVLNode* right;  ///< right son of current node
    int height;             ///< height of node
} AVLNode;

/**
 * @brief goes deep into tree and accumulate its depth
 * @param[in] node - current considered tree
 * @return current height of AVL node
 */
int height(AVLNode* node);

/**
 * @brief creates new node
 * @param[in] key - for future node
 * @return pointer to created node with given key
 */
AVLNode* newNode(char* key);

/**
 * @brief rotates tree to the right
 * @param[in] node - current considered tree
 * @return pointer to rotated tree
 */
AVLNode* rightRotate(AVLNode* node);

/**
 * @brief rotates tree to the left
 * @param[in] node - current considered tree
 * @return pointer to rotated tree
 */
AVLNode* leftRotate(AVLNode* node);

/**
 * @brief calculates factor of balance
 * @param[in] node - current considered node
 * @return factor of balance
 */
int getBalance(AVLNode* node);

/**
 * @brief inserts new node with given key to given node
 * @param[in, out] node - current tree
 * @param[in] key - key for future node
 * @param[out] repeat - if key exists in tree
 * @return
 */
AVLNode* insert(AVLNode* node, char* key, bool* repeat);

/**
 * @brief deallocate node
 * @param[in, out] node
 */
void freeNode(AVLNode* node);

#endif /*__AVL_TREE_H__*/