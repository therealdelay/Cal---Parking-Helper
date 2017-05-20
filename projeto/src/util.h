#include <string>
#include <iostream>

using namespace std;

/**
 * This function removes the excess spaces in a string. If there is more that one space between to words, it is removed.
 */
string removeSpaces(string s);
/**
 * This function compares names and checks if they are the same, not being case sensitive.
 */
int compareNames(string a, string b);

/**
 * This function receives the user input and verifies if is valid and if it is between a certain limit of values.
 *
 * @param a minimum value of input
 * @param b maximum value of input
 *
 * @return -1 if invalid input, user input if valid
 */
int getOption(int a, int b);
/**
 * THis function clears the screen of the console.
 */
void clearBuffer();
/**
 * This function prints "Invalid Option" in the screen.
 */
void invalidOption();
