/* 
 * echoservert_pre.c - A prethreaded concurrent echo server
 */
/* $begin echoservertpremain */
#include "csapp.h"
#define NTHREADS  100
#define SBUFSIZE  16
#define MAX_LINE 1024
/* shared buffer */

typedef struct {
    int *buf;          /* Buffer array */         
    int n;             /* Maximum number of slots */
    int front;         /* buf[(front+1)%n] is first item */
    int rear;          /* buf[rear%n] is last item */
    sem_t mutex;       /* Protects accesses to buf */
    sem_t slots;       /* Counts available slots */
    sem_t items;       /* Counts available items */
} sbuf_t;

void sbuf_init(sbuf_t *sp, int n);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int item);
int sbuf_remove(sbuf_t *sp);

/* thread */

void echo_cnt(int connfd);
void *thread(void *vargp);

sbuf_t sbuf; /* Shared buffer of connected descriptors */

/* echo */

static int byte_cnt;  /* Byte counter */
static sem_t mutex;   /* and the mutex that protects it */
static void init_echo_cnt(void);

/* BST */
typedef struct _Item{
    int ID;
    int left_stock;
    int price;
    int readcnt;
} Item;

typedef struct _Node{
    Item item;
    struct _Node* leftChild;
    struct _Node* rightChild;
} Node;

static Node* root;              /* shared database */
char print_buffer[MAXLINE];
static int sockcnt;
int end_flag;
int save_flag;

Node* BST_insert(Node* root, Item item);
Node* BST_search(Node* root, Item item);
Node* BST_init(Node* root);
void BST_print(Node* root);
void BST_save(Node* root);
void BST_save_next(Node* root);

/* main start */
int main(int argc, char **argv) 
{
    int i, listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid; 
    sockcnt = 0;
    end_flag = 0;
    save_flag = 1;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    root = BST_init(root);

    listenfd = Open_listenfd(argv[1]);
    sbuf_init(&sbuf, SBUFSIZE); //line:conc:pre:initsbuf

    for (i = 0; i < NTHREADS; i++)  /* Create worker threads */ //line:conc:pre:begincreate
	    Pthread_create(&tid, NULL, thread, NULL);               //line:conc:pre:endcreate

    while (1) { 
        clientlen = sizeof(struct sockaddr_storage);
	    connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        sockcnt++;
        end_flag = 1;
	    sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
    }

    BST_save(root);
}

void *thread(void *vargp) 
{  
    Pthread_detach(pthread_self()); 
    while (1) { 
        int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */ //line:conc:pre:removeconnfd
        echo_cnt(connfd);             /* Service client */
        Close(connfd);
    }
}
/* $end echoservertpremain */

/* start sbuf */
/* Create an empty, bounded, shared FIFO buffer with n slots */
/* $begin sbuf_init */
void sbuf_init(sbuf_t *sp, int n)
{
    sp->buf = Calloc(n, sizeof(int)); 
    sp->n = n;                       /* Buffer holds max of n items */
    sp->front = sp->rear = 0;        /* Empty buffer iff front == rear */
    Sem_init(&sp->mutex, 0, 1);      /* Binary semaphore for locking */
    Sem_init(&sp->slots, 0, n);      /* Initially, buf has n empty slots */
    Sem_init(&sp->items, 0, 0);      /* Initially, buf has zero data items */
}
/* $end sbuf_init */

/* Clean up buffer sp */
/* $begin sbuf_deinit */
void sbuf_deinit(sbuf_t *sp)
{
    Free(sp->buf);
}
/* $end sbuf_deinit */

/* Insert item onto the rear of shared buffer sp */
/* $begin sbuf_insert */
void sbuf_insert(sbuf_t *sp, int item)
{
    P(&sp->slots);                          /* Wait for available slot */
    P(&sp->mutex);                          /* Lock the buffer */
    sp->buf[(++sp->rear)%(sp->n)] = item;   /* Insert the item */
    V(&sp->mutex);                          /* Unlock the buffer */
    V(&sp->items);                          /* Announce available item */
}
/* $end sbuf_insert */

/* Remove and return the first item from buffer sp */
/* $begin sbuf_remove */
int sbuf_remove(sbuf_t *sp)
{
    int item;
    P(&sp->items);                          /* Wait for available item */
    P(&sp->mutex);                          /* Lock the buffer */
    item = sp->buf[(++sp->front)%(sp->n)];  /* Remove the item */
    V(&sp->mutex);                          /* Unlock the buffer */
    V(&sp->slots);                          /* Announce available slot */
    return item;
}
/* $end sbuf_remove */
/* $end sbufc */

