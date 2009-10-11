/* huffman-encode.c */
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

#define LARGE_FILE_SUPPORT 1

#if LARGE_FILE_SUPPORT
typedef uint64_t count_t;
#define COUNT_MAX UINT64_MAX
#else
typedef uint32_t count_t; /* uint16_t would also work for most embedded appliances */
#define COUNT_MAX UINT32_MAX
#endif

#define TREE_FILE_SUFFIX ".tree.dot"
#define ENC_FILE_SUFFIX  ".enc.dot"
#define COMP_FILE_SUFFIX ".hfm"

#define TOTAL_NODES (257+256)
#define INVALID_NODE (-2) /* it is important that EOF is different from this */
#define NODE_IS_NOT_LEAF (-3)

#define PREFIX_SIZE 33

typedef struct {
	count_t weight;
	int     has_parent;
	int     value;
	void*   left;
	void*   right;
} node_t;

typedef struct {
	int      value;
	unsigned length;
	uint8_t* encoding;
} encoding_t;

encoding_t* encoding_table;
encoding_t* encoding_lut[257];
uint8_t*    encodings;

count_t histogram[256]; /* histogram of byte appereance, initialized by analysis */
unsigned leaf_count;

node_t nodes[TOTAL_NODES]; /* nodes for the leafs (incl. EOF) and inter-nodes */
node_t *root;

uint8_t prefix[PREFIX_SIZE];

void reset_analyze(void){
	memset(histogram, 0, sizeof(count_t)*256);
	leaf_count = 0;
}

void analyze(FILE* f){
	int t;
	fseek(f, 0, SEEK_SET);
	while((t=fgetc(f))!=EOF){
		histogram[(uint8_t)(t&0xff)]++;
	}
}

void reset_nodes(void){
	memset(nodes, 0, sizeof(node_t)*(TOTAL_NODES));
	unsigned i;
	for(i=0;i<TOTAL_NODES;++i){
		nodes[i].value = INVALID_NODE;
	}
}

void init_nodes(void){
	unsigned i,j=0;
	for(i=0; i<=255; ++i){
		if(histogram[i]!=0){
			nodes[j].value = i;
			nodes[j].has_parent = 0;
			nodes[j].weight = histogram[i];
			++j;
		}
	}
	nodes[j].value = EOF;
	nodes[j].has_parent = 0;
	nodes[j].weight = 1;
	leaf_count = j+1;
}

void find_lightest_two(void** a, void** b, unsigned limit){
	unsigned i,d=0;
	count_t ca=COUNT_MAX, cb=COUNT_MAX;
	for(i=0;i<limit;++i){
		if(nodes[i].has_parent==1)
			continue;
		++d;
		if(nodes[i].weight<cb){
			ca = cb;
			*a = *b;
			cb = nodes[i].weight;
			*b = &(nodes[i]);
		}else{
			if(nodes[i].weight<ca){
				ca = nodes[i].weight;
				*a = &(nodes[i]);
			}
		}
	}

	if((ca==COUNT_MAX) || (cb==COUNT_MAX)){
		fprintf(stderr,"Error while searching nodes (limit==%u,%c,%u)!\n",
		               limit,(ca==COUNT_MAX)?'a':'b', d);
	}
}

void build_huffmantree(void){
	unsigned unparented;
	unsigned node_index;
	node_t *a, *b;

	unparented = leaf_count;
	node_index = leaf_count-1;

	while(unparented>1){
		find_lightest_two((void**)&a, (void**)&b, node_index+1);
		a->has_parent = 1;
		b->has_parent = 1;
		node_index++;
		nodes[node_index].left  = a;
		nodes[node_index].right = b;
		nodes[node_index].weight = a->weight + b->weight;
		nodes[node_index].value = NODE_IS_NOT_LEAF;
		unparented--;
	}
	root = &(nodes[node_index]);
}

void print_subtree(FILE* f, node_t* node){
	if(node->left){
		fprintf(f, "  n%p [label=\"%llu\"]\n",
		        (void*)node, (uint64_t)node->weight);
		fprintf(f, "  n%p -> n%p [label=\"0\"]\n", (void*)node, node->left);
		fprintf(f, "  n%p -> n%p [label=\"1\"]\n", (void*)node, node->right);
		print_subtree(f, node->left);
		print_subtree(f, node->right);
	} else {
		char c;
		c = node->value;
		fprintf(f,"  n%p [label=\"0x%2.2X (%c) [%llu]\",fillcolor=green,style=filled]\n",
		             (void*)node, c, (c>32&&c<128&&c!='"')?c:' ', (uint64_t)node->weight);

	}
}

void print_huffmantree(char* fname){
	FILE* f;
	f = fopen(fname, "w");
	if(f==NULL){
		fprintf(stderr, "Error: could not print huffman tree!\n");
	}
	fprintf(f, "digraph G {\n");
	print_subtree(f, root);
	fprintf(f, "}\n");
	fclose(f);
}

