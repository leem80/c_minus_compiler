#include "globals.h"
#include "util.h"
void printToken(TokenType token,const char* tokenString)
{
    switch(token)
    {
        case IF:
        case ELSE:
        case RETURN:
		case INT:
		case VOID:
        case WHILE:
				//printf("reserved word:%s\n",tokenString);
                fprintf(listing,"reserved word:%s\n",tokenString);
                break;
        case PLUS: fprintf(listing ,"+\n"); /*printf("+\n")*/ break;
        case MINUS: fprintf(listing ,"-\n"); /*printf("-\n")*/break;
        case MULTIPY: fprintf(listing ,"*\n"); /*printf("*\n")*/ break;
        case DIVIDE: fprintf(listing ,"/ \n"); /*printf("/ \n")*/ break;
        case SMALL: fprintf(listing ,"<\n"); /*printf("<\n");*/ break;
        case LARGE: fprintf(listing ,">\n"); /*printf(">\n");*/break;
        case SE: fprintf(listing ,"<=\n"); /*printf("<=\n");*/break;
        case LE: fprintf(listing ,">=\n"); /*printf(">=\n");*/break;
        case EQ: fprintf(listing ,"==\n"); /*printf("==\n");*/break;
        case NEQ: fprintf(listing ,"!=\n"); /*printf("!=\n");*/break;
        case ASSIGN: fprintf(listing ,"=\n"); /*printf("=\n");*/break;
        case SEMI: fprintf(listing ,";\n"); /*printf(";\n");*/break;
        case COMMA: fprintf(listing ,",\n"); /*printf(",\n");*/break;
        case LRPAREN: fprintf(listing ,"(\n");/* printf("(\n");*/ break;
        case RRPAREN: fprintf(listing ,")\n"); /*printf(")\n");*/break;
        case LFPAREN: fprintf(listing ,"[\n"); /*printf("[\n");*/break;
        case RFPAREN: fprintf(listing ,"]\n"); /*printf("]\n");*/break;
        case LDPAREN: fprintf(listing ,"{\n"); /*printf("{\n");*/break;
        case RDPAREN: fprintf(listing ,"}\n"); /*printf("}\n");*/break;
		case ENDFILE: fprintf(listing ,"EOF\n"); /*printf("EOF\n");*/break;
        case NUM:
			//printf("NUM,val=%s\n",tokenString);
			fprintf(listing,"NUM,val=%s\n",tokenString);
            break;
        case ID: 
			//printf("ID,name=%s\n",tokenString);
            fprintf(listing,"ID,name=%s\n",tokenString);
            break;
		case ERROR:
			//printf("ERROR:%s\n",tokenString);
			fprintf(listing,"ERROR:%s\n",tokenString);
            break;			
        default:
			//printf("Unknow token:%d\n",token);
            fprintf(listing,"Unknow token:%d\n",token);
    }        
}

