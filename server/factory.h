#ifndef _FACTORY_
#define _FACTORY_
#include"work_que.h"
#include<mysql/mysql.h>
typedef void*(*pfunc)(void*);

typedef struct{
	char flag;
	pthread_t *pth;
	int pth_num;
	pfunc pthreadFunc;
	pthread_cond_t cond;
	MYSQL *conn;
	que_t que;
	int capability;
}factory, *pf;

void factoryInit(pf p, int pth_num, pfunc pthreadFunc, MYSQL *conn);

void factoryStart(pf p);


#endif
