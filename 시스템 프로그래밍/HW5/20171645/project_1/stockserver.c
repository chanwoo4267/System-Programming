/* 
 * echoserveri.c - An iterative echo server 
 */ 
/* $begin echoserverimain */
#include "csapp.h"

/* define */
#define MAX_CLIENT 100
#define MAX_LINE 1024

/* 소켓 구조체 */
typedef struct _SelectSocket{
    int fd; // select의 대상이 되는 socket
    int (*fn)(int fd, void *fn, int tfd); /* fn은 소켓에서 자료를 읽고 처리할 함수의 시작 주소를 보관 */
    int tfd; // target fd, write대상 소켓
} SelectSocket;

/* select 사용 매개변수 */
static fd_set writeset, readset; // I/O 소켓 감시대상 지정변수
static int flag_loopend; // 루프를 빠져나오는지 확인하는 flag
static int maxsocket; // select에 사용될 I/O소켓값중 가장 큰 값을 보관
static int sockcnt;
int flag_socketend;
static SelectSocket select_socket[MAX_CLIENT+1];

/* Event Server Library Function */
int EventServer_Init();
int EventServer_LoopStart();
int EventServer_AddIO(int fd, void *callback_function, int tfd);
int EventServer_RemoveIO(int fd);

/* select function */
int AcceptFunc(int listenfd, void *fn, int tmpfd);
int ReadFunc(int closefd, void *fn, int tmpfd);

/* 주식 구조체 */
typedef struct _Item{
    int ID;
    int left_stock;
    int price;
    int readcnt;
    sem_t mutex;
} Item;

/* 이진 트리 구조체 */
typedef struct _Node{
    Item item;
    struct _Node* leftChild;
    struct _Node* rightChild;
} Node;

/* 이진 트리 변수 */
Node* root = NULL;
char print_buffer[MAXLINE];

/* 이진 트리 함수 */
Node* BST_insert(Node* root, Item item);
//Node* BST_delete(Node* root, Item item);
Node* BST_search(Node* root, Item item);
Node* BST_init(Node* root);
void BST_print(Node* root);
void BST_save(Node* root);
void BST_save_next(Node* root);

