#include <iostream>
#include <fstream>
#include "Constants.h"
#include "HelperFunctions.h"
#include "LoginMenu.h"
#include "MainMenu.h"

int** GetNonogramFromUserTxt(const char* username, int& difficultyLevel, int& allowedMistakes, int& nonogramSize, bool**& nonogramSolution, int& playerMistakes) {
	char* userTxtPath = GetUserTxtPath(username);
	nonogramSize = 0;

	if (userTxtPath == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return nullptr;
	}

	std::ifstream reader(userTxtPath);
	delete[] userTxtPath;

	if (!reader.is_open()) {
		std::cout << FILE_NOT_FOUND_ERROR;
		PauseConsole();
		return nullptr;
	}

	char currentLine[INPUT_MAX_LENGTH]{};
	reader.getline(currentLine, INPUT_MAX_LENGTH);
	difficultyLevel = CharToInt(currentLine[0]);
	reader.getline(currentLine, INPUT_MAX_LENGTH);
	allowedMistakes = CharToInt(currentLine[0]);

	reader.getline(currentLine, INPUT_MAX_LENGTH);
	nonogramSize = LengthOf(currentLine);
	nonogramSolution = new bool* [nonogramSize] {};

	for (size_t i = 0; i < nonogramSize; i++) {
		nonogramSolution[i] = new bool[nonogramSize] {};
		for (size_t j = 0; j < nonogramSize; j++) {
			nonogramSolution[i][j] = CharToInt(currentLine[j]);
		}

		reader.getline(currentLine, INPUT_MAX_LENGTH);
	}

	playerMistakes = CharToInt(currentLine[0]);
	int** nonogram = new int* [nonogramSize] {};

	for (size_t i = 0; i < nonogramSize; i++) {
		reader.getline(currentLine, INPUT_MAX_LENGTH);

		nonogram[i] = new int[nonogramSize] {};
		for (size_t j = 0; j < nonogramSize; j++) {
			nonogram[i][j] = CharToInt(currentLine[j]);
		}
	}

	reader.close();
	return nonogram;
}

bool UserTxtIsEmpty(const char* username) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return false;
	}

	char* userTxtPath = GetUserTxtPath(username);
	std::ifstream reader(userTxtPath);
	delete[] userTxtPath;

	if (!reader.is_open()) {
		std::cout << FILE_NOT_FOUND_ERROR;
		PauseConsole();
		return true;
	}

	char buffer = TERMINATING_ZERO;
	reader >> buffer;

	if (buffer == TERMINATING_ZERO) {
		return true;
	}

	return false;
}

void ContinueLastNonogram(const char* username, int& difficultyLevel, int& allowedMistakes, int& nonogramSize, bool**& nonogramSolution, int& playerMistakes, int**& nonogram) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
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

	nonogram = GetNonogramFromUserTxt(username, difficultyLevel, allowedMistakes, nonogramSize, nonogramSolution, playerMistakes);
}

