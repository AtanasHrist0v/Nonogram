#include <iostream>
#include <fstream>

const int LOGIN_MENU_LENGTH = 1;
const int MAIN_MENU_LENGTH = 4;
const int INPUT_MAX_LENGTH = 100;
const int BUFFER_SIZE = 100;
const int FIRST_LEVEL = 1;
const int NUMBER_OF_LEVELS = 5;
const int NONOGRAMS_PER_LEVEL = 2;
const int TERMINATING_ZERO_LENGTH = 1;

const char EXIT_CHAR = '3';
const char ZERO_CHAR = '0';
const char TERMINATING_ZERO_CHAR = '\0';
const char NEW_USER_PROMPT[] = "Do you want to register user? [y/n]\n";
const char REGISTER_PROMPT[] = "Enter the username you want to register: ";
const char INVALID_INPUT_ERROR[] = "Error: Invalid input. Press Enter to continue.\n";
const char LOGIN_ERROR[] = "Error: Username is not registered.\n";
const char FILE_NOT_FOUND_ERROR[] = "Error: File could not be found.\n";
const char COULDNT_WRITE_TO_FILE_ERROR[] = "Error: Couldn't write to file.\n";
const char NULLPTR_ERROR[] = "Error: nullptr has been passed as an argument.\n";
const char DIFFICULTY_LEVEL_ERROR[] = "Error: Wrong DIFFICULTY_LEVEL.\0";
const char ALL_USERNAMES_PATH[] = "users/allUsernames.txt";//unnecessary
const char USERNAME_PARENT_FOLDER[] = "users/";
const char LEVEL_1_PARENT_FOLDER[] = "levels/1/";
const char LEVEL_2_PARENT_FOLDER[] = "levels/2/";
const char LEVEL_3_PARENT_FOLDER[] = "levels/3/";
const char LEVEL_4_PARENT_FOLDER[] = "levels/4/";
const char LEVEL_5_PARENT_FOLDER[] = "levels/5/";
const char TEXT_FILE_EXTENTION[] = ".txt";

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
	const char** LOGIN_MENU = new const char* [LOGIN_MENU_LENGTH] {};
	LOGIN_MENU[0] = "Enter your username to login: ";

	return LOGIN_MENU;
}

bool UsernameIsValid(const char* USERNAME) {
	if (USERNAME == nullptr) {
		std::cout << NULLPTR_ERROR;
		return false;
	}

	if (USERNAME[0] == TERMINATING_ZERO_CHAR) {
		return false;
	}

	return true;
}

//unnecessary
//bool StringsAreEqual(const char* string1, int string1Length, const char* string2, int string2Length) {
//	if (string1 == nullptr || string2 == nullptr) {
//		std::cout << NULLPTR_ERROR;
//		return false;
//	}
//
//	for (size_t i = 0; i < string1Length && i < string2Length; i++) {
//		if (string1[i] != string2[i]) {
//			return false;
//		}
//
//		if (string1[i] == TERMINATING_ZERO_CHAR || string2[i] == TERMINATING_ZERO_CHAR) {
//			break;
//		}
//	}
//
//	return true;
//}

int LengthOf(const char* string) {
	if (string == nullptr) {
		std::cout << NULLPTR_ERROR;
		return -1;
	}

	int length = 0;

	while (string[length] != TERMINATING_ZERO_CHAR) {
		length++;
	}

	return length;
}

char* GetTxtPath(const char* PARENT_FOLDERS_PATH, const char* FILE_NAME, const char* FILE_EXTENTION) {
	if (PARENT_FOLDERS_PATH == nullptr || FILE_NAME == nullptr || FILE_EXTENTION == nullptr) {
		std::cout << NULLPTR_ERROR;
		return nullptr;
	}

	char* userTxtPath = new char[LengthOf(PARENT_FOLDERS_PATH) + LengthOf(FILE_NAME) + LengthOf(FILE_EXTENTION) + TERMINATING_ZERO_LENGTH] {};
	int currentIndex = 0;

	for (size_t i = 0; PARENT_FOLDERS_PATH[i] != TERMINATING_ZERO_CHAR; i++, currentIndex++) {
		userTxtPath[currentIndex] = PARENT_FOLDERS_PATH[i];
	}
	for (size_t i = 0; FILE_NAME[i] != TERMINATING_ZERO_CHAR; i++, currentIndex++) {
		userTxtPath[currentIndex] = FILE_NAME[i];
	}
	for (size_t i = 0; FILE_EXTENTION[i] != TERMINATING_ZERO_CHAR; i++, currentIndex++) {
		userTxtPath[currentIndex] = FILE_EXTENTION[i];
	}

	return userTxtPath;
}

char* GetUserTxtPath(const char* USERNAME) {
	if (USERNAME == nullptr) {
		std::cout << NULLPTR_ERROR;
		return nullptr;
	}

	char* userTxtPath = GetTxtPath(USERNAME_PARENT_FOLDER, USERNAME, TEXT_FILE_EXTENTION);

	return userTxtPath;
}

bool UsernameExists(const char* USERNAME) {
	if (USERNAME == nullptr) {
		std::cout << NULLPTR_ERROR;
		return false;
	}

	char* userTxtPath = GetUserTxtPath(USERNAME);

	std::ifstream reader(userTxtPath);

	delete[] userTxtPath;

	if (reader.is_open()) {
		reader.close();
		return true;
	}

	return false;
}

