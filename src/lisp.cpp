#include "lisp.h"

/*
 * Created by Stuart Irwin on 20/12/2019.
 * Lisp type convertions and definitions
 */

//Define function from list
builtin Enviroment::build_function(sexpr func) {
	return [func](Enviroment *env, marker pos, marker end) {
		sexpr vars = env->list_eval(func[0]);
		LISTREMAINS;
		env->shift_env(true);

		//Set function variables
		for(cell c : vars) {
			if(pos == end) {
				pos--;
				DONE;
			}

			string s = env->name_eval(c);
			env->set(s, *pos++);
		}

		cell output = env->eval(func[1]);
		env->shift_env(false);
		return output;
	};
}

//Variable retrieval
cell *Enviroment::get(string s) {
	//Check each enviroment layer for value
	for(int i = vars.size() - 1; i >= 0; i--) {
		auto it = vars[i].find(s);
		if(it != vars[i].end())
			return &(it->second);
	}

	return NULL;
}

//Variable assginment
cell Enviroment::set(string s, cell c) {
	return vars.back()[s] = c;
}

//Shift enviroment
void Enviroment::shift_env(bool in) {
	if(in) {
		std::map<string, cell> lib;
		vars.push_back(lib);
	} else if(vars.size() > 1) {
		vars.pop_back();
	}
	//std::cout << "Moved to layer " << vars.size() << "\n";
}

//Print all env variables
void Enviroment::print_env() {
	for(int i = 0; i < (int)vars.size(); i++) {
		std::cout << "Layer " << i << "\n";
		for(auto mit = vars[i].begin(); mit != vars[i].end(); ++mit)
		    std::cout << mit->first << " = " << str_eval(mit->second) << "\n";
	}
}

//Base eval function
cell Enviroment::eval(cell const &c) {
	if(c.type == EXPR)
		return std::visit([this](auto const &a) { return this->eval(a); }, c.content);
	if(c.type == NAME) {
		cell *var = get(std::get<string>(c.content));
		if(var != NULL)
			return eval(*var);
	}
    return c;
}

//Actual expression evaluation
cell Enviroment::eval(sexpr const &s) {
	if(s.begin() == s.end())
		throw std::domain_error("Empty sexpr");

	//Search library for function
	return eval(function_eval(*s.begin())(this, ++s.begin(), s.end()));
}

string Enviroment::name_eval(cell const &c) {
	string output;
	sexpr array;
	switch(c.type) {
		case EXPR:
			return name_eval(eval(c));
		case LIST:
			array = std::get<sexpr>(c.content);
			for(cell s : array)
				output += str_eval(s);
			return output;
		case CHAR:
			return string(1, std::get<char>(c.content));
		case STRING: case NAME:
			return std::get<string>(c.content);
	}

	return name_eval_cont(c);
}

string Enviroment::name_eval_cont(cell const &c) {
	CONVERTERROR("Name");
}

//Convert to string
string Enviroment::str_eval(cell const &c) {
	string output;
	sexpr array;
	cell *var;
	switch(c.type) {
		case EXPR:
			return str_eval(eval(c));
		case STRING:
			return std::get<string>(c.content);
		case BOOL:
			return (bool)std::get<char>(c.content) ? "true" : "false";
		case NUMBER:
			return std::to_string(std::get<int>(c.content));
		case FUNCTION:
			return "<func>";
		case LIST:
			array = std::get<sexpr>(c.content);
			for(cell s : array)
				output += str_eval(s);
			return output;
		case CHAR:
			return string(1, std::get<char>(c.content));
		case NAME:
			output = std::get<string>(c.content);

			//Check if variable
			var = get(output);
			if(var != NULL)
				return str_eval(*var);

			return output;
	}

	return str_eval_cont(c);
}

string Enviroment::str_eval_cont(cell const &c) {
	CONVERTERROR("String");
}

