#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 6

typedef enum
{
	ENDFILE,ERROR,
    IF,ELSE ,RETURN,WHILE,INT,VOID,
    ID ,NUM,
    PLUS,MINUS,MULTIPY,DIVIDE,SMALL,LARGE,SE,LE,EQ,NEQ,ASSIGN,SEMI,COMMA,LRPAREN,RRPAREN,LFPAREN,RFPAREN,LDPAREN,RDPAREN,
} TokenType;

extern FILE* source;
extern FILE* listing;
extern FILE* code;

extern int lineno;

typedef enum{StmtK,ExpK} NodeKind;
typedef enum{IfK,ElseK,ReturnK,WhileK,AssignK,DecK,VarDecK,FunDecK,ParamListK,ExpStmtK,ParamK,StmtListK,ComK,LocalDecK,CallK,ArgsK,ArgsListK} StmtKind;
typedef enum{OpK,ConstK,IdK} ExpKind;              
typedef enum{Void,Integer} ExpType;
#define MAXCHILDREN 3

typedef struct treeNode
{
    struct treeNode * child[MAXCHILDREN];
    struct treeNode * sibling;
    int lineno;
    NodeKind nodekind;
    union{StmtKind stmt;ExpKind exp;} kind;
    union{
        TokenType op;
        int val;
        char * name;
        } attr;
    ExpType type;
} TreeNode;
extern int EchoSource;
extern int TraceScan;
extern int TraceParse;
#endif
