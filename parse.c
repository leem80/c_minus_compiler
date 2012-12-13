#include "globals.h"
#include "util.h"
#include "scan.h"
#include "parse.h"

static TokenType token;

//static TreeNode * program(void);
static TreeNode * dec_list(void);
static TreeNode * dec(void);
//static TreeNode * var_dec(void);
//static TreeNode * fun_dec(void);
static TreeNode * params(void);
static TreeNode * param(void);
static TreeNode * param_list(void);
static TreeNode * compound_stmt(void);
static TreeNode * local_declarations(void);
////////////////////////////////////////
static TreeNode * stmt_list(void);
static TreeNode * stmt(void);
static TreeNode* var(void);
static TreeNode * exp_stmt(void);
static TreeNode * select_stmt(void);
static TreeNode * iteration_stmt(void);
static TreeNode * return_stmt(void);
//static TreeNode * expression(void);
static TreeNode * assign_stmt(void);
static TreeNode * sim_expression(void);
static TreeNode * add_expression(void);
static TreeNode * term(void);
static TreeNode * factor(void);
//static TreeNode * call(void);
static TreeNode * args(void);
static TreeNode * args_list(void);

static void syntaxError(char * message)
{
    fprintf(listing,"\n>>> ");
    fprintf(listing,"syntax error at line %d: %s",lineno,message);
    //Error=TRUE;
}
static void match(TokenType expected)
{
    if(token==expected) {token=getToken();printf("%s\n",tokenString);}
    else
    {
        syntaxError("unexpected token -> " );
        printToken(token,tokenString);
        fprintf(listing ,"       ");
    }
}
TreeNode * dec_list(void)
{
   TreeNode * t=NULL;
   TreeNode * p=NULL; 
   printf("in dec_list\n");
   t =dec();
   p= t;
 
   while((token==INT)||(token==VOID))
    {
        TreeNode * q=dec();
        if(q!=NULL)
        {
            if(t==NULL)    t=p=q;
            else
            {
                p->sibling=q;
                p=q;
            }
        }
    }
   printf("out dec_list\n");
   return t;
}
TreeNode * dec(void)
{
    TreeNode * t = NULL;
	printf("int dec\n");
    if (token == VOID)
    {
        match(VOID);
    	t=newStmtNode(DecK);
		t->type=Void;
	}
    else if(token == INT)
    {
        match(INT);
    	t=newStmtNode(DecK);
		t->type=Integer;
    }
    else
    {
        syntaxError("unexpected token in Params -> ");
        printToken(token,tokenString);
        token = getToken();
    }
	if((t!=NULL)&&(token==ID))
	{
         t->attr.name=copyString(tokenString);
         t->kind.stmt=VarDecK;
		 match(ID);
	}
    switch(token){
	case SEMI:
		match(SEMI);
		break;
	case LRPAREN:
		t->kind.stmt = FunDecK;
		match(LRPAREN);
		t->child[0] = params();
		match(RRPAREN);
		t->child[1] = compound_stmt();
		break;
	default:
		syntaxError("Unexpected token -> ");
		printToken(token,tokenString);
		token = getToken();
		break;
	}
	printf("out dec \n");
    return t;
}
/*TreeNode * var_dec(void) 
{
    TreeNode * t = newStmtNode(VarDecK);
	if((token!=NULL)&&(token==INT))
	{
		match(INT);
		t->type = Integer;
		match(ID);
		t->attr.name =copyString(tokenString);
	}
	if((token!=NULL)&&(token==VOID))
	{
		match(VOID);
		t->type = Void;
		match(ID);
		t->attr.name =copyString(tokenString);
	}	
	return t;
}
TreeNode * fun_dec(void)
{
    TreeNode * t = newStmtNode(FunDecK);
	if((token!=NULL)&&(token==INT))
	{
		match(INT);
		t->type = Integer;
		match(ID);
		t->attr.name =copyString(tokenString);
	}
	if((token!=NULL)&&(token==VOID))
	{
		match(VOID);
		t->type = Void;
		match(ID);
		t->attr.name =copyString(tokenString);
	}	
	match(LRPAREN);
	if(t!=NULL) t->child[0] = params();
	match(RRPAREN);
	if(t!=NULL) t->child[1] = compound_stmt();
	return t;
}*/
TreeNode * params(void)
{
    TreeNode * t = NULL;
	printf("in params");
	if(token == VOID)
	{
		match(token);
		t = newStmtNode(ParamListK);  
		t->type = Void;
	}
	else
	{
		t = param_list();
	}
	printf("out params");
	return t;
}

