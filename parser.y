%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int line_number;
void yyerror(const char *s);
void print_symbol_table();
%}

%union {
    int num;
    char *id;
}

/* Tokens */
%token <id> IDEN STRINGCONST
%token <num> NUMBER
%token INT FLOAT CHAR VOID
%token IF ELSE WHILE FOR BREAK CONTINUE RETURN PRINTF
%token EQ NE LT GT LE GE
%token PLUS MINUS MUL DIV MOD ASSIGN
%token LPAREN RPAREN LBRACE RBRACE COMMA SEMICOLON

%type <num> expression condition

/* Handle dangling else */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

program:
      stmt_list { print_symbol_table(); }
    ;

stmt_list:
      /* empty */
    | stmt_list statement
    ;

statement:
      declaration
    | expression_stmt
    | selection_stmt
    | iteration_stmt
    | jump_stmt
    | compound_stmt
    ;

declaration:
      type_spec id_list SEMICOLON
    ;

type_spec:
      INT
    | FLOAT
    | CHAR
    | VOID
    ;

id_list:
      IDEN
    | id_list COMMA IDEN
    ;

expression_stmt:
      expression SEMICOLON
    | SEMICOLON
    ;

selection_stmt:
      IF LPAREN condition RPAREN statement %prec LOWER_THAN_ELSE
    | IF LPAREN condition RPAREN statement ELSE statement
    ;

iteration_stmt:
      WHILE LPAREN condition RPAREN statement
    | FOR LPAREN expression_stmt condition SEMICOLON expression RPAREN statement
    ;

jump_stmt:
      RETURN expression SEMICOLON
    | BREAK SEMICOLON
    | CONTINUE SEMICOLON
    ;

compound_stmt:
      LBRACE stmt_list RBRACE
    ;

expression:
      IDEN ASSIGN expression
    | expression PLUS expression
    | expression MINUS expression
    | expression MUL expression
    | expression DIV expression
    | LPAREN expression RPAREN
    | NUMBER
    | IDEN
    | PRINTF LPAREN STRINGCONST RPAREN SEMICOLON
    ;

condition:
      expression EQ expression
    | expression NE expression
    | expression LT expression
    | expression GT expression
    | expression LE expression
    | expression GE expression
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d: %s\n", line_number, s);
}

int main() {
    printf("Parsing started...\n");
    if (yyparse() == 0)
        printf("Parsing completed successfully!\n");
    else
        printf("Parsing failed.\n");
    return 0;
}
