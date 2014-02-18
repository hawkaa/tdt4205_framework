/* global include */
#include <stdlib.h>

/* local includes*/
#include "simplifynodes.h"

/* output stage, located in vslc.c */
extern int outputStage;


/*
 * Default simplifier
 * Nothing to simplify, so proceed to simplify all children 
 * Ideally, vslc.c should use the return value of this function and not trust
 * that the data pointed to by root is altered. However, since the top node
 * goes to the simplify_default method, it will work. This method will only
 * alter the root, and not allocate another node to return.
 */
Node_t*
simplify_default(Node_t *root, int depth)
{
	for(int i = 0; i < root->n_children; ++i) 
		if(root->children[i] != NULL)
			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
	return root;
}

/*
 * Type simplifier
 * Node types: TYPE
 */
Node_t*
simplify_types(Node_t *root, int depth)
{	

	/* output for stage 4 */
	if(outputStage == 4)
		fprintf(stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text);

	if(root->n_children < 1) {
		/* no children, do not simplify */
		return root;
	} else {
		/* 
		 * This is a class type with one and only child containing
		 * the class name, which we need
		 */
		
		root->data_type.class_name = STRDUP(root->children[0]->label);
		
		/* get rid of the child */
		node_finalize(root->children[0]);
		root->children = realloc(root->children, 0);
		root->n_children = 0;
		return root;
	}	
}

/*
 * Function simplifier
 * Node types: FUNCTION
 */
Node_t*
simplify_function(Node_t *root, int depth)
{

	/* debug info for stage 4 */
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	
	/* function type node needs to be simplified, can be stored on stack */
	Node_t *type;
	type = root->children[0]->simplify(root->children[0], depth + 1);

	/* copy data type into root */
	root->data_type = type->data_type;

	/* remove type, has no children */
	node_finalize(type);

	/* we need function label */
	root->label = STRDUP(root->children[1]->label);

	/* remove identifier, has no children */
	node_finalize(root->children[1]);

	/*
	 * Lets generate a new pointers array, we know that the new simplified
	 * function node will have two children: parameter and statement list 
	 */
	Node_t **children;
	children = malloc(2 * sizeof(Node_t*));
	
	/* parameter list */
	if(root->children[2] != NULL)
		children[0] = root->children[2]->simplify(root->children[2], depth + 1);
	else
		children[0] = NULL;
	
	/* statement list */
	children[1] = root->children[3]->simplify(root->children[3], depth + 1);
	
	/* destroy children pointer array and replace it */
	free(root->children);
	root->children = children;
	root->n_children = 2;
	
	return root;
}

/*
 * Class simplifier.
 * Node types: CLASS
 */
Node_t*
simplify_class(Node_t *root, int depth)
{

	/* debug info for stage 4 */
	if(outputStage == 4)
		fprintf(stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text);
	
	/*
	 * A class will end up with two children; the declaration list and
	 * the function list.
	 */
	Node_t **children;
	children = malloc(2 * sizeof(Node_t*));
	
	/* declaration list */
	if(root->children[1] != NULL)
		children[0] = root->children[1]->simplify(root->children[1], depth + 1);
	else
		children[0] = NULL;
	
	/* fuction list */
	if(root->children[2] != NULL)
		children[1] = root->children[2]->simplify(root->children[2], depth + 1);
	else
		children[1] = NULL;

	/* update label */
	root->label = STRDUP(root->children[0]->label);

	/* update data type */
	root->data_type = root->children[0]->data_type;

	/* removing node and updating pointer array */
	node_finalize(root->children[0]);
	free(root->children);
	root->children = children;
	root->n_children = 2;

	return root;
}

/*
 * Declaration simplifier.
 * Node types: DELCARATION_STATEMENT
 */
Node_t*
simplify_declaration_statement(Node_t *root, int depth)
{
	/* debug info for stage 4 */
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );


	/* both children are simplified first */
	for(int i = 0; i < root->n_children; ++i) {
		root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
	}
	
	/* takes type data from type child, destroys it after */
	root->data_type = root->children[0]->data_type;
	node_finalize(root->children[0]);

	/* takes label from variable child, destroys it after */
	root->label = STRDUP(root->children[1]->label);
	node_finalize(root->children[1]);

	/* clean up */
	root->children = realloc(root->children, 0);
	root->n_children = 0;

	return root;
}

