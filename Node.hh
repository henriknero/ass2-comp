#ifndef NODE_H
#define NODE_H
#include <list>
#include <string>
#include <iostream>
#include <math.h>
#include "BlockThree.hh"
extern map <string,double> vars;
extern map <string,BBlock*[2]> userfuncs;
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
			auto exp = children.back().children.begin();
			for(auto var: children.front().children)
			{
			string left = var.convertExp(out);
			string right = exp->convertExp(out);
			out->instructions.push_back(ThreeAd(left,"c",right,right));
			exp++;
			}
			return out;
		}
		if (tag == "Function")
		{
			auto i = children.begin();
			string functionName = i->value;
			i++;
			string parameter = i->children.front().value;
			i++;
			BBlock *startFunc = new BBlock();
			BBlock *endFunc = i->convert(startFunc);
			userfuncs[functionName][0] = startFunc;
			userfuncs[functionName][1] = endFunc;
			return out;
		}
		if (tag == "funccall")
		{
			string left = "_unused";
			string funcname = children.front().convertExp(out);
			if (children.back().tag == "explist")
			{
				for (auto i: children.back().children)
				{
					string right = i.convertExp(out);
					out->instructions.push_back(ThreeAd(left,"call",funcname, right));
				}
			}
			else
			{
				string right = children.back().convertExp(out);
				out->instructions.push_back(ThreeAd(left,"call",funcname, right));
			}
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
			int size = children.size();
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
			if(size>2)
			{
				i++;
				i->convert(falseExit)->fExit = endBlock;
			}
			else
			{
				out->fExit = endBlock;
			}
			return endBlock;
		}
		if (tag == "Repeat")
		{
			BBlock *trueExit = new BBlock();
			BBlock *evalBlock = new BBlock();
			BBlock *endBlock = new BBlock();
			out->tExit = trueExit;

			children.front().convert(trueExit)->tExit = evalBlock;
			children.back().convertExp(evalBlock);
			evalBlock->tExit = trueExit;
			evalBlock->fExit = endBlock;
			return endBlock;
		}
		if (tag == "Return")
		{
			string returnVal = children.front().convertExp(out);
			out->instructions.push_back(ThreeAd("return","c",returnVal,returnVal));
			return out;
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
		else if (tag == "Array")
		{
			return children.front().convertExp(out) + "[" + children.back().convertExp(out) + "]";
		}
		else if (tag == "explist")
			return children.front().convertExp(out);
		else if (tag == "list")
		{
			string name = this->makeNames();
			Node explist = children.front();
			int arrSize = explist.children.size();
			out->instructions.push_back(ThreeAd(name,"list",to_string(arrSize), to_string(arrSize)));
			int index = 0;
			for(auto i = explist.children.begin(); i != explist.children.end(); i++, index++)
			{
				out->instructions.push_back(ThreeAd(name,"[]",to_string(index),i->value));
			}
			return name;
		}
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
			else if(value == ">")
				out->instructions.push_back(ThreeAd(name,">",left,right));
			else if(value == "==")
				out->instructions.push_back(ThreeAd(name,"==",left,right));
			else if(value == "%")
				out->instructions.push_back(ThreeAd(name,"%",left,right));
			else if (value == "#")
				out->instructions.push_back(ThreeAd(name,"#",left,left));
			return name;
		}		
		else if(tag == "Number")
			return value;
		else if(tag == "String")
			return  value;
		else if(tag == "bool")
			return value;
		else if(tag == "funccall")
		{
			string name = this->makeNames();
			string left = children.front().convertExp(out);
			string right = children.back().convertExp(out);
			out->instructions.push_back(ThreeAd(name,"call", left, right));
			return name;
		}

	}
};
#endif
