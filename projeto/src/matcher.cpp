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
	string wordtmp;
	vector<string> textWords;
	istringstream searchBuffer(text);
	while(searchBuffer >> wordtmp){
		textWords.push_back(wordtmp);
	}

	vector<string> wordWords;
	istringstream searchBuffer2(word);
	while(searchBuffer2 >> wordtmp){
		wordWords.push_back(wordtmp);
	}

	float  minAvg = 0, min = 400000.0, minT = 400000.0,  dist;
	for(unsigned int i = 0; i < textWords.size(); i++){
		for(unsigned int j = 0; j < wordWords.size(); j++){
			dist = distApproximateStringMatching(textWords[i], wordWords[j]);
			if(dist < min)
				min = dist;
			if(dist < minT)
				minT = dist;
		}
		minAvg += min;
		min = 400000.0;
	}
	minAvg /= textWords.size();
	return minAvg+minT;
}

int distApproximateStringMatching(const string text, const string word){
	int n=text.length();
	vector<int> d(n+1);
	int old,neww;
	for (int j=0; j<=n; j++)
		d[j]=j;
	int m=word.length();
	for (int i=1; i<=m; i++) {
		old = d[0];
		d[0]=i;
		for (int j=1; j<=n; j++) {
			if (tolower(word[i-1])==tolower(text[j-1])) neww = old;
			else {
				neww = min(old,d[j]);
				neww = min(neww,d[j-1]);
				neww++;
			}
			old = d[j];
			d[j] = neww;
		}
	}
	return d[n];
}
