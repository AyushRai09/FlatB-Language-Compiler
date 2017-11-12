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
static FunctionPassManager *TheFPM;
map <string,int> symbolTable;
int st,en,in;
string itname;
map<string,pair<class fieldCodes*,class callst*> >gotomap;
map<string,vector < int > >arrmap;

FunctionType *FT = llvm::FunctionType::get(Builder.getVoidTy(),false);
Function *F = llvm::Function::Create(FT, Function::ExternalLinkage, "codeblock", TheModule);
// vector< pair< string, pair<class fieldCodes*,class callst*> > > gotovec;
// vector< pair < pair <string,class fieldCodes*>, class callst* > > gotovec;
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
while (true) {
  /* Locate the substring to replace. */
  index = str.find(search, index);
  if (index == std::string::npos) break;

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

///////////////////////////***********Constructors****************////////////////////////////??????
Var::Var(string declType, string name){

string strtemp="";
string sz;
int len1;
if(declType=="Array")
{
  int osb=name.find('[');
  int i;
  for(i=0;name[i]!='[' && name[i]!=0;i++)
    strtemp+=name[i];
  name=strtemp;
  for(i=osb+1;name[i]!=']';i++)
    sz+=name[i];
  // cout << "osb:"<< osb << " " << "sz:" << sz << "\n";
  len1=atoi(sz.c_str());
  for(i=0;i<len1;i++)
    arrmap[strtemp].push_back(0);
  //cout << strtemp << " " << len1 << "\n";
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
fieldDecls::fieldDecls(){
this->cnt = 0;
}

Prog::Prog(class fieldDecls* fields, class fieldCodes* codes){
  this->decls=fields;
  this->codes=codes;
}

arithmeticst::arithmeticst(class exprnewst* lho, string op, class exprnewst* rho){
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
  this->typeExprflag=1;
}
exprnewst::exprnewst(string str){
  string temp="";
  // for(int i=0;str[i]!='[' && str[i]!=0;i++)
  //   temp+=str[i];
  // str=temp;
  this->str=str;
  this->typeExprflag=2;
}
exprnewst::exprnewst(int num){
this->num=num;
this->typeExprflag=3;
}
condst::condst(class exprnewst* lhi, string compopr, class exprnewst* rhi){
  this->lhi=lhi;
  this->compopr=compopr;
  this->rhi=rhi;
}
ifelsest::ifelsest(class condsst* condition, class fieldCodes* ifblock, class fieldCodes* elseblock){
  this->condition=condition;
  this->ifblock=ifblock;
  this->elseblock=elseblock;
}

whilest::whilest(class condsst* condition, class fieldCodes* block){
  this->condition=condition;
  this->block=block;
}
forst::forst(class forloopinit* init, class fieldCodes* block){
  this->init=init;
  this->block=block;
}
forloopinit::forloopinit(char* iteratorname, int start, int finish, int inc){
  this->iteratorname=iteratorname;
  this->start=start;
  this->finish=finish;
  this->inc=inc;
}
gotost::gotost(class fieldCodes* codes, string labelname, class callst* call){
  this->codes=codes;
  this->labelname=labelname;
  this->call=call;
  gotomap[labelname]=make_pair(codes,call);
  // gotovec.push_back(make_pair(labelname,make_pair(codes,call)));
  // gotovec.push_back(make_pair(make_pair(labelname, codes),call));
}
callst::callst(class condsst* conds){
  this->conds=conds;
}

////////////////////////////////////*****************Class defined functions******************////////////////////////////////
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

void fieldDecls::push_back(class fieldDecl*var){
  decl_list.push_back(var);
  cnt++;
}

void fieldCodes::push_back(class fieldCode*var){
  fieldcodes.push_back(var);
  cnt++;
}
// vector<class condst*> condsst::getCondList(){
//   return condlist;
// }
void condsst::push_back(class condst* cond, string str){
  condlist.push_back(make_pair(cond,str));
}
void thingpsst::push_back(string item){
  printList.push_back(item);
}
void thingrsst::push_back(string item){
  readList.push_back(item);
}
////////////////////////////////////////////////////*****TRAVERSALS************/////////////////////////////////////////////////
void Prog::traverse(){
  decls->traverse();
  codes->traverse();
  //   for (auto it = symbolTable.begin(); it != symbolTable.end(); ++it) {
  //     // iterator is a pair of (key, value)
  //     cout << it->first <<"\n"; // key
  //     cout << it->second << "\n"; // value
  // }
}

void fieldDecls::traverse(){

  int i;
  for(i=0;i<decl_list.size();i++)
    decl_list[i]->traverse();
}
void fieldDecl::traverse(){
  int i;
  for(i=0;i<var_list.size();i++)
    var_list[i]->traverse();
}
void Var::traverse(){
  symbolTable[name]=0;
}

void fieldCodes::traverse(){
  int i;
  for(i=0;i<fieldcodes.size();i++)
    fieldcodes[i]->traverse();
}
void expr::traverse(){
  if(lhs.find("[")!=-1)
    {

      int osb=lhs.find("["); string ind; int index;string name;
      for(int i=0;lhs[i]!='[';i++)
        name+=lhs[i];
      for(int i=osb+1;lhs[i]!=']';i++)
        ind+=lhs[i];
      index=atoi(ind.c_str());
      if(to_string(index)!=ind)
        arrmap[name][symbolTable[ind]]=rhs->trav();
      else
        arrmap[name][index]=rhs->trav();
        // cout << to_string(index) << " " << ind << " " << "\n";
        // cout << arrmap[name][symbolTable[ind]] << "\n";
    }
  else
    {
  //    cout << symbolTable[lhs] << "\n";
      symbolTable[lhs]=rhs->trav();
    }
}
int exprnewst::trav(){
  if(typeExprflag==1) //typeExprflag==1 means that the argument is of type class arithmeticst*
    return arthm->trav();
  else if(typeExprflag==2)  //typeExprflag==2 means that the argument is of type string
  {
    if(str.find("[")!=-1)
    {

      int osb=str.find("["); string ind; int index;string name;
        for(int i=0;str[i]!='[';i++)
          name+=str[i];
        for(int i=osb+1;str[i]!=']';i++)
          ind+=str[i];
        index=atoi(ind.c_str());

        if(to_string(index)!=ind){
          return arrmap[name][symbolTable[ind]];}
        else
          return arrmap[name][index];
    }
    else
      return symbolTable[str];
  }
  else if(typeExprflag==3) //typeExprflag==3 means that the argument is of int
    return num;
}
int arithmeticst::trav(){
  int lhv=lho->trav();
  int rhv=rho->trav();
  if(op=="+")
    return lhv+rhv;
  else if(op=="-")
    return lhv-rhv;
  else if(op=="*")
    return lhv*rhv;
  else if(op=="/")
    return lhv/rhv;
 }
void ifelsest::traverse(){

  int flg=0;
  if(elseblock!=NULL)
    flg=1;
  int condFlag=condition->trav();
  if(condFlag)
    ifblock->traverse();
  if(flg==1)
    elseblock->traverse();
}

int condsst::trav(){

  int i=0,condFlag=1,temp=1;
  for(i=condlist.size()-1;i>=0;i--)
  {
    if(temp==1)//and
      condFlag=condFlag && condlist[i].first->trav();
    if(temp==2)//or
      condFlag=condFlag || condlist[i].first->trav();

    if(condlist[i].second=="and")
          temp=1;
    else if(condlist[i].second=="or")
        temp=2;
    else if(condlist[i].second=="")
        temp=3;
  }

  return condFlag;
}

int condst::trav(){
  int l=lhi->trav();
  int r=rhi->trav();
  if(compopr==">")
    return l > r;
  else if(compopr=="<")
    return l < r;
  else if(compopr=="==")
    return l==r;
  else if(compopr==">=")
    return l>=r;
  else if(compopr=="<=")
    return l<=r;
}

void whilest::traverse(){
  int condFlag;
  while(1)
  {
    condFlag=condition->trav();
    if(condFlag==0)
      break;
    else
      block->traverse();
  }
}

void forst::traverse(){
  init->trav();
  int i;
  symbolTable[itname]=st;
  // cout << "itname:" << itname << "\n";
  //
  // cout << "st:" << st << " " << "en:" << en << " " << "in:" << in << "\n";
  for(i=st;i<=en;i=i+in)
  {
    block->traverse();
    symbolTable[itname]+=in;
  }
}
int forloopinit::trav(){
  int i;char* out;
  for(i=0;iteratorname[i]!=0;i++){}

  out=(char*)malloc(sizeof(char)*i);
  for(i=0;iteratorname[i]!=0;i++)
    out[i]=iteratorname[i];
  itname=string(out);
  st=start;
  en=finish;
  in=inc;
  return 0;
}
void gotost::traverse(){

  while(gotomap[labelname].second->trav())
    gotomap[labelname].first->traverse();

}
int callst::trav(){
  return conds->trav();
}
void thingpsst::traverse(){
  int i;
  for(i=0;i<printList.size();i++)
    cout << symbolTable[printList[i]] << "\n";
}

//***************************Codegens***********************************//
Value* Prog::codegen(){
  Value *v;
  v=decls->codegen();
  BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
  Builder.SetInsertPoint(BB);

  v=codes->codegen();
  return v;
}

void Prog::codeDump(){
  cout << "Generating LLVM IR Code" << "\n";
  TheModule->dump();
}

Value* fieldDecls::codegen(){
  int i;
  for(i=0;i<decl_list.size();i++)
    decl_list[i]->codegen();
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,500));
  return v;
}

Value* fieldDecl::codegen(){
  int i;
  for(i=0;i<var_list.size();i++)
  {
    class Var* var=var_list[i];
    llvm::Type *ty;
    ty=Type::getInt32Ty(Context);
    if(var->declType=="Identifier")
    {
      PointerType* ptrTy = PointerType::get(ty,0);
      GlobalVariable* gv = new GlobalVariable(*TheModule, ptrTy ,false,GlobalValue::ExternalLinkage, 0, var->name);
    }
  }
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,500));
  return v;
}

