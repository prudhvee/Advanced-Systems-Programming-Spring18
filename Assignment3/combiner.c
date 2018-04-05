#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h> /* Declares errno and defines error constants */
#include <semaphore.h>

//global variables
int BUFFER_SIZE = 0;
int done = 0;


// structure definations:
struct node{
        int val;
        char c;
        struct node* next;
};

struct tuple{
        int id;
        int score;
        char topic[16];
        struct tuple* next;
};



// structure for shared buffer:
struct buffer{
        struct tuple* tup;
        int in, out;
        sem_t sem;
        sem_t items;
        sem_t avail;
};



void errExit(char *string){
    printf("%s\n",string);
}

//Functions related to consumer:

//function to print tuples linked list
void tupprint(struct tuple* head){
        // printf("End of Thread, printing linkedlist\n");
        if(head == NULL)
                printf("Nothing to print\n");
        while(1){
                printf("(%04d,%s,%d)\n",head->id,head->topic,head->score );
                if (head->next ==NULL)
                        break;
                head = head->next;
        }
}

struct tuple* insertTuple (struct tuple* head,struct tuple* temp){
        // printf("inserting tuple:(%04d,%s,%d)\n",temp->id,temp->topic,temp->score);

        struct tuple* iter=(struct tuple *)head;
        if(head==NULL)
                head = temp;
        else{
                while(1){

                        if(strcmp(iter->topic,temp->topic) == 0){
                                iter->score+=temp->score;
                                // free(temp->topic);
                                // free(temp);
                                break;
                        }

                        if(iter->next == NULL){
                                iter->next = temp;
                                break;
                        }
                        iter = iter->next;
                }

        }
        return head;
}


// function to copy tuples
void tupcpy(struct tuple* out,struct tuple* in){
        out->id=in->id;
        out->score = in->score;
        strcpy(out->topic,in->topic);
        out->next = NULL;

}



// reducer thread function
static void* reducer (void* var){

        int s;
        struct buffer* buff = (struct buffer *) var;
        struct tuple* head = NULL;
        struct tuple* temp = NULL;
        struct tuple* tpr = buff->tup;
        // printf("reducer started\n");
        while (1){


                if (sem_wait(&(buff->items)) == -1)       //acquire buffer lock
                                    errExit("sem_wait");
                if(done && buff->in==buff->out)
                        break;

                
                if (sem_wait(&(buff->sem)) == -1)       //acquire buffer lock
                                    errExit("sem_wait");



                        

                temp = (struct tuple *) malloc(sizeof(struct tuple));
                
                //create a copy of tuple from the buffer

                tupcpy(temp,&tpr[buff->out]);
                temp->next = NULL;

                //insert tuple into a linkedlist
                head = insertTuple(head,temp);

                //move the out pointer
                buff->out = (buff->out+1)%BUFFER_SIZE;

                //decrement buffer counter
        

                if (sem_post(&(buff->sem)) == -1)               // release buffer lock
                        errExit("sem_post");
                if (sem_post(&(buff->avail)) == -1)               // release buffer lock
                        errExit("sem_post");

        }
        
        tupprint(head);
        pthread_exit(NULL);
}







// Functions related to producer:
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

int idtoindex(int* arr, int id,int count){

        for(int i=0;i<count;i++){

                if(arr[i]==id)
                        return i;
        }

        return -1;
}

