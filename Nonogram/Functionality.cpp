#include <iostream>
#include <fstream>

const int LOGIN_MENU_LENGTH = 1;
const int MAIN_MENU_LENGTH = 4;
const int INPUT_MAX_LENGTH = 100;
const int FIRST_LEVEL = 1;
const int NUMBER_OF_LEVELS = 5;
const int NONOGRAMS_PER_LEVEL = 2;
const int TERMINATING_ZERO_LENGTH = 1;

const char EXIT_CHAR = '3';
const char EXIT_STRING[] = "exit";
const char ZERO_CHAR = '0';
const char TERMINATING_ZERO = '\0';
const char LOGIN_PROMPT[] = "Enter your username to login: ";
const char INVALID_INPUT_ERROR[] = "Error: Invalid input.\n";
const char LOGIN_ERROR[] = "Username is not registered.\n";
const char NEW_USER_PROMPT[] = "Do you want to register user? [y/n]\n";
const char FILE_NOT_FOUND_ERROR[] = "Error: File not found.\n";
const char COULDNT_WRITE_TO_FILE_ERROR[] = "Error: Couldn't write to file.\n";
const char NULLPTR_ERROR[] = "Error: nullptr has been passed as an argument.\n";
const char NO_SAVED_GAME_ERROR[] = "Error: No saved game.\n";
const char USERS_PARENT_FOLDER[] = "users/";
const char LEVELS_PARENT_FOLDER[] = "levels/";
const char TEXT_FILE_EXTENTION[] = ".txt";

const char** NewLoginMenu() {
	const char** LOGIN_MENU = new const char* [LOGIN_MENU_LENGTH] {};
	LOGIN_MENU[0] = LOGIN_PROMPT;

	return LOGIN_MENU;
}

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

bool UsernameIsValid(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return false;
	}

	if (username[0] == TERMINATING_ZERO) {
		return false;
	}

	return true;
}

void PauseConsole() {
	system("pause");
}

int LengthOf(const char* string) {
	if (string == nullptr) {
		std::cout << NULLPTR_ERROR;
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

//char* GetTxtPath(const char* PARENT_FOLDERS_PATH, const char* FILE_NAME, const char* FILE_EXTENTION) {
//	if (PARENT_FOLDERS_PATH == nullptr || FILE_NAME == nullptr || FILE_EXTENTION == nullptr) {
//		std::cout << NULLPTR_ERROR;
//		return nullptr;
//	}
//
//	char* userTxtPath = new char[LengthOf(PARENT_FOLDERS_PATH) + LengthOf(FILE_NAME) + LengthOf(FILE_EXTENTION) + TERMINATING_ZERO_LENGTH] {};
//	int currentIndex = 0;
//
//	for (size_t i = 0; PARENT_FOLDERS_PATH[i] != TERMINATING_ZERO; i++, currentIndex++) {
//		userTxtPath[currentIndex] = PARENT_FOLDERS_PATH[i];
//	}
//	for (size_t i = 0; FILE_NAME[i] != TERMINATING_ZERO; i++, currentIndex++) {
//		userTxtPath[currentIndex] = FILE_NAME[i];
//	}
//	for (size_t i = 0; FILE_EXTENTION[i] != TERMINATING_ZERO; i++, currentIndex++) {
//		userTxtPath[currentIndex] = FILE_EXTENTION[i];
//	}
//
//	return userTxtPath;
//}

char* GetUserTxtPath(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return nullptr;
	}
	char* temp = NewString(USERS_PARENT_FOLDER, username);
	char* userTxtPath = NewString(temp, TEXT_FILE_EXTENTION);
	delete[] temp;
	//char* userTxtPath = GetTxtPath(USERS_PARENT_FOLDER, username, TEXT_FILE_EXTENTION);

	return userTxtPath;
}

bool UsernameExists(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
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
		return;
	}

	char* userTxtPath = GetUserTxtPath(username);

	std::ofstream writer(userTxtPath);

	if (!writer.is_open()) {
		std::cout << COULDNT_WRITE_TO_FILE_ERROR;
		return;
	}

	writer.close();

	delete[] userTxtPath;
}

void DeallocateMenuMemory(const char** menu) {
	delete[] menu;
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

bool UserTxtIsEmpty(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return false;
	}

	char* userTxtPath = GetUserTxtPath(username);
	std::ifstream reader(userTxtPath);
	delete[] userTxtPath;

	if (!reader.is_open()) {
		std::cout << FILE_NOT_FOUND_ERROR;
		return false;
	}

	char buffer = TERMINATING_ZERO;
	reader >> buffer;

	if (buffer == TERMINATING_ZERO) {
		return true;
	}

	return false;
}

