#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct node{
	int val;
	char c;
	struct node* next;
	};


struct tuple{
	char* id;
	char action;
	char* topic;
	struct node* next;
};



int searchKey(struct node* rules, char c){
	while(rules){
		if(rules->c== c){
			return rules->val;
		}
		else
			rules = rules->next;
	}
	return -1;
}


void mapper(struct node* rules){
	FILE *fp = fopen("input.txt","r");
	char buff[10000];

	
	while(fgets(buff,10000,fp)){

	if(strlen(buff)==0){
		printf("empty file\n" );
		exit(1);
	}

	char* token;
	
	token=strtok(&buff[1],",");

	struct tuple* tup = (struct tuple *) malloc(sizeof(struct tuple));

	while (token){
		tup->id=token;

		token = strtok(NULL,",");
		tup->action = token[0];

		token = strtok(NULL,")");
		tup->topic=token;
		
		token = strtok(NULL,"(");
		if(token)
			token = strtok(NULL,",");
		
		printf("(%s,%s,%d)\n",tup->id,tup->topic,searchKey(rules,tup->action));

		}
	}
	fclose(fp);


}
	

int main(int argc, char const *argv[])
{

	FILE *fp;
	fp = fopen("rules.txt","r");
	
	char buff[10];
	char* token;

	struct node* rules=NULL;
	struct node* start=NULL;

	
	while (1){
		if(!fgets(buff,10,fp))
			break;
		
		struct node* temp = (struct node* ) malloc(sizeof(struct node));

		token=strtok(buff,"=");

		temp->c = token[0];

		temp->val = atoi(strtok(NULL,"\0"));
		temp->next = NULL;

		if(!start){
			start = temp;
			rules = start;
		}
		else {
			rules->next = temp;
			rules = rules->next;
		}

	}
	fclose(fp);
	rules = start;

	mapper(rules);
	fflush(stdout);
	return 0;
}