static void* mapper(void *temp){

        struct node * rules = (struct node *) temp;
        struct buffer *bfrptr,*ptrbfptr[500];
        struct tuple *tplptr;
        char buff[10000];
        int threadCount=0,arr[500],k,s,val;
        pthread_t tid[500];
        FILE *fp = fopen("input.txt","r");

        while(fgets(buff,10000,stdin)){

                if(strlen(buff)==0){
                        printf("empty file\n" );
                        exit(1);
                }

                char* token;

                token=strtok(&buff[1],",");

                struct tuple* tup = (struct tuple *) malloc(sizeof(struct tuple));

                while (token){
                        tup->id=atoi(token);

                        token = strtok(NULL,",");
                        tup->score = searchKey(rules,token[0]);

                        token = strtok(NULL,")");
                        strcpy(tup->topic,token);

                        token = strtok(NULL,"(");
                        if(token)
                                token = strtok(NULL,",");

                        // printf("(%04d,%s,%d)\n",tup->id,tup->topic,tup->score);
                        k = idtoindex(arr,tup->id,threadCount);
                        // printf("k=%d\n",k );
                        if(k == -1){
                                //create buffer
                                bfrptr = (struct buffer *) malloc(sizeof(struct buffer));
                                bfrptr-> tup = (struct tuple *) malloc (sizeof(struct tuple));
                                
                                //initialize variables
                                bfrptr->in = 0;
                                bfrptr->out = 0;

                                //initialize all the semaphores
                                if (sem_init(&(bfrptr->sem), 0, 1) == -1)
                                    errExit("sem_init");
                                if (sem_init(&(bfrptr->avail), 0, BUFFER_SIZE) == -1)
                                    errExit("sem_init");
                                if (sem_init(&(bfrptr->items), 0, 0) == -1)
                                    errExit("sem_init");

                                
                                //add buffer to ptrbuffptr
                                ptrbfptr[threadCount] = bfrptr;
                                arr[threadCount] = tup->id;

                                //spawn the damn thread
                                if(pthread_create(&tid[threadCount++], NULL, reducer, (void *)bfrptr)!=0){
                                         printf("pthread_create");
                                         exit(1);
                                 }
                                //push data into buffer

                                if (sem_wait(&(bfrptr->avail)) == -1)               // release buffer lock
                                    errExit("sem_post");
                                if (sem_wait(&(bfrptr->sem)) == -1)               // release buffer lock
                                    errExit("sem_post");
                                

                                 tplptr=bfrptr->tup;
                                 tupcpy(&tplptr[bfrptr->in],tup);
                                 bfrptr->in = (bfrptr->in+1)%BUFFER_SIZE;
                                 //bfrptr->count++;
                                
                                if (sem_post(&(bfrptr->sem)) == -1)               // release buffer lock
                                    errExit("sem_post");
                                if (sem_post(&(bfrptr->items)) == -1)       //acquire buffer lock
                                    errExit("sem_wait"); 




                        }
                        else{
                                //push data into kth buffer for the kth thread to handle it.

                                bfrptr = ptrbfptr[k]; // point to the right buffer.

                                if (sem_wait(&(bfrptr->avail)) == -1)               // release buffer lock
                                    errExit("sem_post");
                                if (sem_wait(&(bfrptr->sem)) == -1)               // release buffer lock
                                    errExit("sem_post");
                                

                                 tplptr=bfrptr->tup;
                                 tupcpy(&tplptr[bfrptr->in],tup);
                                 bfrptr->in = (bfrptr->in+1)%BUFFER_SIZE;
                                // bfrptr->count++;
                                
                                if (sem_post(&(bfrptr->sem)) == -1)               // release buffer lock
                                    errExit("sem_post");
                                if (sem_post(&(bfrptr->items)) == -1)       //acquire buffer lock
                                    errExit("sem_wait"); 


                        }
                }
        }
       
        done = 1;
       
        // wake up all reducer threads
         for(int i=0;i<threadCount;i++){
             bfrptr = ptrbfptr[i];
             if (sem_post(&(bfrptr->items)) == -1)       //acquire buffer lock
                  errExit("sem_wait"); 
             // sem_getvalue(&(bfrptr->avail), &val);
             // printf("avail = %d\n",val );
             // sem_getvalue(&(bfrptr->items), &val);
             // printf("items = %d\n",val );
             // sem_getvalue(&(bfrptr->sem), &val);
             // printf("sem = %d\n",val );
    
        //         s = pthread_cond_signal(&(bfrptr->cond));
        //         // s = pthread_join(tid[i], NULL);
        //         // printf("thread joined %d\n", i);
         }

        pthread_exit(NULL);
}




// Main function
int main(int argc, char const *argv[])
{

        if (argc<2){
                printf("Error: Not enough arguments\n");
                exit(1);                
        }
        BUFFER_SIZE = atoi(argv[1]);
	if(BUFFER_SIZE==0){
		printf("Error: Buffer Size 0!");
		exit(1);
	}
        FILE *fp;
        fp = fopen("rules.txt","r");

        char buff[10];
        char* token;

        struct node* rules=NULL;
        struct node* start=NULL;

        //loop to load the rules ie P=50
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

        pthread_t tid;
        int s = pthread_create(&tid, NULL, mapper, (void *)rules);


     
        if (s != 0){
  
            printf("Error creating thread");
            fflush(stdout);
            exit(1);
        }


        pthread_exit(NULL);        
        return 0;
}
