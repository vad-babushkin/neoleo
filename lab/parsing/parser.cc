#include <cassert>
#include <ctype.h>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <cmath>
#include <string>
#include <variant>
#include <vector>

using namespace std;


///////////////////////////////////////////////////////////////////////////////////////////////////
// TYPE DECLARATIONS

class Expr;
typedef vector<Expr> args_t;
typedef double value_t;
typedef function<value_t(args_t)> function_t;
//class Args { public: vector<Expr> args; };
//class FunCall;
class FunCall { 
	public: 
		function_t* fn ; 
		args_t args; 
		//void set(string fnamej
};
class Expr { 
	public: 
		Expr() {};
		Expr(value_t v) : expr(v) {};
		Expr(string fname, Expr x);
		//Expr(string fname, args_t args);
		variant<FunCall, value_t> expr; 
};


extern map<string, function_t> funcmap;

void unknown_function(string function_name)
{
	cerr << "Unknown function " << function_name << "\n";
	throw 667;
}

function_t* lookup_fn(string function_name)
{
	if(funcmap.count(function_name) == 0)
		unknown_function(function_name);

	return funcmap[function_name];
	//if(fn == 0)
	//	unknown_function(function_name);
	//return fn;
}
/*
   Expr::Expr(string fname, args_t args)
   {
   FunCall fc;
   auto fn = &funcmap[fname];
   fc.fn = fn;
   fc.args = args;
   this->expr = fc;
   }
   */
