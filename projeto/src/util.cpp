#include "util.h"

string removeSpaces(string s) {
	string stemp, sf;
	unsigned int i = 0, ii, ipeek;
	if (s == "")
		return s;

	while (i != string::npos)
	{
		ii = s.find_first_not_of(' ', i);
		i = s.find_first_of(' ', ii);
		ipeek = s.find_first_not_of(' ', i);
		if (ipeek == string::npos)
		{
			stemp = s.substr(ii, (i - ii));
			sf.append(stemp);
			i = string::npos;
		}
		else
		{
			stemp = s.substr(ii, (i - ii));
			sf.append(stemp);
			sf.append(" ");
		}
	}

	return sf;
}

int compareNames(string a, string b){
	if(a.size() != b.size())
		return 1;
	for(int i = 0; i < a.size(); i++){
		a[i] = toupper(a[i]);
		b[i] = toupper(b[i]);
		if(a[i] != b[i])
			return 1;
	}
	return 0;
}
