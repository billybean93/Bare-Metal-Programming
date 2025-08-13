// -----------------------------------utils.h -------------------------------------
#ifndef UTILS_H
#define UTILS_H
#include "./types.h"

static inline size_t u_strlen(const char *s)
{
    size_t n = 0;
    while (s && s[n])
        n++;
    return n;
}
static inline int u_strcmp(const char *a, const char *b)
{
    while (*a && (*a == *b))
    {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}
static inline int u_strncmp(const char *a, const char *b, size_t n)
{
    while (n && *a && (*a == *b))
    {
        a++;
        b++;
        n--;
    }
    return n ? ((unsigned char)*a - (unsigned char)*b) : 0;
}
static inline int u_isdigit(char c) { return c >= '0' && c <= '9'; }
static inline int u_isspace(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }

#endif
