#include"func.h"

//init a epfd ,filled while stdin(reading) and sfd (reading)
int epfdInit(int sfd)
{
	struct epoll_event event;
	int ret;
	int epfd = epoll_create(3);
	event.events = EPOLLIN;
	event.data.fd = 0;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event);
	if(ret == -1)
	{
		perror("epoll_ctl:add:0");
		return -1;
	}
	event.data.fd = sfd;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
	if(ret == -1)
	{
		perror("epoll_ctl:add:sfd");
		return -1;
	}
	return epfd;
}

//connect ip:port, sucess return my sfd, fail return -1
int conectByTcp(char ip[], char port[])
{
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==sfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in ser;
	memset(&ser,0,sizeof(ser));
	ser.sin_family=AF_INET;
	ser.sin_port=htons(atoi(port));
	ser.sin_addr.s_addr=inet_addr(ip);
	int ret;
	ret=connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
	if(-1==ret)
	{
		perror("connect");
		close(sfd);
		return -1;
	}
	return sfd;
}


//server listen on ip:port success return the listen sfd, fail return -1
int listenInitByTcp(char ip[], char port[])
{
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sfd == -1)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in server;
	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(port));
	server.sin_addr.s_addr = INADDR_ANY;
	int ret = bind(sfd, (struct sockaddr*)&server, sizeof(struct sockaddr));
	if(ret ==-1)
	{
		perror("bind");
		close(sfd);
		return -1;
	}
	ret = listen(sfd, 10);
	if(ret ==-1)
	{
		perror("listen");
		close(sfd);
		return -1;
	}
	printf("listen.....\n");
	return sfd;
}


//confirm if the num of argv is right, if not exit()
void confirArg(int argc, int exdArgc)
{
	if(argc != exdArgc)
	{
		puts("error args");
		exit(-1);
	}
}


//read from stdin, send it to fdDest
int readAndSend(int fdDest)
{
	char buf[LINE_LEN] = {0};
	int ret;

	read(STDIN_FILENO, buf, sizeof(buf));
	ret = send(fdDest, buf, strlen(buf)-1, 0);
	if(ret < 0)
	{
		perror("send");
		return -1;
	}
	return 0;
}

//recv from fdSrc, print it
int recvAndPrint(int fdSrc)
{
	char buf[LINE_LEN] = {0};
	int ret;

	ret = recv(fdSrc, buf, sizeof(buf)-1, 0);
	if(ret < 0)
	{
		perror("recv");
		return -1;
	}
	else if(ret == 0)
	{
		printf("link has been closed\n");
		return -1;
	}

	printf("%s\n", buf);
	return 0;
}

int recvAndPrintLet(int fdSrc)
{
	char buf[LINE_LEN] = {0};
	int ret;

	ret = 0;
	while(ret != -1)
	{
		ret = recv(fdSrc, buf, sizeof(buf)-1, 0);
		if(ret < -1)
		{
			perror("recv");
			return -1;
		}
		else if(ret == 0)
		{
			printf("link has been closed\n");
			return -1;
		}
		printf("%s", buf);
	}

	printf("\n");
	return 0;
}


int fcntl_noblock(int fd)
{
	int status, ret;
	status = fcntl(fd, F_GETFL);
	if(status == -1)
	{
		perror("fcntl");
		return -1;
	}
	status = status|O_NONBLOCK;
	ret = fcntl(fd, F_SETFL, status);
	if(ret == -1)
	{
		perror("fcntl");
		return -1;
	}

	return 0;
}