/* start echo */
static void init_echo_cnt(void)
{
    Sem_init(&mutex, 0, 1);
    byte_cnt = 0;
}

void echo_cnt(int connfd) 
{
    int n; 
    int stock_id, left_stock;
    char buf[MAXLINE];
    char t[MAXLINE]; 
    rio_t rio;
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    Node* target;
    Item item;

    Pthread_once(&once, init_echo_cnt); //line:conc:pre:pthreadonce
    Rio_readinitb(&rio, connfd);        //line:conc:pre:rioinitb

    while(1) {
        n = Rio_readnb(&rio, buf, MAXLINE);

        P(&mutex);
        if (n == 0 && save_flag == 1) {
            BST_save(root);
            save_flag = 0;
        }

        fprintf(stderr,"server received %d bytes\n",strlen(buf)); //line:conc:pre:cntaccess2

        sscanf(buf,"%s %d %d", t, &stock_id, &left_stock);

        item.ID = stock_id;
        item.left_stock = left_stock;
        
        if (strncmp(buf,"show",4) == 0) {
            strcpy(print_buffer,"");
            BST_print(root);
        }
        else if (strncmp(buf,"buy",3) == 0) {
            target = BST_search(root, item);
            if (target->item.left_stock >= item.left_stock) {
                target->item.left_stock -= item.left_stock;
                strcpy(print_buffer,"[buy] success\n");
            }
            else
                strcpy(print_buffer,"not enough left stock\n");
        }
        else if (strncmp(buf,"sell",4) == 0) {
            target = BST_search(root, item);
            target->item.left_stock += item.left_stock;
            strcpy(print_buffer,"[sell] success\n");
        }
        else {
            sockcnt--;
            if (sockcnt == 0) {
                fprintf(stderr,"save stock.txt\n");
                BST_save(root);
            }
            V(&mutex);
            return;
        }
        write(connfd,print_buffer,MAXLINE);
        strcpy(print_buffer,"");
        V(&mutex);
    }   
}
/* end echo */

Node* BST_insert(Node* root, Item item) {
    if (root == NULL) {
        root = (Node*)malloc(sizeof(Node));
        root->leftChild = root->rightChild = NULL;
        root->item = item;
        return root;
    }
    else {
        if (root->item.ID > item.ID)
            root->leftChild = BST_insert(root->leftChild, item);
        else
            root->rightChild = BST_insert(root->rightChild, item);
    }
    return root;
}

Node* BST_search(Node* root, Item item) {
    if (root == NULL) return NULL;
    if (root->item.ID == item.ID)
        return root;
    else if (root->item.ID > item.ID)
        return BST_search(root->leftChild, item);
    else
        return BST_search(root->rightChild, item);
}

Node* BST_init(Node* root) {
    char line[MAXLINE];
    int a,b,c;
    Item item;
    
    FILE* fp = fopen("stock.txt","r");
    if (fp == NULL) printf("cannot open file\n");

    while(fgets(line,sizeof(line),fp)) {
        sscanf(line, "%d %d %d", &a,&b,&c);
        item.ID = a;
        item.left_stock = b;
        item.price = c;
        item.readcnt = 0;
        root = BST_insert(root,item);
    }
    fclose(fp);
    return root;
}

void BST_print(Node* root) {
    if (root == NULL) return;
    char temp[MAXLINE];
    sprintf(temp,"%d %d %d\n",root->item.ID, root->item.left_stock, root->item.price);
    strcat(print_buffer,temp);
    BST_print(root->leftChild);
    BST_print(root->rightChild);
}

void BST_save(Node* root) {
    if (root == NULL) return;
    FILE* fp = fopen("stock.txt","w"); // 수정필요  
    if (fp == NULL) printf("cannot open file\n");
    else 
        fprintf(fp,"%d %d %d\n", root->item.ID, root->item.left_stock, root->item.price);
    fclose(fp);
    BST_save_next(root->leftChild);
    BST_save_next(root->rightChild);
}

void BST_save_next(Node* root) {
    if (root == NULL) return;
    FILE* fp = fopen("stock.txt","a"); // 수정필요  
    if (fp == NULL) printf("cannot open file\n");
    else 
        fprintf(fp,"%d %d %d\n", root->item.ID, root->item.left_stock, root->item.price);
    fclose(fp);
    BST_save_next(root->leftChild);
    BST_save_next(root->rightChild);
}

