#ifndef _FUNC_
#define _FUNC_
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
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>
#include<dirent.h>
#include<sys/stat.h>
#include<grp.h>
#include<pwd.h>


#define MAX(a, b) a>b?a:b
#define NAME_LEN_SER 20
#define DOWNLOAD "testfile.txt"
#define LINE_LEN 128
#define BUF_LEN 1000
#define LS_LEN 4096
#define CMD_LEN 50
#define PATH_LEN 100
#define ROOT "./"

//#define SER_DEBUG 1

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
int listenInitByTcp(int lisNum, int port);

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

int sendFile(int sfd, const char *fileArgs);//mvfile.c

int recvFile(int sfd, const char *fileName);//mvfile.c

int removeFile(char usrPath[], char fileName[]);//mvfile.c

int cdPath(char usrPath[], char cmdInfo[]);//cmd.c

int lsCmd(char usrPath[], char cmdInfo[], char lsBuf[]);//cmd.c

#endif

