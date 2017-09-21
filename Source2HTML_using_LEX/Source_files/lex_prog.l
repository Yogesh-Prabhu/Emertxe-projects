%{
#include "main.h"	
%}

comments	\/\/.*
datatype	int|float|char|double|short|long|static|const|volatile|struct|union|typedef|void|NULL
keywords	if|for|while|do|continue|break|return
prepro		#define[^/\n]+
header		#include[ \t^=]*((<.*>)|(\".*\"))
string		\"[^"\n]*["\n]
ascii		'.?'
variable	[a-zA-Z0-9_]+
num			[+-]?(([0-9]+)|([0-9]*\.[0-9]+))([eE][+-]?[0-9]+)?

%x COMMENT

%%

"/*"						{ BEGIN COMMENT; func_comments (yytext); /* Enter comment eating state */}
<COMMENT>"*/"				{ BEGIN 0; func_comments (yytext);}
<COMMENT>[a-zA-Z0-9_]+		{ func_comments (yytext);}
<COMMENT>[ ]				{ func_space ();}
<COMMENT>\t					{ func_tab ();}
<COMMENT>\n					{ func_newline ();}
<COMMENT>.					{ func_comments (yytext);}

{comments}		{ func_comments	(yytext);	/* Comments */}

\n				{ func_newline 	();	/* Separate function to introduce newline in HTML */ }
[ ]				{ func_space 	();	/* Separate function to introduce space in HTML */ }
\t				{ func_tab		();	/* Separate function to introduce tab in HTML */ }
{header}		{ func_header	(yytext);	/* Header file*/}
{prepro}		{ func_prepro	(yytext);	/* Preprocessor definitions */}
{datatype}		{ func_datatype	(yytext);	/* C datatypes */}
{keywords}		{ func_keywords	(yytext);	/* Keywords other than datatypes */}
{string}		{ func_string	(yytext);	/* String within double quotes (" ")*/}
{ascii}			{ func_ascii	(yytext);	/* Single ASCII char withing single quotes (' ')*/}
{num}			{ func_num_const(yytext);	/* Numeric constants including float and exponent */}
{variable}		{ func_others	(yytext);	/* C variable names */}
.				{ func_others	(yytext);	/* Remaining characters*/}

%%

main(int argc, char *argv[])
{
/*	
<COMMENT>[^a-zA-Z0-9_ \t\n]	{ func_comments (yytext);}
*/
	extern FILE *fptr_src;

	// Open source and destination files
	if (func_get_file_ptr(argc, argv) != e_success)
	{
		fprintf (stderr, "Function %s failed\n", "func_get_file_ptr()" );
		return 0;
	}

	// Set yyin to point at source C file
	yyin = fptr_src;

	// Write initial header of destination HTML file
	if (func_write_html_start() != e_success)
	{
		fprintf (stderr, "Function %s failed\n",  "func_write_html_start()");
		return 0;
	}

	// Call lexer program
	yylex();

	// Write closing syntax of destination HTML file
	if (func_write_html_end() != e_success)
	{
		fprintf (stderr, "Function %s failed\n", "func_write_html_end()");
		return 0;
	}

	return 0;
}



