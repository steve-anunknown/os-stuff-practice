#ifndef minishell_utils_h
#define minishell_utils_h

#include <stdbool.h>

char *ltrim(char *s);
char *rtrim(char *s);
char *trim(char *s);
void splitLineToWords(char *line, char *words[]);
void executeCommand(char* args[], char* nextargs[], char* file, bool background, bool inputRedir, bool outputRedir, bool pipef);
void checkExit(char* args[]);
bool checkHistory(char* args[], bool* flag, char* last, char* curr);

#endif /* minishell_utils_h */