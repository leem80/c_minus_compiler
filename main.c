#include "globals.h"
#define NO_PARSE FALSE
#define NO_ANALYZE FALSE

#include "util.h"
#if NO_PARSE
#include "scan.h"
#else
#include "parse.h"
#endif

int lineno = 0;
FILE * source;
FILE * listing;
//FILE * result;

int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;

int main (void)
{
    TreeNode * syntaxTree;

	source = fopen("source.txt","r");
	if(!source) 
		printf("opening source.txt failed!\n");
//	listing = fopen("listing.txt","w");
	
//	if(!listing) 
	//	printf("opening listing.txt failed!\n");
	//while(getToken() != ENDFILE)
	//	;

	listing=fopen("listing.txt","w");
	if(!listing)
		printf("opening listing.txt failed!\n");
	syntaxTree=parse();
	printf("11");
	if(TraceParse)
    {
        fprintf(listing,"\nSyntax tree:\n");
        printTree(syntaxTree);
    } 
	fclose(source);
	fclose(listing);  
//	fclose(result);

    return 0;
}