void reset_prefix(void){
	memset(prefix, 0, PREFIX_SIZE);
}

void prefix_shift(void){
	uint8_t carry0=0, carry1;
	unsigned i;
	for(i=0; i<PREFIX_SIZE; ++i){
		carry1=prefix[i]>>7;
		prefix[i]<<=1;
		prefix[i]|=carry0;
		carry0=carry1;
	}
}

void prefix_inc(void){
	unsigned i;
	for(i=0; i<PREFIX_SIZE; ++i){
		prefix[i]+=1;
		if(prefix[i])
			return;
	}
}

void fill_encoding_table(node_t* node, unsigned depth){
	static unsigned index=0;
	if(node==NULL){
		index = 0;
		return;
	}
	if(node->value==NODE_IS_NOT_LEAF){
		fill_encoding_table(node->left,  depth+1);
		fill_encoding_table(node->right, depth+1);
	}else{
		encoding_table[index].encoding = NULL;
		encoding_table[index].length   = depth;
		encoding_table[index].value    = node->value;
		++index;
	}
}

int compare_encodings(const encoding_t* a, const encoding_t* b){
	return (a->length)-(b->length);
}

void sort_encoding_table(void){
	qsort(encoding_table, leaf_count, sizeof(encoding_t),
	      (int(*)(const void*, const void*))compare_encodings);
}

void fix_eof(void){
	unsigned i=leaf_count-1;
	while(encoding_table[i].value != EOF){
		--i;
	}
	encoding_table[i].value = encoding_table[leaf_count-1].value;
	encoding_table[leaf_count-1].value = EOF;
}

void init_encoding_table(void){
	if(encoding_table)
		free(encoding_table);
	encoding_table = malloc(leaf_count*sizeof(encoding_t));
	if(encoding_table==NULL){
		fprintf(stderr, "Error: could not allocate memory\n");
		exit(-1);
	}
	fill_encoding_table(NULL, 0);
	fill_encoding_table(root, 0);
	sort_encoding_table();
	fix_eof();
}

unsigned long compute_encoding_size(void){
	unsigned long size=0;
	unsigned i;
	for(i=0; i<leaf_count; ++i){
		size += (encoding_table[i].length+7)/8;
	}
	return size;
}

void compute_encodings(void){
	encodings = malloc(compute_encoding_size());
	unsigned i,j=0,last_depth=0;
	if(encodings==NULL){
		fprintf(stderr, "Error: could not allocate memory\n");
		exit(-1);
	}
	reset_prefix();
	for(i=0; i<leaf_count; ++i){
		while(last_depth<encoding_table[i].length){
			prefix_shift();
			++last_depth;
		}
		memcpy(encodings+j, prefix, (last_depth+7)/8);
		encoding_table[i].encoding = encodings+j;
		j += (last_depth+7)/8;
		prefix_inc();
	}
}

void print_encoding_binary(uint8_t* data, unsigned length){
	while(length){
		--length;
		putchar((data[length/8]&(1<<(length&0x7)))?'1':'0');
	}
}

void print_encodingtree(char* fname){
	FILE* f;
	int i,j,x,v;
	char t;
	char enc[encoding_table[leaf_count-1].length+1];
	f = fopen(fname, "w");
	if(f==NULL){
		fprintf(stderr, "Error: could not print encoding tree!\n");
	}
	fprintf(f, "digraph G {\n");
	for(i=0; i<leaf_count; ++i){
		x=0;
		for(j=encoding_table[i].length-1; j>=0; --j,++x){
			enc[x]=(encoding_table[i].encoding[j/8]&(1<<(j&7)))?'1':'0' ;
		}
		enc[x]='\0';
		v = encoding_table[i].value;
		fprintf(f, "  l%s [label=\"0x%2.2X (%c)\",fillcolor=green,style=filled]\n",
		        enc, v, (v>32&&v<128&&v!='"')?v:' ');
		fprintf(f, "   l%s -> n", enc);
		--x;
		t=enc[x];
		enc[x] = '\0';
		fprintf(f, "%s [label=\"%c\"]\n", enc, t);
		while(t=='0' && x>0){
			fprintf(f, "  n%s -> n", enc);
			--x;
			t=enc[x];
			enc[x] = '\0';
			fprintf(f, "%s [label=\"%c\"]\n", enc, t);
		}
	}
	fprintf(f, "}\n");
	fclose(f);
}

void reset_lut(void){
	memset(encoding_lut, 0, sizeof(encoding_t*)*257);
}

void build_encoding_lut(void){
	unsigned i;
	for(i=0; i<leaf_count-1; ++i){
		encoding_lut[(uint8_t)encoding_table[i].value]=&(encoding_table[i]);
	}
	encoding_lut[256]=&(encoding_table[leaf_count-1]);
}

