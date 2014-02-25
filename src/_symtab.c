/* global include */
#include <stdlib.h>

/* local include */
#include "symtab.h"
#include "tree.h"

/* These variables is located in vslc.c */
extern int arch;
extern int outputStage; 

/* string pointer array */
static char **strings;
static int strings_size = 1, strings_index = -1;


/*
 * Initializes the symbol table
 */
void
symtab_init(void)
{
   strings = malloc(sizeof(char*) * strings_size);
}

/*
 * Finalizes the symbol table, cleaning up all the allocated data
 */
void
symtab_finalize(void)
{
	int i;
	for (i = 0; i <= strings_index; ++i) {
		free(strings[i]);
	}
	free(strings);
}

/*
 * Adds a string to the symbol table
 * Will take no copy of the string, will only point to the same string
 * constast.
 */
int
strings_add(char *str)
{

	++strings_index;

	/* print debug info for stage 7 */
	if(outputStage == 7)
		fprintf ( stderr, "Add strings (%s), index: %d \n", str, strings_index );

	/* make sure array is big enough */
	if (strings_index == strings_size) {
		strings_size *= 2;
		strings = realloc(strings, sizeof(char*) * strings_size);
	}
	
	/* copy the string */
	strings[strings_index] = STRDUP(str);

	return strings_index;
    
}

/*
 * Prints the data segment of the assembly code which includes all the string
 * constants found.
 * ARM and x86 have different formats
 */
void
strings_output(FILE *stream)
{
	if(arch == 1) {
		/* ARM */
		fputs (
			".syntax unified\n"
			".cpu cortex-a15\n"
			".fpu vfpv3-d16\n"
			".data\n"
			".align	2\n"
			".DEBUG: .ascii \"Hit Debug\\n\\000\"\n"
			".DEBUGINT: .ascii \"Hit Debug, r0 was: %d\\n\\000\"\n"
			".INTEGER: .ascii \"%d \\000\"\n"
			".FLOAT: .ascii \"%f \\000\"\n"
			".NEWLINE: .ascii \"\\n \\000\"\n",
			stream
		);

		for ( int i=0; i<=strings_index; i++ ) {
		    fprintf(stream, ".STRING%d: .ascii %s\n", i, strings[i]);
		    fprintf(stream, ".ascii \"\\000\"\n", i, strings[i]); /* ugly hack */
		}
		fputs(".globl main\n", stream);
		fputs(".align	2\n", stream);
	} else {
		/* x86 */
		fputs (
			".data\n"
			".INTEGER: .string \"%d \"\n"
			".FLOAT: .string \"%f \"\n"
			".NEWLINE: .string \"\\n \"\n",
			stream
		);

		for (int i = 0; i <= strings_index; i++)
			fprintf(stream, ".STRING%d: .string %s\n", i, strings[i]);
		fputs(".globl main\n", stream);
    }
}

