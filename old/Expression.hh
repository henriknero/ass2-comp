#ifndef EXPRESSION_H
#define EXPRESSION_H
#include "BlockThree.hh"
extern map<string,int> vars;
class Expression
{
private:
		static int eCounter;
public:
        string name;
        Expression() : name("")
        {
        }
        virtual string makeNames() 	
        {
			
			name = "_t" + to_string(eCounter++);
			vars[name] = 0;
			return name;
        }
		virtual void dump(int depth)
		{
		}
        virtual string convert(BBlock*) = 0; // Lecture 8 / slide 12.
};

class Add : public Expression
{
public:
        Expression *lhs, *rhs;

        Add(Expression* lhs, Expression* rhs) :
                lhs(lhs), rhs(rhs)
        {
        }

		virtual void dump(int depth)
		{
			for (int i = 0; i<depth;i++)
				cout << "  " ;
			cout << "+" << endl;
			lhs->dump(depth+1);
			rhs->dump(depth+1);
		}
        string convert(BBlock* out)
        {
				string left = lhs->convert(out);
				string right = rhs->convert(out);
				name = this->makeNames();
              	out->instructions.push_back(ThreeAd(name, '+', left,right));
				// Write three address instructions to output
				return name;
        }

};
class Sub : public Expression
{
public:
        Expression *lhs, *rhs;

        Sub(Expression* lhs, Expression* rhs) :
                lhs(lhs), rhs(rhs)
        {
        }

		virtual void dump(int depth)
		{
			for (int i = 0; i<depth;i++)
				cout << "  " ;
			cout << "-" << endl;
			lhs->dump(depth+1);
			rhs->dump(depth+1);
		}
        string convert(BBlock* out)
        {
				string left = lhs->convert(out);
				string right = rhs->convert(out);
				name = this->makeNames();
              	out->instructions.push_back(ThreeAd(name, '-', left,right));
				// Write three address instructions to output
				return name;
        }

};
class Mult : public Expression
{
public:
        Expression *lhs, *rhs;

        Mult(Expression* lhs, Expression* rhs) :
                lhs(lhs), rhs(rhs)
        {
        }

		virtual void dump(int depth)
		{
			for (int i = 0; i<depth;i++)
				cout << "  " ;
			cout << "*" << endl;
			lhs->dump(depth+1);
			rhs->dump(depth+1);
		}
        string convert(BBlock* out)
        {
			string left = lhs->convert(out);
			string right = rhs->convert(out);
			name = this->makeNames();
			out->instructions.push_back(ThreeAd(name,'*',left,right));
                // Write three address instructions to output
			return name;
        }
};
class Div : public Expression
{
public:
        Expression *lhs, *rhs;

        Div(Expression* lhs, Expression* rhs) :
                lhs(lhs), rhs(rhs)
        {
        }

		virtual void dump(int depth)
		{
			for (int i = 0; i<depth;i++)
				cout << "  " ;
			cout << "/" << endl;
			lhs->dump(depth+1);
			rhs->dump(depth+1);
		}
        string convert(BBlock* out)
        {
			string left = lhs->convert(out);
			string right = rhs->convert(out);
			name = this->makeNames();
			out->instructions.push_back(ThreeAd(name,'/',left,right));
                // Write three address instructions to output
			return name;
        }
};
class Square : public Expression
{
public:
        Expression *lhs, *rhs;

        Square(Expression* lhs, Expression* rhs) :
                lhs(lhs), rhs(rhs)
        {
        }

		virtual void dump(int depth)
		{
			for (int i = 0; i<depth;i++)
				cout << "  " ;
			cout << "^" << endl;
			lhs->dump(depth+1);
			rhs->dump(depth+1);
		}
        string convert(BBlock* out)
        {
			string left = lhs->convert(out);
			string right = rhs->convert(out);
			name = this->makeNames();
			out->instructions.push_back(ThreeAd(name,'^',left,right));
                // Write three address instructions to output
			return name;
        }
};
class Constant : public Expression
{
public:
        double value;
		
		Constant(double v): value(v) {}

		virtual void dump(int depth)
		{
			for (int i = 0; i<depth;i++)
				cout << "  " ;
			cout << value << endl;
		}
		virtual string makeName()
		{
			return to_string(value);
		}
		string convert(BBlock* out)
        {
				return to_string(value);
                // Write three address instructions to output
        }

};
class Variable : public Expression
{
public:
	string name;
	Variable(string lhs): name(lhs) {}

		virtual void dump(int depth)
		{
			for (int i = 0; i<depth;i++)
				cout << "  " ;
			cout << name << endl;
		}
		virtual string makeName()
		{
			vars[name] = 0;
			return name;
		}
	string convert(BBlock* out)
    {
		return this->makeName();
            // Write three address instructions to output
    }

};
class Varlist : public Expression
{
public:
	string name;
	list<Variable*> vars;
	Varlist() {}

		virtual void dump(int depth)
		{
			for (int i = 0; i<depth;i++)
				cout << "  " ;
			cout << name << endl;
			for(auto i: vars)
				i->dump(depth);
		}
		virtual string makeName()
		{
	//		vars[name] = 0;
	//		return name;
		}
	string convert(BBlock* out)
    {
		return this->makeName();
            // Write three address instructions to output
    }

};
class Equality : public Expression
{
public:
	Expression *lhs, *rhs;

	Equality(Expression *l, Expression *r): lhs(l), rhs(r) {}
		
		virtual void dump(int depth)
		{
			for (int i = 0; i<depth;i++)
				cout << "  ";
			cout << "==" << endl;
				lhs->dump(depth+1);
				rhs->dump(depth+1);
		}
	string convert(BBlock* out)
    {
		string left = lhs->convert(out);
		string right = rhs->convert(out);
		string name = this->makeNames();
		out->instructions.push_back(ThreeAd(name,'=',left,right));
		return name;
    }


};
#endif
