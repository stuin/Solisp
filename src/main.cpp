#include <stdio.h>
#include <stdlib.h>
#include <editline.h>

#include "lisp.h"

//Editline Iterator
class EditIterator : public Iterator {
	bool exit = false;
	bool multi = false;

public:
	void getline(std::string &line, bool prompt) {
		if(prompt)
			line = readline("solisp>");
		else {
			line = readline("");
			multi = true;
		}

		if(line == "exit")
			exit = true;
	}

	bool eof() {
		return exit;
	}

	void endline(std::string &line) {
		if(multi)
			add_history(line.data());
		multi = false;
	}
};

//Base main function
int main(int argc, char const *argv[])
{
	Enviroment env;

	if(argc > 1) {
		sexpr args;
		int i = 2;

		//Copy arguments into env variable
		while(i < argc)
			args.push_back(env.read(argv[i++]));
		env.set("args", cell(args, LIST));

		//Run actual file
		std::ifstream infile(argv[1]);
		if(infile.good()) {
			std::cout << env.str_print(env.read_stream(infile, STRING));
			while(!infile.eof())
				std::cout << "\n" << env.str_print(env.read_stream(infile, STRING));
		}
		infile.close();
	} else {
		read_history(".solisp_history");
		el_bind_key('\t', []() {
			rl_insert_text("    ");
			return CSmove;
		});
		EditIterator it;

		while(!it.eof()) {
			std::cout << env.str_print(env.read_stream(it, STRING)) << "\n";
		}

		write_history(".solisp_history");
	}

}