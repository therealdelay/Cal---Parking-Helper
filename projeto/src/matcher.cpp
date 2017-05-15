#include "matcher.h"

int kmpMatcher(string text, string word, vector<int> pi){
	int num=0;
	int m=word.length();
	int n=text.length();

	int q=-1;
	for (int i=0; i<n; i++) {
		while (q>-1 && tolower(word[q+1])!=tolower(text[i]))
			q=pi[q];
		if (tolower(word[q+1])==tolower(text[i]))
			q++;
		if (q==m-1) {
			//cout <<"pattern occurs with shift" << i-m+1 << endl;
			num++;
			q=pi[q];
		}
	}
	return num;
}

vector<int> calcPi(string word){
	int m=word.length();
	vector<int> prefix(m);
	prefix[0]=-1;
	int k=-1;
	for (int q=1; q<m; q++) {
		while (k>-1 && tolower(word[k+1])!=tolower(word[q]))
			k = prefix[k];
		if (tolower(word[k+1])==tolower(word[q])) k=k+1;
		prefix[q]=k;
	}
	return prefix;
}

float avgApproximateStringMatching (const string text, const string word){
	//cout << text << " - " << word << endl;
	float avg = 0;
	string wordtmp;
	vector<string> words;
	istringstream searchBuffer(text);
	while(searchBuffer >> wordtmp){
		//cout << wordtmp << endl;
		words.push_back(wordtmp);
	}
	//cout << endl;

	unsigned int wordNo = words.size();
	for(unsigned int i = 0; i < wordNo; i++){
		avg += distApproximateStringMatching(words[i], word);
	}

	return avg/wordNo;
}

int distApproximateStringMatching(const string text, const string word){
	unsigned int m = word.size(), n = text.size();
	m++;
	n++;
	unsigned int dist[m][n];
	for(unsigned int i = 0; i < m; i++)
		dist[i][0] = i;
	for(unsigned int j = 0; j < n; j++)
		dist[0][j] = j;

	for(unsigned int i = 1; i < m; i++){
		for(unsigned int j = 1; j < n; j++){
			if(tolower(word[i-1]) == tolower(text[j-1]))
				dist[i][j] = dist[i-1][j-1];
			else
				dist[i][j] = 1+minVal(dist[i-1][j], dist[i-1][j-1], dist[i][j-1]);
			//cout << dist[i][j] << " ";
		}
		//cout << endl;
	}
	return dist[m-1][n-1];
}

int minVal(const int val1, const int val2, const int val3){
	int min;
	if(val1 < val2)
		min = val1;
	else
		min = val2;

	if(val3 < min)
		min = val3;

	return min;
}
