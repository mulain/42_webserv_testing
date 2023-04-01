
#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <map>
#include <vector>

typedef std::map<std::string, std::string>::const_iterator StringMap_it;
typedef std::map<std::string, std::string> StringMap;

class Utils
{
	public:
		static std::string		splitEraseStr(std::string&, std::string);
		static StringMap		splitEraseStrMap(std::string&, std::string, std::string, char);

		
	
	private:
		Utils(void);
		~Utils(void);
};

#endif
