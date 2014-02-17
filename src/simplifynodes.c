#include "simplifynodes.h"

extern int outputStage; // This variable is located in vslc.c

Node_t* simplify_default ( Node_t *root, int depth )
{
	int i;	

	/* children first */
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
	}
	return root;
}


Node_t *simplify_types ( Node_t *root, int depth )
{	
	int i;
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );
	/* children first */

	/* check for children */
	if(root->n_children < 1) {
		return root;
	} else {
		/* We need the data, this is a class type */
		Node_t * n = node_init_c(type_n, root->label, 
			root->data_type.base_type,
			default_e, 0, NULL);
		n->data_type.class_name = root->children[0]->label;
		return n;


	}
	

	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
	}
	return root;
	return NULL;
	/* children first */
	/*for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}*/
	/* children first */
	//for(int i = 0; i < root->n_children; ++i)
	//	root->children[i]->simplify(root->children[i], depth + 1);
}


Node_t *simplify_function ( Node_t *root, int depth )
{
	int i;

	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );
	
	

	Node_t *simplified_first = root->children[0]->simplify(root->children[0], depth + 1);

	/* new function pointer*/
	Node_t *func; 

	/* pointers for the functions children */
	Node_t *children[2];

	/* parameter list */
	if(root->children[2] != NULL)
		children[0] = root->children[2]->simplify(root->children[2], depth + 1);
	else
		children[0] = NULL;

	/* statement list */
	children[1] = root->children[3]->simplify(root->children[3], depth + 1);

	/* allocating node */
	func = node_init_c(function_n, root->children[1]->label, 
			simplified_first->data_type.base_type,
			default_e, 2, children);
	func->data_type.class_name = simplified_first->data_type.class_name;
	return func;

	
	/* children first */
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
	}
	return root;
	/* children first */
	/*for(int i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}*/
}


Node_t *simplify_class( Node_t *root, int depth )
{
	int i;

	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

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

	/* children first */
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


Node_t *simplify_declaration_statement ( Node_t *root, int depth )
{
	int i;

	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );
	/* children first */
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
	}

	/* lets just create a new node */

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


Node_t *simplify_single_child ( Node_t *root, int depth )
{
	int i;

	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );
	/* children first */

	//return root;
	//fprintf(stderr, "%s\n", root->children[0]->nodetype.text);
	return root->children[0]->simplify(root->children[0], depth +1);
}
/*
 * Ends up here: FUNCTION_LIST, DECLARATION_LIST
 */
Node_t *simplify_list_with_null ( Node_t *root, int depth )
{
	int i;
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	

	if(root == NULL) {
		/* 
		 * Case 1: root node itself is null. Will this ever happen?
		 */
		return NULL;
	} else if(root->children[0] == NULL) {
		/*
		 * Case 2: Left child is null. Get rid of it.
		 */
		Node_t *children[1];
		//fprintf(stderr, "Getting rid of nil left side node\n");
		children[0] = root->children[1]->simplify(root->children[1], depth+1);

		return node_init_c(root->nodetype, root->label, root->data_type.base_type,
		root->expression_type, 1, children);
				
		

	} else {
		/*
	 	* Case 3: Left child is another simplify_list_with_null example
	 	*/

		/* simplify first */
		//Node_t *n = root->children[0]->simplify(root->children[0], depth + 1);

		//fprintf(stderr, "Current node has two children. Simplifying both\n");
		for(i = 0; i < root->n_children; ++i) {
			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
		}

		int grandchildren_count;
		grandchildren_count = root->children[0]->n_children;
		Node_t *children[grandchildren_count+1];

		for(i = 0; i < grandchildren_count; ++i) {
			children[i] = root->children[0]->children[i];	
		}
		children[grandchildren_count] = root->children[1];
		
		return node_init_c(root->nodetype, root->label, root->data_type.base_type,
		root->expression_type, grandchildren_count+1, children);

	}

	/* children first */
	/*for(int i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}*/
}

/*
 * Simplifying a list
 * Ends up here: STATEMENT_LIST
 */
Node_t *simplify_list ( Node_t *root, int depth )
{
	
	
	int i;
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	/* simplify all of the children first */
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i] = root->children[i]->simplify(root->children[i], depth + 1);
	}
	/* allocating new node for return */
	Node_t* n;
	
	/* then we need to flatten the list */
	if(root->n_children == 1) {

		//fprintf(stderr, "Tried to flatten list, but had only one element\n");
		/* only one child, nothing to do */
		return root;
	} else {
		
		int grandchildren_count;
		grandchildren_count = root->children[0]->n_children;
		Node_t *children[grandchildren_count+1];

		for(i = 0; i < grandchildren_count; ++i) {
			children[i] = root->children[0]->children[i];	
		}
		children[grandchildren_count] = root->children[1];
		
		return node_init_c(root->nodetype, root->label, root->data_type.base_type,
		root->expression_type, grandchildren_count+1, children);

	
	}
	
	return n;

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