//Needs total rework
int DifficultyMenu(int difficultyLevel) {
	int userChoice = 0;
	char userInput[INPUT_MAX_LENGTH]{};

	do {
		//will move to a separate function
		{
			ClearConsole();
			std::cout << "Choose your level of difficulty from the given below:" << std::endl;
			for (size_t i = 1; i <= difficultyLevel; i++) {
				std::cout << i << " ";
			}
			std::cout << std::endl << "Your choice: ";
		}

		std::cin.getline(userInput, 100);

		if (userInput[1] == TERMINATING_ZERO) {
			userChoice = CharToInt(userInput[0]);
		}

	} while (userChoice <= 0 || userChoice > difficultyLevel);

	return userChoice;
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

bool** GetNonogramFromTxt(const char* nonogramPath, int& allowedMistakes, int& nonogramSize) {
	if (nonogramPath == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return nullptr;
	}

	std::ifstream reader(nonogramPath);

	if (!reader.is_open()) {
		std::cout << FILE_NOT_FOUND_ERROR;
		PauseConsole();
		return nullptr;
	}

	char currentLine[INPUT_MAX_LENGTH]{};
	reader.getline(currentLine, INPUT_MAX_LENGTH);
	allowedMistakes = CharToInt(currentLine[0]);

	reader.getline(currentLine, INPUT_MAX_LENGTH);
	nonogramSize = LengthOf(currentLine);
	bool** nonogram = new bool* [nonogramSize] {};

	for (size_t i = 0; i < nonogramSize; i++) {
		nonogram[i] = new bool[nonogramSize] {};
		for (size_t j = 0; j < nonogramSize; j++) {
			nonogram[i][j] = CharToInt(currentLine[j]);
		}

		reader.getline(currentLine, INPUT_MAX_LENGTH);
	}

	reader.close();
	return nonogram;
}

bool** RandomNonogram(int difficultyLevel, int& allowedMistakes, int& nonogramSize) {
	char* randomNonogramPath = GetRandomNonogramTxtPath(difficultyLevel);
	bool** randomNonogram = GetNonogramFromTxt(randomNonogramPath, allowedMistakes, nonogramSize);

	delete[] randomNonogramPath;
	return randomNonogram;
}

void DeallocateMemory(int**& nonogram, int nonogramSize) {
	if (nonogram == nullptr) {
		return;
	}

	for (size_t i = 0; i < nonogramSize; i++) {
		delete[] nonogram[i];
	}
	delete[] nonogram;
}

void DeallocateMemory(bool**& nonogram, int nonogramSize) {
	if (nonogram == nullptr) {
		return;
	}

	for (size_t i = 0; i < nonogramSize; i++) {
		delete[] nonogram[i];
	}
	delete[] nonogram;
}

int GetDifficultyLevelFromUserTxt(const char* username) {
	char* userTxtPath = GetUserTxtPath(username);
	std::ifstream reader(userTxtPath);

	if (!reader.is_open()) {
		std::cout << FILE_NOT_FOUND_ERROR;
		PauseConsole();
		return -1;
	}

	//char currentLine[INPUT_MAX_LENGTH]{};
	//reader.getline(currentLine, INPUT_MAX_LENGTH);
	char difficultyLevel = '\0';
	reader >> difficultyLevel;

	reader.close();
	delete[] userTxtPath;

	if (difficultyLevel == TERMINATING_ZERO) {
		return FIRST_LEVEL;
	}

	return CharToInt(difficultyLevel);
}

void StartNewNonogram(const char* username, int& difficultyLevel, int& allowedMistakes, int& nonogramSize, bool**& nonogramSolution, int& playerMistakes, int**& nonogram) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return;
	}

	playerMistakes = 0;

	DeallocateMemory(nonogram, nonogramSize);
	DeallocateMemory(nonogramSolution, nonogramSize);

	if (difficultyLevel == 0) {
		difficultyLevel = GetDifficultyLevelFromUserTxt(username);
	}

	int difficultyChoice = DifficultyMenu(difficultyLevel);
	nonogramSolution = RandomNonogram(difficultyChoice, allowedMistakes, nonogramSize);

	nonogram = new int* [nonogramSize] {};
	for (size_t i = 0; i < nonogramSize; i++) {
		nonogram[i] = new int[nonogramSize] {};
		for (size_t j = 0; j < nonogramSize; j++) {
			nonogram[i][j] = 2;
		}
	}
}

void SaveNonogramToPlayerTxt(const char* username, int difficultyLevel, int allowedMistakes, int nonogramSize, bool** nonogramSolution, int playerMistakes, int** nonogram) {
	if (username == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return;
	}

	if (nonogram == nullptr) {
		//No NULLPTR_ERROR because player can log in and immediately exit game.
		return;
	}

	char* userTxtPath = GetUserTxtPath(username);
	std::ofstream writer(userTxtPath);
	delete[] userTxtPath;

	if (!writer.is_open()) {
		std::cout << COULDNT_WRITE_TO_FILE_ERROR;
		PauseConsole();
		return;
	}

	writer << difficultyLevel << std::endl;
	writer << allowedMistakes << std::endl;
	for (size_t i = 0; i < nonogramSize; i++) {
		for (size_t j = 0; j < nonogramSize; j++) {
			writer << nonogramSolution[i][j];
		}
		writer << std::endl;
	}

	writer << playerMistakes;
	for (size_t i = 0; i < nonogramSize; i++) {
		writer << std::endl;
		for (size_t j = 0; j < nonogramSize; j++) {
			writer << nonogram[i][j];
		}
	}

	writer.close();
}

