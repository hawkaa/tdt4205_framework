#include "bindnames.h"
extern int outputStage; // This variable is located in vslc.c
char* thisClass;

int b_d(node_t* root, int stack_offset);
int b_c(node_t* root, int stack_offset);

static const int OFFSET_SIZE = 4;

symbol_t*
create_symbol(node_t* declaration_node, int stackOffset)
{
	symbol_t *s = malloc(sizeof(symbol_t));
	s->label = STRDUP(declaration_node->label);
	s->stack_offset = stackOffset;
	s->depth = 0; /* ignored */
	s->type = declaration_node->data_type;
	return s;
}

function_symbol_t*
create_function_symbol(node_t* function_node)
{
	/* alloc space on heap */
	function_symbol_t *fs = malloc(sizeof(function_symbol_t));

	/* basic attributes */
	fs->label = STRDUP(function_node->label);
	fs->return_type = function_node->data_type;
	
	if(function_node->children[0] != NULL) {
		/* function has parameter list */

		/* parameter list pointer */
		node_t *vl = function_node->children[0];

		/* allocate and save data types */
		fs->argument_types = malloc(sizeof(data_type_t*)
						* vl->n_children);
		for(int i = 0; i < vl->n_children; ++i)
			fs->argument_types[i] = vl->children[i]->data_type;
		
		/* store number of children */
		fs->nArguments = vl->n_children;
	} else {
		/* no children, so no need to alloc */
		fs->nArguments = 0;
	}
	return fs;
}


int bind_default ( node_t *root, int stackOffset)
{
	return b_d(root,stackOffset);
}


int
bind_function(node_t *root, int stackOffset)
{
	/* debug output for stage 6 */
	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION: Start: %s\n", root->label);
	
	/* function introduces a new scope */
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

int bind_declaration_list ( node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf ( stderr, "DECLARATION_LIST: Start\n");



	if(outputStage == 6)
		fprintf ( stderr, "DECLARATION_LIST: End\n");

}

int
bind_class(node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf(stderr, "CLASS: Start: %s\n", root->children[0]->label);
	
	class_symbol_t *class_symbol = malloc(sizeof(class_symbol));
	class_symbol->size = root->children[0]->n_children;
	

	if(outputStage == 6)
			fprintf(stderr, "CLASS: End\n");

}

int bind_function_list ( node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION_LIST: Start\n");

	/* function list is a new scope */
	scope_add();

	/* all functions must be added to symbol table */
	for(int i = 0; i < root->n_children; ++i)
		function_add(root->children[i]->label, create_function_symbol(
			root->children[i]));

	/* bind all functions */
	for(int i = 0; i < root->n_children; ++i)
		root->children[i]->bind_names(root->children[i], 2);

	/* exit scope */
	scope_remove();


	if(outputStage == 6)
		fprintf ( stderr, "FUNCTION_LIST: End\n");

}

int bind_constant ( node_t *root, int stackOffset)
{
	return b_c(root, stackOffset);
}



int bind_declaration ( node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf(stderr, "DECLARATION: parameter/variable : '%s', offset: %d\n", root->label, stackOffset);
	symbol_insert(root->label, create_symbol(root, stackOffset));

}

int
bind_variable(node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf ( stderr, "VARIABLE: access: %s\n", root->label);
	
	root->entry = symbol_get(root->label);
}

int
bind_expression(node_t* root, int stackOffset)
{
	if(outputStage == 6)
		fprintf( stderr, "EXPRESSION: Start: %s\n", root->expression_type.text);

	switch(root->expression_type.index) {
	case FUNC_CALL_E:
		/* function call, retrieve function */
		root->function_entry = function_get(root->children[0]->label);
		break;
	default:
		for(int i = 0; i < root->n_children; ++i)
			root->children[i]->bind_names(root->children[i], 0);
	}
	if(outputStage == 6)
		fprintf( stderr, "EXPRESSION: End\n");

}


