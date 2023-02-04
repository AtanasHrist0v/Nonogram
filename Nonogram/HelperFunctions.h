#pragma once

void PauseConsole();
void ClearConsole();
void DisplayMenu(const char** menu, int menuLength);
void DeallocateMenuMemory(const char** menu);
int LengthOf(const char* string);
char* NewString(const char* string1, const char* string2);
char* GetUserTxtPath(const char* username);
inline int CharToInt(char ch);
char* IntToString(int number);