const char** NewMainMenu(const char* username) {
	const char** MAIN_MENU = new const char* [MAIN_MENU_LENGTH];
	MAIN_MENU[0] = username;
	MAIN_MENU[1] = "\n\n1) Continue\n";
	MAIN_MENU[2] = "2) Start new Nonogram\n";
	MAIN_MENU[3] = "3) Exit\n";

	return MAIN_MENU;
}

bool UserInputIsCorrect(const char* userInput, int menuLength) {
	if (userInput == nullptr) {
		std::cout << NULLPTR_ERROR;
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

inline int CharToInt(char ch) {
	return ch - ZERO_CHAR;
}

char** GetNonogramFromUserTxt(const char* username) {
	char* userTxtPath = GetUserTxtPath(username);
	std::ifstream reader(userTxtPath);
	delete[] userTxtPath;

	if (!reader.is_open()) {
		std::cout << FILE_NOT_FOUND_ERROR;
		return nullptr;
	}

	char currentLine[INPUT_MAX_LENGTH]{};
	char** nonogram = nullptr;

	while (!reader.eof()) {
		reader.getline(currentLine, INPUT_MAX_LENGTH);

		if (currentLine[0] == TERMINATING_ZERO) {
			std::cout << "Bruh can't continue cuz you're new.";
			return nullptr;
		}

		if (currentLine[0] != ' ') {
			continue;
		}

		//bruh idk
	}

	//bruh idk

	reader.close();

	return nonogram;
}

void ContinueLastNonogram(const char* username, int& difficultyLevel, char**& nonogram) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	if (nonogram != nullptr) {
		//Returning here so that the player can't cheese his way into fixing his mistakes.
		return;
	}

	if (UserTxtIsEmpty(username)) {
		std::cout << NO_SAVED_GAME_ERROR;
		PauseConsole();
		return;
	}
	
	//TODO
	//Load nonogram from userTxt
}

inline char IntToChar(int number) {
	return number + ZERO_CHAR;
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

char* IntToString(int number) {
	int numberLength = LengthOf(number);
	char* numberString = new char[numberLength + TERMINATING_ZERO_LENGTH] {};
	number = ReverseOf(number);

	for (size_t i = 0; i < numberLength; i++) {
		numberString[i] = number % 10;
		number /= 10;
	}

	return numberString;
}

char* GetRandomNonogramTxtPath(int difficultyLevel) {
	int nonogramNumber = 0;

	do {
		nonogramNumber = rand() % 10;
	} while (nonogramNumber >= NONOGRAMS_PER_LEVEL);

	char* difficultyLevelString = IntToString(difficultyLevel);
	char* nonogramNumberString = IntToString(nonogramNumber);
	char* temp1 = NewString(LEVELS_PARENT_FOLDER, difficultyLevelString);
	char* temp2 = NewString(temp1, "/");
	char* temp3 = NewString(temp2, nonogramNumberString);
	char* randomNonogramPath = NewString(temp3, TEXT_FILE_EXTENTION);

	delete[] difficultyLevelString;
	delete[] nonogramNumberString;
	delete[] temp1;
	delete[] temp2;
	delete[] temp3;

	return randomNonogramPath;
}

char** RandomNonogram(int difficultyLevel, int& nonogramHeight) {
	char* randomNonogramPath = GetRandomNonogramTxtPath(difficultyLevel);

	std::ifstream reader(randomNonogramPath);

	if (!reader.is_open()) {
		std::cout << FILE_NOT_FOUND_ERROR;
		PauseConsole();
		return nullptr;
	}

	int nonogramLength = 0;

	char buffer[INPUT_MAX_LENGTH]{};

	reader.getline(buffer, INPUT_MAX_LENGTH);

	nonogramLength = LengthOf(buffer);
	nonogramHeight++;

	while (!reader.eof()) {
		reader.getline(buffer, INPUT_MAX_LENGTH);
		nonogramHeight++;
	}

	reader.close();

	char** nonogram = new char*[nonogramHeight];
	
	reader.open(randomNonogramPath);
	delete[] randomNonogramPath;

	for (size_t i = 0; !reader.eof(); i++) {
		nonogram[i] = new char[nonogramLength + TERMINATING_ZERO_LENGTH] {};
		reader.getline(nonogram[i], nonogramLength + TERMINATING_ZERO_LENGTH);
	}

	reader.close();

	return nonogram;
}

void StartNewNonogram(const char* username, int& difficultyLevel, char**& nonogram, int& nonogramHeight) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	//not sure about this
	for (size_t i = 0; i < nonogramHeight; i++) {
		delete[] nonogram[i];
	}
	delete[] nonogram;

	char* userTxtPath = GetUserTxtPath(username);

	std::ifstream reader(userTxtPath);

	if (!reader.is_open()) {
		std::cout << FILE_NOT_FOUND_ERROR;
		return;
	}

	char currentLine[INPUT_MAX_LENGTH]{};

	reader.getline(currentLine, INPUT_MAX_LENGTH);

	if (currentLine[0] != TERMINATING_ZERO) {
		difficultyLevel = CharToInt(currentLine[0]);
	} else {
		difficultyLevel = FIRST_LEVEL;
	}

	reader.close();

	nonogram = RandomNonogram(difficultyLevel, nonogramHeight);

	std::ofstream writer(userTxtPath);
	delete[] userTxtPath;

	writer << difficultyLevel << std::endl;

	for (size_t i = 0; i < nonogramHeight; i++) {
		writer << nonogram[i] << std::endl;
	}

	writer.close();
}

void DisplayNonogram(char** nonogram, int nonogramHeight) {
	ClearConsole();
	for (size_t i = 0; i < nonogramHeight; i++) {
		std::cout << nonogram[i] << std::endl;
	}
}

bool UserInputIsExit(const char* userInput) {
	for (size_t i = 0; EXIT_STRING[i] != TERMINATING_ZERO; i++) {
		if (userInput[i] != EXIT_STRING[i]) {
			return false;
		}
	}

	return true;
}

void UpdateNonogram(char** nonogram, int nonogramHeight,const char* userInput) {
	std::cout << "updating...\n";
	PauseConsole();
}

void PlayNonogram(char** nonogram, int nonogramHeight, bool** nonogramSolution, int difficultyLevel) {
	if (nonogram == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	char userInput[INPUT_MAX_LENGTH]{};

	do {
		DisplayNonogram(nonogram, nonogramHeight);
		if (difficultyLevel == FIRST_LEVEL) {
			std::cout << "Enter input in this format: 10 1 e\n";
		}
		std::cin.getline(userInput, INPUT_MAX_LENGTH);

		if (UserInputIsExit(userInput)) {
			break;
		}

		UpdateNonogram(nonogram, nonogramHeight, userInput);

	} while (true);
}

void SaveNonogramToPlayerTxt(const char* username, int difficultyLevel, char** nonogram, int nonogramHeight) {
	if (username == nullptr || nonogram == nullptr) {
		//No NULLPTR_ERROR because player can log in and immediately exit game.
		return;
	}

	char* userTxtPath = GetUserTxtPath(username);
	std::ofstream writer(userTxtPath);
	delete[] userTxtPath;

	if (!writer.is_open()) {
		std::cout << COULDNT_WRITE_TO_FILE_ERROR;
		return;
	}

	writer << difficultyLevel << std::endl;
	for (size_t i = 0; i < nonogramHeight; i++) {
		writer << nonogram[i] << std::endl;
	}

	writer.close();
}

void StartGame(const char* username) {
	char mainMenuChoice = TERMINATING_ZERO;
	int difficultyLevel = 0;
	char** nonogram = nullptr;
	int nonogramHeight = 0;
	bool** nonogramSolution = nullptr;

	do {
		mainMenuChoice = MainMenu(username);

		if (mainMenuChoice == EXIT_CHAR) {
			break;
		}

		switch (mainMenuChoice) {
			case '1':
				ContinueLastNonogram(username, difficultyLevel, nonogram);
				break;
			case '2':
				StartNewNonogram(username, difficultyLevel, nonogram, nonogramHeight);
				break;
			default:
				break;
		}

		PlayNonogram(nonogram, nonogramHeight, nonogramSolution, difficultyLevel);

	} while (mainMenuChoice != EXIT_CHAR);

	SaveNonogramToPlayerTxt(username, difficultyLevel, nonogram, nonogramHeight);

	if (nonogram != nullptr) {
		for (size_t i = 0; i < nonogramHeight; i++) {
			delete[] nonogram[i];
		}
		
		delete[] nonogram;
	}

	ClearConsole();
	std::cout << "Have a great day, " << username << "!\n";
}

void StartProgram() {
	srand(time(0));

	char* username = new char[INPUT_MAX_LENGTH] {};

	LoginMenu(username);
	StartGame(username);

	delete[] username;
}