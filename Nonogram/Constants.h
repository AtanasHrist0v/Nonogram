#pragma once

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
const char NO_SAVED_GAME_MESSAGE[] = "No saved game found. Please start a new game.\n";
const char USERS_PARENT_FOLDER[] = "users/";
const char LEVELS_PARENT_FOLDER[] = "levels/";
const char TEXT_FILE_EXTENTION[] = ".txt";