/*
 * Single child simplify.
 * Will remove intermediate node
 * Node types: STATEMENT, PARAMETER_LIST, ARGUMENT_LIST
 */
Node_t*
simplify_single_child(Node_t *root, int depth)
{
	/* debug output for stage 4 */
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	/* save child pointer */
	Node_t* child;
	child = root->children[0]->simplify(root->children[0], depth +1);

	/* clear root */
	node_finalize(root);

	/* return child pointer */
	return child;
}

/*
 * Simplify list with null.
 * Node types: DECLARATION_LIST, FUNCTION_LIST
 */
Node_t*
simplify_list_with_null(Node_t *root, int depth)
{
	/* debug output for stage 4 */	
	if (outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	if(root->children[0] == NULL) {
		/*
		 * This is a bottom of a "list tree", it has no list as a
		 * child. Remove the null node.
		 */

		/* keep pointer */
		Node_t *child;
		child = root->children[1]->simplify(root->children[1], depth+1);
		
		/* reallocate array */
		root->children = realloc(root->children, sizeof(Node_t*) * 1);
		root->children[0] = child;
		root->n_children = 1;

		return root;
	} else {
		/*
		 * We have a list on the left side and a element to be added
		 * to this list at the right side
		 */
		Node_t *list, *elem;	

		list = root->children[0]->simplify(root->children[0], depth + 1);
		elem = root->children[1]->simplify(root->children[1], depth + 1);
		
		/* as we have the pointers, lets reallocate */
		root->children = realloc(root->children, sizeof(Node_t) * (list->n_children + 1));

		/* populate */
		for(int i = 0; i < list->n_children; ++i) {
			root->children[i] = list->children[i];
		}
		root->children[list->n_children] = elem;
		
		/* keep things in a consistent state */
		root->n_children = list->n_children + 1;

		/* clean up */
		node_finalize(list);

		return root;
	}
}

/*
 * Simplifying a list
 * Node types: STATEMENT_LIST, EXPRESSION_LIST, VARIABLE_LIST, CLASS_LIST 
 */
Node_t*
simplify_list(Node_t *root, int depth)
{
	
	/* debug output for stage 4 */
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	/* simplify all of the children */
	for(int i = 0; i < root->n_children; ++i)
		root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);

	if(root->n_children == 1) {
		/* only one child, nothing to do */
		return root;
	} else {
		/* list has one element and one sublist */			
		Node_t *list, *elem;	

		/* already simplified */
		list = root->children[0];
		elem = root->children[1];
		
		/* as we have the pointers, lets reallocate */
		root->children = realloc(root->children, sizeof(Node_t) * (list->n_children + 1));

		/* populate */
		for(int i = 0; i < list->n_children; ++i) {
			root->children[i] = list->children[i];
		}
		root->children[list->n_children] = elem;
		
		/* keep things in a consistent state */
		root->n_children = list->n_children + 1;

		/* clean up */
		node_finalize(list);

		return root;
	}	
}

/*
 * Simplify expression.
 * Node types: EXPRESSION
 */
Node_t*
simplify_expression(Node_t *root, int depth)
{
	/* debug output for stage 4 */
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s (%s) \n", depth, ' ', root->nodetype.text, 
		root->expression_type.text );
	

	/* some expressions here depend on whether the expression is unary */
	if(root->n_children == 1) {
		
		Node_t *child;
	
		switch(root->expression_type.index) {
		case UMINUS_E:
		case NEW_E:
			/* simplify one and only child and keep it */
			root->children[0] = root->children[0]->simplify(root->children[0], depth + 1);
			return root;
		default:
			/* current expression is superfluous, lets ditch it */
			child = root->children[0]->simplify(root->children[0], depth +1);

			/* clear root */
			node_finalize(root);

			/* return child pointer */
			return child;
		}	
	} else {
		/* all children should be simplified */
		for(int i = 0; i < root->n_children; ++i) {
			if(root->children[i] != NULL)
				root->children[i] = root->children[i]->simplify(
						root->children[i], depth + 1);
		}
		return root;
	}
}

