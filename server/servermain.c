#include"factory.h"
#include"cmdAnylasis.h"
#include"mysql.h"

void * pth_func(void* p)
{
	pf pfac = (pf)p;
	pque pq = &pfac->que;
	pNode pN= NULL;
	char cmdInfo[CMD_LEN] = {0};
	char *usrPath = NULL;
	char* lsBuf;
	int nameLen = 0;
	char fileName[NAME_LEN_SER] = {0};
	char filePath[PATH_LEN] = {0};
	char logBuf[BUF_LEN];
	char logEnd[10] = {"\n"};
	int sendRet;
	int cmdRet;
	int cmd = 0;
	MYSQL * conn = pfac->conn;
	lsBuf = (char *)calloc(LS_LEN,sizeof(char));
	if(lsBuf == NULL)
	{
		perror("calloc");
		return NULL;
	}
	int fd = -1;
	while(1)
	{
		pthread_mutex_lock(&pq->mutex);
		if(pq->size == 0)
		{
			pthread_cond_wait(&pfac->cond,&pq->mutex);
		}
		quePop(pq, &pN);
		pthread_mutex_unlock(&pq->mutex);
		printf("serving for:%s\n", inet_ntoa(pN->cliAddr));
		sendRet = loginInterface(conn, pN->new_fd);
		if(sendRet > 0)
		{
			fd = sendRet;
		}
	//	sendRet = 0;
		char * cmdStr[8] = {"error cmd", "cd", "ls", "puts", "gets", "remove", "pwd", "clear"};
		while(sendRet != -1)
		{
			usrPath = pN->usrPath;
			memset(cmdInfo, 0, sizeof(cmdInfo));
			cmd = cmdAnylasis(pN->new_fd, cmdInfo);
			if(fd != -1 && cmd != -1)
			{
				sprintf(logBuf, "%s %s $\n", cmdStr[cmd], cmdInfo);
				write(fd, logBuf, strlen(logBuf));
			}
			
#ifdef SER_DEBUG
			printf("cmdInfo:%s\n",cmdInfo);
#endif
			switch(cmd)
			{
				case 1:					//cd
					cmdRet = cdPath(usrPath, cmdInfo);
#ifdef SER_DEBUG
					printf("cd ret = %d\n", cmdRet);
#endif
					cmdResp(pN->new_fd, cmd, cmdRet);
					break;

				case 2:					//ls
					memset(lsBuf, 0, LS_LEN);
					cmdRet = lsCmd(usrPath, cmdInfo, lsBuf);
#ifdef SER_DEBUG
					printf("ls ret = %d\n", cmdRet);
#endif
					cmdResp(pN->new_fd, cmd, cmdRet);
#ifdef SER_DEBUG
					printf("%s\n", lsBuf);
#endif
					if(cmdRet == 0)
					{

						send(pN->new_fd, lsBuf, strlen(lsBuf), 0);
					}
					break;

				case 3:					//puts
#ifdef SER_DEBUG
					printf("puts\n");
#endif
					cmdResp(pN->new_fd, cmd, 0);
					send(pN->new_fd, cmdInfo, strlen(cmdInfo), 0);
					recv(pN->new_fd, &nameLen, sizeof(nameLen), 0);
					memset(fileName, 0, sizeof(fileName));
					memset(filePath, 0, sizeof(filePath));
					recv(pN->new_fd, fileName, nameLen, 0);
					strcpy(filePath, usrPath);
					strcat(filePath, fileName);
					recvFile(pN->new_fd, filePath);
					break;

				case 4:					//gets

					cmdRet = fileRight(usrPath, cmdInfo);
					cmdResp(pN->new_fd, cmd, cmdRet);
					if(cmdRet == 0)
					{
						recv(pN->new_fd, lsBuf, LS_LEN, 0);
#ifdef SER_DEBUG
						printf("gets ret = %d\n", cmdRet);
#endif
						if(cmdRet == 0)
						{
							sendRet = sendFile(pN->new_fd, cmdInfo);
						}
					}
					break;

				case 5:					//remove
					cmdRet = removeFile(usrPath, cmdInfo);
#ifdef SER_DEBUG
					printf("remove ret = %d\n", cmdRet);
#endif
					cmdResp(pN->new_fd, cmd, cmdRet);
					break;

				case 6:					//pwd
#ifdef SER_DEBUG
					printf("pwd\n");
#endif
					cmdResp(pN->new_fd, cmd, 0);
					send(pN->new_fd, usrPath+1, strlen(usrPath), 0);
					break;
				case 7 :				//clear
#ifdef SER_DEBUG
					printf("clear\n");
#endif
					cmdResp(pN->new_fd, cmd, 0);
					break;
				case -1:				//client has closed the socket			
					sendRet = -1;
					break;
				default:				//others
#ifdef SER_DEBUG
					printf("default\n");
					printf("cmd:%d, cmdInfo:%s\n", cmd, cmdInfo);
#endif
					cmdResp(pN->new_fd, 100, 1);
					break;

			}
		
		}

	//	fsync(fd);
		write(fd, logEnd, strlen(logEnd));
		close(fd);
		fd = -1;
		printf("served for :%s, client has closed the socket\n", inet_ntoa(pN->cliAddr));
		printf("wait accept\n");

		free(pN);
	}
}


int main(int argc, char * argv[])
{

	if(argc != 4)
	{
		printf("args: pthNum port listenNum\n");
		return -1;
	}
	int pthNum = atoi(argv[1]);
	int port = atoi(argv[2]);
	int lisNum = atoi(argv[3]);

	MYSQL *conn;

	factory fac;

	conn = mysqlConnect("localhost", "root", "abcde", "test2");

	factoryInit(&fac, pthNum, pth_func, conn);

	factoryStart(&fac);


	signal(SIGPIPE, SIG_IGN);  //注册忽略信号：SIGPIPE

	int sfd = listenInitByTcp(lisNum, port);
	int new_fd;

	pNode new_pN;
	pque pq = &fac.que;
	struct sockaddr_in client;
	int cliLen = sizeof(client);
	printf("wait accept\n");
	while(1)
	{
		new_fd = accept(sfd, (struct sockaddr*)&client, (socklen_t*)&cliLen);
		new_pN = (pNode)calloc(1, sizeof(Node));
		strcpy(new_pN ->usrPath, ROOT);
		new_pN->cliAddr = client.sin_addr;
		new_pN->new_fd = new_fd;
		pthread_mutex_lock(&pq->mutex);
		quePush(pq, new_pN);
		pthread_mutex_unlock(&pq->mutex);
		pthread_cond_signal(&fac.cond);
	}

}


