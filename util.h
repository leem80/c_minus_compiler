#ifndef _UTIL_H_
#define _UTIL_H_

void printToken( TokenType ,const char * a);
TreeNode * newStmtNode(StmtKind);
TreeNode * newExpNode(ExpKind);
TreeNode * newDecNode(DecKind);
TreeNode * newParNode(ParKind);
char * copyString(char * b);
void printTree(TreeNode * );
#endif
