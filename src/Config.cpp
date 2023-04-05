
#include "../incl/Config.hpp"

#include <fstream.h>

// Constructors and destructors
Config::Config(void)
{}

Config::~Config(void)
{}

Config::Config(const Config& src)
{
	*this = src;
}

// Operator overloads
Config& Config::operator=(const Config& src)
{}

void Config::loadConfigFile(std::string path);
{
	std::ifstream	infile;
	std::string		line;

	
}
