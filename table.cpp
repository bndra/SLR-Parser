#include "table.h"

table::table()
{
	head = NULL;
}

table::table(const table_node &tn)
{
	head = new table_node(tn);
}

table::table(const table &t)
{
	head = t.get_head();
}

table::~table()
{
	table_node* current = head;
	table_node* dummy = current;
	while (current != NULL)
	{
		dummy = current->getNextTableNode();
		delete current;
		current = dummy;
	}
}

table_node* table::get_head() const
{
	return head;
}


void table::fill_table(int nonterm_count, cfg &g)
{
	table_node* current = head;
	while (current != NULL)
	{
		int symbol = current->digitAfterZero();
		if (symbol <= nonterm_count && symbol!=0)//nonterminal symbol
		{
			std::vector<int> r = g.find_rules(symbol);
			for (auto it = r.begin(); it != r.end(); ++it)
			{
				table_node n(g.rule_number(*it));
				n.setNextTableNode(NULL);
				if (!already_exists(n))
				{
					add_node(n);
				}
			}
		}
		current = current->getNextTableNode();
	}
}

void table::add_node(table_node n)//checks for duplicate entry, then adds node
{
	/*if (head == NULL)
	{
		head = (new table_node(n));
		return;
	}*/
	table_node* c = head;
	while (c->getNextTableNode()!=NULL)
	{
		c = c->getNextTableNode();
	}
	c->setNextTableNode(new table_node(n));
}

void table::print()
{
	table_node* current = head;
	while (current != NULL)
	{
		current->print();
		std::cout << "->\n";
		current = current->getNextTableNode();
	}
	std::cout << "NULL" << std::endl;
}

void table::print(cfg_reader &r)
{
	table_node* current = head;
	while (current != NULL)
	{
		current->print(r);
		std::cout << "\n";
		current = current->getNextTableNode();
	}
	std::cout << std::endl;
}

bool table::already_exists(table_node &n)
{
	table_node* c = head;
	do {
		if ((*c) == n)
			return true;
		c = c->getNextTableNode();
	} while (c != NULL);
	return false;
}

