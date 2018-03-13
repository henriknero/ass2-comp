#ifndef BLOCKTHREE_HH
#define BLOCKTHREE_HH
#include <list>
#include <set>
#include <initializer_list>
#include <string>
#include <iostream>
#include <map>
using namespace std;
class ThreeAd
{
public:
        string name,lhs,rhs;
        string op;

        ThreeAd(string name, string op, string lhs, string rhs) :
                name(name), op(op), lhs(lhs), rhs(rhs)
        {
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
			else if (op == "list")
				cout << "double " << name << "["<< lhs << "];" << endl;
			else if (op == "[]")
				cout << name << "[" << lhs << "]" << "=" << rhs << ";" <<  endl;
			else if (op == "l")
				cout << "memcpy(&" << name << ",&" << lhs << ",sizeof(" << rhs << "));" << endl;
			else if (op == "#")
				cout << name << " = sizeof(" << lhs << ")/sizeof(" << lhs << "[0]);" << endl;  
			else if (op == "+")//Arithmetics
				cout << name << " = " << lhs << " + " << rhs << ";" << endl;
			else if (op == "-")
				cout << name << " = " << lhs << " - " << rhs << ";" << endl;
			else if (op == "c")
				cout << name << " = " << lhs << ";" << endl;
			else if (op == "*")
				cout << name << " = " << lhs << " * " << rhs << ";" << endl;
			else if (op == "/")
				cout << name << " = " << lhs << "/ double(" << rhs << ");" << endl;
			else if (op == "%")
				cout << name << " = fmod(" << lhs << "," << rhs << ");" << endl;
			else if (op == "^")
				cout << name << " = pow(" << lhs <<"," <<  rhs << ");" << endl;
			else if (op == "call")//Functinons
			{
				if(lhs == "print")
					cout << "cout << " << rhs << " << endl;" << endl;
				else if (lhs == "io.read")
					cout << "char temp[256];" << endl <<"fgets(temp,255,stdin);" << endl << name << "= atof(temp);" << endl;	
				else if (lhs == "io.write")
					cout << "cout << " << rhs << ";" << endl;
			}
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