char** GetGraphicalNonogram(int** nonogram, int nonogramSize, int& graphicalNonogramSize) {
	const int ROWS = nonogramSize + 2,
		ROW_LENGTH = (nonogramSize + 1) * 2 + TERMINATING_ZERO_LENGTH;

	graphicalNonogramSize = ROWS;

	char** graphicalNonogram = new char* [ROWS] {};

	graphicalNonogram[0] = new char[ROW_LENGTH] {};
	graphicalNonogram[0][0] = '|';
	graphicalNonogram[0][ROW_LENGTH - 2] = '|';

	for (size_t i = 1; i < ROW_LENGTH - 2; i++) {
		graphicalNonogram[0][i] = '=';
	}

	for (size_t i = 0; i < nonogramSize; i++) {
		graphicalNonogram[i + 1] = new char[ROW_LENGTH] {};

		graphicalNonogram[i + 1][0] = '|';
		graphicalNonogram[i + 1][ROW_LENGTH - 2] = '|';
		for (size_t j = 0; j < nonogramSize; j++) {
			switch (nonogram[i][j]) {
				case 0:
					graphicalNonogram[i + 1][2 * j + 1] = '>';
					graphicalNonogram[i + 1][2 * j + 2] = '<';
					break;
				case 1:
					graphicalNonogram[i + 1][2 * j + 1] = '#';
					graphicalNonogram[i + 1][2 * j + 2] = '#';
					break;
				case 2:
					graphicalNonogram[i + 1][2 * j + 1] = ' ';
					graphicalNonogram[i + 1][2 * j + 2] = ' ';
					break;
				default:
					break;
			}
		}
	}

	graphicalNonogram[ROWS - 1] = new char[ROW_LENGTH] {};
	graphicalNonogram[ROWS - 1][0] = graphicalNonogram[ROWS - 1][ROW_LENGTH - 2] = '|';

	for (size_t i = 1; i < ROW_LENGTH - 2; i++) {
		graphicalNonogram[ROWS - 1][i] = '=';
	}

	return graphicalNonogram;
}

bool UserInputIsExit(const char* userInput) {
	for (size_t i = 0; EXIT_STRING[i] != TERMINATING_ZERO; i++) {
		if (userInput[i] != EXIT_STRING[i]) {
			return false;
		}
	}

	return true;
}

void DisplayNonogram(char** nonogram, int nonogramSize) {
	ClearConsole();
	for (size_t i = 0; i < nonogramSize; i++) {
		std::cout << nonogram[i] << std::endl;
	}
}

int ExtractNumberFromString(const char* string, int startIndex, int endIndex) {
	int number = 0;

	for (int i = endIndex - 1; i >= startIndex; i--) {
		number *= 10;
		number += CharToInt(string[i]);
	}

	return number;
}

void ExtractDataFromUserInput(const char* userInput, int& x, int& y, char& state) {
	int firstSpaceIndex = 0,
		secondSpaceIndex = 0;

	for (size_t i = 0; userInput[i] != TERMINATING_ZERO; i++) {
		if (userInput[i] == ' ') {
			firstSpaceIndex = i;
			break;
		}
	}
	for (size_t i = firstSpaceIndex + 1; userInput[i] != TERMINATING_ZERO; i++) {
		if (userInput[i] == ' ') {
			secondSpaceIndex = i;
			break;
		}
	}

	x = ExtractNumberFromString(userInput, 0, firstSpaceIndex);
	y = ExtractNumberFromString(userInput, firstSpaceIndex + 1, secondSpaceIndex);
	state = userInput[secondSpaceIndex + 1];
}

void UpdateNonogram(const char* userInput, int** nonogram, bool** nonogramSolution, int nonogramSize, int& playerMistakes) {
	int x = 0,
		y = 0;
	char state = TERMINATING_ZERO;

	ExtractDataFromUserInput(userInput, x, y, state);

	if (x >= nonogramSize || y >= nonogramSize) {
		std::cout << "Wrong input.\n";
		PauseConsole();
		return;
	}

	switch (state) {
		case 'e':
			nonogram[x][y] = 0;
			break;
		case 'f':
			if (nonogramSolution[x][y] == 1) {
				nonogram[x][y] = 1;
				break;
			}

			playerMistakes++;
			std::cout << "You've made a mistake.\n";
			PauseConsole();
			break;
		default:
			std::cout << "Wrong input.\n";
			PauseConsole();
			return;
	}

	bool rowIsCompleted = true,
		columnCompleted = true;

	for (size_t i = 0; i < nonogramSize; i++) {
		if (nonogramSolution[x][i] == 1) {
			if (nonogram[x][i] != 1) {
				rowIsCompleted = false;
				break;
			}
		}
	}
	for (size_t i = 0; i < nonogramSize; i++) {
		if (nonogramSolution[i][y] == 1) {
			if (nonogram[i][y] != 1) {
				columnCompleted = false;
				break;
			}
		}
	}

	if (rowIsCompleted) {
		for (size_t i = 0; i < nonogramSize; i++) {
			nonogram[x][i] = nonogramSolution[x][i];
		}
	}
	if (columnCompleted) {
		for (size_t i = 0; i < nonogramSize; i++) {
			nonogram[i][y] = nonogramSolution[i][y];
		}
	}
}

bool NonogramsMatch(int** nonogram, bool** nonogramSolution, int nonogramSize) {
	for (size_t i = 0; i < nonogramSize; i++) {
		for (size_t j = 0; j < nonogramSize; j++) {
			if (nonogram[i][j] != nonogramSolution[i][j]) {
				return false;
			}
		}
	}

	return true;
}

