
#include "../incl/Utils.hpp"

// Constructors and destructors
Utils::Utils(void)
{}

Utils::~Utils(void)
{}

// Public member functions
std::string Utils::splitEraseStr(std::string& input, std::string target)
{
	std::string element;
	size_t len;

	element = input.substr(0, len = input.find(target));
	input.erase(0, len + target.length());
	return element;
}

StringMap Utils::splitEraseStrMap(std::string& input, std::string endOfKey, std::string endOfValue, char endOfMap)
{
	StringMap stringMap;
	std::string key, value;

	while (!input.empty() && input[0] != endOfMap)
	{
		key = splitEraseStr(input, endOfKey);
		value = splitEraseStr(input, endOfValue);
		stringMap.insert(std::make_pair(key, value));
	}
	return stringMap;
}