//Convert to boolean
bool Enviroment::bool_eval(cell const &c) {
	cell *var;
	switch(c.type) {
		case EXPR:
			return bool_eval(eval(c));
		case BOOL:
			return (bool)std::get<char>(c.content);
		case NAME:
			var = get(std::get<string>(c.content));
			if(var != NULL)
				return bool_eval(*var);
		case NUMBER: case CHAR: case STRING:
			return (bool)num_eval(c);
	}

	return bool_eval_cont(c);
}

bool Enviroment::bool_eval_cont(cell const &c) {
	CONVERTERROR("Bool");
}

//Convert to number
int Enviroment::num_eval(cell const &c) {
	cell *var;
	string s;
	switch(c.type) {
		case EXPR:
			return num_eval(eval(c));
		case NUMBER:
			return std::get<int>(c.content);
		case CHAR: case BOOL:
			return std::get<char>(c.content);
		case NAME:
			var = get(std::get<string>(c.content));
			if(var != NULL)
				return num_eval(*var);
		case STRING:
			s = std::get<string>(c.content);

			//If numerical string
			if(isdigit(s[0]) || s[0] == '-')
				return std::stoi(s);
	}

	return num_eval_cont(c);
}

int Enviroment::num_eval_cont(cell const &c) {
	CONVERTERROR("Number");
}

//Convert to char
char Enviroment::char_eval(cell const &c) {
	cell *var;
	switch(c.type) {
		case EXPR:
			return char_eval(eval(c));
		case NUMBER:
			return std::get<int>(c.content);
		case CHAR: case BOOL:
			return std::get<char>(c.content);
		case NAME:
			var = get(std::get<string>(c.content));
			if(var != NULL)
				return char_eval(*var);
		case STRING:
			//Check if short enough
			string s = std::get<string>(c.content);
			if(s.length() == 1)
				return s[0];
	}

	return char_eval_cont(c);
}

char Enviroment::char_eval_cont(cell const &c) {
	CONVERTERROR("Char");
}

//Convert to list
sexpr Enviroment::list_eval(cell const &c) {
	cell *var;
	switch(c.type) {
		case EXPR:
			return list_eval(eval(c));
		case LIST:
			return std::get<sexpr>(c.content);
		case NAME:
			var = get(std::get<string>(c.content));
			if(var != NULL)
				return list_eval(*var);
		case STRING:
			string s = std::get<string>(c.content);

			//Convert string to char list
			sexpr output;
			for(char a : s)
				output.push_back(cell(a, CHAR));
			return output;
	}

	return list_eval_cont(c);
}

sexpr Enviroment::list_eval_cont(cell const &c) {
	//Convert to single object list
	sexpr output;
	output.push_back(c);
	return output;
}

//Convert cell to function
builtin Enviroment::function_eval(cell const &c) {
	cell *var;
	switch(c.type) {
		case EXPR:
			return function_eval(eval(c));
		case FUNCTION:
			return std::get<builtin>(c.content);
		case LIST:
			return build_function(std::get<sexpr>(c.content));
		case NAME:
			var = get(std::get<string>(c.content));
			if(var != NULL)
				return function_eval(*var);
	}
	return function_eval_cont(c);
}

builtin Enviroment::function_eval_cont(cell const &c) {
	CONVERTERROR("Function");
}

//Compare two cells
bool Enviroment::equals(const cell &first, const cell &second) {
	if(auto val = std::get_if<sexpr>(&first.content))
			return equals(*val, std::get<sexpr>(force_eval[(int)first.type](this, second).content));
	return first.content == force_eval[(int)first.type](this, second).content;
}

bool Enviroment::equals(const sexpr &first, const sexpr &second) {
	marker pos1 = first.begin();
	marker pos2 = second.begin();
	while(pos1 != first.end() && pos2 != second.end())
		if(!equals(*pos1++, *pos2++))
			return false;

	return pos1 == first.end() && pos2 == second.end();
}