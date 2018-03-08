/* DV1465 / DV1505 / DV1511 Lab-task example code.
   (C) Dr Andrew Moss, Erik Bergenholtz  2016, 2017, 2018
   This code is released into the public domain.

   You are free to use this code as a base for your second assignment after
   the lab sessions (it is not required that you do so). 

   2018: Took out the double-pointers.
*/
#include "Statement.hh"
/************* Three Address Instructions *************/

/* Test cases */
Statement *test = new Seq({
                          new Assignment(
                                  "x",
                                  new Add(
                                          new Variable("x"),
                                          new Constant(1)
                                  )
                          ),new If(
                                  new Equality(
                                          new Variable("x"),
                                          new Constant(10)
                                  ),new Assignment(
                                          "y",
                                          new Add(
                                                  new Variable("x"),
                                                  new Constant(1)
                                          )
                                  ), new Assignment(
                                          "y",
                                          new Mult(
                                                  new Variable("x"),
                                                  new Constant(2)
                                          )
                                  )
                          ), new Assignment(
                                  "x",
                                  new Add(
                                          new Variable("x"),
                                          new Constant(1)
                                  )
                          )
});
/* Test case 2 */

Statement *test2 = new Seq({
							new Assignment(
									"x",
									new Add(
										new Variable("x"),
										new Constant(1)
									)
							),
							new Assignment(
								"y",
								new Add(
									new Variable("y"),
									new Constant(1)
								)
							),
							new If(
								new Equality(
									new Variable("x"),
									new Constant(0)
								), 
								new If(
									new Equality(
										new Variable("y"),
										new Constant(0)
									),
									new Assignment(
										"x",
										new Constant(1)
									),
									new Assignment(
										"y",
										new Constant(2)
									)
								),
								new Assignment(
									"y",
									new Constant(3)
								)
							)
						});


/*
 * Iterate over each basic block that can be reached from the entry point
 * exactly once, so that we can dump out the entire graph.
 * This is a concrete example of the graph-walk described in lecture 7.
 */
void dumpCFG(BBlock *start)
{
        set<BBlock *> done, todo;
        todo.insert(start);
        while(todo.size()>0)
        {
                // Pop an arbitrary element from todo set
                auto first = todo.begin();
                BBlock *next = *first;
                todo.erase(first);
                next->dump();
                done.insert(next);
                if(next->tExit!=NULL && done.find(next->tExit)==done.end())
                        todo.insert(next->tExit);
                if(next->fExit!=NULL && done.find(next->fExit)==done.end())
                        todo.insert(next->fExit);
        }
}
void dumpCode(BBlock *start)
{
	cout << "int main()\n{" << endl;
	for(auto i:vars){
		cout << "int " << i.first << " = " << i.second << ";" << endl;;
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

map<string,int> vars;
int main(){
//	test2->dump(0);
	BBlock *block = new BBlock();
	test2->convert(block);
	dumpCFG(block);
	dumpCode(block);
	return 0;

}