Expr::Expr(string fname, Expr x)
{


	FunCall fc;
	auto fn = &funcmap[fname];
	fc.fn = fn;
	fc.args.push_back(x);
	this->expr = fc;


	/*
	   args_t args{x};	
	   Expr(fname, args);
	   */
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS

value_t eval(Expr expr);


void parse_error()
{
	throw 666;
}

value_t do_plus(args_t args)
{
	value_t val = 0;
	for(auto& arg: args) val += eval(arg);
	return val;
}
value_t do_minus(args_t args)
{
	if(args.size() == 0) return 0;
	value_t val = eval(args[0]);
	if(args.size() == 1) return -val; // if there is only one argument, then return the negative of it
	for(int i = 1; i<args.size(); ++i) val -= eval(args[i]);
	return val;
}
value_t do_mul(args_t args)
{
	value_t val = 1.0;
	for(auto& arg: args) {
		value_t a = eval(arg);
		val *= a;
		//cout << "do_mul a and val " << a << " " << val << "\n";
	}
	return val;
}
value_t do_div(args_t args)
{
	if(args.size() == 0) return 0;
	value_t val = eval(args[0]);
	//cout << "do_div 1/val " << 1.0/val << "\n";
	if(args.size() == 1) return 1.0/val;
	for(int i = 1; i<args.size(); ++i) val /= eval(args[i]);
	return val;
}

value_t do_sqrt(args_t args)
{
	if(args.size() !=1) parse_error();
	value_t val = eval(args[0]);
	return sqrt(val);
}
map<string, function_t> funcmap= {
	{"+", &do_plus},
	{"-", &do_minus},
	{"*", &do_mul},
	{"/", &do_div},
	{"sqrt", &do_sqrt}
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// LEXER
typedef pair<int, string> token_t;
typedef deque<token_t> tokens_t;

//enum Tokens { EOI, UNK, NUMBER, ID, PLUS };
enum Tokens { EOI = 128, NUMBER, ID };


tokens_t tokenise(string str)
{
	tokens_t tokens;

	auto found = [&tokens](auto toketype, auto token) { tokens.push_back(make_pair(toketype, token)); };
	cout << "Parsing: " << str << "\n";
	const char* cstr = str.c_str();
	int pos = 0;
	auto it = str.begin();
loop:
	string token;
	char ch = cstr[pos];
	if(ch == 0) {
		goto finis;
	} else if ( isdigit(ch)) {
		while(isdigit(ch) || ch == '.' ) { token += ch; ch = cstr[++pos]; }
		found(NUMBER, token);
	} else if (isalpha(ch)) {
		while(isalnum(ch)) { token += ch; ch = cstr[++pos]; }
		found(ID, token);
		cout << "found id: " << token << "\n";
	} else {
		token = ch;
		pos++;
		found(ch, token);
	}
	goto loop;
finis:
	found(EOI, "End of stream"); // Add this so that we can look ahead into the void
	return tokens;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// SCANNER (the "yacc" side of things)


/*
   template<class R, class Q>
   R pop(Q& qs)
   {
   R front = qs.front();
   qs.pop_front();
   return front;
   }
   */

	template<class Q>
Q rest(Q qs)
{
	qs.pop_front();
	return qs;
}

/*
   tokens_t rest(tokens_t tokes)
   {
   tokes.pop_front();
   return tokes;
   }
   */

typedef deque<string> ops_t;

// ARITHMETIC
// Adopt the algorithm at 
// https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm#classic
// for computing arithmetic
//
// Here's the original derivations
//  E --> T {( "+" | "-" ) T}
//  T --> F {( "*" | "/" ) F}
//  F --> P ["^" F]
//  P --> v | "(" E ")" | "-" T
//
// Here's mine:
// I extend BNF with the notion of a function, prefixed by &
// {} zero or more repetitions
// [] optional
//  &M(o, X) --> X {o X} // e.g. &M(("+"|"-"), T) --> T { ("+"|"-") T }
//  E --> &M(( "<" | "<=" | ">" | ">=" | "==" | "!=" ), R)
//  R --> &M(( "+" | "-" ), T)
//  T --> &M(( "*" | "/" ), F)
//  F --> ["+"|"-"] (v | "(" E ")")



Expr parse_e(tokens_t& tokes);
Expr parse_t(tokens_t& tokes);

Expr parse_fn(string fname, tokens_t& tokes)
{
	cout << "parse_fn name " << fname << "\n";
	auto fn = lookup_fn(fname);
	cout << (*fn)(args_t{12}) << "\n";

	FunCall fc;
	args_t args;
	Expr x;
	return x;
	//return args; // TODO fix this
	//token_t toke = tokes.front();
	//if(toke.first != '(')) parse_error();
	//tokes.pop_front();

}

Expr parse_p(tokens_t& tokes)
{
	//Expr t{parse_t(tokes)};
	token_t toke = tokes.front();
	tokes.pop_front();
	switch(toke.first) {
		case EOI:
			return Expr();
		case NUMBER:
			return Expr(stoi(toke.second));
		case ID:
			return parse_fn(toke.second, tokes);	
		case '(': {
				  //tokes.pop_front();
				  Expr x1{parse_e(tokes)};
				  if(tokes.front().first == ')')
					  tokes.pop_front();
				  else
					  parse_error();
				  return x1;
			  }
		case '-':
			  return Expr("-", parse_t(tokes));
		default:
			  parse_error();
	}
	return Expr(); // should never reach here
}
Expr parse_f(tokens_t& tokes) { return parse_p(tokes); }


FunCall _parse_t(tokens_t& tokes)
{
	FunCall fc;
	fc.fn = &funcmap["*"];

	fc.args.push_back(parse_f(tokes));
	//return fc;
	while(1) {
		auto nid = tokes.front().first;
		if(nid == EOI) return fc;
		if(nid == '*') {
			tokes.pop_front();
			fc.args.push_back(parse_f(tokes));
		} else  if(nid == '/') {
			tokes.pop_front();
			Expr eneg =parse_f(tokes);
			FunCall fneg;
			fneg.fn = &funcmap["/"];
			fneg.args = args_t{eneg};
			Expr x2;
			x2.expr = fneg;
			fc.args.push_back(x2);		
		} else {
			return fc;
		}
	}
}
Expr parse_t(tokens_t& tokes)
{
	FunCall fc{_parse_t(tokes)};

	if(fc.args.size() == 1)
		return Expr(fc.args[0]);
	else {
		Expr x;
		x.expr = fc;
		return x;
	}

}

FunCall _parse_e(tokens_t& tokes)
{
	FunCall fc;
	fc.fn = &funcmap["+"];

	fc.args.push_back(parse_t(tokes));
	while(1) {
		auto nid = tokes.front().first;
		//cout << "nid is " << nid << "\n";
		if(nid == EOI) return fc;
		//tokes.pop_front();
		if(nid == '+') {
			tokes.pop_front();
			//cout <<" nod is +\n";
			fc.args.push_back(parse_t(tokes));
		} else  if(nid == '-') {
			tokes.pop_front();
			Expr eneg =parse_t(tokes);
			FunCall fneg;
			fneg.fn = &funcmap["-"];
			fneg.args = args_t{eneg};
			Expr x2;
			x2.expr = fneg;
			fc.args.push_back(x2);		
		} else {
			return fc;
		}
	}
}
Expr parse_e(tokens_t& tokes)
{
	FunCall fc{_parse_e(tokes)};

	if(fc.args.size() == 1)
		return Expr(fc.args[0]);
	else {
		Expr x;
		x.expr = fc;
		return x;
	}

}



///////////////////////////////////////////////////////////////////////////////////////////////////
// EVAL

//template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
//template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

value_t eval(Expr expr)
{
	value_t val = 667;
	if(std::holds_alternative<value_t>(expr.expr))
		val = std::get<value_t>(expr.expr);
	else { // must be a function call		
		//auto &fn = std::get<FunCall>(expr.fn);
		auto &fc = std::get<FunCall>(expr.expr);
		auto fn = fc.fn;
		return (*fn)(fc.args);
	}

	return val;


}
///////////////////////////////////////////////////////////////////////////////////////////////////
int interpret(string s, int expected)
{
	tokens_t tokes{tokenise(s)};

	if constexpr (0) {
		for(auto& t:tokes) {
			cout << "Found: " << t.first << " " << t.second << "\n";
		}
	}

	Expr expr{parse_e(tokes)};
	value_t val = eval(expr);
	cout << "Evaluates to " << val << " ";
	if(val == expected) 
		cout << "PASS";
	else
		cout << "FAIL";
	cout << "\n\n";

	return 0;
}
int main()
{

	interpret("sqrt1(9)+3", 6);
	return 0;
	interpret("42", 42);
	interpret("42+3", 45);
	interpret("1+3+5+7", 16);
	interpret("1-3-5-7", -14);
	interpret("1-3+5-7", -4);
	interpret("1+2*3", 7);
	interpret("1+12/3", 5);
	interpret("1+12/3*2/2", 5);
	interpret("(1+2)*3", 9);
	interpret("-(1+2)*-3", 9);


	return 0;
}
