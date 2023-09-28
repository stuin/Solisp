#pragma once

#include <vector>
#include <string>
#include <variant>
#include <functional>
#include <map>
#include <stdexcept>
#include <list>
#include <iostream>
#include <fstream>
#include <initializer_list>

/*
 * Created by Stuart Irwin on 28/10/2019.
 * Base list structures and systems
 * Loosely based on https://gist.github.com/KayEss/45a2e88675832228f57e2d598afc02ae and https://gist.github.com/ofan/721464
 */

//Main simple data types
struct cell;
class Enviroment;
using std::string;
using sexpr = std::vector<cell>;
using marker = sexpr::const_iterator;
using builtin = std::function<auto(Enviroment*, marker, marker)->cell>;
using force_builtin = std::function<cell(Enviroment*, const cell&)>;

//General function macros
#define DONE if(pos != end) throw std::domain_error("Too many arguments: " + env->name_eval(*pos))
#define LISTREMAINS sexpr args; if(pos != end && pos == --end) { args = env->list_eval(*pos); pos = args.begin(); end = args.end();} else end++
#define CONVERTERROR(goal) throw std::domain_error("Cannot convert " + name_eval(c) + " to " + goal + " from type " + type_name[(int)c.type])

//Base data types
#define EXPR 0
#define FUNCTION 1
#define NAME 2
#define STRING 3
#define BOOL 4
#define NUMBER 5
#define CHAR 6
#define LIST 7

#define MAX_TYPE 20

//Main data sructure
struct cell {
	char type;
	std::variant<sexpr, string, int, char, builtin> content;

	//Constructors
	cell() { cell(0); }
	cell(string s, char t = STRING) : content{std::move(s)} { type = t; }
	cell(int s, char t = NUMBER) : content{std::move(s)} { type = t; }
	cell(char s, char t = CHAR) : content{std::move(s)} { type = t; }
	cell(sexpr s, char t = EXPR) : content{std::move(s)} { type = t; }
	cell(builtin s, char t = FUNCTION) : content{std::move(s)} { type = t; }

	friend bool operator==(const cell &first, const cell &second) {
		return first.content == second.content;
	}
};

//Custom iterator for input
class Iterator {
public:
	virtual void getline(std::string &line, bool prompt) {}
	virtual bool eof() {
		return false;
	}
	virtual void endline(std::string &line) {}
};

//Enviroment of system variables
class Enviroment {
private:
	int start_line = 0;
	std::vector<std::map<string, cell>> vars;

	//Library structure
	void build_library();
	builtin build_function(sexpr func);

	//Reader functions
	std::list<std::string> tokenize(const std::string & str);
	cell atom(const std::string & token);
	cell read_from(std::list<std::string> & tokens);

	//Lambda builder functions
	template <class T> cell comparitor(T func);
	template <class T> cell arithmetic(T func);

public:
	force_builtin force_eval[MAX_TYPE];
	string type_name[MAX_TYPE];

	//Variable retrieval
	cell *get(string s);
	cell set(string s, cell c);
	void shift_env(bool in);
	void print_env();

	//Base eval functions
	cell eval(cell const &c);
	cell eval(sexpr const &c);

	//Convert types
	string name_eval(cell const &c);
	string str_eval(cell const &c, bool literal=false);
	bool bool_eval(cell const &c);
	int num_eval(cell const &c);
	char char_eval(cell const &c);
	sexpr list_eval(cell const &c);
	builtin function_eval(cell const &c);

	//Allow additional type conversions
	virtual string name_eval_cont(cell const &c);
	virtual string str_eval_cont(cell const &c, bool literal=false);
	virtual bool bool_eval_cont(cell const &c);
	virtual int num_eval_cont(cell const &c);
	virtual char char_eval_cont(cell const &c);
	virtual sexpr list_eval_cont(cell const &c);
	virtual builtin function_eval_cont(cell const &c);

	//Public reader functions
	cell read(const std::string & s);
	cell read_stream(Iterator &in, int type, int new_line = -1);
	cell read_stream(std::istream &in, int type, int new_line = -1);

	//Comparisons with proper conversion
	bool equals(const cell &first, const cell &second);
	bool equals(const sexpr &first, const sexpr &second);

	Enviroment() {
		shift_env(true);
		build_library();
		shift_env(true);
	}
};