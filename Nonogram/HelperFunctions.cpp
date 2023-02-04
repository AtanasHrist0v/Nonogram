/**
*
* Solution to course project # 1
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Atanas Hristov
* @idnumber 8MI0600242
* @compiler VC
*
* <file with helper functions>
*
*/

#include <iostream>
#include "Constants.h"

void PauseConsole() {
	system("pause");
}

void ClearConsole() {
	system("cls");
}

void DisplayMenu(const char** menu, int menuLength) {
	if (menu == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return;
	}

	ClearConsole();

	for (size_t i = 0; i < menuLength; i++) {
		if (menu[i] == nullptr) {
			std::cout << NULLPTR_ERROR;
			PauseConsole();
			return;
		}

		std::cout << menu[i];
	}
}

void DeallocateMenuMemory(const char** menu) {
	delete[] menu;
}

int LengthOf(const char* string) {
	if (string == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return -1;
	}

	int length = 0;

	while (string[length] != TERMINATING_ZERO) {
		length++;
	}

	return length;
}

char* NewString(const char* string1, const char* string2) {
	if (string1 == nullptr || string2 == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return nullptr;
	}

	int string1Length = LengthOf(string1),
		string2Length = LengthOf(string2);
	char* newString = new char[string1Length + string2Length + TERMINATING_ZERO_LENGTH] {};

	for (size_t i = 0; i < string1Length; i++) {
		newString[i] = string1[i];
	}
	for (size_t i = 0; i < string2Length; i++) {
		newString[string1Length + i] = string2[i];
	}

	return newString;
}

char* GetUserTxtPath(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return nullptr;
	}

	char* temp = NewString(USERS_PARENT_FOLDER, username);
	char* userTxtPath = NewString(temp, TEXT_FILE_EXTENTION);
	delete[] temp;

	return userTxtPath;
}

inline int CharToInt(char ch) {
	return ch - ZERO_CHAR;
}

int LengthOf(int number) {
	if (number == 0) {
		return 1;
	}

	int length = 0;

	while (number != 0) {
		length++;
		number /= 10;
	}

	return length;
}

int ReverseOf(int number) {
	int reversedNumber = 0;

	while (number != 0) {
		reversedNumber *= 10;
		reversedNumber += number % 10;
		number /= 10;
	}

	return reversedNumber;
}

inline char IntToChar(int number) {
	return number + ZERO_CHAR;
}

char* IntToString(int number) {
	int numberLength = LengthOf(number);
	char* numberString = new char[numberLength + TERMINATING_ZERO_LENGTH] {};
	number = ReverseOf(number);

	for (size_t i = 0; i < numberLength; i++) {
		numberString[i] = IntToChar(number % 10);
		number /= 10;
	}

	return numberString;
}