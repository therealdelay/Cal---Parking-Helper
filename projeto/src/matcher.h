#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctype.h>

using namespace std;

int kmpMatcher(string text, string word, vector<int> pi);

vector<int> calcPi(string word);

float avgApproximateStringMatching (const string text, const string word);

int distApproximateStringMatching(const string text, const string word);
