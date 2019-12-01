#pragma once
#include <vector>
#include <iostream>

class cfg
{
private:
	std::vector<std::vector<int>> rules;

public:
	cfg();
	void add_rule(std::vector<int> r);
	std::vector<int> find_rules(int left);	//find production rules by left hand side non-terminal, return query_index
	std::vector<int> rule_number(int n);	//return rule at index n
	int rule_number_of(std::vector<int> &r);//return index of r
	int get_size();
	void print();

};
