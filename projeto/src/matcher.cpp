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
	vector<string> textWords;
	istringstream searchBuffer(text);
	while(searchBuffer >> wordtmp){
		//cout << wordtmp << endl;
		textWords.push_back(wordtmp);
	}

	vector<string> wordWords;
	istringstream searchBuffer2(word);
	while(searchBuffer2 >> wordtmp){
		//cout << wordtmp << endl;
		wordWords.push_back(wordtmp);
	}

	//unsigned int wordNo = wordWords.size();//+textWords.size();
	unsigned int wordNo = textWords.size();
	float min = 400000.0, dist;
	for(unsigned int i = 0; i < textWords.size(); i++){
		/*
		for(unsigned int j = 0; j < wordWords.size(); j++){
			//cout << textWords[i].size() << " - " << text.size() << endl;
			//float fraction = (float) textWords[i].size()/(float) text.size();
			//cout << fraction << endl;
			avg += distApproximateStringMatching(wordWords[j], textWords[i]);
		}
		*/
		//float fraction = (float) text.size()/(float) textWords[i].size();
		//avg += distApproximateStringMatching(textWords[i], word);
		dist = distApproximateStringMatching(textWords[i], word);
		if(dist < min)
			min = dist;
	}
	//return avg/wordNo;
	return min;
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
				/*
				if(neww == old)
					neww += 1;
				else if(neww == d[j])
					neww += 2;
				else
					neww += 2;
				*/
				neww++;
			}
			old = d[j];
			d[j] = neww;
		}
	}
	return d[n];
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
