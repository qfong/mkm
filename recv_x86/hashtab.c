#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "hashtab.h"
#include "recv.h"
#include "mm.h"

void create_list(struct hashtab_node *cur)
{
	struct  id_node *i_cur,*temp;
	temp = i_cur = malloc(sizeof(*i_cur));
	if(i_cur == NULL) {exit(1);}
	memset(i_cur,0,sizeof(*cur));
	i_cur->next = NULL;
	if(cur->idnode == NULL){
		cur->idnode = i_cur;	
	}else{
	
	}
	
}

void insert_list(struct hashtab_node *cur,int id)
{
	struct id_node *i_cur,*i_prev,*i_newnode;
    i_prev = NULL;
    if(cur == NULL) {
        printf("hashtab node cur not exist\n ");
        return ;
    }
	i_cur = cur->idnode;
	while(i_cur && ((id - i_cur->id) !=0)){
			i_prev = i_cur;
			i_cur = i_cur->next;			
	}
	
	if(i_cur && ((id - i_cur->id) == 0)){
			i_cur->count ++;
			return 0;	
	}

	i_newnode = malloc(sizeof(*i_newnode));
	if(i_newnode == NULL) {
        printf("malloc id_node newnode error");
        return -1;
    }
	memset(i_newnode,0,sizeof(*i_newnode));

	i_newnode->next = NULL;
	i_newnode->id = id;
	i_newnode->count =1;

	if(cur->idnode == NULL){
		cur->idnode = i_newnode;
		return 0;	
	}
/*
	if(i_prev){
			//i_newnode->next = i_prev->next;
			i_prev->next = i_newnode;
			i_newnode->next = NULL;	
	}else{
			i_newnode->next = NULL;
	}
*/
	if(i_prev){
		
		i_newnode->next = i_prev->next;
		i_prev->next = i_newnode;
	}else{
		
		 cur->idnode = i_newnode;
	}
}

struct hashtab * hashtab_create(int (*hash_value)(struct hashtab *h,const void *key),int(*keycmp)(struct hashtab *h,const void *key1,const void *key2),int size)
{
	struct hashtab *p;
	int i;
	p =malloc(sizeof(*p));
	if(p == NULL) return ;
	p->size = size;
	p->nel = 0;
	p->hash_value = hash_value;
	p->keycmp = keycmp;
	p->htable = malloc(sizeof(*(p->htable))*size);
	if(p->htable == NULL){
		free(p);
		return NULL;
	}
	for(i= 0; i < size; i++){
		p->htable[i] = NULL;	
	}
	return p;
}

int list_destroy(struct hashtab_node *cur)
{
	struct  id_node *temp,*i_cur;

	i_cur = cur->idnode;
	
	if(!i_cur) return ;
    temp = i_cur; 
	while(i_cur != NULL ){
			//temp = i_cur;
			i_cur = i_cur->next;
            temp->next= NULL;
			if(temp) free(temp);
            temp=i_cur;

	}
	cur->idnode = NULL;

	return 0;
}
int list_show(struct hashtab_node *cur)
{
	struct  id_node *temp,*i_cur;
	i_cur = cur->idnode;
	while(i_cur){
			//temp = i_cur;
			//printf("------sys_id [%d] ,count [ %d ]\n",i_cur->id,i_cur->count);
			store_syscall(cur,i_cur);
			i_cur= i_cur->next;			
	}
	return 0;
}

int hashtab_show(struct hashtab *h)
{
	int i;
//	printf("\x1b[H\x1b[J");
	struct hashtab_node * cur,*temp;
	for(i = 0; i< h->size;i++){
		cur = h->htable[i];
		while(cur){
			temp = cur;
			//printf("----pid [%d] ,inode [ %d ]",cur->pid,cur->datum);
			list_show(cur);
			cur = cur->next;
			
		}
		}
}

int hashtab_insert(struct hashtab *h,void *key,int pid,unsigned long datum,int id)
{
	int hvalue;

	struct hashtab_node *prev,*cur,*newnode;
	if(!h|| h->nel == HASH_MAX_NODES)
		return -1;

	hvalue = h->hash_value(h,key);

	prev = NULL;
	cur = h->htable[hvalue];
	while(cur && h->keycmp(h,key,cur->key)>0){
		prev = cur;
		cur = cur->next;	
	}
	if(cur && (h->keycmp(h,key,cur->key)==0)){
		if((cur->datum - datum) == 0){
        	    insert_list(cur,id);
			
				return 0;
		}else{
			if(cur->iflags > 5){
				cur->datum = datum;
				insert_list(cur,id);
				return ;
			}
			char 		buffer[200];
			int fd;
		//	fd = open(LOG_W_FILE,O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IRGRP|S_IROTH);
		//	if(fd == NULL){
		//		printf("error");
		//		exit(1);
		//	}
			memset(buffer,0,200);
			sprintf(buffer,"%d\t%lu\t%d\t\n",cur->pid,datum,id);
			if(write(mkm_wfd,buffer,strlen(buffer)) == -1)
			{
				perror("write error");
				exit(1);
			}
		//	close(fd);
			cur->iflags ++;
			//printf("warning ...\n");
		}
		return -2;
		
	}
	

	newnode = malloc(sizeof(*newnode));
	if(newnode==NULL) return -1;
    memset(newnode,0,sizeof(*newnode));

	newnode->key = key;
	newnode->datum = datum;
	newnode->pid = pid;
	newnode->iflags = 1; 

	insert_list(newnode,id);

	if(prev){
		newnode->next = prev->next;
		prev->next = newnode;
	}else{
		newnode->next = h->htable[hvalue];
		h->htable[hvalue] = newnode;
	}
	h->nel++;
	return 0;
}

int hashtab_delete(struct hashtab *h,void *key)
{
	int hvalue;
	struct hashtab_node *prev,*cur;
	
	if(!h|| h->nel == HASH_MAX_NODES)
		return -1;
	hvalue = h->hash_value(h,key);
	prev = NULL;
	cur = h->htable[hvalue];

	while(cur && h->keycmp(h,key,cur->key)>0)
	{
		prev = cur;
		cur = cur->next;
	}
	if(cur && (h->keycmp(h,key,cur->key)==0))
	{
		if(prev){
			prev->next = cur->next;
			free(cur);
			cur = NULL;
			}else{
			free(cur);
			cur = NULL;
			h->htable[hvalue] = NULL;			
		}
		h->nel--;
	}
	return 0;

}


void *hashtab_search(struct hashtab *h,const void *key)
{
	int hvalue;
	struct hashtab_node *cur;
	if(!h) return NULL;
	hvalue = h->hash_value(h,key);
	cur = h->htable[hvalue];
	while(cur && h->keycmp(h,key,cur->key)>0)
		cur = cur->next;

	if(cur == NULL || (h->keycmp(h,key,cur->key)!= 0))
		return NULL;
	return cur->datum;
}

void hashtab_destroy(struct hashtab *h)
{
	int i;
	struct hashtab_node * cur,*temp;
	if(!h) return ;
	for(i = 0; i< h->size;i++){
		cur = h->htable[i];
		while(cur){
			list_destroy(cur);
			temp = cur;
			cur = cur->next;
			if(temp) free(temp);		
		}
		h->htable[i] = NULL;

	}
	free(h->htable);
	h->htable = NULL;
	free(h);
}


