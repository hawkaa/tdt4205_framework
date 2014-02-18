/* global include */
#include <stdlib.h>

/* local include */
#include "bindnames.h"

/* output stage, located in vslc.c */
extern int outputStage;

char* thisClass;

/*
 * Default symtab binding function.
 * Will recursively call the corresponding binding function on its children,
 * not adding anything itself
 */
int
bind_default(node_t *root, int stackOffset)
{
	int i;
	for(i = 0; i < root->n_children; ++i)
		if(root->children[i] != NULL)
			root->children[i]->bind_names(root->children[i], stackOffset + 1);
}

/*
 * Symtab constant binding function.
 * Will bind strings to the symbol table.
 * Return value not used.
 */
int
bind_constant(node_t *root, int stackOffset)
{
	/* output for stage 6 */
	if(outputStage == 6)
		fprintf(stderr, "CONSTANT\n");	

	/* check data type */	
	switch(root->data_type.base_type) {
	case STRING_TYPE:
		strings_add(root->string_const);
		break;
	}

}


