%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"

int line_number = 1;

/* --- Symbol Table --- */
typedef struct Symbol {
    char name[64];
    char type[32];
    int freq;           
    int line;
    struct Symbol *next;
} Symbol;

Symbol *symbol_table = NULL;
char current_type[32] = "";

/* Add symbol if not already declared */
void add_symbol(const char *name, const char *type, int line) {
    Symbol *p = symbol_table;
    while (p) {
        if (strcmp(p->name, name) == 0) {
            p->freq++;
            return;
        }
        p = p->next;
    }
    // New declaration
    Symbol *s = (*Symbol)malloc(sizeof(Symbol));
    strcpy(s->name, name);
    strcpy(s->type, type);
    s->line = line;
    s->freq = 1;
    s->next = symbol_table;
    symbol_table = s;
}

/* Print symbol table */
void print_symbol_table() {
    printf("\n--- SYMBOL TABLE ---\n");
    printf("%-20s %-10s %-6s %-6s\n", "Name", "Type", "Line", "Freq");
    printf("-----------------------------------------------\n");
    for (Symbol *p = symbol_table; p; p = p->next)
        printf("%-20s %-10s %-6d %-6d\n", p->name, p->type, p->line, p->freq);
    printf("-----------------------------------------------\n");
}
%}

digit       [0-9]
letter      [a-zA-Z_]
id          {letter}({letter}|{digit})*
number      {digit}+

%%

[ \t]+          ;               /* ignore spaces */
\n              { line_number++; }

"int"           { strcpy(current_type, "int"); return INT; }
"float"         { strcpy(current_type, "float"); return FLOAT; }
"char"          { strcpy(current_type, "char"); return CHAR; }
"void"          { strcpy(current_type, "void"); return VOID; }

"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"for"           { return FOR; }
"break"         { return BREAK; }
"continue"      { return CONTINUE; }
"return"        { return RETURN; }
"printf"        { return PRINTF; }


"=="            { return EQ; }
"!="            { return NE; }
"<="            { return LE; }
">="            { return GE; }
"<"             { return LT; }
">"             { return GT; }
"="             { return ASSIGN; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MUL; }
"/"             { return DIV; }
"%"             { return MOD; }

"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
";"             { return SEMICOLON; }
","             { return COMMA; }

{number}        { yylval.num = atoi(yytext); return NUMBER; }
\"(\\.|[^\\"])*\"  { yylval.id = strdup(yytext); return STRINGCONST; }

{id} {
    // If type is declared, add symbol with type; else increment frequency if already exists
    if (strlen(current_type) > 0)
        add_symbol(yytext, current_type, line_number);
    else
        add_symbol(yytext, "unknown", line_number);
    yylval.id = strdup(yytext);
    return IDEN;
}

"//".*          ;
"/*"([^*]*|\*+[^*/])*\*+"/" ;

.               ;

%%

int yywrap() { return 1; }