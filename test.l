%{
#include <cstdio>
#include <cstdlib>
%}

INT_DEX     [1-9][0-9]*|[0]
INT_HEX     [0][Xx]([1-9][0-9]*|[0])
INT_OCT     [0][0-7]*
FLOAT       [0-9]*[.][0-9]+([eE][+-]?[0-9]*)?
PLUS        \+
MINUS       \-
TIMES       \*
DIV         \/
COLON       \:
ASSIGN      :=
LES         \<
LEQ         <=
NEQ         <>
GRT         \>
GRE         >=
EQU         \=
SEMI        \;
LBRK        \(
RBRK        \)
SIGN        \#
MAINSYM     main
IFSYM       if
THENSYM     then
WHILESYM    while
DOSYM       do
STATICSYM   static
DEFAULTSYM  default
INTSYM      int
DBSYM       double
STRSYM      struct
BREAKSYM    break
ELSESYM     else
LONGSYM     long
SWITCHSYM   switch
CASE        case
TYSYM       typedef
CHATSYM     char
RETURNSYM   return
CONSTSYM    const
FLOATSYM    float
SHORTSYM    short
CONTSYM     continue
FORSYM      for
VOIDSYM     void
SIZEOFSYM   sizeof
ID          [a-zA-Z][a-zA-Z0-9]*
ERROR       [!@$%^&_\\\[\]?,.]
COMM        \/\/(.*)
COMMM       \/\*(.*\n)*.*\*\/
OTHER       .

%%
{COMM}      {printf("COMM: %s\n", yytext);}
{COMMM}     {printf("COMM: %s >\n", yytext);}
{INT_DEX}   {printf("< 26, %s >\n", yytext);}
{INT_HEX}   {printf("< 26, %s >\n", yytext);}
{INT_OCT}   {printf("< 26, %s >\n", yytext);}
{FLOAT}     {printf("< 26, %s >\n", yytext);}
{PLUS}      {printf("< 27, %s >\n", yytext);}
{MINUS}     {printf("< 28, %s >\n", yytext);}
{TIMES}     {printf("< 29, %s >\n", yytext);}
{DIV}       {printf("< 30, %s >\n", yytext);}
{COLON}     {printf("< 31, %s >\n", yytext);}
{ASSIGN}    {printf("< 32, %s >\n", yytext);}
{LES}       {printf("< 33, %s >\n", yytext);}
{NEQ}       {printf("< 34, %s >\n", yytext);}
{LEQ}       {printf("< 35, %s >\n", yytext);}
{GRT}       {printf("< 36, %s >\n", yytext);}
{GRE}       {printf("< 37, %s >\n", yytext);}
{EQU}       {printf("< 38, %s >\n", yytext);}
{SIGN}      {printf("< 0, %s >\n", yytext);}
{SEMI}      {printf("< 41, %s >\n", yytext);}
{LBRK}      {printf("< 42, %s >\n", yytext);}
{RBRK}      {printf("< 43, %s >\n", yytext);}
{MAINSYM}   {printf("< 1, %s >\n", yytext);}
{IFSYM}     {printf("< 2, %s >\n", yytext);}
{THENSYM}   {printf("< 3, %s >\n", yytext);}
{WHILESYM}  {printf("< 4, %s >\n", yytext);}
{DOSYM}     {printf("< 5, %s >\n", yytext);}
{STATICSYM} {printf("< 6, %s >\n", yytext);}
{INTSYM}    {printf("< 7, %s >\n", yytext);}
{DBSYM}     {printf("< 8, %s >\n", yytext);}
{STRSYM}    {printf("< 9, %s >\n", yytext);}
{BREAKSYM}  {printf("< 10, %s >\n", yytext);}
{ELSESYM}   {printf("< 11, %s >\n", yytext);}
{LONGSYM}   {printf("< 12, %s >\n", yytext);}
{SWITCHSYM} {printf("< 13, %s >\n", yytext);}
{CASE}      {printf("< 14, %s >\n", yytext);}
{TYSYM}     {printf("< 15, %s >\n", yytext);}
{CHATSYM}   {printf("< 16, %s >\n", yytext);}
{RETURNSYM} {printf("< 17, %s >\n", yytext);}
{CONSTSYM}  {printf("< 18, %s >\n", yytext);}
{FLOATSYM}  {printf("< 19, %s >\n", yytext);}
{SHORTSYM}  {printf("< 20, %s >\n", yytext);}
{CONTSYM}   {printf("< 21, %s >\n", yytext);}
{FORSYM}    {printf("< 22, %s >\n", yytext);}
{VOIDSYM}   {printf("< 23, %s >\n", yytext);}
{SIZEOFSYM} {printf("< 24, %s >\n", yytext);}
{ID}        {printf("< 25, %s >\n", yytext);}
{ERROR}     {printf("Error: %s\n", yytext);}
{OTHER}     {}


%%
int yywrap() {
    return 1;
}
int main(int argc, char* argv[]) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            perror(argv[1]);
            return 1;
        }
    }
    while (yylex());
    return 0;
}