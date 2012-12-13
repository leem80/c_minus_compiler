#include "globals.h"
#include "util.h"
#include "scan.h"

typedef enum
    {START,INNEQ,INCOMMENT,ENTERINGCOMMENT,EXITINGCOMMENT,INNUM,INID,INLARGE,INSMALL,INEQ,DONE}
    StateType;

char tokenString[MAXTOKENLEN+1];

#define BUFLEN 256

static char lineBuf[BUFLEN];
static int linepos=0;
static int bufsize=0;

static char getNextChar(void)
{
	if(!(linepos<bufsize))
	{
		lineno++;
		if(fgets(lineBuf,BUFLEN-1,source))
		{
			if(EchoSource)	fprintf(listing,"%4d:%s",lineno,lineBuf);
			bufsize = strlen(lineBuf);
			linepos=0;
			return lineBuf[linepos++];
		}
		else return EOF;
	}
	else return lineBuf[linepos++];
} 

static void ungetNextChar(void)
{	linepos--;}

static struct
{
	char *str;
	TokenType tok;
}	reservedWords[MAXRESERVED]
={{"if",IF},{"else",ELSE},{"int",INT},{"return",RETURN},{"void",VOID},{"while",WHILE}};

static TokenType reservedLookup(char *s)
{
	int i;
	for(i=0;i<MAXRESERVED;i++)
		if(!strcmp(s,reservedWords[i].str))
			return reservedWords[i].tok;
	return ID;
}

TokenType getToken(void)
{
	int tokenStringIndex=0;
	TokenType currentToken;
	StateType state=START;
	int save;
	while(state!=DONE)
	{
		char c=getNextChar();
		save=TRUE;
		switch(state)
		{
		case START:
			if(isdigit(c))
				state=INNUM;
			else if(isalpha(c))
				state=INID;
			else if(c=='!')
				state=INNEQ;
			else if(c=='=')
				state=INEQ;
			else if(c=='>')
				state=INLARGE;
			else if(c=='<')
				state=INSMALL;
			else if((c==' ')||(c=='\t')||(c=='\n'))
				save=FALSE;
			else if(c=='/')
			{
				state=ENTERINGCOMMENT;
				save=FALSE;
			}
			else 
			{
				state=DONE;
				switch(c)
				{
				case EOF:
					save=FALSE;
					currentToken=ENDFILE;
					break;	
				case '+':
					currentToken=PLUS;
					break;
				case '-':
					currentToken=MINUS;
					break;
				case '*':
					currentToken=MULTIPY;
					break;
				case ';':
					currentToken=SEMI;
					break;
				case ',':
					currentToken=COMMA;
					break;
				case '(':
					currentToken=LRPAREN;
					break;
				case ')':
					currentToken=RRPAREN;
					break;
				case '[':
					currentToken=LFPAREN;
					break;
				case ']':
					currentToken=RFPAREN;
					break;
				case '{':
					currentToken=LDPAREN;
					break;
				case '}':
					currentToken=RDPAREN;
					break;
				default:
					currentToken=ERROR;
					break;
				}
			}
			break;
		case INNUM:
			if(!isdigit(c))
			{
				ungetNextChar();
				save=FALSE;
				state=DONE;
				currentToken=NUM;
			}
			break;
		case INID:
			if(!isalpha(c))
			{
				ungetNextChar();
				save=FALSE;
				state=DONE;
				currentToken=ID;
			}
			break;
		case INEQ:
			state=DONE;
			if(c=='=')
				currentToken=EQ;
			else
			{
				ungetNextChar();
				save=FALSE;
				currentToken=ASSIGN;
			}
			break;
		case INNEQ:
			state=DONE;
			if(c=='=')
				currentToken=NEQ;
			else
			{
				ungetNextChar();
				save=FALSE;
				currentToken=ERROR;
			}
			break;
		case INLARGE:
			state=DONE;
			if(c=='=')
				currentToken=LE;
			else
			{
				ungetNextChar();
				save=FALSE;
				currentToken=LARGE;
			}
			break;
		case INSMALL:
			state=DONE;
			if(c=='=')
				currentToken=SE;
			else
			{
				ungetNextChar();
				save=FALSE;
				currentToken=SMALL;
			}
			break;
		case ENTERINGCOMMENT:
			if(c=='*')
			{
				state=INCOMMENT;
				save=FALSE;
			}
			else
			{
				state=DONE;
				ungetNextChar();
				save=FALSE;
				currentToken=DIVIDE;
			}
			break;
		case INCOMMENT:
			save=FALSE;
			if(c=='*')
				state=EXITINGCOMMENT;
			break;
		case EXITINGCOMMENT:
			save=FALSE;
			if(c=='/')
				state=START;
			else if(c=='*')
				state=EXITINGCOMMENT;
			else 
				state=INCOMMENT;
			break;
		case DONE:

		default:
			fprintf(listing ,"Scanner Bug:state=%d\n",state);
			state=DONE;
			currentToken=ERROR;
			break;
		}
		if((save)&&(tokenStringIndex<=MAXTOKENLEN))
			tokenString[tokenStringIndex++]=c;
		if(state==DONE)
		{
			tokenString[tokenStringIndex]='\0';
			if(currentToken==ID)
				currentToken=reservedLookup(tokenString);
		}
	}
	if(TraceScan)
	{
		fprintf(listing,"\t%d:",lineno);
		printToken(currentToken,tokenString);
	}
	return currentToken;

}
