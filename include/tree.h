#ifndef TREE_H
#define TREE_H

/* global includes */
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>

/* local includes */
#include "symtab.h"
#include "nodetypes.h"

/*
 * Macro for creating a heap-allocated duplicate of a string.
 * This macro mirrors the function 'strdup' (which is itself a pretty
 * common standard extension by GCC and many others). The function is not
 * part of the C99 standard because it allocates heap memory as a
 * side-effect, so it is reimplemented here in terms of std. calls.
 */
#define STRDUP(s) strncpy ( (char*)malloc ( strlen(s)+1 ), s, strlen(s)+1 )




/*
 * Basic data structure for syntax tree nodes.
 */
typedef struct node_t {
	
	/* type of this node*/
	nodetype_t nodetype;

	/* data label for variables */
	char *label;

	/* if the node is an expression, the kind of expression */
	expression_type_t expression_type;

	/* the data type, only used for declarations */
	data_type_t data_type;

	/* data type for variables in the symbol table entry */
	int string_index;

	/* pointer to symbol table entry */
	symbol_t *entry;
	
	/* pointer to class symbol table entry */
	class_symbol_t* class_entry;
	
	/* pointer to function symbol table entry */
	function_symbol_t* function_entry;
	
	/* number of children */
	int n_children;
	
	/* child pointer variables */
	struct node_t **children;
	
	/* simplify function */
	struct node_t *(*simplify)( struct node_t*, int );
	
	/* bind names function */
	int (*bind_names)( struct node_t*, int );
	
	/* typecheck function */
	data_type_t (*typecheck)( struct node_t*);

	/* generate function */
	void (*generate)( struct node_t*, int);
	
	/* constant node values */
	union {
		double double_const;
		int int_const;
		float float_const;
		char char_const;
		char* string_const;
		bool bool_const;
	};

} node_t;


/*
 *  Function prototypes: implementations are found in tree.c
 */
node_t * node_init ( nodetype_t type,
		char* label,
		base_data_type_t base_type,
		expression_type_t expression_type,
		int n_children,
		va_list child_list );

node_t * node_init_c ( nodetype_t type,
		char* label,
		base_data_type_t base_type,
		expression_type_t expression_type,
		int n_children,
		node_t **children);
void node_print ( FILE *output, node_t *root, int nesting );
void node_print_entries ( FILE *output, node_t *root, int nesting );
void node_finalize ( node_t *discard );

void destroy_subtree ( FILE *output, node_t *discard );
node_t *simplify_tree ( node_t *root );
void bind_names ( node_t *root );

#endif
