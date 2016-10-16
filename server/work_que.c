#include"work_que.h"

void que_init(que_t * pq)
{
	pq->phead = NULL;
	pq->ptail = NULL;
	pthread_mutex_init(&pq->mutex,NULL);
	pq->size = 0;
}

void quePop(pque pq, pNode *pN)
{
	*pN = pq->phead;
	pq->phead = (*pN)->pnext;
	pq->size--;

	if(pq->phead == NULL)
	{
		pq->ptail = NULL;
	}
}

void quePush(pque pq, pNode pN)
{
	if(pq->ptail == NULL)
	{
		pq->phead = pN;
		pq->ptail = pN;
	}
	else
	{
		pq->ptail->pnext = pN;
	}
	pq->size++;
}


