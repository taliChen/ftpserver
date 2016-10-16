#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <unistd.h>
#include<stdlib.h>
#include<sys/select.h>
#include<sys/epoll.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

#define MAX(a, b) a>b?a:b
#define LINE_LEN 128
#define DOWNLOAD "1.rmvb"
#define LS_LEN 4096
#define BUF_LEN 1000
#define NAME_LEN 20
#define CMD_LEN 100
//#define CLI_DEBUG 1


typedef struct{
	pid_t pid;
	char busy;
	int fdw;
} Child, *Pchild;

typedef struct{
	int len;
	char buf[1000];
}data;// data unit for send


//init a epfd ,filled while stdin(reading) and sfd (reading)
int epfdInit(int sfd);

//connect ip:port, sucess return my sfd, fail return -1
int conectByTcp(char ip[], char port[]);

//server listen on ip:port success return the listen sfd, fail return -1
int listenInitByTcp(char ip[], char port[]);

//confirm if the num of argv is right, if not exit()
void confirArg(int argc, int exdArgc);

//read from stdin, send it to fdDest
int readAndSend(int fdDest);

//recv from fdSrc, print it
int recvAndPrint(int fdSrc);


void send_fd(int sfdw, int fd);//sendfd.c

void recv_fd(int sfdr, int * fd);//sendfd.c

int fcntl_noblock(int fd);//

int send_n(int sfd, char * p, int len);// poll_n.c

int recv_n(int sfd, char * p, int len);//poll_n.c

int sendFile(int sfd, const char *fileName);//mvfile.c

int recvFile(int sfd, const char *fileArgs);//mvfile.c

int handleRet(char cmdRet[], int sfd); // handleRet.c

