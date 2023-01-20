#include <iostream>
#include <fstream>

const int LOGIN_MENU_LENGTH = 1,
MAIN_MENU_LENGTH = 4,
INPUT_MAX_LEGTH = 100,
BUFFER_SIZE = 100;

const char EXIT_CHAR = '3';
const char INVALID_INPUT_ERROR[] = "Error: Invalid input. Press Enter to continue.\n";
const char LOGIN_ERROR[] = "Error: Username is not registered.\n";
const char NEW_USER_PROMPT[] = "Do you want to register user? [y/n]\n";
const char REGISTER_PROMPT[] = "Enter the username you want to register: ";
const char FILE_NOT_FOUND_ERROR[] = "Error: File could not be found.\n";
const char NULLPTR_ERROR[] = "Error: nullptr has been passed as an argument.\n";
const char ALL_USERNAMES_PATH[] = "users/allUsernames.txt";
const char USERNAME_PARENT_FOLDER[] = "users/";
const char USERNAME_FILE_EXTENTION[] = ".txt";

void ClearConsole() {
	system("cls");
}

void DisplayMenu(const char** menu, int menuLength) {
	if (menu == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	ClearConsole();

	for (size_t i = 0; i < menuLength; i++) {
		if (menu[i] == nullptr) {
			std::cout << NULLPTR_ERROR;
			return;
		}

		std::cout << menu[i];
	}
}

void DeallocateMenuMemory(const char** menu) {
	delete[] menu;
}

const char** NewLoginMenu() {
	const char** LOGIN_MENU = new const char* [LOGIN_MENU_LENGTH];
	LOGIN_MENU[0] = "Enter your username to login: ";

	return LOGIN_MENU;
}

bool UsernameIsValid(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return false;
	}

	if (username[0] == '\0') {
		return false;
	}

	return true;
}

bool StringsAreEqual(const char* string1, int string1Length, const char* string2, int string2Length) {
	if (string1 == nullptr || string2 == nullptr) {
		std::cout << NULLPTR_ERROR;
		return false;
	}

	for (size_t i = 0; i < string1Length && i < string2Length; i++) {
		if (string1[i] != string2[i]) {
			return false;
		}

		if (string1[i] == '\0' || string2[i] == '\0') {
			break;
		}
	}

	return true;
}

bool UsernameExists(const char* username, int usernameLength) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return false;
	}

	std::ifstream reader(ALL_USERNAMES_PATH);

	if (!reader.is_open()) {
		std::cout << FILE_NOT_FOUND_ERROR;
		return false;
	}

	char buffer[BUFFER_SIZE]{};
	while (!reader.eof()) {
		reader.getline(buffer, BUFFER_SIZE);

		if (StringsAreEqual(username, usernameLength, buffer, BUFFER_SIZE)) {
			reader.close();
			return true;
		}
	}

	reader.close();
	return false;
}

void RegisterUser(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	std::ofstream writer(ALL_USERNAMES_PATH, std::ios::app);

	if (!writer.is_open()) {
		std::cout << FILE_NOT_FOUND_ERROR;
		return;
	}

	writer << username << std::endl;
	writer.close();
}

void CopyString(const char* source, int sourceLength, char* destination, int destinationLength) {
	if (source == nullptr || destination == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	for (size_t i = 0; i < sourceLength && i < destinationLength; i++) {
		destination[i] = source[i];
	}
}

void LoginMenu(char* username) {
	const char** LOGIN_MENU = NewLoginMenu();
	char userInput[INPUT_MAX_LEGTH]{};
	char newUserPromptUserInput[INPUT_MAX_LEGTH]{};
	bool usernameExists = false;

	do {
		DisplayMenu(LOGIN_MENU, LOGIN_MENU_LENGTH);
		std::cin.getline(userInput, 100);

		if (!UsernameIsValid(userInput)) {
			std::cout << INVALID_INPUT_ERROR;
			std::cin.getline(userInput, 100);
			continue;
		}

		usernameExists = UsernameExists(userInput, INPUT_MAX_LEGTH);

		if (usernameExists) {
			break;
		}

		std::cout << LOGIN_ERROR;
		std::cout << NEW_USER_PROMPT;
		std::cin.getline(newUserPromptUserInput, 100);

		if (newUserPromptUserInput[0] == 'y' && newUserPromptUserInput[1] == '\0') {
			RegisterUser(userInput);
			usernameExists = true;
		}

	} while (!usernameExists);

	CopyString(userInput, INPUT_MAX_LEGTH, username, INPUT_MAX_LEGTH);

	DeallocateMenuMemory(LOGIN_MENU);
}

const char** NewMainMenu(const char* username) {
	const char** MAIN_MENU = new const char* [MAIN_MENU_LENGTH];
	MAIN_MENU[0] = username;
	MAIN_MENU[1] = "\n\n1) Continue\n";
	MAIN_MENU[2] = "2) Start new Nonogram\n";
	MAIN_MENU[3] = "3) Exit\n";

	return MAIN_MENU;
}

int LengthOf(const char* string) {
	if (string == nullptr) {
		std::cout << NULLPTR_ERROR;
		return -1;
	}

	int length = 0;

	for (size_t i = 0; string[i] != '\0'; i++) {
		length++;
	}

	return length;
}

char* GetUserTxtPath(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return nullptr;
	}

	char* userTxtPath = new char[LengthOf(USERNAME_PARENT_FOLDER) + LengthOf(username) + LengthOf(USERNAME_FILE_EXTENTION) + 1] {};
	int currentIndex = 0;

	for (size_t i = 0; USERNAME_PARENT_FOLDER[i] != '\0'; i++, currentIndex++) {
		userTxtPath[currentIndex] = USERNAME_PARENT_FOLDER[i];
	}
	for (size_t i = 0; username[i] != '\0'; i++, currentIndex++) {
		userTxtPath[currentIndex] = username[i];
	}
	for (size_t i = 0; USERNAME_FILE_EXTENTION[i] != '\0'; i++, currentIndex++) {
		userTxtPath[currentIndex] = USERNAME_FILE_EXTENTION[i];
	}

	return userTxtPath;
}

void ContinueLastNonogram(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	char* userTxtPath = GetUserTxtPath(username);

	std::ifstream reader(userTxtPath);

	if (!reader.is_open()) {
		std::wcout << FILE_NOT_FOUND_ERROR;
		return;
	}

	delete[] userTxtPath;
}

void StartNewNonogram(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	//TODO
}

void MainMenu(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	const char** MAIN_MENU = NewMainMenu(username);
	char userInput[INPUT_MAX_LEGTH]{};

	do {
		DisplayMenu(MAIN_MENU, MAIN_MENU_LENGTH);
		std::cin.getline(userInput, 100);

		if (userInput[1] != '\0') {
			std::cout << INVALID_INPUT_ERROR;
			std::cin.getline(userInput, 100);
			continue;
		}

		switch (userInput[0]) {
			case '1':
				ContinueLastNonogram(username);
				break;
			case '2':
				StartNewNonogram(username);
				break;
			default:
				break;
		}

	} while (userInput[0] != EXIT_CHAR);

	std::cout << "\nHave a great day, " << username << "!\n";

	DeallocateMenuMemory(MAIN_MENU);
}

void StartProgram() {
	char* username = new char[INPUT_MAX_LEGTH] {};

	LoginMenu(username);
	MainMenu(username);

	delete[] username;
}