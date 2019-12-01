#include "table_node.h"

table_node::table_node(std::vector<int> newProcessedChar)
{
	processedChars = newProcessedChar;
	nextTableNode = NULL;
	insert_zero();
}

table_node::table_node(const table_node& n)
{
	processedChars = n.getProcessedChars();
	nextTableNode = n.getNextTableNode();
	insert_zero();
}

//getters
table_node* table_node::getNextTableNode() const
{
	return nextTableNode;
}

std::vector<int> table_node::getProcessedChars() const
{
	return processedChars;
}

std::vector<int> table_node::get_rule()
{
	std::vector<int> dummy;
	for (auto it = processedChars.begin(); it != processedChars.end(); ++it)
	{
		if (*it != 0)
			dummy.push_back(*it);
	}
	return dummy;
}

//setters
void table_node::setNextTableNode(table_node *newNextTableNode)
{
	nextTableNode = newNextTableNode;
}

int table_node::digitAfterZero()
{
	for (auto i = processedChars.begin(); i != processedChars.end(); ++i)
	{
		if (*i == 0)
		{
			if (i + 1 != processedChars.end())
				return *(i + 1);
			else
				return 0;
		}
	}
	return -1;
}

void table_node::shiftZero()
{
	for (auto i = processedChars.begin(); i != processedChars.end(); ++i)
	{
		if (*i == 0)
		{
			if (i + 1 != processedChars.end()) {
				iter_swap(i, i + 1);
				return;
			}
		}
	}
}

void table_node::insert_zero()
{
	for (auto it = processedChars.begin(); it != processedChars.end(); ++it)
	{
		if (*it == 0)
			return;
	}
	auto it = processedChars.begin() + 1;
	processedChars.insert(it, 0);
}

void table_node::print()
{

	for (auto it = processedChars.begin(); it != processedChars.end(); ++it)
	{
		std::cout << *it << ".";
	}
}

void table_node::print(cfg_reader &r)
{
	int count = 0;
	for (auto it = processedChars.begin(); it != processedChars.end(); ++it)
	{
		
		if (*it == 0)
			std::cout << "@ ";
		else 
			std::cout << r.get_symbol(*it -1)<<" ";
		if (count == 0)
		{
			std::cout << "-> ";
			count++;
		}
	}
}
