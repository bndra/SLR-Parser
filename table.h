#pragma once
#include <vector>
#include <iostream>
#include "table_node.h"
#include "cfg.h"

class table
{
private:
	table_node* head;

public:
	table();
	table(const table_node &tn);
	table(const table &t);
	~table();
	void add_node(table_node n);
	table_node* get_head() const;
	void fill_table(int nonterm_count, cfg &g);
	void print();
	void print(cfg_reader &r);
	bool already_exists(table_node &n);
};