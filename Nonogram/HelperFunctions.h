#pragma once

void PauseConsole();
void ClearConsole();
void DisplayMenu(const char** menu, int menuLength);
void DeallocateMenuMemory(const char** menu);
int LengthOf(const char* string);
char* NewString(const char* string1, const char* string2);
char* GetUserTxtPath(const char* username);
int CharToInt(char ch);
int LengthOf(int number);
inline char IntToChar(int number);
char* IntToString(int number);
void DeleteMatrix(int** matrix, int size);
void DeleteMatrix(char** matrix, int size);
int GreatestNumberInMatrix(int** matrix, int width, int height);