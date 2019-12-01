#include "cfg_reader.h"

cfg_reader::cfg_reader(){};
cfg_reader::cfg_reader(std::string dir)
{
	file_name = dir;
	file.open(file_name, std::ifstream::in);
	index_symbols();
	translate();
}

bool cfg_reader::open(std::string dir)
{
	file.close();
	file_name = dir;
	file.open(file_name, std::ifstream::in);
	return (file.is_open());
	index_symbols();
}

void cfg_reader::print_symbols()
{
	int count=1;
	std::cout << "Nonterminals:" << std::endl;
	for (auto it = symbols.begin(); it != symbols.end(); ++it)
	{
		std::cout << count++ << ": " << *it << std::endl;
		if (count == nonterminals+1)
			std::cout << "Terminals:" << std::endl;
	}
}

void cfg_reader::index_symbols()
{
	open(file_name);
	nonterminals = 0;
	while (!file.eof())
	{
		std::getline(file, current_line);
		std::string first_token = current_line.substr(0, current_line.find(" "));
		if (!contains(first_token) && first_token.substr(0, 1) != "\n" && !first_token.empty())
		{
			symbols.push_back(first_token);
			nonterminals++;
		}
	}
	open(file_name);
	while (!file.eof())
	{
		std::string token = "";
		file >> token;
		if (!contains(token))
			symbols.push_back(token);
	}
}

bool cfg_reader::contains(std::string s)
{
	for (auto it = symbols.begin(); it != symbols.end(); ++it)
	{
		if (*it == s)
			return true;
	}
	return false;
}

int cfg_reader::get_enum(std::string s)
{
	int count = 1;
	for (auto it = symbols.begin(); it != symbols.end(); ++it)
	{
		if (*it == s)
			return count;
		count++;
	}
	return -1;
}

void cfg_reader::translate()
{
	open(file_name);
	std::stringstream line;
	std::string token;
	while (!file.eof())
	{
		std::vector<int> rule;
		std::getline(file, current_line);
		line.flush();
		line.clear();
		line << current_line;
		/*line >> token;
		rule.push_back(get_enum(token));
		rule.push_back(0);*/
		while (!line.eof())
		{
			line >> token;
			int e = get_enum(token);
			if (token != "" && e!=-1)
				rule.push_back(e);
			token = "";
		}
		production_rules.add_rule(rule);
	}
}

cfg cfg_reader::get_cfg()
{
	return production_rules;
}

int cfg_reader::nonterm_count()
{
	return nonterminals;
}

std::string cfg_reader::get_symbol(int n)
{
	return symbols.at(n);
}