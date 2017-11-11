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
	class thingpsst* pr;
	class thingrsst* rd;

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
		pr=NULL;
		rd=NULL;
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
public:
	string declType;
	string name;
public:
	Var(string,string);
	void traverse();
	string getName();
};

class Vars:public astNode{
public:
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
public:
	string dataType;
	vector<class Var*> var_list;
public:
	fieldDecl(class Vars*);
	vector<class Var*> getVarsList();
	void traverse();
	Value* codegen();
};

class fieldDecls:public astNode{
public:
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
	virtual void traverse(){}
	virtual int trav(){}
	virtual Value* codegen(){}
	// virtual void traverse(){}
	// virtual int traverse(){}
};

class fieldCodes:public astNode{
public:
	vector<class fieldCode* > fieldcodes;
	int cnt;
public:
	fieldCodes(){}
	void push_back(class fieldCode *);
	void traverse();
	Value *codegen();
};

class Prog:public astNode{
public:
	class fieldDecls* decls;
	class fieldCodes* codes;
public:
	Prog(class fieldDecls*, class fieldCodes*);
	void traverse();
	Value* codegen();
	void codeDump();
};

class expr:public fieldCode{
public:
	string lhs;
	class exprnewst* rhs;
public:
	expr(string, class exprnewst*);
	void traverse();
	Value * codegen();
};


class arithmeticst:public fieldCode{
public:
	string op;
	class exprnewst* lho;
	class exprnewst* rho;
public:
	arithmeticst(class exprnewst*,string, class exprnewst*);
	int trav();
	Value* codegen();
};

class exprnewst:public fieldCode{
public:
	class arithmeticst* arthm;
	string str;
	int num;
	int typeExprflag;
public:
	exprnewst(class arithmeticst*);
	exprnewst(string);
	exprnewst(int);
	int trav();
	Value * codegen();
};

class condsst:public fieldCode{
public:
	vector < pair <class condst*, string> > condlist;
public:
	condsst(){};
	void push_back(class condst*,string);
	vector<class condst*> getCondList();
	void traverse(){};
	int trav();
	Value *codegen(){};
};

class condst:public fieldCode{
public:
	class exprnewst* lhi;
	class exprnewst* rhi;
	string compopr;
public:
	condst(class exprnewst*, string, class exprnewst*);
	void traverse(){};
	int trav();
	Value *codegen(){};
};

class ifelsest:public fieldCode{
public:
	class condsst* condition;
	class fieldCodes* ifblock;
	class fieldCodes* elseblock;
public:
	ifelsest(class condsst*,class fieldCodes*, class fieldCodes*);
	void traverse();
	int trav(){};
	Value *codegen(){};
};

class whilest:public fieldCode{
public:
	class condsst* condition;
	class fieldCodes* block;
public:
	whilest(class condsst*, class fieldCodes*);
	void traverse();
	int trav(){};
	Value *codegen(){};
};

class forst:public fieldCode{
public:
	class forloopinit* init;
	class fieldCodes* block;
public:
	forst(class forloopinit*, class fieldCodes*);
	void traverse();
	int trav(){};
	Value *codegen(){};
};

class forloopinit:public fieldCode{
public:
	char * iteratorname;
	int start,finish,inc;
public:
	forloopinit(char*,int,int,int);
	void traverse(){};
	int trav();
	Value *codegen(){};
};

class gotost:public fieldCode{
public:
	string labelname;
	class callst* call;
	class fieldCodes* codes;

public:
	gotost(class fieldCodes*,string,class callst*);
	void traverse();
	int trav(){};
	Value *codegen(){};
};

class callst:public fieldCode{
public:
	class condsst* conds;
public:
	callst(class condsst*);
	void traverse(){};
	int trav();
	Value *codegen(){};
};

class thingpsst:public fieldCode{
public:
	vector<string> printList;
public:
	thingpsst(){}
	void traverse();
	int trav(){};
	void push_back(string);
	Value *codegen(){};
};

class thingrsst:public fieldCode{
public:
	vector<string> readList;
public:
	thingrsst(){}
	void traverse(){};
	int trav(){};
	void push_back(string);
	Value *codegen(){};
};
