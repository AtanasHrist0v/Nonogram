#include <iostream>

const int MAIN_MENU_LENGTH = 3,
	INPUT_MAX_LEGTH = 100;
const char ESCAPE_CHAR = 27;

const char** NewMainMenu() {
	const char** MAIN_MENU = new const char* [MAIN_MENU_LENGTH];
	MAIN_MENU[0] = "1) bro\n";
	MAIN_MENU[1] = "2) imma\n";
	MAIN_MENU[2] = "3) cum\n";

	return MAIN_MENU;
}

void ClearConsole() {
	system("cls");
}

void DisplayMenu(const char** menu, int menuLength) {
	ClearConsole();

	for (size_t i = 0; i < menuLength; i++) {
		std::cout << menu[i];
	}
}

void DeallocateMemory(const char** pointer, int length) {
	for (size_t i = 0; i < length; i++) {
		delete[] pointer[i];
	}

	delete[] pointer;
}

void MainMenu() {
	const char** MAIN_MENU = NewMainMenu();
	char userInput[INPUT_MAX_LEGTH]{};

	do {
		DisplayMenu(MAIN_MENU, MAIN_MENU_LENGTH);
		std::cin.getline(userInput, 100);

		if (userInput[1] != '\0') {
			std::cout << "Wrong input! Press Enter to continue.";
			std::cin.getline(userInput, 100);
			continue;
		}

	} while (userInput[0] != ESCAPE_CHAR);
	
	DeallocateMemory(MAIN_MENU, MAIN_MENU_LENGTH);
}

void StartProgram() {
	MainMenu();
}