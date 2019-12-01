#include "parser.h"

parser::parser()
{
	s_symbol.push_back(0);
	s_next.push_back(0);
	g_symbol.push_back(0);
	g_next.push_back(0);
};

void parser::fill_tables(cfg_reader &grammar_reader)
{
	if (tables.empty())
	{
		tables.push_back(new table(*new table_node(grammar_reader.get_cfg().rule_number(0))));
		tables[0]->fill_table(grammar_reader.nonterm_count(), grammar_reader.get_cfg());
	}
	table_node* node_iter;
	int index = 0;
	for (std::vector<table*>::iterator table_iter = tables.begin(); table_iter != tables.end(); ++table_iter)
	{
		bool shift_performed = false;
		bool goto_performed = false;
		node_iter = (*table_iter)->get_head();
		std::vector<table_node> checklist;
		while (node_iter != NULL)
		{
			checklist.push_back(*node_iter);
			node_iter = node_iter->getNextTableNode();
		}
		reduce.push_back(0);
		node_iter = (*table_iter)->get_head();
		int count = 0;
		while (node_iter != NULL)
		{
			table_node dummy(*node_iter);
			dummy.setNextTableNode(NULL);
			table_node* d_ptr = &dummy;
			int symbol = node_iter->digitAfterZero();
			if (symbol <= 0)
			{
				reduce_entry(index, grammar_reader.get_cfg().rule_number_of(dummy.get_rule()));
			}
			else
			{
				if (symbol > grammar_reader.nonterm_count())
				{
					shift_entry(index, symbol, tables.size());
					shift_performed = true;
				}
				else
				{
					goto_entry(index, symbol, tables.size());
					goto_performed = true;
				}

				auto it = checklist.begin();
				while (it != checklist.end())
				{
					if (it->digitAfterZero() == symbol && !(*it == *node_iter))
					{
						table_node* tn = new table_node(*it);
						tn->shiftZero();
						d_ptr->setNextTableNode(tn);
						d_ptr = d_ptr->getNextTableNode();
						d_ptr->setNextTableNode(NULL);
						it = checklist.erase(it);
					}
					else
					{
						++it;
					}
				}

				bool push_ok = false;
				for (it = checklist.begin(); it != checklist.end(); ++it)
				{
					if (*it == *node_iter && it->digitAfterZero() != 0)
					{
						push_ok = true;
						it = checklist.erase(it);
						break;
					}
				}
				if (push_ok)
				{
					dummy.shiftZero();
					table* n = new table(dummy);
					if (already_exists(*n))
					{
						if (symbol > grammar_reader.nonterm_count())
						{
							shift_entry(index, symbol, index_of(*n));
							shift_performed = true;
						}
						else
						{
							goto_entry(index, symbol, index_of(*n));
							goto_performed = true;
						}
					}
					else
					{
						//std::cout << "n: ";
						//n->print();
						n->fill_table(grammar_reader.nonterm_count(), grammar_reader.get_cfg());
						//std::cout << "n filled: ";
						//n->print();
						tables.push_back(n);
						//shift entry (symbol, index of last table)
					}
				}
			}
			table_iter = tables.begin() + index;
			node_iter = (*table_iter)->get_head();
			for (int i = 0; i < count; i++)
				node_iter = node_iter->getNextTableNode();
			node_iter = node_iter->getNextTableNode();


			count++;
			//print();
		}
		if (shift_performed)
		{
			if (*(s_symbol.end() - 1) != 0)
			{
				s_symbol.push_back(0);
				s_next.push_back(0);

			}
		}
		if (goto_performed)
		{
			if (*(g_symbol.end() - 1) != 0)
			{
				g_symbol.push_back(0);
				g_next.push_back(0);
			}
		}
		index++;
	}
	s_switch.resize(tables.size());
	g_switch.resize(tables.size());
}

void parser::print()
{
	int table_num = 0;
	for (auto it = tables.begin(); it != tables.end(); ++it)
	{
		std::cout << "Table A" << table_num << ":" << std::endl;
		std::cout << "-----------------------------" << std::endl;
		(*it)->print();
		std::cout << std::endl;
		table_num++;
	}
}

void parser::print(cfg_reader &r)
{
	int table_num = 0;
	for (auto it = tables.begin(); it != tables.end(); ++it)
	{
		std::cout << "Table A" << table_num << ":" << std::endl;
		std::cout << "-----------------------------" << std::endl;
		(*it)->print(r);
		table_num++;
	}
}

bool parser::already_exists(table &t)
{
	for (auto it = tables.begin(); it != tables.end(); ++it)
	{
		/*((*it)->get_head())->print();
		std::cout << " != ";
		(t.get_head())->print();
		std::cout << std::endl;*/
		if ((*(*it)->get_head()) == (*(t.get_head())))
		{
			return true;
		}
	}
	//std::cout << std::endl;
	return false;
}

int parser::index_of(table &t)
{
	int i = 0;
	for (auto it = tables.begin(); it != tables.end(); ++it)
	{
		if (*((*it)->get_head()) == *(t.get_head()))
			return i;
		i++;
	}
	return -1;
}

void parser::shift_entry(int table, int symbol, int next)
{
	if (table >= s_switch.size() || s_switch[table]==0)
	{
		s_switch.resize(table + 1, 0);	
		s_switch[table] = s_symbol.size() + 1;
	}
	int count = 0;
	bool added = false;
	for (auto it = s_symbol.rbegin(); it != s_symbol.rend(); ++it)
	{
		if (*it == 0)
			break;
		if (*it == symbol)
		{
			if (*(s_next.rbegin() + count) > next)
				*(s_next.rbegin() + count) = next;
			added = true;
			break;
		}
		count++;
	}
	if (!added)
	{
		s_symbol.push_back(symbol);
		s_next.push_back(next);
	}
}

