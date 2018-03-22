#ifndef _CSTRING_H
#define _CSTRING_H
#include<sys/defs.h>
//#include<sys/init_pd.h>

size_t strlen(char* input);
int mystrcmp(char *dest, char *src);
void strcopy(char* source, char* destination);
void clearstring(char* s, int len);
char* strcpy(char *dest,  char *src);
void parser(char command[]);

#endif