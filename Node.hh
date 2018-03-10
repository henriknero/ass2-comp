#ifndef NODE_H
#define NODE_H
#include <list>
#include <string>
#include <iostream>
#include <math.h>
#include "BlockThree.hh"
extern map <string,double> vars;
class Node {
private:
	static int eCounter;
public:
	std::string tag, value;
	int id;
    std::list<Node> children;
	Node(std::string t, std::string v, int i) : tag(t), value(v), id(i) {}	
	Node() { tag="uninitialised"; value="uninitialised"; id =-1;}   // Bison needs this.
    void dump(int depth=0)
	{
    for(int i=0; i<depth; i++)
        std::cout << "  ";
    std::cout << tag << ":" << value << std::endl;
    for(std::list<Node>::iterator i=children.begin(); i!=children.end(); i++)
        (*i).dump(depth+1);
	}
	void dump_visual()
	{
		std::cout << id << " [label=\"" << tag << ":" << value << "\"];" << std::endl;	
		for (std::list<Node>::iterator i=children.begin(); i!=children.end();i++){
			(*i).dump_visual();
			std::cout << id << " -> " << (*i).id << ";" << std::endl;
		}
	}
	string makeNames()
	{
		string name = "_t" + to_string(eCounter++);
		vars[name] = 0.0;
		return name;
	}
//TODO Create list, handle unop and assign multiple variables	
	BBlock* convert(BBlock *out)
	{
		if (tag == "chunk")
		{
			for (auto i: children)
			{
				out = i.convert(out);
			}
			return out;
		}
		if (tag == "assign")
		{
			string left = children.front().convertExp(out);
			string right = children.back().convertExp(out);
			out->instructions.push_back(ThreeAd(left,"c",right,right));
			return out;
		}
		if (tag == "funccall")
		{
			string left = "_unused";
			string funcname = children.front().convertExp(out);
			string right = children.back().convertExp(out);
			out->instructions.push_back(ThreeAd(left,"call",funcname, right));
			return out;
		}
		if (tag == "for")
		{
			//for(i = 1; i<n;i++)
			auto i = children.begin();
			//Adding the counter (i)
			string var = i->convertExp(out);
			i++;
			//Assigning a value to the counter (1)
			string value = i->convertExp(out);
			i++;
			out->instructions.push_back(ThreeAd(var,"c",value,value));
			//Setting a max-value for counter (n)
			string upTo = i->convertExp(out);
			i++;
			//Declaring all the blcos for the for loop.
			BBlock *evalBlock = new BBlock();
			BBlock *trueBlock = new BBlock();
			BBlock *endBlock = new BBlock();
			
			//Setting evaluation
			string tmpName = this->makeNames();
			evalBlock->instructions.push_back(ThreeAd(tmpName,"+",var,"1"));
			evalBlock->instructions.push_back(ThreeAd("cmp","<", tmpName, upTo));
			//Setting pointers
			out->tExit = evalBlock;
			evalBlock->tExit = trueBlock;
			evalBlock->fExit = endBlock;
			trueBlock->tExit = evalBlock;
			i->convert(trueBlock)->tExit = evalBlock;

			return endBlock;

		}
	if (tag == "If")
	{
		auto i = children.begin();
		i->convertExp(out);
		i++;
		//Create Blocks for if Statement
		BBlock *trueExit = new BBlock();
		BBlock *falseExit = new BBlock();
		BBlock *endBlock = new BBlock();
		out->tExit = trueExit;
		out->fExit = falseExit;
		i->convert(trueExit)->tExit = endBlock;
		i++;
		i->convert(falseExit)->fExit = endBlock;
		return endBlock;
	}

	}
	string convertExp(BBlock *out)
	{
		if (tag == "varlist")
		{
			return children.front().convertExp(out);
		}
		else if (tag == "var")
		{
			string varName = "";
			varName += value;
			if(!children.empty())
				varName += "." + children.front().convertExp(out);
			return varName;
		}
		else if (tag == "explist")
			return children.front().convertExp(out);
		else if(tag == "args")
			return children.front().convertExp(out);
		else if(tag == "OP")
		{
			string left = children.front().convertExp(out);
			string right = children.back().convertExp(out);
			string name = this->makeNames();
			if(value == "+")
				out->instructions.push_back(ThreeAd(name,"+",left,right));
			else if(value == "-")
				out->instructions.push_back(ThreeAd(name,"-",left,right));
			else if(value == "*")
				out->instructions.push_back(ThreeAd(name,"*",left,right));
			else if(value == "/")
				out->instructions.push_back(ThreeAd(name,"/",left,right));
			else if(value == "^")
				out->instructions.push_back(ThreeAd(name,"^",left,right));
			else if(value == "<")
				out->instructions.push_back(ThreeAd(name,"<",left,right));
			else if(value == "==")
				out->instructions.push_back(ThreeAd(name,"==",left,right));
			else if(value == "%")
				out->instructions.push_back(ThreeAd(name,"%",left,right));
			return name;
		}		
		else if(tag == "Number")
			return value;
		else if(tag == "String")
			return  value;
		else if(tag == "funccall")
		{
			string name = this->makeNames();
			string left = children.front().convertExp(out);
			string right = children.back().convertExp(out);
			out->instructions.push_back(ThreeAd(name,"call", left, right));
			return name;
		}

	}
/*	Statement* tranState()
	{
		Statement *returnValue;
		if (tag == "chunk")
		{
			Seq *seq = new Seq();
			for(auto i: children)
			{
				seq->statements.push_back(i.tranState());	
			}
			returnValue = seq;
		}
		else if(tag == "assign")
		{
			string left = children.front().children.front().value;
			returnValue = new Assignment(left, children.back().tranExp());
		}
		else if(tag == "funccall")
		{
			string left = children.front().children.front().value;
			returnValue = new SFunc(left, children.back().tranExp());
		}
		return returnValue;
	}
	Expression* tranExp()
	{
		Expression *exp;
		if(tag == "explist")
		{
			exp = children.front().tranExp();
		}
		else if(tag == "OP")
		{
			if(value == "+")
				exp = new Add(children.front().tranExp(),children.back().tranExp());
			else if(value == "-")
				exp = new Sub(children.front().tranExp(),children.back().tranExp());
			else if(value == "*")
				exp = new Mult(children.front().tranExp(),children.back().tranExp());
			else if(value == "/")
				exp = new Div(children.front().tranExp(),children.back().tranExp());
			else if(value == "^")
				exp = new Square(children.front().tranExp(),children.back().tranExp());
		}		
		else if (tag == "varlist")
		{
			Varlist *temp = new Varlist();
			for(auto i: children)
				temp->vars.push_back(new Variable(i.value));
			exp = temp;
		}
		else if(tag == "Number")
		{
			exp = new Constant(stod(value));
		}
		return exp;
	}
*/
};
#endif
