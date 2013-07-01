#ifndef HASHTAB_H
#define HASHTAB_H
#define HASH_MAX_NODES 0xfffffff
#include <stdio.h>
#include <stdlib.h>
struct hashtab{
	struct hashtab_node **htable;/*hash table*/
	int size;						//number of slots in hash table
	int nel;						//number of elements in hash table
	int (*hash_value)(struct hashtab *h,const void *key);//hash function
	int (*keycmp)(struct hashtab *h,const void *key1,const void *key2);//key comparison function;

};

struct hashtab_node{
	void *key;
	unsigned long	datum;
	int pid;
	int iflags;
	struct id_node *idnode;
	struct hashtab_node *next;
};

struct id_node{
	int id;
	unsigned long long count;
	struct id_node *next;
};
struct hashtab_info{
	int slots_used;
	int max_chain_len;
};


static int symhash(struct hashtab *h,const void *key)
{

	const char *p,*keyp;
	unsigned int size;
	unsigned int val;
	val = 0;
	keyp = key;
	size = strlen(keyp);

	for(p = keyp;(p-keyp) < size; p++)
		val = (val << 4|(val >>(8*sizeof(unsigned int)-4))) ^(*p);

	return val & (h->size -1);
}

static int symcmp(struct hashtab *h,const void *key1,const void *key2)
{
	const char *keyp1,*keyp2;
		keyp1= key1;
		keyp2 = key2;
		return strcmp(keyp1,keyp2);
}

#endif
