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
		std::cout << NO_SAVED_GAME_MESSAGE;
		PauseConsole();
		return;
	}

	nonogram = GetNonogramFromUserTxt(username, difficultyLevel, allowedMistakes, nonogramSize, nonogramSolution, playerMistakes);
}

void DisplayDifficultyMenu(int difficultyLevel) {
	ClearConsole();

	std::cout << DIFFICULTY_LEVEL_PROMPT;
	for (size_t i = 1; i <= difficultyLevel; i++) {
		std::cout << i << " ";
	}
	std::cout << std::endl << "Your choice: ";
}

bool DifficultyChoiseIsValid(const char* userInput, int difficultyLevel) {
	char firstChar = userInput[0];
	return userInput[1] == TERMINATING_ZERO && firstChar > ZERO_CHAR && firstChar <= IntToChar(difficultyLevel);
}

int DifficultyMenuChoice(int difficultyLevel) {
	char userInput[INPUT_MAX_LENGTH]{};

	do {
		DisplayDifficultyMenu(difficultyLevel);
		std::cin.getline(userInput, 100);

	} while (!DifficultyChoiseIsValid(userInput, difficultyLevel));

	return CharToInt(userInput[0]);
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

	int difficultyChoice = DifficultyMenuChoice(difficultyLevel);
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

int** GetTopNumbersArray(int rows, int columns, bool** nonogramSolution, int nonogramSize) {
	int** topNumbersArray = new int* [rows] {};
	for (size_t i = 0; i < rows; i++) {
		topNumbersArray[i] = new int[columns] {};
	}

	int index = 0;
	for (size_t col = 0; col < nonogramSize; col++) {
		index = rows - 1;

		for (int row = nonogramSize - 1; row >= 0; row--) {
			if (nonogramSolution[row][col]) {
				topNumbersArray[index][col]++;
				continue;
			}

			if (topNumbersArray[index][col] > 0) {
				index--;
			}
		}
	}

	return topNumbersArray;
}

int** GetSideNumbersArray(int rows, int columns, bool** nonogramSolution, int nonogramSize) {
	int** sideNumbersArray = new int* [rows] {};
	for (size_t i = 0; i < rows; i++) {
		sideNumbersArray[i] = new int[columns] {};
	}

	int index = 0;
	for (size_t row = 0; row < nonogramSize; row++) {
		index = columns - 1;

		for (int col = nonogramSize - 1; col >= 0; col--) {
			if (nonogramSolution[row][col]) {
				sideNumbersArray[row][index]++;
				continue;
			}

			if (sideNumbersArray[row][index] > 0) {
				index--;
			}
		}
	}

	return sideNumbersArray;
}

char** GetTopNumbersArrayGraphical(int** topNumbersArray, int rows, int columns, int& width, int& height) {
	width = columns * 2 + TERMINATING_ZERO_LENGTH;
	height = rows;

	bool rowIsEmpty = false;
	for (size_t i = 0; i < rows; i++) {
		rowIsEmpty = true;

		for (size_t j = 0; j < columns; j++) {
			if (topNumbersArray[i][j] > 0) {
				rowIsEmpty = false;
				break;
			}
		}

		if (rowIsEmpty) {
			height--;
		} else {
			break;
		}
	}

	int lastEmptySpace = width - 1;
	char** topNumbersArrayGraphical = new char* [height] {};

	for (size_t i = 0; i < height; i++) {
		topNumbersArrayGraphical[i] = new char[width] {};
		for (size_t j = 0; j < lastEmptySpace; j++) {
			topNumbersArrayGraphical[i][j] = ' ';
		}
	}

	int currentNumber = 0,
		rowsAndHeightDifference = rows - height;
	char currentNumberChar = TERMINATING_ZERO;
	for (int i = rows - 1; i >= rowsAndHeightDifference; i--) {
		for (size_t j = 0; j < columns; j++) {
			currentNumber = topNumbersArray[i][j];
			currentNumberChar = ' ';

			if (currentNumber != 0) {
				currentNumberChar = IntToChar(currentNumber % 10);
			}

			topNumbersArrayGraphical[i - rowsAndHeightDifference][j * 2 + 1] = currentNumberChar;
			currentNumber /= 10;

			if (currentNumber == 0) {
				continue;
			}

			topNumbersArrayGraphical[i - rowsAndHeightDifference][j * 2] = IntToChar(currentNumber);
		}
	}

	return topNumbersArrayGraphical;
}

char** GetSideNumbersArrayGraphical(int** sideNumbersArray, int rows, int columns, int& width, int& height) {
	int sideNumbersArrayGreatestNumber = GreatestNumberInMatrix(sideNumbersArray, rows, columns),
		sideNumbersArrayGreatestNumberLength = LengthOf(sideNumbersArrayGreatestNumber),
		longestNumberLengthWithSpace = sideNumbersArrayGreatestNumberLength + 1;

	width = columns * longestNumberLengthWithSpace;
	height = rows;

	bool columnIsEmpty = false;
	for (size_t i = 0; i < columns; i++) {
		columnIsEmpty = true;

		for (size_t j = 0; j < rows; j++) {
			if (sideNumbersArray[j][i] > 0) {
				columnIsEmpty = false;
				break;
			}
		}

		if (columnIsEmpty) {
			width -= longestNumberLengthWithSpace;
		} else {
			break;
		}
	}

	int widthAndColumnsDifference = columns - width / longestNumberLengthWithSpace;
	width += TERMINATING_ZERO_LENGTH;

	int lastEmptySpace = width - 1;
	char** sideNumbersArrayGraphical = new char* [height] {};

	for (size_t i = 0; i < height; i++) {
		sideNumbersArrayGraphical[i] = new char[width] {};
		for (size_t j = 0; j < lastEmptySpace; j++) {
			sideNumbersArrayGraphical[i][j] = ' ';
		}
	}

	int currentNumber = 0;
	char currentNumberChar = TERMINATING_ZERO;

	for (int i = 0; i < height; i++) {
		for (size_t j = columns - 1; j >= widthAndColumnsDifference; j--) {
			currentNumber = sideNumbersArray[i][j];
			currentNumberChar = ' ';

			if (currentNumber != 0) {
				currentNumberChar = IntToChar(currentNumber % 10);
			}

			sideNumbersArrayGraphical[i][(j - widthAndColumnsDifference + 1) * longestNumberLengthWithSpace - 1] = currentNumberChar;
			currentNumber /= 10;

			if (currentNumber == 0) {
				continue;
			}

			sideNumbersArrayGraphical[i][(j - widthAndColumnsDifference + 1) * longestNumberLengthWithSpace - 2] = IntToChar(currentNumber);
		}
	}

	return sideNumbersArrayGraphical;
}

void PutTogetherGraphicalNonogram(char** graphicalNonogram, int graphicalNonogramHeight, int graphicalNonogramWidth, char** topNumbersArrayGraphical, int topNumbersArrayGraphicalHeight, int topNumbersArrayGraphicalWidth, char** sideNumbersArrayGraphical, int sideNumbersArrayGraphicalHeight, int sideNumbersArrayGraphicalWidth) {
	for (size_t i = 0; i < topNumbersArrayGraphicalHeight; i++) {
		for (size_t j = 0; j < topNumbersArrayGraphicalWidth; j++) {
			graphicalNonogram[i][j + sideNumbersArrayGraphicalWidth] = topNumbersArrayGraphical[i][j];
		}
	}

	int offset = 0;
	for (size_t i = 0; i < sideNumbersArrayGraphicalHeight; i++) {
		offset = i + topNumbersArrayGraphicalHeight + 1;
		for (size_t j = 0; j < sideNumbersArrayGraphicalWidth; j++) {
			graphicalNonogram[offset][j] = sideNumbersArrayGraphical[i][j];
		}
	}

	for (size_t i = topNumbersArrayGraphicalHeight; i < graphicalNonogramHeight; i++) {
		graphicalNonogram[i][sideNumbersArrayGraphicalWidth - 1] = '|';
		graphicalNonogram[i][graphicalNonogramWidth - 2] = '|';
	}

	for (size_t i = sideNumbersArrayGraphicalWidth; i < graphicalNonogramWidth - 2; i++) {
		graphicalNonogram[topNumbersArrayGraphicalHeight][i] = '=';
		graphicalNonogram[graphicalNonogramHeight - 1][i] = '=';
	}
}

char** GetGraphicalNonogram(bool** nonogramSolution, int nonogramSize, int& graphicalNonogramHeight, int& offsetX, int& offsetY) {
	int numbersArrayRows = nonogramSize / 2,
		numbersArrayColumns = nonogramSize;
	if (nonogramSize & 1) numbersArrayRows++;

	int** topNumbersArray = GetTopNumbersArray(numbersArrayRows, numbersArrayColumns, nonogramSolution, nonogramSize);
	int** sideNumbersArray = GetSideNumbersArray(numbersArrayColumns, numbersArrayRows, nonogramSolution, nonogramSize);

	int topNumbersArrayGraphicalWidth = 0,
		topNumbersArrayGraphicalHeight = 0,
		sideNumbersArrayGraphicalWidth = 0,
		sideNumbersArrayGraphicalHeight = 0;


	char** topNumbersArrayGraphical = GetTopNumbersArrayGraphical(topNumbersArray, numbersArrayRows, numbersArrayColumns, topNumbersArrayGraphicalWidth, topNumbersArrayGraphicalHeight);
	char** sideNumbersArrayGraphical = GetSideNumbersArrayGraphical(sideNumbersArray, numbersArrayColumns, numbersArrayRows, sideNumbersArrayGraphicalWidth, sideNumbersArrayGraphicalHeight);

	DeleteMatrix(topNumbersArray, numbersArrayRows);
	DeleteMatrix(sideNumbersArray, numbersArrayColumns);

	graphicalNonogramHeight = topNumbersArrayGraphicalHeight + sideNumbersArrayGraphicalHeight + 2;
	int graphicalNonogramWidth = topNumbersArrayGraphicalWidth + sideNumbersArrayGraphicalWidth + TERMINATING_ZERO_LENGTH;

	char** graphicalNonogram = new char* [graphicalNonogramHeight] {};

	int lastEmptySpace = graphicalNonogramWidth - 1;
	for (size_t i = 0; i < graphicalNonogramHeight; i++) {
		graphicalNonogram[i] = new char[graphicalNonogramWidth] {};
		for (size_t j = 0; j < lastEmptySpace; j++) {
			graphicalNonogram[i][j] = ' ';
		}
	}

	offsetX = sideNumbersArrayGraphicalWidth;
	offsetY = topNumbersArrayGraphicalHeight + 1;

	PutTogetherGraphicalNonogram(graphicalNonogram, graphicalNonogramHeight, graphicalNonogramWidth, topNumbersArrayGraphical, topNumbersArrayGraphicalHeight, topNumbersArrayGraphicalWidth, sideNumbersArrayGraphical, sideNumbersArrayGraphicalHeight, sideNumbersArrayGraphicalWidth);

	DeleteMatrix(topNumbersArrayGraphical, topNumbersArrayGraphicalHeight);
	DeleteMatrix(sideNumbersArrayGraphical, sideNumbersArrayGraphicalHeight);

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
	int number = 0,
		multiplier = 1;

	for (size_t i = startIndex + 1; i < endIndex; i++) {
		multiplier *= 10;
	}

	for (int i = startIndex; i < endIndex; i++) {
		number += multiplier * CharToInt(string[i]);
		multiplier /= 10;
	}

	return number;
}

void ExtractDataFromUserInput(const char* userInput, int& x, int& y, char& state) {
	int firstNonSpaceIndex = 0,
		firstSpaceIndex = 0,
		secondSpaceIndex = 0;

	for (size_t i = 0; userInput[i] != TERMINATING_ZERO; i++) {
		if (userInput[i] != ' ') {
			firstNonSpaceIndex = i;
			break;
		}
	}
	for (size_t i = firstNonSpaceIndex + 1; userInput[i] != TERMINATING_ZERO; i++) {
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

	x = ExtractNumberFromString(userInput, firstNonSpaceIndex, firstSpaceIndex);
	y = ExtractNumberFromString(userInput, firstSpaceIndex + 1, secondSpaceIndex);
	state = userInput[secondSpaceIndex + 1];
}

bool UserInputCoordinatesAreValid(int x, int y, int nonogramSize) {
	return x >= 0 && y >= 0 && x < nonogramSize&& y < nonogramSize;
}

void UpdateNonogram(const char* userInput, int** nonogram, bool** nonogramSolution, int nonogramSize, int& playerMistakes) {
	int x = 0,
		y = 0;
	char state = TERMINATING_ZERO;

	ExtractDataFromUserInput(userInput, x, y, state);

	if (!UserInputCoordinatesAreValid(x, y, nonogramSize)) {
		//std::cout << "Wrong input.\n";
		//PauseConsole();
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
			return;
		default:
			//std::cout << "Wrong input.\n";
			//PauseConsole();
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

void UpdateGraphicalNonogram(char** graphicalNonogram, int** nonogram, int nonogramSize, int offsetX, int offsetY) {
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

			graphicalNonogram[offsetY + i][offsetX + 2 * j] = ch1;
			graphicalNonogram[offsetY + i][offsetX + 2 * j + 1] = ch2;
		}
	}
}

void PlayNonogram(int& difficultyLevel, int allowedMistakes, int nonogramSize, bool** nonogramSolution, int& playerMistakes, int** nonogram) {
	if (nonogram == nullptr) {
		return;
	}

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

	int graphicalNonogramHeight = 0,
		offsetX = 0,
		offsetY = 0;
	char** graphicalNonogram = GetGraphicalNonogram(nonogramSolution, nonogramSize, graphicalNonogramHeight, offsetX, offsetY);
	UpdateGraphicalNonogram(graphicalNonogram, nonogram, nonogramSize, offsetX, offsetY);

	char userInput[INPUT_MAX_LENGTH]{};

	while (!UserInputIsExit(userInput)) {
		if (playerMistakes > allowedMistakes) {
			std::cout << "You have made too many mistakes, you can't continue playing this nonogram.\n";
			PauseConsole();
			return;
		}

		DisplayNonogram(graphicalNonogram, graphicalNonogramHeight);
		if (difficultyLevel == FIRST_LEVEL) {
			std::cout << "Enter input in this format: <row> <column> <state(e/f)> (Example: 0 2 e)\n";
			std::cout << "Or enter \"exit\" to go back to main menu.\n";
		}
		std::cin.getline(userInput, INPUT_MAX_LENGTH);

		UpdateNonogram(userInput, nonogram, nonogramSolution, nonogramSize, playerMistakes);
		UpdateGraphicalNonogram(graphicalNonogram, nonogram, nonogramSize, offsetX, offsetY);

		if (NonogramsMatch(nonogram, nonogramSolution, nonogramSize)) {
			difficultyLevel++;
			DisplayNonogram(graphicalNonogram, graphicalNonogramHeight);
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