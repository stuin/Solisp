#include "lisp.h"

/*
 * Created by Stuart Irwin on 7/11/2019.
 * Functions for reading files into lisp structure.
 */

//Convert given string to list of tokens
std::list<std::string> Enviroment::tokenize(const std::string & str) {
    std::list<std::string> tokens;
    const char *s = str.c_str();
    while(*s) {
        while(*s == ' ')
            ++s;
        if(*s == '(' || *s == ')')
            tokens.push_back(*s++ == '(' ? "(" : ")");
        else if(*s == '{' || *s == '}')
            tokens.push_back(*s++ == '{' ? "{" : "}");
        else if(*s == '"') {
        	//Get full string
        	const char *t = ++s;
            while(*t && *t != '"')
                ++t;
            tokens.push_back(std::string(s, t));
            s = ++t;
        } else {
            const char * t = s;
            while(*t && *t != ' ' && *t != '(' && *t != ')' && *t != '{' && *t != '}')
                ++t;
            tokens.push_back(std::string(s, t));
            s = t;
        }
    }
    return tokens;
}

//Numbers become Numbers; every other token is a String or char
cell Enviroment::atom(const std::string & token) {
    if(isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])))
        return cell(stoi(token));
    if(token.length() == 1)
    	return cell(token[0], CHAR);
    return cell(token);
}

//Return the Lisp expression in the given tokens
cell Enviroment::read_from(std::list<std::string> & tokens) {
    const std::string token(tokens.front());
    int type = (token == "{") ? LIST : EXPR;
    tokens.pop_front();
    if(token == "(" || token == "{") {
        sexpr output;
        while(tokens.front() != ")" && tokens.front() != "}")
            output.push_back(read_from(tokens));
        tokens.pop_front();
        return cell(output, type);
    }
    else
        return atom(token);
}

//Return the Lisp expression represented by the given string
cell Enviroment::read(const std::string & s) {
    std::list<std::string> tokens(tokenize(s));
    return read_from(tokens);
}

//Read objects spanning multiple lines into one string
cell Enviroment::read_stream(std::istream &in, int type, int new_line) {
	std::string object;
	int levels = -1;
	bool literal = false;

	if(!in.good())
		throw std::domain_error("File not found.");

	if(new_line != -1)
		start_line = new_line;
	int end_line = new_line = start_line;

	//For each incomplete line
	while((levels != 0 || object.length() < 1) && !in.eof()) {
		bool comment = false;
		std::string line;
		std::getline(in, line);
		end_line++;

		if(line.length() > 0) {
			if(levels == -1)
				levels = 0;

			//For each letter
			for(char c : line) {
				if(!literal && !comment) {
					//Count parenthesis and lists
					if(c == '(' || c == '{') {
						levels++;
					} else if(c == ')' || c == '}') {
						levels--;
					} else if(c == '#') {
						if(object.length() > 0)
							object += ' ';
						comment = true;
					}
				}

				if(c == '"')
					literal = !literal;

				//Add other character
				if(c == '\t')
					object += ' ';
				else if(!comment)
					object += c;
			}
			if(!comment)
				object += ' ';

			if(new_line == end_line - 1 && comment)
				new_line++;
		} else if(new_line == end_line - 1)
			new_line++;
	}

	//Run actual evaluation with error checking
	try {
		if(object.length() > 0) {
			if(in.eof() && levels != 0)
				throw std::domain_error(std::to_string(levels) + " non matched parenthesis");

			start_line = end_line;
			return force_eval[type](this, eval(read(object)));
		}
	} catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::cerr << "Within lines " << new_line << " to " << end_line << "\n";
		std::cerr << object << "\n";

		throw std::domain_error("Message displayed above");
	}

	start_line = end_line;
	return cell("");
}