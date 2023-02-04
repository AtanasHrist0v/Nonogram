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
* <MainMenu cpp file>
*
*/

#include <iostream>
#include "Constants.h"
#include "HelperFunctions.h"

const char** NewMainMenu(const char* username) {
	const char** MAIN_MENU = new const char* [MAIN_MENU_LENGTH] {};
	MAIN_MENU[0] = username;
	MAIN_MENU[1] = "\n\n1) Continue\n";
	MAIN_MENU[2] = "2) Start new Nonogram\n";
	MAIN_MENU[3] = "3) Exit\n";

	return MAIN_MENU;
}

bool UserInputIsCorrect(const char* userInput, int menuLength) {
	if (userInput == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return false;
	}

	if (userInput[1] != TERMINATING_ZERO) {
		return false;
	}

	switch (userInput[0]) {
		case '1':
		case '2':
		case '3':
			return true;
		default:
			return false;
	}
}

char MainMenu(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return TERMINATING_ZERO;
	}

	const char** MAIN_MENU = NewMainMenu(username);
	char userInput[INPUT_MAX_LENGTH]{};

	do {
		DisplayMenu(MAIN_MENU, MAIN_MENU_LENGTH);
		std::cin.getline(userInput, INPUT_MAX_LENGTH);

	} while (!UserInputIsCorrect(userInput, MAIN_MENU_LENGTH));

	DeallocateMenuMemory(MAIN_MENU);

	return userInput[0];
}