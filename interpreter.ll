%top{
#include "interpreter.tab.hh"
#define YY_DECL yy::parser::symbol_type yylex()
}
%option noyywrap nounput batch noinput
%x STRING
%%
\^											{return yy::parser::make_RAISE(yytext);}
[%*/]										{return yy::parser::make_HIGH_OP(yytext);}
for											{return yy::parser::make_FOR(yytext);}
[+%<>\-]|<=|>=|==|~=|and|or|\.\.|#|not		{return yy::parser::make_LOW_OP(yytext);}
[ \t\n]*									{return yy::parser::make_BLANK(yytext);}	
[0-9]+										{return yy::parser::make_NUMERIC(yytext);}
[(]											{return yy::parser::make_LEFT_PARA(yytext);}	
[)]											{return yy::parser::make_RIGHT_PARA(yytext);}
[{]											{return yy::parser::make_LCURL(yytext);}
[}]											{return yy::parser::make_RCURL(yytext);}
\[											{return yy::parser::make_LHARD(yytext);}
\]											{return yy::parser::make_RHARD(yytext);}
[.]											{return yy::parser::make_DOT(yytext);}
[,]											{return yy::parser::make_COMMA(yytext);}
[=]											{return yy::parser::make_EQUALS(yytext);}
function									{return yy::parser::make_FUNCTION(yytext);}
return										{return yy::parser::make_RETURN(yytext);}
break										{return yy::parser::make_BREAK(yytext);}
repeat										{return yy::parser::make_REPEAT(yytext);}
until										{return yy::parser::make_UNTIL(yytext);}
do											{return yy::parser::make_DO(yytext);}
end											{return yy::parser::make_ENDD(yytext);}
if											{return yy::parser::make_IF(yytext);}
else										{return yy::parser::make_ELSE(yytext);}
true										{return yy::parser::make_TRUE(yytext);}
false										{return yy::parser::make_FALSE(yytext);}
then										{return yy::parser::make_THEN(yytext);}
in											{return yy::parser::make_IN(yytext);}
[a-zA-Z_][a-zA-Z0-9_]*						{return yy::parser::make_NAME(yytext);}
["]											{BEGIN(STRING);}
<STRING>["]									{BEGIN(0);}
<STRING>[*a-zA-Z _:\\n]*					{return yy::parser::make_STRING(yytext);}
<<EOF>>       								return yy::parser::make_END();
%%
