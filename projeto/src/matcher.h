#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctype.h>

using namespace std;

//int numStringMatching (string filename, string toSearch);

int kmpMatcher(string text, string word, vector<int> pi);

vector<int> calcPi(string word);

//float numApproximateStringMatching(string filename, string toSearch);

float avgApproximateStringMatching (const string text, const string word);

int distApproximateStringMatching(const string text, const string word);

int minVal(const int val1, const int val2, const int val3);
