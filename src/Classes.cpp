#include <bits/stdc++.h>
#include "ClassDefs.h"

using namespace std;
using namespace llvm;

#define TBS printTabs()
#define outs(x)cout<<#x<<" is "<<x<<endl
ofstream out("XML_Vistor.txt");
int tabs_needed = 0;
const int tab_width = 4;
extern int errors;
/* Usefull Variables */
static Module *TheModule = new Module("Decaf compiler jit",llvm::getGlobalContext());
static LLVMContext &Context = getGlobalContext();
static IRBuilder<> Builder(Context);
static std::map<std::string, llvm::AllocaInst *> NamedValues;
static FunctionPassManager *TheFPM;

/* Usefull Functions */

static AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, const std::string &VarName, string type) {
  /* Allocates memory for local variables  on the stack of the function */

  /* Get the builder for current context */
  IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  AllocaInst* Alloca;
  if(type == "int"){
    Alloca = TmpB.CreateAlloca(Type::getInt32Ty(getGlobalContext()), 0, VarName.c_str());
  }
  else if (type == "boolean"){
    Alloca = TmpB.CreateAlloca(Type::getInt1Ty(getGlobalContext()), 0, VarName.c_str());
  }
  return Alloca;
}

string getOperation(string opr){
if(opr.compare("+") == 0){
  return string("Addition");
}
else if (opr.compare("-") == 0){
  return string("Subtraction");
}
else if(opr.compare("*")  == 0){
  return string("Multiplication");
}
else if (opr.compare("/")  == 0){
  return string("Division");
}
else if (opr.compare("%") == 0){
  return string("Modulus");
}
else if (opr.compare("<") == 0){
  return string("Less_than");
}
else if (opr.compare(">") == 0){
  return string("Greater_than");
}
else if (opr.compare("<=") == 0){
  return string("Less_than_Equal_to");
}
else if (opr.compare(">=") == 0){
  return string("Greater_than_Equal_to");
}
else if (opr.compare("==") == 0){
  return string("Compare_equal");
}
else if (opr.compare("&&") == 0){
  return string("Conditional_and");
}
else if (opr.compare("||") == 0){
  return string("Conditional_or");
}
else if (opr.compare("=") == 0){
  return string("Equal_assign");
}
else if (opr.compare("-=") == 0){
  return string("Subequal_assign");
}
else if (opr.compare("=") == 0){
  return string("Addequal_assign");
}
}

string replace_newline(string str){
size_t index = 0;
string search="\\n";
//  cout << "Called replace for " << str << endl;
while (true) {
  /* Locate the substring to replace. */
  index = str.find(search, index);
  if (index == std::string::npos) break;

  /* Make the replacement. */
//  cout << "Replaced\n";
  str.erase( index, search.length() );
  str.insert( index, "\n" );

  /* Advance index forward so the next iteration doesn't pick it up as well. */
  index += 1;
}
return str;
}

void printTabs(){
for(int i = 0; i < tabs_needed; i++){
  for(int j = 0; j < tab_width; j++)
  out << " ";
}
}

/* --------------------- Constructors ---------------------*/
// Var::Var(string declType, string name, unsigned int length){
// this->declType = declType;
// this->name = name;
// this->length = length;
// }


Var::Var(string declType, string name){

string strtemp="";
cout << name << "\n";
// cout << declType << "\n";
if(declType=="Array")
{
  for(int i=0;name[i]!='[' && name[i]!=0;i++)
    strtemp+=name[i];
  name=strtemp;
}
this->name = name;
this->declType = declType;
}


fieldDecl::fieldDecl(class Vars* vars){
this->var_list = vars->getVarsList();
// for(int i = 0; i < var_list.size(); i++){
//   var_list[i]->setDataType(dataType);
// }
}
Prog::Prog(class fieldDecls* fields, class fieldCodes* codes){
  this->decls=fields;
  this->codes=codes;
}

arithmeticst::arithmeticst(class exprnewst* lho, string op, class exprnewst* rho){
  // cout << op <<"\n";
  this->lho=lho;
  this->op=op;
  this->rho=rho;
}
expr::expr(string str,class exprnewst* exprnew){
  this->lhs=str;
  this->rhs=exprnew;
}
exprnewst::exprnewst(class arithmeticst* arthm){
  this->arthm=arthm;
}
exprnewst::exprnewst(string str){
  string temp="";
  for(int i=0;str[i]!='[' && str[i]!=0;i++)
    temp+=str[i];
  str=temp;
  this->str=str;
}
exprnewst::exprnewst(int num){
this->num=num;
}
cond::cond(class exprnewst* lhi, string compopr, class exprnewst* string multcond){
  this->lhi=lhi;
  this->compopr=compopr;
  this->rhi=rhi;
  this->multcond=multcond;
}

string Var::getName(){
  return name;
}

vector< class Var*> Vars::getVarsList(){
  return vars_list;
}

void Vars::push_back(class Var* var){
  vars_list.push_back(var);
}

vector<class Var*> fieldDecl::getVarsList(){
  return var_list;
}

fieldDecls::fieldDecls(){
this->cnt = 0;
}
void fieldDecls::push_back(class fieldDecl*var){
  decl_list.push_back(var);
  cnt++;
}

void fieldCodes::push_back(class fieldCode*var){
  fieldcodes.push_back(var);
  cnt++;
}
vector<class cond*> conds::getCondList(){
  return condlist;
}
void conds::push_back(class cond* cond){
  condlist.push_back(cond);
}