void encoding_writer(FILE* f, int c){
	static uint8_t bitbuffer;
	static unsigned used;
	int j;
	if(f==NULL){
		used=0;
		return;
	}
	if(c>=0&&c<256){
		for(j=encoding_lut[(uint8_t)c]->length-1; j>=0; --j){
			bitbuffer<<=1;
			bitbuffer|=((encoding_lut[(uint8_t)c]->encoding[j/8])&(1<<(j&7)))?1:0;
			++used;
			if(used==8){
				fputc((uint8_t)bitbuffer, f);
				used=0;
			}
		}
	}else{
		/* EOF */
		c=256;
		for(j=encoding_lut[c]->length-1; j>=0; --j){
			bitbuffer<<=1;
			bitbuffer|=((encoding_lut[c]->encoding[j/8])&(1<<(j&7)))?1:0;
			++used;
			if(used==8){
				fputc(bitbuffer, f);
				used=0;
			}
		}
		if(used){
			bitbuffer<<=(8-used);
			fputc(bitbuffer, f);
		}
	}
}

void compress_file(FILE* fin, FILE* fout){
	int t;
	fseek(fin, 0, SEEK_SET);
	encoding_writer(NULL, 0);
	do{
		t=fgetc(fin);
		encoding_writer(fout, t);
	}while(t!=EOF);
}

void header_writer(FILE* f){
	unsigned i;
	unsigned old_depth=1;
	int old_i=0, v;
	fputc(0xC0, f);
	fputc((leaf_count>=256)?0xDF:0xDE, f);
	fputc((uint8_t)leaf_count, f);
	for(i=0; i<leaf_count; ++i){
		if(old_depth<encoding_table[i].length || i==leaf_count-1){
			if(i!=leaf_count-1){
				if(i-old_i>=255){
					fputc(255, f);
					fputc(i-old_i-255, f);
				}else{
					fputc(i-old_i, f);
				}
				for(;old_i<i;++old_i){
					v = encoding_table[old_i].value;
					fputc((uint8_t)v, f);
				}
				while(++old_depth<encoding_table[i].length){
					fputc(0x00, f);
				}
			}else{
				if(i-old_i+1>=255){
					fputc(255, f);
					fputc(i-old_i-255+1, f);
				}else{
					fputc(i-old_i+1, f);
				}
				for(;old_i<=i;++old_i){
					v = encoding_table[old_i].value;
					fputc((uint8_t)v, f);
				}
			}
		}
	}
}

int main(int argc, char** argv){
	fprintf(stdout, "huffman-encode TESTING (C) 2009, Daniel Otte\n\t%s - %s\n",
	                __DATE__,__TIME__);
	unsigned fileno;
	unsigned verbosity=0;
	for(fileno=1;fileno<argc;fileno++){
		char treefilename[strlen(argv[fileno])+strlen(TREE_FILE_SUFFIX)+1];
		char encfilename[strlen(argv[fileno])+strlen(ENC_FILE_SUFFIX)+1];
		char compfilename[strlen(argv[fileno])+strlen(COMP_FILE_SUFFIX)+1];
		FILE *fin, *fout;
		fprintf(stdout, "  processing %s ...\n", argv[fileno]);
		strcpy(treefilename, argv[fileno]);
		strcpy(encfilename,  argv[fileno]);
		strcpy(compfilename, argv[fileno]);
		strcat(treefilename, TREE_FILE_SUFFIX);
		strcat(encfilename,  ENC_FILE_SUFFIX);
		strcat(compfilename, COMP_FILE_SUFFIX);
		fin = fopen(argv[fileno], "r");
		if(fin==NULL){
			fprintf(stderr, "    could not open file: %s\n", argv[fileno]);
			continue;
		}
		reset_analyze();
		reset_nodes();
		analyze(fin);
		init_nodes();
		build_huffmantree();
		fprintf(stdout, "    writing tree diagram ... (%s)\n", treefilename);
		print_huffmantree(treefilename);

		init_encoding_table();
		compute_encodings();
		unsigned i;
		int v;
		if(verbosity>8){
			for(i=0; i<leaf_count; ++i){
				v=encoding_table[i].value;
				fprintf(stdout, "%3d: 0x%2.2X (%c); length = %3d; encoding: ", i,
								v, (v>32&&v<128)?v:' ', encoding_table[i].length);
				print_encoding_binary(encoding_table[i].encoding, encoding_table[i].length);
				putchar('\n');
			}
		}
		fprintf(stdout, "    writing encoding tree diagram ... (%s)\n", encfilename);
		print_encodingtree(encfilename);
		reset_lut();
		build_encoding_lut();
		fout = fopen(compfilename, "w");
		if(fout==NULL){
			fprintf(stderr, "   could not open file: %s\n", compfilename);
			continue;
		}
		fprintf(stdout, "    writing compressed file ... (%s)\n", compfilename);
		header_writer(fout);
		compress_file(fin, fout);
		fclose(fin);
		fclose(fout);
	}
	return 0;
}

