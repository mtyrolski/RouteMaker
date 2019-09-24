/** @file
 * File which contains utilities
 *
 * @author Michał Tyrolski
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

/**
 * @brief converts int to string
 * @warning result should be freed
 * @param[in] x - value to convert
 * @param[in] format - format to print string
 * @return char converted from int
 */
char* intToString(int x, char* format);

/**
 * @brief concatenate two strings
 * @warning result should be freed
 * @param[in] first string to merge
 * @param[in] second string to merge
 * @return merged strings
 */
char* mergeStrings(char* first, char* second);

/**
 * @brief deep copy of string
 * @param[in] src - source from copy should be done
 * @return pointer to copy of source
 */
char* strdupl(const char *src);

/**
 * @brief converts string to integer in use of strol function. Treats leading zeros as errors.
 * @param[in] str - number hidden in string
 * @param[in] base - target base
 * @param[out] flag - boolean to catch errors
 * @param[in] min - min value of string
 * @param[in] max - max value of string
 * @return value of string
 */
long str2int(char* str, int base, bool* flag, long min, long max);

/**
 * @brief checks if in string are marks from [beginCode;endCode] or contains forbidden marks
 * @param[in] str - string to check
 * @param[in] beginCode - begin of interval of forbidden marks
 * @param[in] endCode - end of interval of forbidden makrs
 * @param[in] forbiddenMarks - additional forbidden makrs
 * @param[in] forbiddenMarksSize - size of forbidden marks
 * @return true if string is correct, false otherwise
 */
bool checkString(const char* str, int beginCode, int endCode, char* forbiddenMarks, int forbiddenMarksSize);

/**
 * @brief returns max from x,y
 * @param[in] x - first arg
 * @param[in] y - second arg
 * @return max(x,y)
 */
int max(int x, int y);


#endif /* __UTILS_H__ */