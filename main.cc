#include<iostream>
#include <fstream>
#include <map>
#include "interpreter.tab.hh"
#include "Node.hh"
extern Node root;
extern FILE *yyin;

int Node::eCounter = 0;
int BBlock::nCounter = 0;
void yy::parser::error(std::string const&err)
{
  std::cout << "It's one of the bad ones... " << err << std::endl;
}
map<string,double> vars;
map<string,double> lists;
map <string, BBlock*[2]> userfuncs;
void dumpCFG(BBlock *start)
{
		cout << "digraph {\n";
        set<BBlock *> done, todo;
        todo.insert(start);
        while(todo.size()>0)
        {
                // Pop an arbitrary element from todo set
                auto first = todo.begin();
                BBlock *next = *first;
                todo.erase(first);
                cout << next->name << " [label=\"";
				next->dump();
				cout << "\", shape=\"rect\"]" << endl;
                done.insert(next);
                if(next->tExit!=NULL && done.find(next->tExit)==done.end())
				{
					cout << next->name << "->" << next->tExit->name << " [label=\"true\"]"<< endl;
					todo.insert(next->tExit);
                }
				else if(next->tExit!=NULL)
					cout << next->name << "->" << next->tExit->name << " [label=\"true\"]"<< endl;
					
				if(next->fExit!=NULL && done.find(next->fExit)==done.end())
                {
					cout << next->name << "->" << next->fExit->name << " [label=\"false\"]" << endl;
					todo.insert(next->fExit);
				}
				else if(next->fExit!=NULL)
					cout << next->name << "->" << next->tExit->name << " [label=\"false\"]"<< endl;
        }
		cout << endl << "}\n";
}
void dumpCode(BBlock *start)
{
	cout << "#include <iostream>" << endl;
	cout << "#include <stdio.h>" << endl;
	cout << "#include <stdlib.h>" << endl;
	cout << "#include <string.h>" << endl;
	cout << "#include <math.h>" << endl;
	cout << "using namespace std;" << endl;
    cout << "int main()\n{" << endl;
    for(auto i:vars){
        cout << "double " << i.first << " = " << i.second << ";" << endl;;
    }
    for(auto i:lists){
        cout << "double " << i.first << "["<< i.second << "];" << endl;;
    }
    set<BBlock *> done, todo;
    todo.insert(start);
    while(todo.size()>0)
    {
        auto first = todo.begin();
        BBlock *next = *first;
        todo.erase(first);
        next->dumpC();
        done.insert(next);
        if(next->tExit!=NULL && done.find(next->tExit)==done.end())
            todo.insert(next->tExit);
        if(next->fExit!=NULL && done.find(next->fExit)==done.end())
            todo.insert(next->fExit);
    }
    cout << "}" << endl;
}

int main(int argc, char **argv)
{
	yyin = fopen(argv[1],"r");
	yy::parser parser;
    if(!parser.parse()){
		BBlock *block = new BBlock();
		root.convert(block);
		
		freopen("parse.txt","w",stdout);
		std::cout << "digraph {" << std::endl;
		root.dump_visual();
		std::cout << "}" << std::endl;
		freopen("cfg.dot","w",stdout);
		dumpCFG(block);
		freopen("target.cc","w", stdout);
		dumpCode(block);
	}

	
	return 0;
 }

