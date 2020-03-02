#include "lisp.h"

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
			while(!infile.eof())
				std::cout << env.str_eval(env.read_stream(infile, STRING)) + "\n";
		}
		infile.close();
	} else {
		while(true) {
			std::cout << "test>";
			std::cout << env.str_eval(env.read_stream(std::cin, STRING)) + "\n";
		}
	}

}