TreeNode * param_list(void)
{
	TreeNode * t =NULL;
	TreeNode * p = NULL;
	TreeNode * q=NULL;
	printf("in param_list\n");
	t= param();
	p=t;
	while(token != RRPAREN)
	{
		match(COMMA);
		 q= param();
		if((q!= NULL)&& (t!=NULL))
		{
			p->sibling = q;
			p=q;
		}
	}
	printf("out param_list\n");
	return t;
}

TreeNode * param(void)
{
	TreeNode * t = NULL;
	printf("in param\n");
	t=	newStmtNode(ParamK);
	t->type =Integer;
	match(INT);
	t->attr.name =copyString(tokenString);
	match(ID);
	printf("out param\n");
	return t;
}
TreeNode * compound_stmt(void)
{
	TreeNode * t = NULL;
	printf("in com_stmt\n");
	t = newStmtNode(ComK);
	match(LDPAREN);
	if(t!=NULL) 
	{
		t->child[0] = local_declarations();
		t->child[1] = stmt_list();
	}
	match(RDPAREN);
	printf("out com_stmt\n");
	return t;
}
TreeNode * local_declarations(void)
{
	TreeNode * t =NULL;
	TreeNode * p=NULL;
	printf("in local_dec\n");
	t = newStmtNode(LocalDecK);
	p=t;
	while(token == INT)
	{
		TreeNode * q=dec();
		if(q!=NULL)
		{
			if(t==NULL)	t=p=q;
			else
			{	
				p->sibling=q;
				p=q;
			}
		}
	}
	printf("out local_dec\n");
	return t;
} 
TreeNode * stmt_list(void)
{
    TreeNode * t=NULL;
    TreeNode * p=NULL;
	printf("in stmt_list\n");
	if(token==IF||token==WHILE||token==RETURN||token==INT||token==LRPAREN||token==NUM||token==ID)//by wi
		t= newStmtNode(StmtListK);
	if(t == NULL) return t;
	p=stmt();
	t->child[0] =p;
    while(token==IF||token==WHILE||token==RETURN||token==INT||token==LRPAREN||token==NUM||token==ID)//by wi
    {
		TreeNode *q;
        q=stmt();
        if(q!=NULL)
        {
            if(t==NULL)    t=p=q;
            else
            {
                p->sibling = q;
                p = q;
            }
        }
    }
	printf("out stmt_list\n");
    return t;
}

TreeNode * stmt(void)
{
    TreeNode * t=NULL;
	printf("in stmt\n");
    switch(token)
    {
        case IF:    t=select_stmt();        break;
        case ID:    t=exp_stmt();          break;
        case WHILE: t=iteration_stmt();     break;
        case RETURN:t=return_stmt();        break;
        case LDPAREN:t=compound_stmt();     break;
        default:
            syntaxError("unexpected token -> ");
            printToken(token,tokenString);
            token=getToken();
            break;
    }
	printf("out stmt\n");
    return t;
}
TreeNode * exp_stmt(void)
{
	TreeNode * t=NULL;
	if(token==SEMI)
	{
		match(SEMI);
		return t;
	}
	else
	{
		t=newStmtNode(ExpStmtK);
		t->child[0]=assign_stmt();
		match(SEMI);
		return t;
	}
	
}
TreeNode * select_stmt(void)
{
    TreeNode * t=NULL;
	printf("in select\n");
	t=newStmtNode(IfK);
    match(IF);
    match(LRPAREN);
    if(t!=NULL)    t->child[0]=sim_expression();//by wi
    match(RRPAREN);
    if(t!=NULL)    t->child[1]=stmt_list();  
    if(token==ELSE)
    {
        match(ELSE);
		if (t!=NULL) t->child[2] = newStmtNode(ElseK);
		t->child[2]->child[0] = stmt_list();

    }
	printf("out select\n");
    return t;
}
TreeNode * iteration_stmt(void)
{
    TreeNode * t=NULL;
	printf("in iteration\n");
	t=newStmtNode(WhileK);
    match(WHILE);
    match(LRPAREN);
    if(t!=NULL)    t->child[0]=sim_expression();//by wi
    match(RRPAREN);
    if(t!=NULL)    t->child[1]=stmt_list();
	printf("out iteration\n");
    return t;
}
TreeNode * return_stmt(void)
{
    TreeNode * t=NULL;
	printf("in return_stmt\n");
	t=newStmtNode(ReturnK);
    match(RETURN);
    if(token!=SEMI)
    {
        if(t!=NULL)    t->child[0]=sim_expression();
    }
	match(SEMI);//by wi
	printf("out return \n");
    return t;
}
TreeNode * assign_stmt(void)
{
    TreeNode * t=NULL;
	printf("in assign\n");
	t=newStmtNode(AssignK);
	if((t!=NULL)&&(token==ID))
    t->attr.name=copyString(tokenString);
	match(ID);
	if(token==ASSIGN)
	{
		match(ASSIGN);
		if(t!=NULL)    t->child[0]=sim_expression();
	}
	else
	{
		t->child[0]=sim_expression();
	}
    
	printf("out assign\n");
    return t;
}
TreeNode * sim_expression(void)
{
    TreeNode * t=NULL;
	printf("in sim\n");
	t=add_expression();
    if((token==SMALL)||(token==LARGE)||(token==SE)||(token==LE)||(token==EQ)||(token==NEQ))
    {
        TreeNode * p=newExpNode(OpK);
        if(p!=NULL)
        {
            p->child[0]=t;
            p->attr.op=token;
            t=p;
        }
        match(token);
        if(t!=NULL)
        {
            t->child[1]=add_expression();
        }
    }
	printf("out sim\n");
    return t;
}
TreeNode * add_expression(void)
{
    TreeNode * t=NULL;
	printf("in add\n");
	t=term();
    while((token==PLUS)||(token==MINUS))
    {
        TreeNode * p=newExpNode(OpK);
        if(p!=NULL)
        {
            p->child[0]=t;
            p->attr.op=token;
            t=p;
            match(token);
            t->child[1]=term();
        }
    }
	printf("out add\n");
    return t;
}

