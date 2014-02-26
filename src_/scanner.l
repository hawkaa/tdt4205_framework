%{
/* This %{ ... %} section is code that is glued into the generated scanner. */
#include "parser.h"
#define YYDEBUG 1
extern int outputStage; // This variable is located in vslc.c
    #define RETURN(t) do {                                             \
        if ( outputStage == 1 )                                        \
            fprintf ( stderr, "TOKEN ( %d,\t'%s' )\n", t, yytext );    \
        return t;                                                      \
    } while ( 0 )
    
    
%}

%option array
%option noyywrap
%option yylineno


%%

\/\/.*\n	{}
[\ \t\n]	{}
INT		{ RETURN(INT); }
FLOAT		{ RETURN(FLOAT);}
BOOL		{ RETURN(BOOL); }
VOID		{ RETURN(VOID); }
IDENTIFIER	{ RETURN(IDENTIFIER); }
TRUE		{ RETURN(TRUE_CONST); }
FALSE		{ RETURN(FALSE_CONST); }


FUNC		{ RETURN(FUNC); }
START		{ RETURN(START); }
PRINT		{ RETURN(PRINT); }
RETURN		{ RETURN(RETURN);}
IF		{ RETURN(IF);}
THEN		{ RETURN(THEN);}
ELSE		{ RETURN(ELSE);}
END		{ RETURN(END); }
WHILE		{ RETURN(WHILE); }
DO		{ RETURN(DO); }

CLASS		{ RETURN(_CLASS_); }
THIS		{ RETURN(THIS); }
HAS		{ RETURN(HAS); }
WITH		{ RETURN(WITH); }
NEW		{ RETURN(NEW); }

:=		{ RETURN(ASSIGN); }

\=\=		{ RETURN(EQUAL); }
\>\=		{ RETURN(GEQUAL); }
\<\=		{ RETURN(LEQUAL); }
!\=		{ RETURN(NEQUAL); }
&&		{ RETURN(AND); }
\|\|		{ RETURN(OR); }



\"(\\.|[^\\"])*\"	{ RETURN(STRING_CONST); }
[a-zA-Z\_][a-zA-Z\_0-9]*	{ RETURN(267); }
[0-9]+\.[0-9]+	{ RETURN(FLOAT_CONST); }
[0-9]*		{ RETURN(INT_CONST); }




.		{ RETURN(yytext[0]); }
%%