TreeNode * newStmtNode(StmtKind kind)
{
    TreeNode * t=(TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if(t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else
    {
        for(i=0;i<MAXCHILDREN;i++)	t->child[i] = NULL;
        t->sibling=NULL;
        t->nodekind=StmtK;
        t->kind.stmt=kind;
        t->lineno=lineno;       
    }
    return t;
}    

TreeNode * newExpNode(ExpKind kind)
{
    TreeNode*t=(TreeNode *)malloc(sizeof(TreeNode));
    int i;
    if(t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else
    {
        for(i=0;i<MAXCHILDREN;i++)            t->child[i]=NULL;
        t->sibling=NULL;
        t->nodekind=ExpK;
        t->kind.exp=kind;
        t->lineno=lineno;
        t->type=Void;   
    }         
    return t;
}
char *copyString(char* s)
{
    int n;
    char *t;
    if(s==NULL)     return NULL;
    n=strlen(s)+1;
    t=malloc(n);
    if(t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else        strcpy(t,s);
    return t;
}

static indentno=0;
#define INDENT indentno+=2
#define UNINDENT indentno-=2

static void printSpaces(void)
{
    int i;
    for(i=0;i<indentno;i++)
    {
        fprintf(listing," "); 
		printf(" ");
    }
}
void printTree(TreeNode *tree)
{
    int i;
    INDENT;
    while(tree!=NULL)
    {
        printSpaces();
        if(tree->nodekind==StmtK)
        {
            switch(tree->kind.stmt)
            {
                case IfK:
					//printf("If\n");
                    fprintf(listing,"If\n");
                    break;
                case ElseK:
                    fprintf(listing,"else\n");
					//printf("else\n");
                    break;
                case ReturnK:
                    fprintf(listing,"return\n");
					//printf("return\n");
                    break;
                case WhileK:
                    fprintf(listing,"while\n");
					//printf("while\n");
                    break;
                case AssignK:
                    fprintf(listing,"Assign to:%s\n",tree->attr.name);
				//	printf("Assign to:%s\n",tree->attr.name);
                    break;
				case DecK:
					fprintf(listing,"declaration:\n");
					//printf("declaration:\n");
					break;
				case VarDecK:
					fprintf(listing,"Var Declaration: ");
					//printf("Var Declaration: ");
					switch(tree->type)
					{
						case Integer:
							fprintf(listing,"Name: %s Type: %s \n",tree->attr.name,"[int]");
							//printf("Name: %s Type: %s \n",tree->attr.name,"[int]");
							break;
						case Void:
							fprintf(listing,"Type %s :\n","[void]");
							//printf("Type %s :\n","[void]");
							break;
					}
					break;
				case FunDecK:
					fprintf(listing,"Function Declaration: ");
					//printf("Function Declaration: ");
					switch(tree->type)
					{
						case Integer:
							fprintf(listing,"Return Type: %s Name: %s \n","[int]",tree->attr.name);
							//printf("Return Type: %s Name: %s \n","[int]",tree->attr.name);
							break;
						case Void:
							fprintf(listing,"Return Type %s Name: %s \n","[void]",tree->attr.name);
							//printf("Return Type %s Name: %s \n","[void]",tree->attr.name);
							break;
						default:break;
					}
					break;
				case ParamListK:
					fprintf(listing,"param-list:\n");
					//printf("param-list:\n");
					break;
				case ParamK:
					switch(tree->type)
					{
					case Integer: 
						fprintf(listing,"Type: %s Name: %s \n","[int]",tree->attr.name);
						//printf("Type: %s Name: %s \n","[int]",tree->attr.name);
						break;
					case Void:
						fprintf(listing,"EMPTY\n");
						//printf("EMPTY\n");
						break;
					}
					break;
					case ExpStmtK:
						fprintf(listing,"expression-stmt:\n");
					break;
				case ComK:
					fprintf(listing,"compound-stmt:\n");
					//printf("compound-stmt:\n");
					break;
				case LocalDecK:
					fprintf(listing,"local-declarations:\n");
					///printf("local-declarations:\n");
					break;
				case StmtListK:
					fprintf(listing,"statement-list:\n");
					//printf("statement-list:\n");
					break;
				case CallK:
					fprintf(listing,"call function:%s \n",tree->attr.name);
					//printf("call function:%s \n",tree->attr.name);
					break;
				case ArgsK:
					fprintf(listing,"args:\n");
					//printf("args:\n");
					break;
				case ArgsListK:
					fprintf(listing,"args-list:\n");
					//printf("args-list:\n");
					break;
				default:
                    fprintf(listing,"Unknow ExpNode kind\n");
					//printf("Unknow ExpNode kind\n");
                    break;      
            }
        }		
        else if(tree->nodekind==ExpK)
        {
            switch(tree->kind.exp)
            {
                case OpK:
                    fprintf(listing,"Op: ");
					//printf("Op: ");
                    printToken(tree->attr.op,"\0");
                    break;
                case ConstK:
                    fprintf(listing,"const:%d\n",tree->attr.val);
					//printf("const:%d\n",tree->attr.val);
                    break;
                case IdK:
                    fprintf(listing,"Id:%s\n",tree->attr.name);
					//printf("Id:%s\n",tree->attr.name);
                    break;
                default:
                    fprintf(listing,"Unknown ExpNode kind\n");
					//printf("Unknown ExpNode kind\n");
                    break;               
            }
        }    
        else {fprintf(listing,"Unknown node kind\n");
			//	printf("Unknown node kind\n");
		}
		for(i=0;i<MAXCHILDREN;i++)
                printTree(tree->child[i]);
        tree=tree->sibling;       
    }
    UNINDENT;
}






















