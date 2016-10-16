#include"factory.h"

void factoryInit(pf p, int pth_num, pfunc pthreadFunc, MYSQL *conn)
{
	p->pth = (pthread_t *)calloc(pth_num, sizeof(pthread_t));
	p->pth_num = pth_num;
	p->pthreadFunc = pthreadFunc;
	pthread_cond_init(&p->cond, NULL);
	que_init(&p->que);
	p->capability = pth_num;
	p->flag = 0;
	p->conn = conn;
}

void factoryStart(pf p)
{
	int i;
	if(p->flag == 0)
	{
		p->flag = 1;
		for(i = 0; i < p->pth_num; i++)
		{
			pthread_create(p->pth+i, NULL, p->pthreadFunc, p);
		}
	}
}