void RegisterUser(const char* USERNAME) {
	if (USERNAME == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	char* userTxtPath = GetUserTxtPath(USERNAME);

	std::ofstream writer(userTxtPath);

	if (!writer.is_open()) {
		std::cout << COULDNT_WRITE_TO_FILE_ERROR;
		return;
	}

	writer.close();

	delete[] userTxtPath;
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

void LoginMenu(char* USERNAME) {
	const char** LOGIN_MENU = NewLoginMenu();
	char userInput[INPUT_MAX_LENGTH]{};
	bool usernameExists = false;

	do {
		DisplayMenu(LOGIN_MENU, LOGIN_MENU_LENGTH);
		std::cin.getline(USERNAME, 100);

		if (!UsernameIsValid(USERNAME)) {
			std::cout << INVALID_INPUT_ERROR;
			std::cin.getline(USERNAME, 100); //If not for this get getline(), DisplayMenu() would clear the console and the user won't see that his input is invalid.
			continue;
		}

		usernameExists = UsernameExists(USERNAME);

		if (usernameExists) {
			break;
		}

		std::cout << LOGIN_ERROR;
		std::cout << NEW_USER_PROMPT;
		std::cin.getline(userInput, 100);

		if (userInput[0] == 'y' && userInput[1] == TERMINATING_ZERO_CHAR) {
			RegisterUser(USERNAME);
			usernameExists = true;
		}

	} while (!usernameExists);

	//CopyString(userInput, INPUT_MAX_LENGTH, USERNAME, INPUT_MAX_LENGTH);

	DeallocateMenuMemory(LOGIN_MENU);
}

const char** NewMainMenu(const char* USERNAME) {
	const char** MAIN_MENU = new const char* [MAIN_MENU_LENGTH];
	MAIN_MENU[0] = USERNAME;
	MAIN_MENU[1] = "\n\n1) Continue\n";
	MAIN_MENU[2] = "2) Start new Nonogram\n";
	MAIN_MENU[3] = "3) Exit\n";

	return MAIN_MENU;
}

inline char IntToChar(int number) {
	return number + ZERO_CHAR;
}

//char** RandomNonogram(int difficultyLevel = 1) {
//	char difficultyLevelAsCharArray[2]{};
//	difficultyLevelAsCharArray[0] = IntToChar(difficultyLevel);
//
//	char* currentLevelPath;
//	
//	switch (difficultyLevel) {
//		case 1:
//			currentLevelPath = GetTxtPath(LEVEL_1_PARENT_FOLDER, difficultyLevelAsCharArray, TEXT_FILE_EXTENTION);
//			break;
//		case 2:
//			currentLevelPath = GetTxtPath(LEVEL_2_PARENT_FOLDER, difficultyLevelAsCharArray, TEXT_FILE_EXTENTION);
//			break;
//		case 3:
//			currentLevelPath = GetTxtPath(LEVEL_3_PARENT_FOLDER, difficultyLevelAsCharArray, TEXT_FILE_EXTENTION);
//			break;
//		case 4:
//			currentLevelPath = GetTxtPath(LEVEL_4_PARENT_FOLDER, difficultyLevelAsCharArray, TEXT_FILE_EXTENTION);
//			break;
//		case 5:
//			currentLevelPath = GetTxtPath(LEVEL_5_PARENT_FOLDER, difficultyLevelAsCharArray, TEXT_FILE_EXTENTION);
//			break;
//		default:
//			std::cout << DIFFICULTY_LEVEL_ERROR;
//			currentLevelPath = new char[0];
//			break;
//	}
//
//	//TODO
//
//	delete[] currentLevelPath;
//}

void ContinueLastNonogram(const char* USERNAME) {
	if (USERNAME == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	char* userTxtPath = GetUserTxtPath(USERNAME);

	std::ifstream reader(userTxtPath);

	if (!reader.is_open()) {
		std::ofstream writer(userTxtPath);

		if (!writer.is_open()) {
			std::cout << COULDNT_WRITE_TO_FILE_ERROR;
			return;
		}

		//char** newNonogram = RandomNonogram();

		writer << FIRST_LEVEL << std::endl;
		//writer << newNonogram;

		//delete[] newNonogram;

		writer.close();
		reader.clear();
	}

	//TODO

	delete[] userTxtPath;
}

void StartNewNonogram(const char* USERNAME) {
	if (USERNAME == nullptr) {
		std::cout << NULLPTR_ERROR;
		return;
	}

	//TODO
}

bool UserInputIsCorrect(const char* userInput, int menuLength) {
	if (userInput == nullptr) {
		std::cout << NULLPTR_ERROR;
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

char MainMenu(const char* USERNAME) {
	if (USERNAME == nullptr) {
		std::cout << NULLPTR_ERROR;
		return TERMINATING_ZERO_CHAR;
	}

	const char** MAIN_MENU = NewMainMenu(USERNAME);
	char userInput[INPUT_MAX_LENGTH]{};

	do {
		DisplayMenu(MAIN_MENU, MAIN_MENU_LENGTH);
		std::cin.getline(userInput, 100);

	} while (!UserInputIsCorrect(userInput, MAIN_MENU_LENGTH));

	DeallocateMenuMemory(MAIN_MENU);

	return userInput[0];
}

void StartGame(const char* USERNAME) {
	char mainMenuChoice = TERMINATING_ZERO_CHAR;

	do {
		mainMenuChoice = MainMenu(USERNAME);

		switch (mainMenuChoice) {
			case '1':
				ContinueLastNonogram(USERNAME);
				break;
			case '2':
				StartNewNonogram(USERNAME);
				break;
			default:
				break;
		}
	} while (mainMenuChoice != EXIT_CHAR);

	ClearConsole();
	std::cout << "Have a great day, " << USERNAME << "!\n";
}

void StartProgram() {
	srand(time(0));

	char* username = new char[INPUT_MAX_LENGTH] {};

	LoginMenu(username);
	StartGame(username);

	delete[] username;
}