void UpdateGraphicalNonogram(char** graphicalNonogram, int** nonogram, int nonogramSize, int firstSquareX, int firstSquareY) {
	char ch1 = TERMINATING_ZERO,
		ch2 = TERMINATING_ZERO;

	for (size_t i = 0; i < nonogramSize; i++) {
		for (size_t j = 0; j < nonogramSize; j++) {
			switch (nonogram[i][j]) {
				case 0:
					ch1 = '>';
					ch2 = '<';
					break;
				case 1:
					ch1 = '#';
					ch2 = '#';
					break;
				case 2:
					ch1 = ' ';
					ch2 = ' ';
					break;
				default:
					break;
			}

			graphicalNonogram[firstSquareX + i][firstSquareY + 2 * j] = ch1;
			graphicalNonogram[firstSquareX + i][firstSquareY + 2 * j + 1] = ch2;
		}
	}
}

void PlayNonogram(int& difficultyLevel, int allowedMistakes, int nonogramSize, bool** nonogramSolution, int& playerMistakes, int** nonogram) {
	if (nonogram == nullptr) {
		std::cout << NULLPTR_ERROR;
		PauseConsole();
		return;
	}

	//not sure
	if (playerMistakes > allowedMistakes) {
		ClearConsole();
		std::cout << "You have made too many mistakes, you can't continue playing this nonogram.\n";
		PauseConsole();
		return;
	}

	if (NonogramsMatch(nonogram, nonogramSolution, nonogramSize)) {
		ClearConsole();
		std::cout << "You have already finished this nonogram.\n";
		PauseConsole();
		return;
	}

	int graphicalNonogramSize = 0;
	char** graphicalNonogram = GetGraphicalNonogram(nonogram, nonogramSize, graphicalNonogramSize);

	char userInput[INPUT_MAX_LENGTH]{};

	while (!UserInputIsExit(userInput)) {
		if (playerMistakes > allowedMistakes) {
			std::cout << "You have made too many mistakes, you can't continue playing this nonogram.\n";
			PauseConsole();
			return;
		}

		DisplayNonogram(graphicalNonogram, graphicalNonogramSize);
		if (difficultyLevel == FIRST_LEVEL) {
			std::cout << "Enter input in this format: <row> <column> <state(e/f)>\n";
		}
		std::cin.getline(userInput, INPUT_MAX_LENGTH);

		UpdateNonogram(userInput, nonogram, nonogramSolution, nonogramSize, playerMistakes);
		UpdateGraphicalNonogram(graphicalNonogram, nonogram, nonogramSize, 1, 1);

		if (NonogramsMatch(nonogram, nonogramSolution, nonogramSize)) {
			difficultyLevel++;
			DisplayNonogram(graphicalNonogram, graphicalNonogramSize);
			std::cout << "Congrats! You have successfully guessed the nonogram!\n";
			PauseConsole();
			return;
		}
	}
}

void StartGame(const char* username) {
	int difficultyLevel = 0,
		allowedMistakes = 0,
		playerMistakes = 0,
		nonogramSize = 0;
	int** nonogram = nullptr;
	bool** nonogramSolution = nullptr;

	char mainMenuChoice = MainMenu(username);

	while (mainMenuChoice != EXIT_CHAR) {
		switch (mainMenuChoice) {
			case '1':
				ContinueLastNonogram(username, difficultyLevel, allowedMistakes, nonogramSize, nonogramSolution, playerMistakes, nonogram);
				break;
			case '2':
				StartNewNonogram(username, difficultyLevel, allowedMistakes, nonogramSize, nonogramSolution, playerMistakes, nonogram);
				SaveNonogramToPlayerTxt(username, difficultyLevel, allowedMistakes, nonogramSize, nonogramSolution, playerMistakes, nonogram);
				break;
			default:
				break;
		}

		PlayNonogram(difficultyLevel, allowedMistakes, nonogramSize, nonogramSolution, playerMistakes, nonogram);
		mainMenuChoice = MainMenu(username);
	}

	SaveNonogramToPlayerTxt(username, difficultyLevel, allowedMistakes, nonogramSize, nonogramSolution, playerMistakes, nonogram);

	if (nonogram != nullptr) {
		for (size_t i = 0; i < nonogramSize; i++) {
			delete[] nonogram[i];
		}

		delete[] nonogram;
	}

	if (nonogramSolution != nullptr) {
		for (size_t i = 0; i < nonogramSize; i++) {
			delete[] nonogramSolution[i];
		}

		delete[] nonogramSolution;
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