TreeNode * term(void)
{
    TreeNode * t=NULL;
	printf("in term\n");
	t =factor();
    while((token==MULTIPY)||(token==DIVIDE))
    {
        TreeNode * p=newExpNode(OpK);
        if(p!=NULL)
        {
            p->child[0]=t;
            p->attr.op=token;
            t=p;
            match(token);
            p->child[1]=factor();
        }
    }
	printf("out term\n");
    return t;
}
TreeNode * factor(void)
{
    TreeNode * t=NULL;
	TreeNode * p=NULL;
	printf("in factor\n");
	switch(token)
        {

            case NUM:
                t=newExpNode(ConstK);
                if((t!=NULL)&&(token==NUM))
                    t->attr.val=atoi(tokenString);
                match(NUM);
                break;
            case ID:
				t = var();
				if(token == LRPAREN)
				{
					p = newStmtNode(CallK);
					p->attr.name = t->attr.name;
					t=p;
					match(LRPAREN);
					t->child[0] = args();
					match(RRPAREN);
				}
                break;
            case LRPAREN:
                match(LRPAREN);
                t=stmt_list();
                match(RRPAREN);
                break;
            default:
                syntaxError("unexpected token -> ");
                printToken(token,tokenString);
                token=getToken();
                break;
        }
	printf("out fuck\n");
    return t;   
}
TreeNode* var(void)
{
	TreeNode* t = newExpNode(IdK);
	if ((t!=NULL) && (token==ID))
		t->attr.name = copyString(tokenString);
	match(ID);
	return t;
}

/*TreeNode * call(void)
{
    TreeNode * t =NULL;
	printf("in call\n");
	t=newStmtNode(CallK);
	t->attr.name =copyString(tokenString);
    match(ID);
    match(LRPAREN);
    t->child[0]=args();
    match(RRPAREN);
	printf("out call\n");
    return t;
}*/
TreeNode * args(void)
{
    TreeNode * t=NULL;
    TreeNode * p=NULL;
	printf("in args\n");
	t=newStmtNode(ArgsListK);
	p=t;
	while(token != RRPAREN)
	{
		TreeNode * q =args_list();
		if(q!=NULL)
		{
			if(t==NULL)	t=p=q;
			else
			{	
				p->sibling=q;
				p=q;
			}
		}
	}
	printf("out args\n");
	return t;
}
TreeNode * args_list(void)
{
    TreeNode * t=NULL;
	TreeNode * p =NULL;
	printf("in argslist\n");
	t = add_expression();
	p=t;
	while(token != RRPAREN)
	{
		TreeNode * q;
		match(COMMA);
		q = add_expression();
		if(t!=NULL && q!=NULL)
		{
			p->sibling = q;
			p=q;
		}
	}
	printf("out argslist\n");
	return t;
} 
TreeNode * parse(void)
{
	TreeNode * t;
	token=getToken();printf("%s\n",tokenString);
	t=dec_list();
	if(token!=ENDFILE)
		syntaxError("Code ends before file\n");
	return t;
}