int main(int argc, char **argv) 
{
    root = BST_init(root);
    
    int one=1;
    int socketfd;
    struct sockaddr_in serv_addr;

    socketfd = socket(AF_INET, SOCK_STREAM, 0 );
    if( socketfd < 0 ) {
        perror("socket error\n");
        exit(0);
    }

    if((setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,(char *)&one,
        sizeof(one))) == -1) {
        perror("setsockopt error\n");
        exit(0);
    }

    memset((char*)&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(socketfd,(struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind error\n");
        exit(0);
    }
    listen(socketfd, 5);

    EventServer_Init();
    EventServer_AddIO(socketfd, AcceptFunc, 1); /* */

    EventServer_LoopStart();
    
    close(socketfd);
    exit(0);
}
/* $end echoserverimain */

/* Event based Server Functions */
int EventServer_Init() {
    int i;
    
    flag_loopend = 0;
    maxsocket = 0;
    sockcnt = 0;
    flag_socketend = 0;

    FD_ZERO(&writeset);
    FD_ZERO(&readset);
    for(i=0; i<MAX_CLIENT; i++) {
        select_socket[i].fd = -1;
        select_socket[i].tfd = -1;
    }
    return 0;
}

int EventServer_LoopStart() {
    int i,len;

    while(!flag_loopend) {
        FD_ZERO(&readset);
        for(i=0; i<MAX_CLIENT; i++) {
            if (select_socket[i].fd != -1)
                FD_SET(select_socket[i].fd, &readset);
        }

        len=select(maxsocket+1,&readset,NULL,NULL,NULL);

        if (len) {
            for(i=0; i<MAX_CLIENT; i++) {
                if (select_socket[i].fd != -1) {
                    if (FD_ISSET(select_socket[i].fd, &readset)) {
                        /* callback 함수 */
                        select_socket[i].fn(select_socket[i].fd, select_socket[i].fn, select_socket[i].tfd);
                    }
                }
            }
        }
    } /* end of while */
    
    /* close sockets */
    for(i=0; i<MAX_CLIENT; i++) {
        if (select_socket[i].fd != -1)
            shutdown(select_socket[i].fd, 2);
        if (select_socket[i].tfd != -1)
            shutdown(select_socket[i].tfd, 2);
    }
    return 0;
}

int EventServer_AddIO(int fd, void *callback_function, int tfd) {
    int i;
    
    for(i=0; i<MAX_CLIENT; i++) {
        if (select_socket[i].fd == -1) {
            select_socket[i].fd = fd;
            select_socket[i].fn = callback_function;
            select_socket[i].tfd = tfd;
            break;
        }
    }
    
    if (fd > maxsocket) maxsocket = fd;
    
    FD_SET(fd, &readset);
    return 0;
}

int EventServer_RemoveIO(int fd) {
    int i;

    if (flag_socketend && sockcnt == 0) {
        BST_save(root);
        //flag_loopend = 1;
    }

    FD_CLR( fd, &readset );

    for( i=0; i<MAX_CLIENT; i++ ) {
        if( select_socket[i].fd == fd ) 
            select_socket[i].fd = -1;
        else if( select_socket[i].tfd == fd ) 
            select_socket[i].tfd = -1;
    }

    if( fd == maxsocket ) {
        maxsocket = 0;
        for( i=0; i<MAX_CLIENT; i++ ) {
            if( select_socket[i].fd > maxsocket )
                maxsocket = select_socket[i].fd;
            else if( select_socket[i].tfd > maxsocket )
                maxsocket = select_socket[i].tfd;
        }
    }

    return 0;
}

// select functions

int AcceptFunc( int listenfd, void *fn, int tmpfd )
{
    int socketfd_new;
    int client_len;
    struct sockaddr_in client_address;

    client_len = sizeof(client_address);
    socketfd_new = accept(listenfd, (struct sockaddr*) &client_address, &client_len);
    
    sockcnt++;
    flag_socketend = 1;
    EventServer_AddIO( socketfd_new, ReadFunc, 1 );

    return 0;
}

int ReadFunc( int closefd, void *fn, int tmpfd )
{
    int len;
    char buf[MAXLINE];
    //tokenize variables
    char* tline;
    char token[3][MAXLINE];
    int idx=0;
    //function variable
    Node* target;
    Item item;
    
    if( (len = read(closefd, buf, sizeof(buf)-1)) <= 0 ) {
        sockcnt--;
        shutdown(closefd,2);
        EventServer_RemoveIO( closefd );
        return -1;
    }
    buf[len-1] = '\0';
    
    printf("server received %d bytes\n",strlen(buf)+1);
    
    if (strcmp(buf,"show") == 0) {
        strcpy(print_buffer,"");
        BST_print(root);
        write(closefd, print_buffer, MAXLINE);
        strcpy(print_buffer,"");
        return 0;
    }
    
    //tokenize
    tline = strtok(buf,"\t\n ");
    while(tline != NULL) {
        strcpy(token[idx++], tline);
        tline = strtok(NULL, "\t\n ");
    }
    item.ID = atoi(token[1]);
    item.left_stock = atoi(token[2]);
    
    if (strcmp(token[0],"buy") == 0) {
        target = BST_search(root, item);
        if (target->item.left_stock >= item.left_stock) {
            target->item.left_stock -= item.left_stock;
            write(closefd, "[buy] success\n", MAXLINE);
        }
        else
            write(closefd, "not enough left stock\n", MAXLINE);
        return 0;
    }
    else if (strcmp(token[0], "sell") == 0) {
        target = BST_search(root, item);
        target->item.left_stock += item.left_stock;
        write(closefd, "[sell] success\n", MAXLINE);
        return 0;
    }

    write(closefd, "", MAXLINE);
    return 0;
}

/* Binary Search Tree 관련 함수 */
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
    char line[MAX_LINE];
    char token[3][MAX_LINE];
    char *pline; //use to open
    char *tline; //use to tokenize
    int idx=0;
    Item item;
    
    FILE* fp = fopen("stock.txt","r"); //수정 필요
    if (fp == NULL) printf("cannot open file\n");
    while(!feof(fp)) {
        pline = fgets(line,MAX_LINE,fp);
        if (pline == NULL) break;
        tline = strtok(line, "\t\n ");
        while(tline != NULL) {
            //printf("%s ",tline);
            strcpy(token[idx++], tline);
            tline = strtok(NULL, "\t\n ");
        }
        idx=0;
        
        item.ID = atoi(token[0]);
        item.left_stock = atoi(token[1]);
        item.price = atoi(token[2]);
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
