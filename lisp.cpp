#include "lisp.h"

/*
 * Created by Stuart Irwin on 20/12/2019.
 * Lisp type convertions and definitions
 */

//Variable retrieval
cell *Enviroment::get(string s) {
	//Check each enviroment layer for value
	for(int i = current; i >= 0; i--) {
		auto it = vars[i].find(s);
		if(it != vars[i].end())
			return &(it->second);
	}

	return NULL;
}

//Variable assginment
cell Enviroment::set(string s, cell c) {
	return vars[current][s] = c;
}

//Shift enviroment
void Enviroment::shift_env(bool in) {
	if(in) {
		std::map<string, cell> lib;
		vars.push_back(lib);
		current++;
	} else if(current > 1) {
		vars.pop_back();
		current--;
	}
}

//Base eval function
cell Enviroment::eval(cell const &c) {
	if(c.type == EXPR)
		return std::visit([this](auto const &c) { return this->eval(c); }, c.content);
    if(c.type == STRING) {
    	cell *var = get(std::get<string>(c.content));
		if(var != NULL)
			return *var;
    }
    return c;
}

//Actual expression evaluation
cell Enviroment::eval(sexpr const &s) {
	if(s.begin() == s.end())
		throw std::domain_error("Empty sexpr");

	//Search library for function
	return function_eval(*s.begin())(this, ++s.begin(), s.end());
}

//Convert to string
string Enviroment::str_eval(cell const &c, bool literal) {
	if((c.type == EXPR && !literal))
		return str_eval(eval(c));

	string output;
	sexpr array;
	switch(c.type) {
		case NUMBER:
			return std::to_string(std::get<int>(c.content));
		case CHAR:
			return string(1, std::get<int>(c.content));
		case FUNCTION:
			return "f";
		case LIST: case EXPR:
			array = std::get<sexpr>(c.content);
			for(cell s : array)
				output += str_eval(s, literal) + " ";
			return output;
		case STRING:
			output = std::get<string>(c.content);

			//Check if variable
			cell *var = get(output);
			if(!literal && var != NULL)
				return str_eval(*var);

			return output;
	}

	return str_eval_cont(c, literal);
}

string Enviroment::str_eval_cont(cell const &c, bool literal) {
	CONVERTERROR("string");
}

//Convert to number
int Enviroment::num_eval(cell const &c) {
	switch(c.type) {
		case EXPR:
			return num_eval(eval(c));
		case NUMBER: case CHAR:
			return std::get<int>(c.content);
		case LIST:
			return std::get<sexpr>(c.content).size();
		case STRING:
			string s = std::get<string>(c.content);

			//If numerical string
			if(isdigit(s[0]) || s[0] == '-')
				return std::stoi(s);

			//Check if variable
			cell *var = get(s);
			if(var != NULL)
				return num_eval(*var);

			//Try boolean values
			if(s[0] == 't' || s[0] == 'T')
				return 1;
			if(s[0] == 'f' || s[0] == 'F')
				return 0;

			throw std::domain_error("No variable or value found for " + s);
	}

	return num_eval_cont(c);
}

int Enviroment::num_eval_cont(cell const &c) {
	CONVERTERROR("number");
}

//Convert to char
char Enviroment::char_eval(cell const &c) {
	switch(c.type) {
		case EXPR:
			return char_eval(eval(c));
		case NUMBER: case CHAR:
			return std::get<int>(c.content);
		case STRING:
			//Check if short enough
			string s = std::get<string>(c.content);
			if(s.length() == 1)
				return s[0];

			//Check if variable
			cell *var = get(s);
			if(var != NULL)
				return char_eval(*var);

			throw std::domain_error("Cannot convert to char from multi-char string");
	}

	return char_eval_cont(c);
}

char Enviroment::char_eval_cont(cell const &c) {
	CONVERTERROR("char");
}

//Convert to list
sexpr Enviroment::list_eval(cell const &c) {
	switch(c.type) {
		case EXPR:
			return list_eval(eval(c));
		case LIST:
			return std::get<sexpr>(c.content);
		case STRING:
			string s = std::get<string>(c.content);

			//Check if variable
			cell *var = get(s);
			if(var != NULL)
				return list_eval(*var);

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
		case STRING:
			//Check if variable
			var = get(std::get<string>(c.content));
			if(var != NULL)
				return function_eval(*var);
		case CHAR:
			//Check if variable
			var = get(string(1, std::get<int>(c.content)));
			if(var != NULL)
				return function_eval(*var);
	}
	CONVERTERROR("function");
}

//Compare two cells
bool Enviroment::equals(const cell &first, const cell &second) {
	if(auto val = std::get_if<sexpr>(&first.content))
			return equals(*val, std::get<sexpr>(force_eval[first.type](this, second).content));
	return first.content == force_eval[first.type](this, second).content;
}

bool Enviroment::equals(const sexpr &first, const sexpr &second) {
	marker pos1 = first.begin();
	marker pos2 = second.begin();
	while(pos1 != first.end() && pos2 != second.end())
		if(!equals(*pos1++, *pos2++))
			return false;

	return pos1 == first.end() && pos2 == second.end();
}