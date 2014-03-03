#include "typecheck.h"
extern int outputStage;

void
type_error(node_t* root)
{
	fprintf(stderr, "Type error at:\n");
	if(root != NULL){
		fprintf(stderr,"%s", root->nodetype.text);
		if(root->nodetype.index == EXPRESSION){
			fprintf(stderr," (%s)", root->expression_type.text);
		}
		fprintf(stderr,"\n");
	}
	fprintf(stderr,"Exiting\n");
	exit(-1);
}

data_type_t
typecheck_default(node_t* root)
{
	for(int i = 0; i < root->n_children; ++i) {
		if (root->children[i] != NULL)
			root->children[i]->typecheck(root->children[i]);
	}

}

data_type_t
typecheck_expression(node_t* root)
{
	if(outputStage == 10)
		fprintf( stderr, "Type checking expression %s\n", root->expression_type.text);
	
	data_type_t op1, op2;
	switch(root->expression_type.text) {
	case "+":
		op1 = root->children[0]->typecheck(root->children[0]);
		op2 = root->children[1]->typecheck(root->children[1]);
		if(op1.base_type == INT_TYPE && op2.base_type == INT_TYPE) {
			
		} else if (op1.base_type == FLOAT_TYPE && op2.base_type == FLOAT TYPE) {

		} else {
			type_error(root);
		}
			
	}
	
}

data_type_t typecheck_variable(node_t* root)
{
	return root->entry->type;
}
