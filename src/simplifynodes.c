#include "simplifynodes.h"

extern int outputStage; // This variable is located in vslc.c

Node_t* simplify_default ( Node_t *root, int depth )
{
	int i;
	

	/* children first */
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}
}


Node_t *simplify_types ( Node_t *root, int depth )
{	
	int i;
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	/* children first */
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}
	/* children first */
	//for(int i = 0; i < root->n_children; ++i)
	//	root->children[i]->simplify(root->children[i], depth + 1);
}


Node_t *simplify_function ( Node_t *root, int depth )
{
	int i;

	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	/* children first */
	for(int i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}
}


Node_t *simplify_class( Node_t *root, int depth )
{
	int i;

	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	/* children first */
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}
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
			root->children[i]->simplify(root->children[i], depth + 1);
	}
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
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}
	/* children first */
	//for(int i = 0; i < root->n_children; ++i)
	//	root->children[i]->simplify(root->children[i], depth + 1);
}

Node_t *simplify_list_with_null ( Node_t *root, int depth )
{
	int i;
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );

	/* children first */
	for(int i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}
}


Node_t *simplify_list ( Node_t *root, int depth )
{
	int i;
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s \n", depth, ' ', root->nodetype.text );
		
	/* children first */
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}

}


Node_t *simplify_expression ( Node_t *root, int depth )
{
	int i;
	if(outputStage == 4)
		fprintf ( stderr, "%*cSimplify %s (%s) \n", depth, ' ', root->nodetype.text, root->expression_type.text );
	/* children first */
	for(int i = 0; i < root->n_children; ++i) {
		//root->children[i]->simplify(root->children[i], depth + 1);
	}
	/* children first */
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL)
			root->children[i]->simplify(root->children[i], depth + 1);
	}

	/* children first */
	//for(int i = 0; i < root->n_children; ++i)
	//	root->children[i]->simplify(root->children[i], depth + 1);
}

