%skeleton "lalr1.cc" 
%defines
%define api.value.type variant
%define api.token.constructor
%code requires{
	#include "Node.hh"
	#include <string>
}
%code{
	Node root;
	int id = 0;
	#define YY_DECL yy::parser::symbol_type yylex()
	YY_DECL;
}
%type  <Node> block
%type  <Node> chunk
%type  <Node> stat
%type  <Node> functioncall
%type  <Node> explist
%type  <Node> exp
%type  <Node> prefixexp
%type  <Node> args
%type  <Node> raiseop
%type  <Node> highop
%type  <Node> assign
%type  <Node> if
%type  <Node> else
%type  <Node> tableconstr
%type  <Node> lowop
%type  <Node> anything
%type  <Node> optspace
%type  <Node> spaceeater
%type  <Node> var
%type  <Node> namelist
%type  <Node> varlist
%type  <Node> funcname
%type  <Node> laststat
%token <std::string> NAME
%token <std::string> BLANK
%token <std::string> NL
%token <std::string> NUMERIC
%token <std::string> RAISE
%token <std::string> HIGH_OP
%token <std::string> LOW_OP
%token <std::string> LOGIC
%token <std::string> LEFT_PARA
%token <std::string> RIGHT_PARA
%token <std::string> LCURL
%token <std::string> RCURL
%token <std::string> LHARD
%token <std::string> RHARD
%token <std::string> DOT
%token <std::string> COMMA
%token <std::string> EQUALS
%token <std::string> STRING

%token <std::string> FUNCTION
%token <std::string> RETURN
%token <std::string> BREAK
%token <std::string> REPEAT
%token <std::string> UNTIL
%token <std::string> DO
%token <std::string> WHILE
%token <std::string> ENDD
%token <std::string> IF
%token <std::string> ELSE
%token <std::string> TRUE
%token <std::string> FALSE
%token <std::string> THEN
%token <std::string> FOR
%token <std::string> IN
%token END 0 "end of file"

%%
block      : chunk {$$ = $1;
                        root = $$;
                        }
            ;
chunk		: optspace stat {$$ = Node("chunk","",id++);$$.children.push_back($2);}
			| chunk stat 		{$$ = $1;
                                $$.children.push_back($2);
                                root = $$;
                                } 
			| optspace laststat {$$ = $2;}
			| chunk laststat {$$ = $1; $$.children.push_back($2);}
			;
stat	    : assign {$$ = $1;} 
			| functioncall {$$ = $1;}
			| FOR BLANK NAME optspace EQUALS optspace exp optspace COMMA exp DO block ENDD optspace {
					$$ = Node("for","",id++);
					$$.children.push_back(Node("var",$3,id++));
					$$.children.push_back($7);
					$$.children.push_back($10);
					$$.children.push_back($12);
					}			
			| REPEAT block UNTIL exp {$$ = Node("Repeat","",id++); 
										$$.children.push_back($2);
										$$.children.push_back($4);}	
			| if ENDD optspace{$$ = $1;}
			| if else ENDD optspace {$$ = $1; 
										$$.children.push_back($2);}
			| FUNCTION BLANK funcname LEFT_PARA namelist RIGHT_PARA block ENDD optspace {
						$$ = Node("Function", "", id++);
						$$.children.push_back($3);
						$$.children.push_back($5);
						$$.children.push_back($7);
						}
			;
if			: IF exp THEN block {$$ = Node("If","",id++); 
														$$.children.push_back($2);
														$$.children.push_back($4);}
			;
else		: ELSE block {$$= $2;}

assign		: varlist EQUALS optspace explist {$$ = Node("assign",$2,id++);
											  $$.children.push_back($1);
											  $$.children.push_back($4);
											  }
			;	

functioncall: prefixexp args optspace {$$ = Node("funccall","",id++); 
												$$.children.push_back($1); 
												$$.children.push_back($2);}
			;

optspace	: {}
			| BLANK {}
			;

explist		: exp {$$ = Node("explist","", id++);$$.children.push_back($1);}
			| explist COMMA exp {$$ = $1;
								$$.children.push_back($3);}
			;
exp			: lowop {$$ = $1;}
			;	
lowop		: highop {$$ = $1;}
			| lowop LOW_OP highop {$$ = Node("OP",$2,id++);
													$$.children.push_back($1);
													$$.children.push_back($3);
													}
			;
highop		: raiseop {$$ = $1;}
			| highop HIGH_OP raiseop {$$ = Node("OP",$2,id++);
									$$.children.push_back($1);
									$$.children.push_back($3);
									}
			;

raiseop		: spaceeater			{$$ = $1;}
			| raiseop RAISE spaceeater {$$ = Node("OP",$2,id++); 
									$$.children.push_back($1); 
									$$.children.push_back($3);}
			
			;
spaceeater	: optspace anything optspace {$$ = $2;}
			;

anything	: prefixexp {$$ = $1;}
			| STRING {$$ = Node("String",$1,id++);} 
			| NUMERIC {$$ = Node("Number",$1,id++);}
			| LEFT_PARA explist RIGHT_PARA {$$ = $2;}
			| tableconstr {$$ = $1;}
			| TRUE {$$ = Node("bool",$1,id++);}
			| FALSE {$$ = Node("bool",$1,id++);}
			| LOW_OP exp {$$ = Node("OP",$1,id++); $$.children.push_back($2);} 
			;

tableconstr	: LCURL explist RCURL {$$ = Node("list","",id++); $$.children.push_back($2);}
			;

args		: optspace LEFT_PARA optspace RIGHT_PARA {$$ = Node("emptypara","",id++);} 
			| STRING {$$ = Node("String", $1,id++);}
			| optspace LEFT_PARA explist RIGHT_PARA {$$ = $3;}
//														{$$ = Node("args","",id++); 
//														$$.children.push_back($3);}
			;

prefixexp	: varlist {$$ = $1;}
			| functioncall {$$ = $1;}
			;
varlist		: var optspace {$$ = Node("varlist","",id++);$$.children.push_back($1);}
			| varlist COMMA optspace var optspace {$$ = $1; $$.children.push_back($4);}
			;
var			: NAME {$$ = Node("var",$1,id++);}
			| var DOT NAME {$$ = $1; $$.children.push_back(Node("var", $3, id++));}
			| var LHARD exp RHARD {$$ = Node("Array","",id++); 
									$$.children.push_back($1);
									$$.children.push_back($3);}
			;
namelist	: NAME {$$ = Node("Namelist","",id++); $$.children.push_back(Node("Name", $1, id++));}
			| namelist optspace COMMA optspace NAME {$$ = $1; 
													$$.children.push_back(Node("Name",$5,id++));}
			;
funcname	: NAME {$$ = Node("funcname",$1,id++);}
			| funcname DOT NAME {$$ = $1; $$.children.push_back(Node("subfunc",$3,id++));}
			;
laststat	: RETURN explist {$$ = Node("Return","",id++); $$.children.push_back($2);}
			| BREAK {$$ = Node("Break","",id++);}
%%
