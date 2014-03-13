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
 * Checks if two data types are equal. If the types are of type class,
 * class names are checked
 */
static int
equal_types(data_type_t a, data_type_t b)
{	
	/* compare base types */
	if (a.base_type != b.base_type)
		return 0;

	/* compare class names */
	if (a.base_type == CLASS_TYPE) {
		if (strcmp(a.class_name, b.class_name)) {
			return 0;
		}
	}

	/* comparison check ok, return true */
	return 1;
}

/*
 * Type error
 * Will effectively halt the program.
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

/*
 * Typecheck default
 * Function already implemented.
 */
data_type_t
typecheck_default(node_t* root)
{
	return td(root);
}

/*
 * Typecheck expression
 * Additional code for class field access, function and method calls added.
 */
data_type_t
typecheck_expression(node_t* root)
{
	data_type_t toReturn;
	data_type_t actual_param, formal_param;
	node_t *params;

	/* debug output for stage 10 */
	if(outputStage == 10)
		fprintf( stderr, "Type checking expression %s\n", root->expression_type.text);

	/* fetching from the predefined function */
	toReturn = te(root);

	//Insert additional checking here
	switch (root->expression_type.index) {
	case FUNC_CALL_E:
	case METH_CALL_E:
		/* parameters have different locations on method and function
		 calls */
		if (root->expression_type.index == FUNC_CALL_E)
			params = root->children[1];
		else
			params = root->children[2];

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
		/* will only return the type of the class field */
		return root->children[1]->entry->type;
	}

	/* return "default" value if none of above applies */
	return toReturn;	
	
}

/*
 * Typecheck variable
 * Function already implemented
 */
data_type_t
typecheck_variable(node_t* root)
{
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

	/* get left and right hand side types */
	lhs = root->children[0]->typecheck(root->children[0]);
	rhs = root->children[1]->typecheck(root->children[1]);

	/* compare types */
	if (!equal_types(lhs, rhs))
		type_error(root);
	
	/* they are equal, so just return one of them */
	return lhs;

}