Value *fieldCodes::codegen(){
  int i;
  for(i=0;i<fieldcodes.size();i++)
    fieldcodes[i]->codegen();
  Value* v = ConstantInt::get(getGlobalContext(), APInt(32,500));
  return v;
}

Value *expr::codegen(){
  Value *v= TheModule->getNamedGlobal(lhs);
  Value *rhseval=rhs->codegen();
  // v=Builder.CreateLoad(v);
  return Builder.CreateStore(rhseval,TheModule->getNamedGlobal(lhs));
}

Value *exprnewst::codegen(){
  if(typeExprflag==1)
  {
      Value *v= arthm->codegen();
      return v;
  }

  else if(typeExprflag==2)
  {
    Value *v=TheModule->getNamedGlobal(str);
    v=Builder.CreateLoad(v);
    return v;
  }
  else if(typeExprflag==3)
  {
    Value* v = ConstantInt::get(getGlobalContext(), APInt(32,num));
    // cout << "dsfdsfa" << num << "\n";
    return v;
  }

}

Value *arithmeticst::codegen(){
  Value *lhseval=lho->codegen();
  Value *rhseval=rho->codegen();
  Value *v;
  if(op=="+")
    v=Builder.CreateAdd(lhseval,rhseval,"addtmp");
  else if(op=="-")
    v=Builder.CreateSub(lhseval,rhseval,"subtmp");
  else if(op=="*")
    v=Builder.CreateMul(lhseval,rhseval,"multmp");
  else if(op=="/")
    v=Builder.CreateUDiv(lhseval,rhseval,"divtmp");
  return v;
}

