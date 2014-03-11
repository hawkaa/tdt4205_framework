/* global includes */
#include <string.h>

/* local includes */
#include "typecheck.h"

/* defined in vslc.c */
extern int outputStage;

/* function prototypes */
data_type_t td(node_t* root);
data_type_t te(node_t* root);
data_type_t tv(node_t* root);

/*
 * Type error
 * Will effectively halt the program
 */
void
type_error(node_t* root) {
	fprintf(stderr, "Type error at:\n");
	if (root != NULL) {
		fprintf(stderr, "%s" , root->nodetype.text);
		if (root->nodetype.index == EXPRESSION)
			fprintf(stderr, " (%s)", root->expression_type.text);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "Exiting\n");
	exit(-1);
}

static int
equal_types(data_type_t a, data_type_t b)
{	
	//fprintf(stderr, "%i %i\n", a.base_type, b.base_type);
	//return a.base_type == b.base_type;
	if (a.base_type != b.base_type)
		return 0;
	
	if (a.base_type == CLASS_TYPE)
		if (!strcmp(a.class_name, b.class_name))
			return 0;
	return 1;
}

data_type_t typecheck_default(node_t* root)
{
	return td(root);
}

data_type_t typecheck_expression(node_t* root)
{
	data_type_t toReturn;

	if(outputStage == 10)
		fprintf( stderr, "Type checking expression %s\n", root->expression_type.text);

	toReturn = te(root);

	data_type_t actual_param, formal_param;
	node_t *params;
	//Insert additional checking here
	switch (root->expression_type.index) {
	case FUNC_CALL_E:
	case METH_CALL_E:
		/* we have a function call */
		params = root->children[1];

		if (params != NULL) {
			/* check number of arguments */
			if (params->n_children != root->function_entry->nArguments)
				type_error(root);

			/* iterate over children */
			for (int i = 0; i < params->n_children; ++i) {
				actual_param = params->children[i]->typecheck(params->children[i]);
				formal_param = root->function_entry->argument_types[i];
				if (!equal_types(actual_param, formal_param)) {
					type_error(root);
				}
			}
		}
		return root->function_entry->return_type;
	
	case CLASS_FIELD_E:
		return root->children[1]->entry->type;
		
	case VARIABLE_E:
	case NEW_E:
	case DEFAULT_E:
	case THIS_E:
		break;
	}

	

	return toReturn;	
	
}

data_type_t typecheck_variable(node_t* root){
	return tv(root);
}

/*
 * Typecheck assignment
 * Checks that left and right hand side of assignments match
 */
data_type_t
typecheck_assignment(node_t* root)
{
	data_type_t lhs, rhs;
	lhs = root->children[0]->typecheck(root->children[0]);
	rhs = root->children[1]->typecheck(root->children[1]);

	if (!equal_types(lhs, rhs))
		type_error(root);
	
	return lhs;

}
