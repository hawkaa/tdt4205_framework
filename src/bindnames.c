/* global include */
#include <stdlib.h>

/* local include */
#include "bindnames.h"

/* output stage, located in vslc.c */
extern int outputStage;

char* thisClass;

static const int OFFSET_SIZE = 4;

/*
 * Default symtab binding function.
 * Will recursively call the corresponding binding function on its children,
 * not adding anything itself
 */
int
bind_default(node_t *root, int stackOffset)
{
	for(int i = 0; i < root->n_children; ++i)
		if(root->children[i] != NULL)
			root->children[i]->bind_names(root->children[i], 0);
}

/*
 * Symtab constant binding function.
 * Will bind strings to the symbol table.
 * Return value not used.
 */
int
bind_constant(node_t *root, int stackOffset)
{
	/* output for stage 6 */
	if(outputStage == 6)
		fprintf(stderr, "CONSTANT\n");	

	/* check data type */	
	switch(root->data_type.base_type) {
	case STRING_TYPE:
		strings_add(root->string_const);
		break;
	}

}

int
bind_function(node_t *root, int stackOffset)
{
	
	int automatic_var_offset, formal_var_offset;

	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION: Start: %s\n", root->label);

	/* variable list */
	node_t *vl = root->children[0];
	if(vl != NULL) {
		formal_var_offset = stackOffset + vl->n_children - 1;
		for(int i = 0; i < vl->n_children; ++i) {
			vl->children[i]->bind_names(vl->children[i], formal_var_offset*OFFSET_SIZE);
			--formal_var_offset;
		}
	}
		

	/* function statement list */
	node_t *sl = root->children[1];
	automatic_var_offset = -1;
	for(int i=0; i < sl->n_children; ++i) {
		if(sl->children[i]->nodetype.index == DECLARATION_STATEMENT) {
			sl->children[i]->bind_names(sl->children[i], automatic_var_offset*OFFSET_SIZE);
			--automatic_var_offset;
		} else {
			sl->children[i]->bind_names(sl->children[i], 0);
		}
	}

	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION: End\n");

}

int
bind_declaration_list ( node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf ( stderr, "DECLARATION_LIST: Start\n");
	
	/* stack offset will contain the last stack offset in this list */

	stackOffset += root->n_children;
	for(int i = 0; i < root->n_children; ++i) {
		root->children[i]->bind_names(root->children[i], stackOffset * OFFSET_SIZE);
		--stackOffset;
	}

	if(outputStage == 6)
		fprintf ( stderr, "DECLARATION_LIST: End\n");

}

int
bind_class(node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf(stderr, "CLASS: Start: %s\n", root->children[0]->label);
	
	/* declaration list, skipped... */
	node_t *dl = root->children[0];

	/* function list */
	node_t *fl = root->children[1];

	/* iterate through function list */
	for(int i = 0; i < fl->n_children; ++i)
		fl->children[i]->bind_names(fl->children[i], 3);

	if(outputStage == 6)
			fprintf(stderr, "CLASS: End\n");

}

function_symbol_t* create_function_symbol(node_t* function_node)
{

}

int bind_function_list ( node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION_LIST: Start\n");
	/* these are all functions*/
	for(int i = 0; i < root->n_children; ++i)
		root->children[i]->bind_names(root->children[i], 2);
	

	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION_LIST: End\n");

}


symbol_t*
create_symbol(node_t* declaration_node, int stackOffset)
{

}

int
bind_declaration(node_t *root, int stackOffset)
{

	if(outputStage == 6)
		fprintf(stderr, "DECLARATION: parameter/variable : '%s', offset: %d\n", root->label, stackOffset);

	for(int i = 0; i < root->n_children; ++i)
		if(root->children[i] != NULL)
			root->children[i]->bind_names(root->children[i], stackOffset + 1);
}

int
bind_variable(node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf ( stderr, "VARIABLE: access: %s\n", root->label);

	for(int i = 0; i < root->n_children; ++i)
		if(root->children[i] != NULL)
			root->children[i]->bind_names(root->children[i], stackOffset + 1);
}

int bind_expression( node_t* root, int stackOffset)
{
	if(outputStage == 6)
		fprintf( stderr, "EXPRESSION: Start: %s\n", root->expression_type.text);
	switch(root->expression_type.index) {
	case CLASS_FIELD_E:
		root->children[0]->bind_names(root->children[0], 0);
		break;
	case FUNC_CALL_E:
		/*
		 * Its a function call. Skip the variable access and bind
		 * the expression list.
		 */
		if(root->children[1] != NULL)
			root->children[1]->bind_names(root->children[1], 0);
			
		break;		
	case METH_CALL_E:
		root->children[0]->bind_names(root->children[0], 0);
		
		if(root->children[2] != NULL)
			root->children[2]->bind_names(root->children[2], 0);

		break;
	default:
		for(int i = 0; i < root->n_children; ++i)
			if(root->children[i] != NULL)
				root->children[i]->bind_names(root->children[i], stackOffset + 1);
	}

	if(outputStage == 6)
		fprintf( stderr, "EXPRESSION: End\n");

}