Value* ifelsest::codegen(){
  Value* condeval=condition->codegen();
  BasicBlock* ifblockeval=BasicBlock::Create(Context,"ifblock",F);
  BasicBlock* elseblockeval=BasicBlock::Create(Context,"elseblock");
  BasicBlock* mergeblockeval=BasicBlock::Create(Context,"ifcontinue");
  Builder.CreateCondBr(condeval,ifblockeval,elseblockeval);

  Builder.SetInsertPoint(ifblockeval);
  Value* ifval = ifblock->codegen();
  Builder.CreateBr(mergeblockeval);
  // ifblockeval = Builder.GetInsertBlock();
  F->getBasicBlockList().push_back(elseblockeval);

  Builder.SetInsertPoint(elseblockeval);
  Value *elseval;
  if(elseblock!=NULL)
  {
    elseval= elseblock->codegen();
  }
  Builder.CreateBr(mergeblockeval);
  // elseblockeval = Builder.GetInsertBlock();
  F->getBasicBlockList().push_back(mergeblockeval);

  Builder.SetInsertPoint(mergeblockeval);
  // PHINode *Phi=Builder.CreatePHI(Type::getInt32Ty(getGlobalContext()),2,"iftmp");
  // Phi->addIncoming(ifval,ifblockeval);
  // Phi->addIncoming(elseval,elseblockeval);
  Value *V = ConstantInt::get(getGlobalContext(), APInt(32,500));
  return V;
  // return Phi;
}