void parser::reduce_entry(int table, int rule_num)
{
	if (reduce[table] != 0)
		std::cout << "R/R conflict. Table: " << table << " Rules: "
		<< reduce[table] << ", " << rule_num << std::endl;
	reduce[table] = rule_num;
}

void parser::goto_entry(int table, int symbol, int next)
{
	if (table >= g_switch.size() || g_switch[table] == 0)
	{
		g_switch.resize(table + 1, 0);
		g_switch[table] = g_symbol.size() + 1;
	}
	int count = 0;
	bool added = false;
	for (auto it = g_symbol.rbegin(); it != g_symbol.rend(); ++it)
	{
		if (*it == 0)
			break;
		if (*it == symbol)
		{
			if (*(g_next.rbegin() + count) > next)
				*(g_next.rbegin() + count) = next;
			added = true;
			break;
		}
		count++;
	}
	if (!added)
	{
		g_symbol.push_back(symbol);
		g_next.push_back(next);
	}
}

void parser::print_lookups()
{
	std::cout << "Shift Table:\t\t\t\tReduce:\t\tGoTo:" << std::endl;
	int count = 0;
	for (auto it = s_symbol.begin() + 1; it != s_symbol.end(); ++it)
	{
		if (int(s_switch.size()) > count)
		{
			std::cout << "A" << count << ":\t";
			if (s_switch[count] == 0)
				std::cout << "0";
			else
				std::cout << s_switch[count] - 1;
		}
		else
			std::cout << "\t";
		std::cout << "\t" << count + 1 << ":";
		std::cout << "\t" << *it << "\t" << s_next[count + 1] << "\t";
		if (int(reduce.size()) > count)
		{
			std::cout << "A" << count << ":\t";
			std::cout << reduce[count];
		}
		else
			std::cout << "\t";
		std::cout << "\t";
		
		if (int(g_switch.size()) > count)
		{
			std::cout << "A" << count << ":\t";
			if (g_switch[count] == 0)
				std::cout << "0";
			else
				std::cout << g_switch[count] - 1;
		}
		else
			std::cout << "\t";
		std::cout << "\t";

		if (int(g_symbol.size()) > count+1)
		{
			std::cout << count + 1 << ":\t";
			std::cout << g_symbol[count + 1] << "\t" << g_next[count + 1];
		}
		std::cout << std::endl;
		count++;
	}
}

bool parser::parse_action(std::ifstream &tokens, cfg_reader &r)
{
	int state = 0;
	std::vector<int> stack;
	std::vector<int> output_rules;
	stack.push_back(state);
	std::string next_token = "";
	tokens >> next_token;
	int symbol = r.get_enum(next_token);
	while (!tokens.eof())
	{
		int s = shift(state, symbol);//returns s_next, or -1 if no shifts exist
		if(s<0)//reduce stuff, there was no shift action defined
		{
			int p;
			p = reduce[state];//p is the production rule#
			
			if (p <= 0)//no reduce action at this state, error
			{
				std::cout << "Error: No shift or reduce actions specified for A"
					<< state << ": (" << symbol << ", " << p << ")" << std::endl;
				std::cout << "reject" << std::endl;
				return false;
			}
			std::cout << "table: A" << state << "\t\treduced: " << p << ": ";
			std::vector<int> rule = r.get_cfg().rule_number(p - 1);
			for (auto it = rule.begin(); it != rule.end(); ++it)
				std::cout << r.get_symbol((*it)-1) << " ";
			std::cout << std::endl;

			if (p==1 && next_token=="@")//p is the initial production rule, we're done
			{
				std::cout << "accept\n" << std::endl;
				output_rules.push_back(1);
				for (auto it = output_rules.rbegin(); it != output_rules.rend(); ++it)
					std::cout << *it << " ";
				std::cout << std::endl;
				return true;
			}
			//p is some other rule, so we replace the handle with the nonterminal
			int lhs_symbol = r.get_cfg().rule_number(p - 1)[0];
			int handle_size = r.get_cfg().rule_number(p - 1).size() - 1;
			for (int i = 0; i < handle_size; i++)//get rid of the handle from the stack
				stack.pop_back();
			state = move(*(stack.end()-1), lhs_symbol);//check goto table for the next state
			if (state>0)
				stack.push_back(state);//put that state on the stack
			output_rules.push_back(p);
		}
		else//it was a shift
		{

			std::cout << "table: A" << state << "\t\tshifted: " << next_token << std::endl;
			state = s;
			stack.push_back(state);//perform the shift by placing s_next onto the stack
			//we only want to read another token when we shifted in the last one, or at the beginning when we always shift
			tokens >> next_token;
			symbol = r.get_enum(next_token);
			//std::cout << " lookahead: " << next_token << std::endl;

		}
	}
	std::cout << "reject" << std::endl;
	return false;
}

int parser::shift(int table, int symbol)
{
	int index = s_switch[table] -1;
	if (index >= 0)
	{
		//std::cout << index << std::endl;
		while (s_symbol[index] != 0)
		{
			//std::cout << s_symbol[index] << std::endl;
			if (s_symbol[index] == symbol)
				return s_next[index];
			index++;
		}
	}
	
	return -1;
}

int parser::move(int table, int symbol)
{
	int index = g_switch[table] - 1;
	if (index >= 0)
	{
		while (g_symbol[index] != 0)
		{
			if (g_symbol[index] == symbol)
				return g_next[index];
			index++;
		}
	}
	return -1;
}