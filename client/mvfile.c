#include"func.h"

/*
name:sendFile
ret: 0:  fail to open file
    -1: socket has been closed;
    other: it is normal
 */
int sendFile(int sfd, const char *fileName)
{
	data d;
	int sendRet;
	memset(&d, 0, sizeof(d));
	d.len = strlen(fileName);
	strcpy(d.buf, fileName);
	sendRet = send(sfd, &d, d.len+4, 0);
	int fd = open(fileName, O_RDONLY|O_EXCL);
	if(fd == -1)
	{
		perror("open");
		return 0;
	}
	while(memset(&d, 0, sizeof(d)), (d.len = read(fd, d.buf, sizeof(d.buf)))> 0)
	{
		sendRet = send_n(sfd, (char*)&d, d.len+4);
		if(sendRet == -1)
		{
			break;
		}
	}
	d.len = 0;
	send_n(sfd, (char *)&d, 4);
	close(fd);
	return sendRet;
}

int recvFile(int sfd, const char *fileArgs)
{
	char buf[BUF_LEN] = {0};
	int len;
	int fd;
	char fileName[NAME_LEN] = {0};
	int seekNum = 0;
	char * chp;
	int recvRet = 0;
	strcpy(fileName, fileArgs);
	chp = strstr(fileName, " ");
	if(chp != NULL)
	{
		*(chp) = '\0';
		seekNum = atoi(chp+1);
	}
	fd = open(fileName, O_RDWR|O_CREAT, 0666);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	lseek(fd, seekNum, SEEK_SET);
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		recv_n(sfd, (char *)&len, 4);
		if(len > 0)
		{
			recvRet = recv_n(sfd, buf, len);
			if(recvRet == -1)
			{
				break;
			}
			write(fd, buf, len);
		}
		else
		{
			break;
		}
	}
	close(fd);
	return recvRet;
}


