#pragma once

#include <vector>
#include <string>

using namespace std;

class NativeWheel
{
public:
	NativeWheel(string);
	~NativeWheel();
	string Name;
	std::vector<string> stationList;
};

