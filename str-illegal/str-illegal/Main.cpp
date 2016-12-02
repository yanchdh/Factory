#include "StrIllegal.h"
#include "ForbiddenWords.h"
#include <iostream>
#include <assert.h>

using namespace std;

int main()
{
	StrIllegal strIllegal;
	strIllegal.Init("forbidden_words.txt");

	ForbiddenWords forbidderWords;
	forbidderWords.Init("forbidden_words.txt");

	fstream fin("in.txt");
	if (fin)
	{
		string str;
		while (getline(fin, str))
		{
			if (strIllegal.HasPunctuation(str))
			{
				cout << str << " is illegal 1" << endl;
			}
			else if (!strIllegal.IsStrLegal(str))
			{
				cout << str << " is illegal 2" << endl;
			}
			else
			{
				cout << str << " is legal" << endl;
				assert(0);
			}

			if (!strncmp(str.c_str(), "xinshou", strlen("xinshou")))
				return false;

			//if (forbidderWords.HasPunctuation(str))
			//{
			//	//cout << str << " is illegal 1" << endl;
			//}
			//else if (!forbidderWords.IsStrLegal(str))
			//{
			//	//cout << str << " is illegal 2" << endl;
			//}
			//else
			//{
			//	assert(0);
			//	cout << str << " is legal" << endl;
			//}

		}
	}
	fin.close();
	return 0;
}