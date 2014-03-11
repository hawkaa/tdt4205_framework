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
	//Insert additional checking here
	
	fprintf(stderr, "hehe\n");

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
