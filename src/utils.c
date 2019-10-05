#include "utils.h"

char* intToString(int x, char* format)
{
    char* buffer = malloc(sizeof(char) * sizeof(int) * 4 + 1);
    if(buffer)
    {
        sprintf(buffer, format, x);
    }
    return buffer;
}

char* mergeStrings(char* first, char* second)
{
    const size_t len1 = strlen(first);
    const size_t len2 = strlen(second);
    char* result = malloc(len1 + len2 + 1);
    memcpy(result, first, len1);
    memcpy(result + len1, second, len2 + 1);
    free(first);
    return result;
}

char* strdupl(const char* src)
{
    if(src == NULL)
    {
        return NULL;
    }

    size_t len = strlen(src) + 1;
    char* dst = malloc(len);
    if(dst == NULL)
    {
        return NULL;
    }

    for(unsigned long i = 0; i < strlen(src); i++)
    {
        dst[i] = src[i];
    }
    dst[strlen(src)] = '\0';
    return dst;
}

long str2int(char* str, int base, bool* flag, long min, long max)
{
    char* end = NULL;
    if(str[0] == '\0' || str[0] == '+' || isspace(str[0]) ||
       (str[0] == '0' && strlen(str) > 1) ||
       (strlen(str) > 2 && str[0] == '-' && str[1] == '0'))
    {
        *flag = false;
        return 0;
    }
    errno = 0;
    long l = strtol(str, &end, base);
    if((l > max || (errno == ERANGE && l == LONG_MAX)) ||
       (l < min || (errno == ERANGE && l == LONG_MIN)) || (*end != '\0'))
    {
        *flag = false;
        return 0;
    }
    *flag = true;
    return (int)l;
}

bool checkString(const char* str, int beginCode, int endCode,
                 char* forbiddenMarks, int forbiddenMarksSize)
{
    int itr = 0;
    while(str[itr] != '\0')
    {
        if(str[itr] >= beginCode && str[itr] <= endCode)
        {
            return false;
        }

        for(int i = 0; i < forbiddenMarksSize; i++)
        {
            if(str[itr] == forbiddenMarks[i])
            {
                return false;
            }
        }
        itr++;
    }
    return itr != 0;
}

int max(int x, int y) { return x < y ? y : x; }