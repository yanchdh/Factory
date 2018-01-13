#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>

std::string funcname;
std::vector<std::string> funcheaders;
std::vector<std::string> funcparams;

inline void writecpp(const std::string& filename)
{
	funcheaders.push_back("iostream");
	funcheaders.push_back("stdlib.h");
	int headersize = funcheaders.size();
	int paramsize = funcparams.size();

	// write cpp
	std::string sp = "";
	std::ofstream wt((filename + ".cpp").c_str());
	for (int i = 0; i < headersize; ++i)
	{
		wt << "#include <" << funcheaders[i] << ">" << std::endl;
	}
	wt << std::endl;

	wt << sp << "int main(int argc, char* argv[])" << std::endl;
	wt << sp << "{" << std::endl;
	sp.push_back('\t');

	wt << sp << "if(argc != " << paramsize + 1 << ")" << std::endl;
	wt << sp << "{" << std::endl;
	sp.push_back('\t');
	wt << sp << "std::cout << \"params num error\" << std::endl;" << std::endl;
	wt << sp << "exit(1);" << std::endl;
	sp.erase(--sp.end());
	wt << sp << "}" << std::endl;

	wt << sp << "std::cout << " << funcname << "(";
	for (int i = 0; i < paramsize; ++i)
	{
		if (i != 0)
			wt << ", ";
		if (funcparams[i] == "int")
			wt << "atoll(argv[" << i + 1 << "])";
		else if (funcparams[i] == "str")
			wt << "argv[" << i + 1 << "]";
	}
	wt << ") << std::endl;" << std::endl;

	wt << sp << "return 0;" << std::endl;
	sp.erase(--sp.end());
	wt << sp << "}";
	wt.close();
}

inline void help()
{
	printf("*************************************************\n");
	printf("* ./yyshell -f strcmp -i string.h -p str -p str *\n");
	printf("* ./yy_strcmp str1 str2                         *\n");
	printf("* -f function name                              *\n");
	printf("* -i function inclue headers file               *\n");
	printf("* -p function params type[str, int]             *\n");
	printf("*************************************************\n");
}

int main(int argc, char* argv[])
{
	int opt;
	while ((opt = getopt(argc, argv, "f:i:p:?")) != -1)
	{
		opt = tolower(opt);
		switch (opt)
		{
		case 'i':
			funcheaders.push_back(optarg);
			break;
		case 'f':
			funcname = optarg;
			break;
		case 'p':
			funcparams.push_back(optarg);
			break;
        case '?':
			help();
			exit(1);
		}
	}
	//check funcname
	if (funcname.empty())
	{
		help();
		exit(1);
	}
	//set filename
	std::string filename = "yy_" + funcname;
	//write cpp file
	writecpp(filename);
	//compile use g++
	system(("g++ " + filename + ".cpp -o " + filename).c_str());
    //rm cpp file
	system(("rm " + filename + ".cpp -f").c_str());
	return 0;
}