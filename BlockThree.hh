#ifndef BLOCKTHREE_HH
#define BLOCKTHREE_HH
#include <list>
#include <set>
#include <initializer_list>
#include <string>
#include <iostream>
#include <map>
using namespace std;
extern map<string,double> vars;
class ThreeAd
{
private:
static int tCounter;
public:
        string name,lhs,rhs;
        string op;

        ThreeAd(string name, string op, string lhs, string rhs) :
                name(name), op(op), lhs(lhs), rhs(rhs)
        {
        }
		string makeNames()
		{
			return "_n" + to_string(tCounter++);
		}
        void dump()
        {
                cout << name << " <- ";
				if(rhs[0] == '"')
					cout << lhs << " " << op << " " << rhs.substr(1,rhs.size()-2) << "\\n";
				else
					cout << lhs << " " << op << " " << rhs << "\\n";
        }
		void dumpC()
		{
			if (op == "==")
				cout << "if(" << lhs << "==" << rhs << ") ";
			else if (op == "<=")
				cout << "if(" << lhs << "<=" << rhs << ") ";
			else if (op == ">")
				cout << "if(" << lhs << ">" << rhs << ") ";
			else if (op == "<")
				cout << "if(" << lhs << "<" << rhs << ") ";
			else if (op == "list")
				cout << "double " << name << "["<< lhs << "];" << endl;
			else if (op == "[]")
				cout << name << "[" << lhs << "]" << "=" << rhs << ";" <<  endl;
			else if (op == "l")
				cout << "memcpy(&" << name << ",&" << lhs << ",sizeof(" << rhs << "));" << endl;
			else if (op == "#")
				cout << name << " = sizeof(" << lhs << ")/sizeof(" << lhs << "[0]);" << endl;  
			else if (op == "+")//Arithmetics
			{
				//name = left + right
				string left = lhs;
				string right = rhs;
				if(vars.find(lhs) == vars.end())
					left = "n"+lhs;
				if(vars.find(rhs) == vars.end())
					right = "n"+rhs;
				cout << "asm(" << endl;
				cout << "	\"movsd \%[" << left << "], \%\%xmm0 \\n\\t\""<< endl;
				cout << "	\"movsd \%[" << right << "], \%\%xmm1 \\n\\t\"" << endl;
				cout << "	\"addsd \%\%xmm0, \%\%xmm1 \\n\\t\"" << endl;
				cout << "	\"movsd \%\%xmm1, \%[" << name << "] \\n\\t\"" << endl;
				cout << ": [" << name << "] \"=x\" (" << name << ")" << endl;
				cout << ": [" << left << "] \"x\" (double(" << lhs << "))";
				if(left != right)
					cout << ",\n" << "  [" << right << "] \"x\" (double(" << rhs << "))" << endl;
				cout << ": \"xmm0\", \"xmm1\",\"cc\"" << endl;
				cout << ");" << endl;
			}
			else if (op == "-")
			{
				string left = lhs;
				string right = rhs;
				if(vars.find(lhs) == vars.end())
					left = "n"+lhs;
				if(vars.find(rhs) == vars.end())
					right = "n"+rhs;
				cout << "asm(" << endl;
				cout << "	\"movsd \%[" << right << "], \%\%xmm0 \\n\\t\""<< endl;
				cout << "	\"movsd \%[" << left << "], \%\%xmm1 \\n\\t\"" << endl;
				cout << "	\"subsd \%\%xmm0, \%\%xmm1 \\n\\t\"" << endl;
				cout << "	\"movsd \%\%xmm1, \%[" << name << "] \\n\\t\"" << endl;
				cout << ": [" << name << "] \"=x\" (" << name << ")" << endl;
				cout << ": [" << left << "] \"x\" (double(" << lhs << "))";
				if(left != right)
					cout << ",\n" << "  [" << right << "] \"x\" (double(" << rhs << "))" << endl;
				cout << ": \"xmm0\", \"xmm1\",\"cc\"" << endl;
				cout << ");" << endl;
				//cout << name << " = " << lhs << " - " << rhs << ";" << endl;
			}
			else if (op == "c")
				cout << name << " = " << lhs << ";" << endl;
			else if (op == "*")
			{
				string left = lhs;
				string right = rhs;
				if(vars.find(lhs) == vars.end())
					left = "n"+lhs;
				if(vars.find(rhs) == vars.end())
					right = "n"+rhs;
				cout << "asm(" << endl;
				cout << "	\"movsd \%[" << left << "], \%\%xmm0 \\n\\t\""<< endl;
				cout << "	\"movsd \%[" << right << "], \%\%xmm1 \\n\\t\"" << endl;
				cout << "	\"mulsd \%\%xmm0, \%\%xmm1 \\n\\t\"" << endl;
				cout << "	\"movsd \%\%xmm1, \%[" << name << "] \\n\\t\"" << endl;
				cout << ": [" << name << "] \"=x\" (" << name << ")" << endl;
				cout << ": [" << left << "] \"x\" (double(" << lhs << "))";
				if(left != right)
					cout << ",\n" << "  [" << right << "] \"x\" (double(" << rhs << "))" << endl;
				cout << ": \"xmm0\", \"xmm1\",\"cc\"" << endl;
				cout << ");" << endl;
				//cout << name << " = " << lhs << " * " << rhs << ";" << endl;
			}
			else if (op == "/")
			{
				string left = lhs;
				string right = rhs;
				if(vars.find(lhs) == vars.end())
					left = "n"+lhs;
				if(vars.find(rhs) == vars.end())
					right = "n"+ rhs;
				cout << "asm(" << endl;
				cout << "	\"movsd \%[" << right << "], \%\%xmm0 \\n\\t\""<< endl;
				cout << "	\"movsd \%[" << left << "], \%\%xmm1 \\n\\t\"" << endl;
				cout << "	\"divsd \%\%xmm0, \%\%xmm1 \\n\\t\"" << endl;
				cout << "	\"movsd \%\%xmm1, \%[" << name << "] \\n\\t\"" << endl;
				cout << ": [" << name << "] \"=x\" (" << name << ")" << endl;
				cout << ": [" << left << "] \"x\" (double(" << lhs << "))";
				if(left != right)
					cout << ",\n" << "  [" << right << "] \"x\" (double(" << rhs << "))" << endl;
				cout << ": \"xmm0\", \"xmm1\",\"cc\"" << endl;
				cout << ");" << endl;
			}
			else if (op == "%")
				cout << name << " = fmod(" << lhs << "," << rhs << ");" << endl;
			else if (op == "^")
				cout << name << " = pow(double(" << lhs <<"),double(" <<  rhs << "));" << endl;
			else if (op == "call")//Functinons
			{
				if(lhs == "print")
				{
					if(rhs[0] == '"')
						cout << "printf(" << rhs.insert(rhs.size()-1,"\\n") << ");" << endl;
					else
						cout << "printf(\"\%.16g\\n\"," << rhs << ");" << endl;
				}
				else if (lhs == "io.read")
					cout << "char temp[256];" << endl <<"fgets(temp,255,stdin);" << endl << name << "= atof(temp);" << endl;	
				else if (lhs == "io.write")
					if(rhs[0] == '"')
						cout << "printf(" << rhs << ");" << endl;
					else
						cout << "printf(\"\%.16g\"," << rhs << ");" << endl;
				else
					cout << name << " = " << lhs << "(" << rhs << ");" << endl;
			}
			else if (op == "ret")
				cout << "return " << lhs << ";" << endl;
			else
			{
            	cout << name << " <- " ; 
				cout << lhs << " " << op << " " << rhs << endl;
				}
		}
};


/* Basic Blocks */
class BBlock
{
private:
        static int nCounter;
public:
        list<ThreeAd> instructions;
        BBlock *tExit, *fExit;
        string name;

        BBlock() :
                tExit(NULL), fExit(NULL), name("blk" + to_string(nCounter++))
        {
        }

        void dump()
        {
                cout << "BBlock @ " << this->name << "\\n";
                for(auto i : instructions)
                        i.dump();
				if (tExit)
					cout << "True:  " << tExit->name << endl;
				if (fExit)
					cout << "False: " << fExit->name << endl;
        }
		void dumpC()
		{
			cout << this->name <<":"<< endl;
			for(auto i: instructions)
				i.dumpC();
			if (tExit)
            	cout << "goto " << tExit->name << ";";
            if (fExit)
				cout << "else goto " << fExit->name << ";";
			if(tExit == NULL)
				cout << "return 0;";
			cout << endl;
		}
};
#endif
