#include "lisp.h"

/*
 * Created by Stuart Irwin on 29/10/2019.
 * library functions for lisp system.
 */

//Build a number comparison function
template <class T> cell Enviroment::comparitor(T func) {
	return cell([func](Enviroment *env, marker pos, marker end) {
		LISTREMAINS;
		int source = env->num_eval(*pos++);
		int next = 0;

		while(pos != end) {
			next = env->num_eval(*pos++);
			if(!func(source, next))
				return cell((char)0, BOOL);
			source = next;
		}
		return cell((char)1, BOOL);
	});
}

//Build a number arithmatic function
template <class T> cell Enviroment::arithmetic(T func) {
	return cell([func](Enviroment *env, marker pos, marker end) {
		LISTREMAINS;
		int value = env->num_eval(*pos++);
		while(pos != end)
			value = func(value, env->num_eval(*pos++));
		return value;
	});
}

void Enviroment::build_library() {
	//Build force evaluators
	type_name[EXPR] = "Expression";
	force_eval[EXPR] = [](Enviroment *env, cell const &c) {
		return c;
	};
	type_name[NAME] = "Name";
	force_eval[NAME] = [](Enviroment *env, cell const &c) {
		return cell(env->name_eval(c), NAME);
	};
	type_name[STRING] = "String";
	force_eval[STRING] = [](Enviroment *env, cell const &c) {
		return cell(env->str_eval(c), STRING);
	};
	type_name[BOOL] = "Boolean";
	force_eval[BOOL] = [](Enviroment *env, cell const &c) {
		return cell(env->bool_eval(c), BOOL);
	};
	type_name[NUMBER] = "Number";
	force_eval[NUMBER] = [](Enviroment *env, cell const &c) {
		return cell(env->num_eval(c), NUMBER);
	};
	type_name[CHAR] = "Char";
	force_eval[CHAR] = [](Enviroment *env, cell const &c) {
		return cell(env->char_eval(c), CHAR);
	};
	type_name[LIST] = "List";
	force_eval[LIST] = [](Enviroment *env, cell const &c) {
		return cell(env->list_eval(c), LIST);
	};

	//Add constants
	set("true", cell((char)1, BOOL));
	set("false", cell((char)0, BOOL));
	set("newline", cell("\n"));

	//Basic arithmatic
	set("+", arithmetic(std::plus<int>()));
	set("-", arithmetic(std::minus<int>()));
	set("*", arithmetic(std::multiplies<int>()));
	set("/", arithmetic(std::divides<int>()));
	set("%", arithmetic(std::modulus<int>()));

	//Numerical comparisons
	set(">", comparitor(std::greater<int>()));
	set(">=", comparitor(std::greater_equal<int>()));
	set("<", comparitor(std::less<int>()));
	set("<=", comparitor(std::less_equal<int>()));

	//Not gate
	set("Not", cell([](Enviroment *env, marker pos, marker end) {
		bool b = env->bool_eval(*pos++);
		DONE;
		return cell((char)!b, BOOL);
	}));

	//Universal comparisons
	set("==", cell([](Enviroment *env, marker pos, marker end) {
		LISTREMAINS;
		cell c = env->eval(*pos++);

		while(pos != end)
			if(!(env->equals(c, *pos++)))
				return cell((char)0, BOOL);
		return cell((char)1, BOOL);
	}));
	set("!=", cell([](Enviroment *env, marker pos, marker end) {
		LISTREMAINS;
		cell c = env->eval(*pos++);

		while(pos != end)
			if(env->equals(c, *pos++))
				return cell((char)0, BOOL);
		return cell((char)1, BOOL);
	}));

	//Type comparison
	set("Type", cell([](Enviroment *env, marker pos, marker end) {
		return env->type_name[(int)env->eval(*pos).type];
	}));
	set("Force", cell([](Enviroment *env, marker pos, marker end) {
		string type = env->name_eval(*pos++);

		for(int i = 0; i < MAX_TYPE; i++)
			if(env->type_name[i] == type)
				return env->force_eval[i](env, *pos++);
		throw std::domain_error("Type " + type + " not found");
	}));

	//String functions
	set("JoinD", cell([](Enviroment *env, marker pos, marker end) {
		string delim = env->str_eval(*pos++);
		LISTREMAINS;
		string output;

		//Perform actual appending
		while(pos != end)
			output += env->str_eval(*pos++) + delim;

		DONE;
		return output;
	}));
	//Old join function, deprecated because of argument ordering
	set("Join", cell([](Enviroment *env, marker pos, marker end) {
		sexpr array = env->list_eval(*pos++);
		string output;

		//Set deliminator if provided
		string delim = "";
		if(pos != end)
			delim += env->str_eval(*pos++);

		//Perform actual appending
		for(cell s : array)
			output += env->str_eval(s) + delim;

		DONE;
		return output;
	}));
	set("Print",  cell([](Enviroment *env, marker pos, marker end) {
		string s = env->str_eval(*pos++);
		std::cout << s;
		DONE;
		return cell(s);
	}));

	//List building functions
	set("Append", cell([](Enviroment *env, marker pos, marker end) {
		LISTREMAINS;
		sexpr output;

		while(pos != end) {
			sexpr array = env->list_eval(*pos++);
			output.insert(output.end(), array.begin(), array.end());
		}
		return cell(output, LIST);
	}));
	set("Seq", cell([](Enviroment *env, marker pos, marker end) {
		int first = env->num_eval(*pos++);
		int last = env->num_eval(*pos++);
		sexpr output;

		//Add numbers to list
		for(int i = first; i <= last; i++)
			output.push_back(cell(i));

		DONE;
		return cell(output, LIST);
	}));
	set("Remove", cell([](Enviroment *env, marker pos, marker end) {
		cell remove = env->eval(*pos++);
		LISTREMAINS;
		sexpr output;

		//Copy all non-matching cells
		while(pos != end) {
			if(!(env->equals(remove, *pos)))
				output.push_back(*pos);
			pos++;
		}

		return cell(output, LIST);
	}));
	set("Tail", cell([](Enviroment *env, marker pos, marker end) {
		LISTREMAINS;
		sexpr output;
		if(pos != end)
			++pos;

		//Copy all other cells
		while(pos != end)
				output.push_back(*pos++);

		return cell(output, LIST);
	}));
	set("Clone", cell([](Enviroment *env, marker pos, marker end) {
		int count = env->num_eval(*pos++);
		LISTREMAINS;
		sexpr output;

		//Copy list multiple times
		for(int i = 0; i < count; i++)
			output.insert(output.end(), pos, end);

		return cell(output, LIST);
	}));
	set("Reverse", cell([](Enviroment *env, marker pos, marker end) {
		LISTREMAINS;
		sexpr output;

		while(pos != end)
			output.push_back(*--end);

		return cell(output, LIST);
	}));

	//Control flow
	set("If", cell([](Enviroment *env, marker pos, marker end) {
		cell output = cell(0);
		if(env->bool_eval(*pos++)) {
			//If true
			output = *pos++;
			if(pos != end)
				++pos;
		} else {
			//If false
			++pos;
			if(pos != end)
				output = *pos++;
		}
		DONE;
		return output;
	}));
	set("Step", cell([](Enviroment *env, marker pos, marker end) {
		LISTREMAINS;
		cell output;

		while(pos != end)
			output = env->eval(*pos++);

		return output;
	}));
	set("Switch", cell([](Enviroment *env, marker pos, marker end) {
		LISTREMAINS;
		sexpr array;

		while(pos != end) {
			array = env->list_eval(*pos++);
			if(env->bool_eval(array[0]))
				return array[1];
		}

		return cell(0);
	}));

	//List evaluation
	set("Get", cell([](Enviroment *env, marker pos, marker end) {
		int index = env->num_eval(*pos++);
		LISTREMAINS;

		//Copy all other cells
		while(index != 0 && pos != end) {
			++pos;
			--index;
		}

		return *pos;
	}));
	set("Length", cell([](Enviroment *env, marker pos, marker end) {
		int size = 0;
		LISTREMAINS;

		//Count all other cells
		while(pos != end) {
			++pos;
			++size;
		}

		return cell(size);
	}));
	set("Max", cell([](Enviroment *env, marker pos, marker end) {
		LISTREMAINS;
		int max = env->num_eval(*pos++);

		//Check each cell
		while(pos != end) {
			int value = env->num_eval(*pos++);
			if(value > max)
				max = value;
		}

		return cell(max);
	}));

	//High level functions
	set("Map", cell([](Enviroment *env, marker pos, marker end) {
		string var = env->name_eval(*pos++);
		sexpr array = env->list_eval(*pos++);
		sexpr output;
		env->shift_env(true);

		//Re list each value
		for(cell c : array) {
			env->set(var, c);
			output.push_back(env->eval(*pos));
		}

		env->shift_env(false);
		++pos;
		DONE;
		return cell(output, LIST);
	}));
	set("MapI", cell([](Enviroment *env, marker pos, marker end) {
		string var = env->name_eval(*pos++);
		string index = env->name_eval(*pos++);
		sexpr array = env->list_eval(*pos++);
		sexpr output;
		env->shift_env(true);
		int i = 0;

		//Re list each value
		for(cell c : array) {
			env->set(var, c);
			env->set(index, i++);
			output.push_back(env->eval(*pos));
		}

		env->shift_env(false);
		++pos;
		DONE;
		return cell(output, LIST);
	}));
	set("Repeat", cell([](Enviroment *env, marker pos, marker end) {
		cell value = *pos++;
		int count = 0;

		//Run loop until false
		while(env->bool_eval(value))
			++count;

		DONE;
		return cell(count);
	}));

	//Variable management
	set("Set", cell([](Enviroment *env, marker pos, marker end) {
		string name = env->name_eval(*pos++);

		//Check for existing value
		if(env->get(name) != NULL)
			throw std::domain_error("Already defined variable: " + name + " (use Mutate)");

		cell output = env->set(name, *pos++);
		DONE;
		return output;
	}));
	set("Mutate", cell([](Enviroment *env, marker pos, marker end) {
		string name = env->name_eval(*pos++);
		cell output = env->set(name, *pos++);

		DONE;
		return output;
	}));
}