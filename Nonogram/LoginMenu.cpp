#include <iostream>
#include <fstream>
#include "Constants.h"
#include "HelperFunctions.h"

const char** NewLoginMenu() {
	const char** LOGIN_MENU = new const char* [LOGIN_MENU_LENGTH] {};
	LOGIN_MENU[0] = LOGIN_PROMPT;

	return LOGIN_MENU;
}

bool UsernameIsValid(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return false;
	}

	if (username[0] == TERMINATING_ZERO) {
		return false;
	}

	return true;
}

bool UsernameExists(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return false;
	}

	char* userTxtPath = GetUserTxtPath(username);
	std::ifstream reader(userTxtPath);
	delete[] userTxtPath;

	if (reader.is_open()) {
		reader.close();
		return true;
	}

	return false;
}

void RegisterUser(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return;
	}

	char* userTxtPath = GetUserTxtPath(username);
	std::ofstream writer(userTxtPath);

	if (!writer.is_open()) {
		std::cout << COULDNT_WRITE_TO_FILE_ERROR;
		PauseConsole();
		return;
	}

	writer.close();
	delete[] userTxtPath;
}

void LoginMenu(char* username) {
	const char** LOGIN_MENU = NewLoginMenu();
	char userInput[INPUT_MAX_LENGTH]{};
	bool usernameExists = false;

	do {
		DisplayMenu(LOGIN_MENU, LOGIN_MENU_LENGTH);
		std::cin.getline(username, 100);

		if (!UsernameIsValid(username)) {
			std::cout << INVALID_INPUT_ERROR;
			PauseConsole();
			continue;
		}

		usernameExists = UsernameExists(username);

		if (usernameExists) {
			break;
		}

		std::cout << LOGIN_ERROR;
		std::cout << NEW_USER_PROMPT;
		std::cin.getline(userInput, 100);

		if (userInput[0] == 'y' && userInput[1] == TERMINATING_ZERO) {
			RegisterUser(username);
			usernameExists = true;
		}

	} while (!usernameExists);

	DeallocateMenuMemory(LOGIN_MENU);
}