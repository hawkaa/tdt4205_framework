/* global include */
#include <stdlib.h>

/* local include */
#include "bindnames.h"

/* output stage, located in vslc.c */
extern int outputStage;

char* thisClass;

static const int OFFSET_SIZE = 4;


symbol_t*
create_symbol(node_t* declaration_node, int stackOffset)
{
	symbol_t *s = malloc(sizeof(symbol_t));
	s->label = STRDUP(declaration_node->label);
	s->stack_offset = stackOffset;
	s->type = declaration_node->data_type;
	return s;
}

function_symbol_t*
create_function_symbol(node_t* function_node)
{
	function_symbol_t *fs = malloc(sizeof(function_symbol_t));
	fs->label = STRDUP(function_node->label);
	fs->return_type = function_node->data_type;
	
	if(function_node->children[0] != NULL) {
		node_t *vl = function_node->children[0];
		
		fs->argument_types = malloc(sizeof(data_type_t*) * vl->n_children);
		for(int i = 0; i < vl->n_children; ++i)
			fs->argument_types[i] = vl->children[i]->data_type;


		fs->nArguments = vl->n_children;
	} else {
		fs->nArguments = 0;
	}
	return fs;
}

class_symbol_t*
create_class_symbol(node_t* class_node)
{
	class_symbol_t *cs = malloc(sizeof(class_symbol_t));
	cs->size = 0;
	cs->symbols = malloc(sizeof(hash_t));
	cs->functions = malloc(sizeof(hash_t));
	return cs;

}


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
	/* debug output for stage 6 */
	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION: Start: %s\n", root->label);

	/* function requires a new scope */
	scope_add();

	/* variable list */
	node_t *vl = root->children[0];
	if(vl != NULL) {
		int formal_var_offset = stackOffset + vl->n_children - 1;
		for(int i = 0; i < vl->n_children; ++i) {
			vl->children[i]->bind_names(vl->children[i],
					formal_var_offset*OFFSET_SIZE);
			--formal_var_offset;
		}
	}
		
	/* statement list */	
	node_t *sl = root->children[1];
	int automatic_var_offset = -1;
	for(int i = 0; i < sl->n_children; ++i) {
		sl->children[i]->bind_names(sl->children[i],
				automatic_var_offset*OFFSET_SIZE);
		if(sl->children[i]->nodetype.index == DECLARATION_STATEMENT) {
			--automatic_var_offset;
		}
	}

	/* exit function scope */	
	scope_remove();

	/* debug output for stage 6 */
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
	/* debug output for stage 6 */
	if(outputStage == 6)
		fprintf(stderr, "CLASS: Start: %s\n", root->children[0]->label);
	
	/* add class */
	class_add(root->label, create_class_symbol(root));

	/* declaration list, skipped... */
	node_t *dl = root->children[0];
	for(int i = 0; i < dl->n_children; ++i) {
		class_insert_field(root->label, dl->children[i]->label, create_symbol(dl->children[i], stackOffset));

	}

	/* function list */
	node_t *fl = root->children[1];

	/* iterate through function list */
	for(int i = 0; i < fl->n_children; ++i)
		fl->children[i]->bind_names(fl->children[i], 3);

	if(outputStage == 6)
			fprintf(stderr, "CLASS: End\n");

}

int
bind_function_list(node_t *root, int stackOffset)
{
	/* debug output for stage 6 */
	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION_LIST: Start\n");

	/* function list requires a new scope */
	scope_add();

	/* add all functions in list to symbol table first */
	for(int i = 0; i < root->n_children; ++i)
		function_add(root->children[i]->label, create_function_symbol(
			root->children[i]));


	/* bind all functions */
	for(int i = 0; i < root->n_children; ++i)
		root->children[i]->bind_names(root->children[i], 2);

	/* exit scope */
	scope_remove();

	/* debug output for stage 6 */
	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION_LIST: End\n");
}


int
bind_declaration(node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf(stderr, "DECLARATION: parameter/variable : '%s', offset: %d\n",
			root->label, stackOffset);
	/* insert variable in symbol table */
	symbol_insert(root->label, create_symbol(root, stackOffset)); 
/*
	for(int i = 0; i < root->n_children; ++i)
		if(root->children[i] != NULL)
			root->children[i]->bind_names(root->children[i], stackOffset + 1);
*/
}

int
bind_variable(node_t *root, int stackOffset)
{
	/* prologue */
	if(outputStage == 6)
		fprintf ( stderr, "VARIABLE: access: %s\n", root->label);

	/* retrieve and save */
	root->entry = symbol_get(root->label);
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
		root->function_entry = function_get(root->children[0]->label);
		
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

