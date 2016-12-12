#include <iostream>
#include <string>
#include "jsonparse.h"
#include "json2str.h"

using namespace std;

int main(int args, char* argv[])
{
	//test json2str
	json2str j2s(Json::objectValue);
	j2s.add("key1", 1);
	j2s.add("key2", 2);
	json2str arr(Json::arrayValue);
	arr.append(3);
	arr.append(4);
	j2s.add("key3", arr.get_value());
	cout << j2s.to_string();

	//test jsonparse
	jsonparse jp(j2s.to_string());
	if (!jp.success())
	{
		fprintf(stderr, "jsonparse parse error[%s]\n", j2s.to_string());
		exit(1);
	}

	int ret = 0;
	if (!jp.get_value(&ret, "key1"))
	{
		fprintf(stderr, "jsonparse get_value error\n");
		exit(1);
	}
	cout << ret << endl;

	if (!jp.get_value(&ret, "key3", "0"))
	{
		fprintf(stderr, "jsonparse get_value error\n");
		exit(1);
	}
	cout << ret << endl;

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}