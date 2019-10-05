#include "avl_tree.h"

int height(AVLNode* node)
{
    if(node == NULL)
    {
        return 0;
    }
    return node->height;
}

AVLNode* newNode(char* key)
{
    AVLNode* node = malloc(sizeof(AVLNode));

    if(node == NULL)
    {
        return NULL;
    }

    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* rightRotate(AVLNode* node)
{
    AVLNode* newNode = node->left;
    AVLNode* temp = newNode->right;
    newNode->right = node;
    node->left = temp;
    node->height = max(height(node->left), height(node->right))+1;
    newNode->height = max(height(newNode->left), height(newNode->right))+1;

    return newNode;
}

AVLNode* leftRotate(AVLNode* node)
{
    AVLNode* newNode = node->right;
    AVLNode* temp = newNode->left;
    newNode->left = node;
    node->right = temp;
    node->height = max(height(node->left), height(node->right))+1;
    newNode->height = max(height(newNode->left), height(newNode->right))+1;

    return newNode;
}

int getBalance(AVLNode* node)
{
    if (node == NULL)
    {
        return 0;
    }

    return height(node->left) - height(node->right);
}


AVLNode* insert(AVLNode* node, char* key, bool* repeat)
{
    if(node == NULL)
    {
        return(newNode(key));
    }

    if(strcmp(key, node->key) < 0)
    {
        node->left = insert(node->left, key, repeat);
    }
    else if(strcmp(key, node->key) > 0)
    {
        node->right = insert(node->right, key, repeat);
    }
    else
    {
        *repeat = true;
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if(balance > 1)
    {
        if (strcmp(key, node->left->key) < 0)
        {
            return rightRotate(node);
        }

        if (balance > 1 && strcmp(key, node->left->key) > 0)
        {
            node->left =  leftRotate(node->left);
            return rightRotate(node);
        }
    }else if(balance < -1)
    {
        if (strcmp(key, node->right->key) > 0)
        {
            return leftRotate(node);
        }

        if (strcmp(key, node->right->key) < 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }

    return node;
}

void freeNode(AVLNode* node)
{
    if(node == NULL)
    {
        return;
    }

    if(node->right != NULL)
    {
        freeNode(node->right);
    }

    if(node->left != NULL)
    {
        freeNode(node->left);
    }

    free(node);
}
