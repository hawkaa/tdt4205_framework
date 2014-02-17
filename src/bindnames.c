#include "bindnames.h"
extern int outputStage; // This variable is located in vslc.c
char* thisClass;

int bind_default ( node_t *root, int stackOffset)
{
	//fprintf(stderr, "BIND_DEFAULT\n");	
	int i;
	for(i = 0; i < root->n_children; ++i) {
		if(root->children[i] != NULL) {
			root->children[i]->bind_names(root->children[i], stackOffset + 1);
		}
	}
}

int bind_constant ( node_t *root, int stackOffset)
{
	if(outputStage == 6)
		fprintf(stderr, "CONSTANT\n");	
	if(root == NULL)
		return 0;
	
	if(root->data_type.base_type == STRING_TYPE)
		strings_add(root->string_const);
	
	//int i;

}


