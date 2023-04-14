
#include "../incl/Config.hpp"

// Constructors and destructors
Config::Config(void)
{}

Config::~Config(void)
{}

std::string trim(std::string& input) 
{
	size_t start = input.find_first_not_of(" \t\r\n");
	size_t end = input.find_last_not_of(" \t\r\n");
	if (start == std::string::npos)
		input = "";
	else
		input = input.substr(start, end - start + 1);
	return input;
}

void setField(Server& newServer, std::string key, std::string value)
{
	if (key == SERVERNAME)
		newServer._name = value;
		
}

void Config::loadConfigFile(std::string path)
{
	std::ifstream	infile(path.c_str());
	std::string		line;
	std::string		key;
	std::string		value;
	std::vector<Server>	serverList;

	if (!infile.is_open())
		throw std::range_error("Could not open config file.");		
	while (std::getline(infile, line))
	{
		if (trim(line) == "<server>")
		{
			Server newServer;
			
			while (std::getline(infile, line) && trim(line) != "</server>")
			{
				key = Utils::splitEraseStr(line, ":");
				value = trim(line);
				setField(newServer, key, value);
				std::cout << "key: " << key << "\tvalue: " << value << std::endl;
			}
			serverList.push_back(newServer);
		}
	}
	
	
		/* size_t delim = line.find(',');
		if (delim == std::string::npos)
			continue;
		if (!date.parseDate(line.substr(0, delim)))
			continue;
		if (!parsePrice(line.substr(delim + 1, line.size()), &price))
			continue;
		_btc_price.insert(std::make_pair(date, price)); */
	infile.close();
}


