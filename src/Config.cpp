
#include "../incl/Config.hpp"

// Constructors and destructors
Config::Config(void)
{}

Config::~Config(void)
{}

std::string trim(const std::string& input)
{
	size_t start = input.find_first_not_of(" \t\r\n");
	size_t end = input.find_last_not_of(" \t\r\n");
	if (start == std::string::npos || end == std::string::npos)
		return "";
	return input.substr(start, end - start + 1);
}

void Config::loadConfigFile(std::string path)
{
	std::ifstream	infile(path.c_str());
	std::string		line;

	if (!infile.is_open())
		throw std::range_error("Could not open config file.");		
	while (std::getline(infile, line))
	{
		line = trim(line);
		if (line != "<server>")
			continue;
		std::cout << "kundel" << std::endl;
		/* size_t delim = line.find(',');
		if (delim == std::string::npos)
			continue;
		if (!date.parseDate(line.substr(0, delim)))
			continue;
		if (!parsePrice(line.substr(delim + 1, line.size()), &price))
			continue;
		_btc_price.insert(std::make_pair(date, price)); */
	}
	infile.close();
}


