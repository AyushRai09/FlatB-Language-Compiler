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
enum stmtType { Return = 1, NonReturn = 2};
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
	class exprnewst* exprnewsts;
	class arithmeticst* arthm;
	class expr* exp;
	class condsst* conds;
	class condst* cond;
	class forloopinit* fors;
	class callst* called;

	Node(){
		number = 0;
		value = NULL;
		fields = NULL;
		field = NULL;
		codes= NULL;
		code = NULL;
		exprnewsts=NULL;
		arthm=NULL;
		conds=NULL;
		cond=NULL;
		fors=NULL;
		called=NULL;
	}
	~Node(){};
};

 typedef union Node YYSTYPE;
 #define YYSTYPE_IS_DECLARED 1s

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
// private:
public:
	string declType; /* Array or Normal */
	string name; /* Name of the variable */
	// string dataType; /* type of variable */
	// unsigned int length; /* if it is an Array then length */
public:
	/* Constructors */
	// Var(string,string,unsigned int);
	Var(string,string);
	// bool isArray();
	/* Methods */
	// void setDataType(string); /* Set the data Type */
	void traverse();
	string getName();
	//Value* codegen();
	// int getLength(){return length;}
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
// protected:
// 	stmtType stype;
// public:
// 	virtual void traverse(){}
// 	// virtual Value* codegen(){}
// 	virtual bool has_return(){return false;}
// 	void setStype(stmtType x){this->stype = x;}
// 	stmtType getStype(){return this->stype;}

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

class expr:public fieldCode{
private:
	string lhs;
	class exprnewst* rhs;
public:
	expr(string,class exprnewst*);
	void traverse();
};


class arithmeticst:public fieldCode{
public:
	string op;
	class exprnewst* lho;
	class exprnewst* rho;
public:
	arithmeticst(class exprnewst*,string, class exprnewst*);
	void traverse();
};

class exprnewst:public fieldCode{
public:
	class arithmeticst* arthm;
	string str;
	int num;
public:
	exprnewst(class arithmeticst*);
	exprnewst(string);
	exprnewst(int);
	void traverse();
};

class condsst:public fieldCode{
private:
	vector<class condst*> condlist;
public:
	condsst(){};
	void push_back(class condst*);
	vector<class condst*> getCondList();
	void traverse();
};

class condst:public fieldCode{
public:
	class exprnewst* lhi;
	class exprnewst* rhi;
	string compopr, multcond;
public:
	condst(class exprnewst*, string, class exprnewst*, string);
	void traverse();
};

class ifelsest:public fieldCode{
private:
	class condsst* condition;
	class fieldCodes* ifblock;
	class fieldCodes* elseblock;
public:
	ifelsest(class condsst*,class fieldCodes*, class fieldCodes*);
	void traverse();
};

class whilest:public fieldCode{
private:
	class condsst* condition;
	class fieldCodes* block;
public:
	whilest(class condsst*, class fieldCodes*);
	void traverse();
};

class forst:public fieldCode{
private:
	class forloopinit* init;
	class fieldCodes* block;
public:
	forst(class forloopinit*, class fieldCodes*);
	void traverse();

};

class forloopinit:public fieldCode{
public:
	char * iteratorname;
	int start,finish,inc;
public:
	forloopinit(char*,int,int,int);
	void traverse();
};

class gotost:public fieldCode{
private:
	string labelname;
	class callst* call;
public:
	gotost(string,class callst*);
	void traverse();
};

class callst:public fieldCode{
private:
	class condsst* conds;
public:
	callst(class condsst*);
	void traverse();
};
