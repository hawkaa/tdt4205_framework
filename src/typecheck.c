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

static data_type_t
get_base_data_type(base_data_type_t t)
{
	data_type_t data_type;
	data_type.base_type = t;
	return data_type;
}

data_type_t
typecheck_default(node_t* root)
{
	for(int i = 0; i < root->n_children; ++i) {
		if (root->children[i] != NULL)
			root->children[i]->typecheck(root->children[i]);
	}
	return root->data_type;
}

data_type_t
typecheck_expression(node_t* root)
{
	if(outputStage == 10)
		fprintf( stderr, "Type checking expression %s\n", root->expression_type.text);
	
	data_type_t op1, op2;
	switch(root->expression_type.index) {
	case ADD_E:
	case SUB_E:
	case MUL_E:
	case DIV_E:
		op1 = root->children[0]->typecheck(root->children[0]);
		op2 = root->children[1]->typecheck(root->children[1]);
		if(op1.base_type == INT_TYPE && op2.base_type == INT_TYPE) {	
			return get_base_data_type(INT_TYPE);
		} else if (op1.base_type == FLOAT_TYPE && op2.base_type == FLOAT_TYPE) {
			return get_base_data_type(FLOAT_TYPE);
		} else {
			type_error(root);
		}
		break;	
	case LEQUAL_E:
	case GEQUAL_E:
	case LESS_E:
	case GREATER_E:
		op1 = root->children[0]->typecheck(root->children[0]);
		op2 = root->children[1]->typecheck(root->children[1]);
		if(op1.base_type == INT_TYPE && op2.base_type == INT_TYPE) {	
			return get_base_data_type(BOOL_TYPE);
		} else if (op1.base_type == FLOAT_TYPE && op2.base_type == FLOAT_TYPE) {
			return get_base_data_type(BOOL_TYPE);
		} else {
			type_error(root);
		}
		break;	
	case EQUAL_E:
	case NEQUAL_E:
		op1 = root->children[0]->typecheck(root->children[0]);
		op2 = root->children[1]->typecheck(root->children[1]);
		if(op1.base_type == INT_TYPE && op2.base_type == INT_TYPE) {	
			return get_base_data_type(BOOL_TYPE);
		} else if (op1.base_type == FLOAT_TYPE && op2.base_type == FLOAT_TYPE) {
			return get_base_data_type(BOOL_TYPE);
		} else if (op1.base_type == BOOL_TYPE && op2.base_type == BOOL_TYPE) {
			return get_base_data_type(BOOL_TYPE);
		} else {
			type_error(root);
		}
		break;	
	case UMINUS_E:
		op1 = root->children[0]->typecheck(root->children[0]);
		if (op1.base_type == INT_TYPE) {
			return get_base_data_type(INT_TYPE);
		} else if (op1.base_type == FLOAT_TYPE) {
			return get_base_data_type(FLOAT_TYPE);
		} else {
			type_error(root);
		}	
		break;
	case NOT_E:
		op1 = root->children[0]->typecheck(root->children[0]);
		if (op1.base_type == BOOL_TYPE) {
			return get_base_data_type(BOOL_TYPE);
		} else {
			type_error(root);
		}	
		break;
	case AND_E:
	case OR_E:
		op1 = root->children[0]->typecheck(root->children[0]);
		op2 = root->children[1]->typecheck(root->children[1]);
		if (op1.base_type == BOOL_TYPE && op2.base_type == BOOL_TYPE) {
			return get_base_data_type(BOOL_TYPE);
		} else {
			type_error(root);
		}
		break;
	case FUNC_CALL_E:
		if(root->children[1] != NULL)
			root->children[1]->typecheck(root->children[1]);
		break;
	case METH_CALL_E:
	case CLASS_FIELD_E:
		root->children[0]->typecheck(root->children[0]);
		break;
	case VARIABLE_E:
	case CONSTANT_E:
	case NEW_E:
	case DEFAULT_E:
	case THIS_E:
		/* dunno what to do with these */
		break;
	}
	
}


data_type_t typecheck_variable(node_t* root)
{
	return root->entry->type;
}
