#include "table.h"
#include "cfg_reader.h"

class parser
{
private:
	std::vector<table*> tables;
	std::vector<int> s_switch;
	std::vector<int> s_symbol;
	std::vector<int> s_next;
	std::vector<int> reduce;
	std::vector<int> g_switch;
	std::vector<int> g_symbol;
	std::vector<int> g_next;

public:
	parser();
	void fill_tables(cfg_reader &grammar_reader);
	void print();
	void print(cfg_reader &r);
	bool already_exists(table &t);
	int index_of(table &t);

	void shift_entry(int table, int symbol, int next);
	void reduce_entry(int table, int rule_num);
	void goto_entry(int table,int symbol, int next);
	void print_lookups();

	bool parse_action(std::ifstream &tokens, cfg_reader &r);
	int shift(int table, int symbol);
	int move(int table, int symbol);
};