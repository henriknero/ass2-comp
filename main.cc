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
                cout << next->name << "[label=\"";
				next->dump();
				cout << "\", shape=\"rect\"]";
                done.insert(next);
                if(next->tExit!=NULL && done.find(next->tExit)==done.end())
                        todo.insert(next->tExit);
                if(next->fExit!=NULL && done.find(next->fExit)==done.end())
                        todo.insert(next->fExit);
        }
		cout << endl << "}\n";
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
	}

	
	return 0;
 }

