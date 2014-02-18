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
	int i;	

	for(i = 0; i < root->n_children; ++i) 
		if(root->children[i] != NULL)
			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
	return root;
}

/*
 * Type simplifier
 */
Node_t*
simplify_types(Node_t *root, int depth)
{	
	int i;

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
	}	
}

/*
 * Function simplifier
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
	root->data_type = type->data_type;
	node_finalize(type);

	/* we need function label */
	root->label = STRDUP(root->children[1]->label);
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

	


	
	/*
	TBR
	func = node_init_c(function_n, root->children[1]->label, 
			simplified_first->data_type.base_type,
			default_e, 2, children);
	func->data_type.class_name = simplified_first->data_type.class_name;
	return func; */
}


Node_t*
simplify_class(Node_t *root, int depth)
{
	int i;

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

	/*
	Node_t *declaration_list, *function_list;

	if(root->children[1] != NULL) {
		declaration_list = root->children[1]->simplify(root->children[1], depth + 1);
	} else {
		declaration_list = NULL;
	}
	
	if(root->children[2] != NULL) {
		function_list = root->children[2]->simplify(root->children[2], depth + 1);
	} else {
		function_list = NULL;
	}
	
	Node_t *children[2];
	children[0] = declaration_list;
	children[1] = function_list;
	
	return node_init_c(root->nodetype, root->children[0]->label, root->data_type.base_type,
		root->expression_type, 2, children);

	children first
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
	}
	return root;

	/* children first */
	/*for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}*/
	/* children first */
	//for(int i = 0; i < root->n_children; ++i)
	//	root->children[i]->simplify(root->children[i], depth + 1);
}

/*
 * Declaration simplifier.
 */
Node_t*
simplify_declaration_statement(Node_t *root, int depth)
{
	int i;

	/* debug info for stage 4 */
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );


	/* both children are simplified first */
	for(i = 0; i < root->n_children; ++i) {
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

	/* lets just create a new node *

	Node_t *declaration_statement;

	declaration_statement = node_init(declaration_statement_n, NULL, NO_TYPE, default_e, 0, 0);
	declaration_statement->data_type = root->children[0]->data_type;

	declaration_statement->label = root->children[1]->label;
	
	return declaration_statement;

	return root;
	/* children first */
	/*for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}*/
	/* children first */
	//for(int i = 0; i < root->n_children; ++i)
	//	root->children[i]->simplify(root->children[i], depth + 1);
}

/*
 * Single child simplify
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
 * Simplify list with null
 */
Node_t*
simplify_list_with_null(Node_t *root, int depth)
{
	int i;
	/* debug output for stage 4 */	
	if (outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	if(root->children[0] == NULL) {
		/*
		 * This is a bottom of a "list tree", get rid of the empty
		 * node at the bottom.
		 */

		/* keep pointer */
		Node_t *child = root->children[1]->simplify(root->children[1], depth+1);
		
		/* reallocate array */
		root->children = realloc(root->children, sizeof(Node_t*) * 1);
		root->children[0] = child;
		root->n_children = 1;

		return root;

	}
	else {
		Node_t *list, *elem;	

		list = root->children[0]->simplify(root->children[0], depth + 1);
		elem = root->children[1]->simplify(root->children[1], depth + 1);

		/* we need to merge all the list elements with the element at this level*/
		Node_t **children;
		children = malloc(sizeof(Node_t) * (list->n_children + 1));
		
		for(int i = 0; i < list->n_children; ++i) {
			children[i] = list->children[i];
		}

		children[list->n_children] = elem;

		/* clean up */
		free(root->children);
		root->children = children;
		root->n_children = list->n_children + 1;
		node_finalize(list);
	
		return root;

	}

	
	

//	if(root == NULL) {
//		/* 
//		 * Case 1: root node itself is null. Will this ever happen?
//		 */
//		return NULL;
//	} else if(root->children[0] == NULL) {
//		/*
//		 * Case 2: Left child is null. Get rid of it.
//		 */
//		Node_t *children[1];
//		//fprintf(stderr, "Getting rid of nil left side node\n");
//		children[0] = root->children[1]->simplify(root->children[1], depth+1);
//
//		return node_init_c(root->nodetype, root->label, root->data_type.base_type,
//		root->expression_type, 1, children);
//				
//		
//
//	} else {
//		for(i = 0; i < root->n_children; ++i) {
//			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
//		}
//
//		int grandchildren_count;
//		grandchildren_count = root->children[0]->n_children;
//		Node_t *children[grandchildren_count+1];
//
//		for(i = 0; i < grandchildren_count; ++i) {
//			children[i] = root->children[0]->children[i];	
//		}
//		children[grandchildren_count] = root->children[1];
//		
//		return node_init_c(root->nodetype, root->label, root->data_type.base_type,
//		root->expression_type, grandchildren_count+1, children);
//
//	}
//
//	/* children first */
//	/*for(int i = 0; i < root->n_children; ++i) {
//		if(root->children[i] != NULL)
//			root->children[i]->simplify(root->children[i], depth + 1);
//	}*/
}

/*
 * Simplifying a list
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
				
		Node_t *list, *elem;	

		list = root->children[0];
		elem = root->children[1];

		/* we need to merge all the list elements with the element at this level*/
		Node_t **children;
		children = malloc(sizeof(Node_t) * (list->n_children + 1));
		
		for(int i = 0; i < list->n_children; ++i) {
			children[i] = list->children[i];
		}

		children[list->n_children] = elem;

		/* clean up */
		free(root->children);
		root->children = children;
		root->n_children = list->n_children + 1;
		node_finalize(list);
	
		return root;
	
	}
	

	/*
	if(root->n_children  == 1) {
		/* discard current node 
		root = root->children[0];
		fprintf(stderr, "HER DA\n");
	} else {
		/* children first *
		/*for(i = 0; i < root->n_children; ++i) {
			if(root->children[i] != NULL)
				root->children[i]->simplify(root->children[i], depth + 1);
		}
	}*/
	
}


Node_t *simplify_expression ( Node_t *root, int depth )
{
	int i;
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s (%s) \n", depth, ' ', root->nodetype.text, 
		root->expression_type.text );
	
	//fprintf(stderr, "%i\n", root->n_children);
	if(root->n_children == 1) {
		switch(root->expression_type.index) {
		case UMINUS_E:
		case NEW_E:
			root->children[0] = root->children[0]->simplify(root->children[0], depth + 1);
			return root;
		default:
			return root->children[0]->simplify(root->children[0], depth + 1);
		}
	}

	/* children first */
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
	}
	return root;
}

