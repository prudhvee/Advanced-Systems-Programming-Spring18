#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct tuple{
	char* id;
	int action;
	char topic[15];
	struct node* next;
};


void insert(const struct tuple* head,struct tuple* temp){
	if(head==NULL){
		head = temp;
		return;
	}
	struct tuple* iter = (struct tuple *)head;
	struct tuple* last;

	while(iter!=NULL){
		if((strcmp(iter->id,temp->id)==0) && (strcmp(iter->topic,temp->topic)==0)){
			iter->action+=temp->action;
			return;
		}
		last = iter;
		iter = (struct tuple *) iter->next;

	}
	last->next = (struct tuple *) temp;

}

void freeList(struct tuple* head){
	struct tuple* prev;
	while(head){
		prev = (struct tuple *)head;
		head = (struct tuple *) head->next;
		free(prev->id);
		free(prev);
	}

}

void printList(struct tuple* head){
	while (head){
		printf("(%s,%s,%d)\n",head->id,head->topic,head->action );
		if(head->next == NULL)
			break;
		head = (struct tuple *)head->next;
	}


}
int main(int argc, char const *argv[])
{
	char buffer[100];
	char* token;
	struct tuple* head;
	head = NULL;
	struct tuple* temp;

    while(fgets(buffer,100, stdin)!=NULL){
    	fflush(stdout);
    	temp = (struct tuple* ) malloc(sizeof(struct tuple));
		token = strtok(buffer,",");

		temp->id = (char *) malloc((strlen(token)-1)*sizeof(char));
		strcpy(temp->id,&token[1]);

		token=strtok(NULL,",");
		strcpy(temp->topic,token);

		token=strtok(NULL,")");
		temp->action = atoi(token);

		temp->next = NULL;

		if(head==NULL)
			head = temp;
		else{
			if(strcmp(head->id,temp->id)!=0){
				printList(head);
				freeList(head);
				head = temp;
			}
			else
				insert(head, temp);
		}

    }

    printList(head);
}
