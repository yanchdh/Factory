//#include "StrIllegal.h"
#include "ForbiddenWords.h"
#include <iostream>
#include <assert.h>

using namespace std;

int main()
{
	ForbiddenWords forbidderWords;
	forbidderWords.Init("forbidden_words.txt");

	fstream fin("in.txt");
	if (fin)
	{
		string str;
		while (getline(fin, str))
		{
			for (int i = 0; i < str.size(); ++i) {
				cout << (int)((unsigned char)(str[i])) << " ";
			}
			cout << "\t";
			if (forbidderWords.HasPunctuation(str))
			{
				cout << str << " is illegal 1" << endl;
			}
			else if (!forbidderWords.IsStrLegal(str))
			{
				cout << str << " is illegal 2" << endl;
			}
			else
			{
				assert(0);
				cout << str << " is legal" << endl;
			}
		}
	}
	fin.close();
	return 0;
}