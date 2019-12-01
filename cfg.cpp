#include "cfg.h"

cfg::cfg(){};
void cfg::add_rule(std::vector<int> r)
{
	rules.push_back(r);
}

std::vector<int> cfg::find_rules(int left)
{
	int index = 0;
	std::vector<int> query_index;
	for (auto it = rules.begin(); it != rules.end(); ++it)
	{
		if (*((*it).begin()) == left)
		{
			query_index.push_back(index);
		}
		index++;
	}
	return query_index;
}

std::vector<int> cfg::rule_number(int n)
{
	return rules.at(n);
}

int cfg::rule_number_of(std::vector<int> &r)
{
	int count = 1;
	for (auto it = rules.begin(); it != rules.end(); ++it)
	{
		if (r == *it)
			return count;
		count++;
	}
	return 0;
}

int cfg::get_size()
{
	return rules.size();
}

void cfg::print()
{
	int rule_num = 1;
	for (auto it = rules.begin(); it != rules.end(); ++it)
	{
		std::cout << rule_num << ":\t";
		for (auto jt = (*it).begin(); jt != (*it).end(); ++jt)
			std::cout << *jt << " ";
		std::cout << std::endl;
		rule_num++;
	}
}