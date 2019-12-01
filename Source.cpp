#include "parser.h"
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << argc << endl;
		cout << "please enter 3 arguments: filenames of tokenized cfg and source code to be parsed" << endl;
		return -1;
	}
	string cfg_file = argv[1];
	string source_code = argv[2];
	cfg_reader toy(cfg_file);
	toy.print_symbols();
	cout << endl;
	toy.get_cfg().print();
	parser t;
	t.fill_tables(toy);

	cout << "\n\nprinting all tables:" << endl;
	t.print(toy);
	t.print_lookups();
	cout << "Parsing " << source_code << endl << endl;
	ifstream file;
	file.open(source_code, ifstream::in);
	t.parse_action(file, toy);
	//cin.get();
}