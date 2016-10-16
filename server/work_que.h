#ifndef __WORK_QUE__
#define __WORK_QUE__
#include"func.h"
typedef struct new_fd{
	int new_fd;
	struct in_addr cliAddr;
	char usrPath[PATH_LEN];
	struct new_fd * pnext;
}Node, *pNode;

typedef struct{
	pNode phead, ptail;
	pthread_mutex_t mutex;
	int size;
}que_t, *pque;


void que_init(que_t *pq);

void quePop(pque pq, pNode *pN);

void quePush(pque pq, pNode pN);

#endif
