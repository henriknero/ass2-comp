#ifndef STATEMENT_H
#define STATEMENT_H
/******************** Statements ********************/
#include "Expression.hh"
class Statement
{
public:
        string name;

        Statement()
        {
        }
		virtual void dump(int depth)
		{
				
		}
        virtual BBlock* convert(BBlock *) = 0;

};


class Assignment : public Statement
{
public:
        Variable *lhs;
        Expression *rhs;

        Assignment(string lhs, Expression *rhs) :
                lhs(new Variable(lhs)), rhs(rhs)
        {
        }
		Assignment() {}

		virtual void dump(int depth)
		{
			for(int i = 0; i<depth;i++)
				cout << "  ";
			cout << "Statement(A)" << endl;
			lhs->dump(depth+1);
			rhs->dump(depth+1);
		}
        BBlock* convert(BBlock *out)
        {
			string left = lhs->convert(out);
			string right = rhs->convert(out);
			out->instructions.push_back(ThreeAd(left, 'c', right,right));
			// Write three address instructions to output
			return out;
        }
};
class SFunc : public Statement
{
public:
        Variable *lhs;
        Expression *rhs;

        SFunc(string lhs, Expression *rhs) :
                lhs(new Variable(lhs)), rhs(rhs)
        {
        }
		SFunc() {}

		virtual void dump(int depth)
		{
			for(int i = 0; i<depth;i++)
				cout << "  ";
			cout << "sFunc" << endl;
			lhs->dump(depth+1);
			rhs->dump(depth+1);
		}
        BBlock* convert(BBlock *out)
        {
			string left = lhs->convert(out);
			string right = rhs->convert(out);
			out->instructions.push_back(ThreeAd(left, 'c', right,right));
			// Write three address instructions to output
			return out;
        }
};

class If : public Statement
{
public:
	Expression *condition;
	Statement *iftrue;
	Statement *iffalse;
	
	If(Expression * c, Statement *t, Statement *f): condition(c), iftrue(t), iffalse(f) {}
	
		virtual void dump(int depth)
		{
			for(int i = 0; i<depth;i++)
				cout << "  ";
			cout << "Statement(I)" << endl;	
			condition->dump(depth+1);
			iftrue->dump(depth+1);
			iffalse->dump(depth+1);
		}
    BBlock* convert(BBlock *out)
    {
		condition->convert(out);
		BBlock *trueExit = new BBlock();
		BBlock *falseExit = new BBlock();
		BBlock *endBlock = new BBlock();
		out->tExit = trueExit;
		out->fExit = falseExit;
		
		iftrue->convert(trueExit)->tExit = endBlock;
		iffalse->convert(falseExit)->tExit = endBlock;
        // Write three address instructions to output
		return endBlock;
   }
};
/************* Seq? ***********/
class Seq : public Statement
{
public:
	list<Statement*> statements;	
	
	Seq(initializer_list<Statement*> stats): statements(stats) {}
	Seq() {}
	BBlock* convert(BBlock *out)
    {
			for(auto i: statements)
				out = i->convert(out);
			return out;
            // Write three address instructions to output
    }
		virtual void dump(int depth)
		{
			cout << "Statement(S)" << endl;
			for(auto i:statements){
				for(int i = 0; i<depth;i++)
					cout << "  ";
				i->dump(depth+1);
			}

		}
};
#endif
