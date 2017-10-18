#include <bits/stdc++.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/IR/Verifier.h>
//#include <llvm/Assembly/PrintModulePass.h>
#include <llvm/IR/IRBuilder.h>
//#include <llvm/ModuleProvider.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/raw_ostream.h>

using namespace std;
using namespace llvm;

// enum exprType { binary = 1, location = 2, literal = 3, enclExpr = 4 , Unexpr = 5};
// enum literalType { Int = 1, Bool = 2, Char = 3, String = 4 };
// enum stmtType { Return = 1, NonReturn = 2};
union Node{
	int number;
	char* value;
	class Prog* prog;
	class fieldDecls* fields;
	class fieldDecl* field;
	class Vars* vars;
	class Var* var;
	class fieldCodes* codes;
	class fieldCode* code;
	// class Block* block;
	// class varDecls* var_decls;
	// class varDecl* var_decl;
	// class Stmts* stmts;
	// class Stmt* stmt;
	// class Expr* expr;
	// class calloutArg* callout_arg;
	// class calloutArgs* callout_args;
	// class Params* parameters;
	// class methodArgs* method_args;
	// class methodArg* method_arg;
	// class methodCall* method_call;
	// class Location* location;
	// class Assignment* assignment;
	// class Literal* literal;
	// class stringList* mylist;

	Node(){
		number = 0;
		value = NULL;
		fields = NULL;
		field = NULL;
		codes= NULL;
		code = NULL;
		// var_decls = NULL;
		// stmts = NULL;
		// callout_args = NULL;
		// method_args = NULL;
	}
	~Node(){};
};

 typedef union Node YYSTYPE;
 #define YYSTYPE_IS_DECLARED 1

class reportError{
	/* Class for error handling */
    public:
        static llvm::Value* ErrorV(string str) {
            cout<<str<<endl;
            return 0;
        }
};

class astNode{
};


class Var:public astNode{
private:
	string declType; /* Array or Normal */
	string name; /* Name of the variable */
	string dataType; /* type of variable */
	unsigned int length; /* if it is an Array then length */
public:
	/* Constructors */
	// Var(string,string,unsigned int);
	Var(string,string);
	bool isArray();
	/* Methods */
	void setDataType(string); /* Set the data Type */
	void traverse();
	string getName();
	//Value* codegen();
	int getLength(){return length;}
};

class Vars:public astNode{
private:
	vector<class Var*> vars_list;
	int cnt;
public:
	Vars(){}
	void push_back(class Var*);
	vector<class Var*> getVarsList();
	void traverse();
	//Value* codegen();
};

class fieldDecl:public astNode{
private:
	string dataType; /* Field declaration can have datatype and vaariables */
	vector<class Var*> var_list;
public:
	fieldDecl(class Vars*);
	vector<class Var*> getVarsList();
	void traverse();
	Value* codegen();
};

class fieldDecls:public astNode{
private:
	vector<class fieldDecl*> decl_list;
	int cnt;
public:
	fieldDecls();
	void push_back(class fieldDecl*);
	void traverse();
	Value* codegen();
};



class fieldCode:public astNode{
public:
	virtual void traverse(){};
};

class fieldCodes:public astNode{
private:
	vector<class fieldCode* > fieldcodes;
	int cnt;
public:
	fieldCodes(){}
	void push_back(class fieldCode *);
	void traverse();
};

class Prog:public astNode{
private:
	class fieldDecls* decls;
	class fieldCodes* codes;
public:
	Prog(class fieldDecls*, class fieldCodes*);
	void traverse();
	void generateCode();
};
