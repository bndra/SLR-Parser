#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "cfg.h"

class cfg_reader
{
private:
	std::string current_line;
	std::ifstream file;
	std::string file_name;
	std::vector<std::string> symbols;	//stores all the nonterminals then the terminals
	int nonterminals;					//to know where the terminals start in the symbol vector
	cfg production_rules;

public:
	cfg_reader();
	cfg_reader(std::string dir);
	bool open(std::string dir);
	void print_symbols();
	void index_symbols();
	bool contains(std::string s);
	int get_enum(std::string s);
	void translate();//the important function. creates production_rules
	cfg get_cfg();
	int nonterm_count();
	std::string get_symbol(int n);
};