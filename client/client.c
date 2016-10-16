#include"func.h"


int main(int argc, char * argv[])
{
	if(argc != 3)
	{
		printf("Server IP port\n");
		return -1;
	}
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	memset(&ser, 0, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(atoi(argv[2]));
	ser.sin_addr.s_addr = inet_addr(argv[1]);
	int ret;
	printf("connecting\n");
	ret = connect(sfd, (struct sockaddr*)&ser, sizeof(ser));
	if(ret == -1)
	{
		perror("connect:");
		close(sfd);
		return -1;
	}
	printf("connect success\n");
	signal(SIGPIPE, SIG_IGN);

	char buf[CMD_LEN] = {0};
	char retBuf[2] = {0};
	struct epoll_event evs[2];
	int epfd = epfdInit(sfd);
	int i = 0, sRet = 0;
	char * inputNote = "^v^: ";
	int logInFlag = 0;
	//struct stat stbuf;
	//char fileName[NAME_LEN] = {0};
	//char fileSize[NAME_LEN] = {0};
	while(1)
	{

		memset(evs, 0, sizeof(evs));
		fflush(stdout);
		ret = epoll_wait(epfd, evs, 2, -1);
		if(ret > 0)
		{
			
			for(i = 0; i < ret; i++)
			{
				if(evs[i].events == EPOLLIN && evs[i].data.fd == 0)
				{
#ifdef CLI_DEBUG
					printf("stdin is reading\n");
#endif
					memset(buf, 0, sizeof(buf));
					sRet = read(STDIN_FILENO, buf, sizeof(buf));
					if(sRet <= 0)
					{
						printf("byebye\n");
						close(sfd);
						return 0;
					}
			/*		if(strncmp(buf, "gets", 4) == 0)
					{
						strcpy(fileName, buf+5);
						memset(&stbuf, 0, sizeof(stbuf));
						fileName[strlen(fileName)-1] = '\0';
						if(stat(fileName,&stbuf) == 0)
						{
							sprintf(fileSize, "%ld\n", stbuf.st_size);
							printf("local file Size:%s\n", fileSize);
							buf[strlen(buf)-1] = '\0';
							strcat(buf, " ");
							strcat(buf, fileSize);
							printf("buf:%s$\n",buf);
						}
					}*/
					sRet = send(sfd, buf, strlen(buf) - 1, 0);
					if(sRet < 0)
					{
						perror("send");
						printf("the socket has been closed by server!\n");
						close(sfd);
						return -1;
					}
				}
				else if(evs[i].events == EPOLLIN && evs[i].data.fd == sfd)
				{
					memset(retBuf, 0, sizeof(retBuf));
					sRet = recv(sfd, retBuf, sizeof(retBuf), 0);
					if(sRet < 0)
					{
						perror("recv");
						close(sfd);
						return -1;
					}
					else if(sRet == 0)
					{
						printf("the socket has been closed by server\n");
						close(sfd);
						return 0;
					}
#ifdef CLI_DEBUG
					printf("sfd is recv retBuf[0] = %d, retBuf[1]%d\n", retBuf[0], retBuf[1]);
#endif
					
					if(handleRet(retBuf, sfd) == 1)
					{
						logInFlag = 1;
					}
					if(logInFlag ==  1)
					{
						printf("%s", inputNote);
					}
					fflush(stdout);
				}
			}
		}
	}
	close(sfd);
	return 0;
}


