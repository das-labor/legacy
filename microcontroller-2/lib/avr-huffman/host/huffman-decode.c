/* huffman-decode.c */
/*
    This file is part of the AVR-Huffman.
    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define V_NODE (-2)
#define V_EOF  (-1)

#define DEBUG 0

#define TREE_SUFFIX ".dectree.dot"

typedef struct {
	int16_t value;
	void*   left;
	void*   right;
} node_t;

typedef int16_t leaf_t;

node_t* tree=NULL;

int ferrorgetc(FILE* f){
	int t;
	t=fgetc(f);
	if(t==EOF){
		fprintf(stderr, "Error: got EOF when expected data\n");
		exit(-1);
	}
	return t;
}

void build_tree(FILE* f){
	int t;
	int leaf_count;
	unsigned partition_size=1;
	node_t* node_ptr;
	node_t* freenode_ptr;
	leaf_t* leaf_ptr;
	leaf_t* leaf_end_ptr;
	fseek(f, 0, SEEK_SET);
	t=ferrorgetc(f);
	if(t!=0xC0){
		fprintf(stderr, "Error: wrong magic value\n");
		exit(-1);
	}
	t=ferrorgetc(f);
	if(t>>1!=(0xDE>>1)){
		fprintf(stderr, "Error: wrong magic value\n");
		exit(-1);
	}
	leaf_count=(t&1)<<8;
	t=ferrorgetc(f);
	leaf_count += t;
	if(tree)
		free(tree);
	tree=calloc(leaf_count*sizeof(leaf_t)+(leaf_count-1)*sizeof(node_t),1);
	if(!tree){
		fprintf(stderr, "Error: could not allocate memory\n");
		exit(-1);
	}
	node_ptr = tree;
	freenode_ptr = tree+1; //(uint8_t*)tree + sizeof(node_t);
	leaf_ptr     = (leaf_t*)(((uint8_t*)tree) + (leaf_count-1)*sizeof(node_t));
	leaf_end_ptr = leaf_ptr + leaf_count;
	int i;
	while(leaf_ptr!=leaf_end_ptr){
		t=ferrorgetc(f);
		if(t==255){
			t+=ferrorgetc(f);
		}
		fprintf(stdout, "DBG: t==%d, partition_size=%d\n", t, partition_size);
		for(i=0;i<partition_size*2; ++i){
			node_ptr->value = V_NODE;
			if(i<t){
				if((i&1)==0){
					node_ptr->left = leaf_ptr;
					fprintf(stdout, "DBG: %p --> %p (left leaf)\n",
					        (void*)node_ptr, (void*)(leaf_ptr));
				}else{
					node_ptr->right = leaf_ptr;
					fprintf(stdout, "DBG: %p --> %p (right leaf)\n",
					        (void*)node_ptr, (void*)(leaf_ptr));
				}
				leaf_ptr++;
			}else{
				if((i&1)==0){
					node_ptr->left  = freenode_ptr;
					fprintf(stdout, "DBG: %p --> %p (left node)\n",
					        (void*)node_ptr, (void*)freenode_ptr);
				}else{
					node_ptr->right = freenode_ptr;
					fprintf(stdout, "DBG: %p --> %p (right node)\n",
					        (void*)node_ptr, (void*)freenode_ptr);
				}
				freenode_ptr++;
			}
			if(i&1){
				++node_ptr;
			}
		}
		partition_size = ((uint8_t*)freenode_ptr-(uint8_t*)node_ptr)/(sizeof(node_t));
		int j, v;
		for(j=0;j<t;++j){
			v = ferrorgetc(f);
			fprintf(stdout, "DBG: %p --> %2.2X (%c)\n", (void*)&(leaf_ptr[j-t]), v, (v>32&&v<128)?v:' ');
			leaf_ptr[j-t]=v;
		}
	}
	*(--leaf_end_ptr) = V_EOF;
}

void print_huffmantree_node(FILE* f, node_t* node){
	if(node->value==V_NODE){
		fprintf(f,"  n%p -> n%p [label=\"0\"]\n", (void*)node, (void*)(node->left));
		fprintf(f,"  n%p -> n%p [label=\"1\"]\n", (void*)node, (void*)(node->right));
		print_huffmantree_node(f, node->left);
		print_huffmantree_node(f, node->right);
	}else{
		int v = *((leaf_t*)node);
		fprintf(f,"  n%p [label=\"0x%2.2X (%c)\", fillcolor=green,style=filled]\n",
		          (void*)node, v, (v>32&&v<128&&v!='"')?v:' ');
	}
}

void print_huffmantree(const char* fname){
	FILE* f;
	f=fopen(fname, "w");
	if(f==NULL){
		fprintf(stderr, "Error: could not open file for tree dump (%s)!\n", fname);
		exit(-1);
	}
	fputs("digraph G {\n", f);
	print_huffmantree_node(f, tree);
	fputs("}\n", f);
	fclose(f);
}

int read_bit(FILE* f){
	static uint8_t bitbuffer;
	static unsigned fill;
	int t;
	if(f==NULL){
		fill=0;
		return -2;
	}
	if(fill==0){
		t=fgetc(f);
		if(t==EOF)
			return EOF;
		bitbuffer=t;
		fill=8;
	}
	--fill;
	t=bitbuffer>>7;
	bitbuffer<<=1;
	return t;
}

void decompress_file_corpus(FILE* fin, FILE* fout){
	node_t* current_node;
	int t;
	read_bit(NULL);
	for(;;){
		current_node=tree;
		while(current_node->value==V_NODE){
			t=read_bit(fin);
			if(t==EOF){
				fprintf(stderr, "Error: early end of file!\n");
				return;
			}
			if(t){
				current_node=current_node->right;
			}else{
				current_node=current_node->left;
			}
		}
		if(*((leaf_t*)current_node)==V_EOF){
			return;
		}
		fputc((uint8_t)*((leaf_t*)current_node), fout);
	}
}

int fileexists(char* fname){
	FILE* f;
	f=fopen(fname, "r");
	if(f){
		fclose(f);
		return 1;
	}
	return 0;
}

int endswith(const char* a, const char* suffix){
	unsigned i;
	i=strlen(suffix);
	if(strlen(a)<i)
		return 0;
	a += strlen(a)-i;
	return !strcmp(a, suffix);
}

int main(int argc, char** argv){
	int i;
	FILE *fin, *fout;
	unsigned index;
	for(i=1;i<argc;++i){
		char foutname[strlen(argv[i])+10];
		char ftreename[strlen(argv[i])+strlen(TREE_SUFFIX)+3];
		fin=fopen(argv[i], "r");
		if(fin==NULL){
			fprintf(stderr, "Error: could not open input file %s\n", argv[i]);
			continue;
		}
		build_tree(fin);
		strcpy(ftreename, argv[i]);
		strcat(ftreename, TREE_SUFFIX);
		print_huffmantree(ftreename);
		index=0;
		strcpy(foutname, argv[i]);
		unsigned v=strlen(foutname);
		if(endswith(foutname, ".hfm")){
			v-=4;
			foutname[v]='\0';
		}
		while(fileexists(foutname)){
			char appendix[8];
			sprintf(appendix,".%03d", index++);
			foutname[v] = '\0';
			strcat(foutname, appendix);
		}
		fout=fopen(foutname, "w");
		if(fout==NULL){
			fprintf(stderr, "Error: could not open output file %s\n", argv[i]);
			continue;
		}
		decompress_file_corpus(fin, fout);
		fclose(fin);
		fclose(fout);
	}
	return 0;
}







