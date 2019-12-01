#include <iostream>
#include <vector>
#include "cfg_reader.h"

class table_node
{
private:
	std::vector<int> processedChars;
	table_node *nextTableNode;
public:
	table_node(std::vector<int> newProcessedChar);
	table_node(const table_node& n);
	table_node* getNextTableNode() const;
	std::vector<int> getProcessedChars() const;
	std::vector <int> get_rule();//returns the rule without 0
	void setNextTableNode(table_node *newNextTableNode);
	int digitAfterZero();
	void shiftZero();
	void insert_zero();
	void print();
	void print(cfg_reader &r);
	friend const bool operator==(const table_node &t, const table_node &n)
	{
		if (t.getProcessedChars() == n.getProcessedChars())
			return true;
		else
			return false;
	};
};