Value *condsst::codegen(){
  int i;
  for(i=0;i<condlist.size();i++)
  {
    return condlist[i].first->codegen();
  }
  Value *v =ConstantInt::get(getGlobalContext(), APInt(32,1));
  return v;
}

Value *condst::codegen(){
  Value * lhieval= lhi->codegen();
  Value * rhieval= rhi->codegen();
  Value *v;
  if(compopr==">")
    {
      cout << "Hi ayush rai" << "\n";
      v=Builder.CreateICmpUGT(lhieval,rhieval,"gtcomparetmp");
    }
  else if(compopr=="<")
    v=Builder.CreateICmpULT(lhieval, rhieval,"ltcomparetmp");
  else if(compopr==">=")
    v=Builder.CreateICmpUGE(lhieval,rhieval,"gecomparetmp");
  else if(compopr=="<=")
    v=Builder.CreateICmpULE(lhieval,rhieval,"lecomparetmp");
  else if(compopr=="==")
    v=Builder.CreateICmpEQ(lhieval,rhieval,"compopromparetmp");
  return v;
}

Value *forst::codegen(){
  Value* starteval=ConstantInt::get(getGlobalContext(), APInt(32,init->start));
  Value* itreval=TheModule->getNamedGlobal(init->iteratorname);
  Value* finisheval=ConstantInt::get(getGlobalContext(), APInt(32,init->finish));
  Value* inceval=ConstantInt::get(getGlobalContext(), APInt(32,init->inc));


  Builder.CreateStore(starteval,itreval);
  BasicBlock *upperBB=Builder.GetInsertBlock();
  BasicBlock* body= BasicBlock::Create(getGlobalContext(), "loop",F);
  Builder.CreateBr(body);

  Builder.SetInsertPoint(body);
  PHINode *Variable = Builder.CreatePHI(Type::getInt32Ty(llvm::getGlobalContext()), 2, init->iteratorname);
  Variable->addIncoming(starteval, upperBB);
  block->codegen();


  Value *curvar=Builder.CreateLoad(itreval);
  Value *updatedcurvar=Builder.CreateAdd(curvar,inceval,"UpdatedIterator");
  // Builder.CreateStore(updatedcurvar,itreval);

  Value *cond = Builder.CreateICmpULE(updatedcurvar, finisheval, "loopcondition");
  BasicBlock *endBB = Builder.GetInsertBlock();
  BasicBlock* afterloop=BasicBlock::Create(getGlobalContext(),"afterloop",F);
  Builder.CreateCondBr(cond,body,afterloop);
  Builder.SetInsertPoint(afterloop);
  Variable->addIncoming(updatedcurvar, endBB);

  return ConstantInt::get(getGlobalContext(), APInt(32,500));
}

Value *whilest::codegen(){
  BasicBlock* body= BasicBlock::Create(getGlobalContext(), "whileloop",F);
  BasicBlock* afterloop=BasicBlock::Create(getGlobalContext(),"afterloop",F);
  Value *whilecondeval=condition->codegen();
  Builder.CreateCondBr(whilecondeval,body,afterloop);


  Builder.SetInsertPoint(body);

  block->codegen();
  whilecondeval=condition->codegen();
  Builder.CreateCondBr(whilecondeval,body,afterloop);

  Builder.SetInsertPoint(afterloop);
  return ConstantInt::get(getGlobalContext(), APInt(32,500));
}
