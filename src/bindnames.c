/* global include */
#include <stdlib.h>

/* local include */
#include "bindnames.h"

/* output stage, located in vslc.c */
extern int outputStage;

char* thisClass;

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
	int offset = -4;

	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION: Start: %s\n", root->label);

	/* first child */
	if(root->children[0] != NULL)
		root->children[0]->bind_names(root->children[0], 0);
	/* function statement list */
	node_t *sl = root->children[1];
	for(int i=0; i < sl->n_children; ++i) {
		if(sl->children[i]->nodetype.index == DECLARATION_STATEMENT) {
			sl->children[i]->bind_names(sl->children[i], offset);
			offset -= 4;
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


	for(int i = 0; i < root->n_children; ++i)
		if(root->children[i] != NULL)
			root->children[i]->bind_names(root->children[i], stackOffset + 1);

	if(outputStage == 6)
		fprintf ( stderr, "DECLARATION_LIST: End\n");

}

int
bind_class(node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf(stderr, "CLASS: Start: %s\n", root->children[0]->label);

	for(int i = 0; i < root->n_children; ++i)
		if(root->children[i] != NULL)
			root->children[i]->bind_names(root->children[i], stackOffset + 1);
	

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

	for(int i = 0; i < root->n_children; ++i)
		if(root->children[i] != NULL)
			root->children[i]->bind_names(root->children[i], stackOffset + 1);
	

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

	for(int i = 0; i < root->n_children; ++i)
		if(root->children[i] != NULL)
			root->children[i]->bind_names(root->children[i], stackOffset + 1);
	

	if(outputStage == 6)
		fprintf( stderr, "EXPRESSION: End\n");

}

