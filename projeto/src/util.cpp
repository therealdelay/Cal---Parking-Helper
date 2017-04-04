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
