#include"func.h"

/*
name:sendFile
ret: 0:  fail to open file
    -1: socket has been closed;
    other: it is normal
 */
int sendFile(int sfd, const char *fileArgs)
{
	data d;
	int sendRet;
	char *chp;
	int seekNum = 0;
	char fileName[NAME_LEN_SER] = {0};
	memset(&d, 0, sizeof(d));
	d.len = strlen(fileArgs);
	strcpy(d.buf, fileArgs);
	sendRet = send(sfd, &d, d.len+4, 0);
	strcpy(fileName, fileArgs);
	chp = strstr(fileName, " ");
	if(chp != NULL)
	{
		*chp = '\0';
		seekNum = atoi(chp +1);
	}
	int fd = open(fileName, O_RDONLY);
	if(fd == -1)
	{
		perror("open");
		return 0;
	}
	lseek(fd, seekNum, SEEK_SET);
	int ret;
	while(memset(&d, 0, sizeof(d)), (d.len = read(fd, d.buf, sizeof(d.buf)))> 0)
	{
		ret = send_n(sfd, (char*)&d, d.len+4);
		if(ret == -1)
		{
			return -1;
		}
	}
	d.len = 0;
	send_n(sfd, (char *)&d, 4);
	close(fd);
	return sendRet;
}

int recvFile(int sfd, const char *fileName)
{
	char buf[BUF_LEN] = {0};
	int len;
	int fd;
	int recvRet = 0;
	fd = open(fileName, O_RDWR|O_CREAT, 0666);
	if(fd == -1)
	{
		perror("open");
		return -1;
	}
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		recvRet = recv_n(sfd, (char *)&len, 4);
		if(recvRet == -1)
		{
			return -1;
		}
		if(len > 0)
		{
			recvRet = recv_n(sfd, buf, len);
			if(recvRet == -1)
			{
				return -1;
			}
			write(fd, buf, len);
		}
		else
		{
			break;
		}
	}
	close(fd);
	return 0;
}


int removeFile(char usrPath[], char fileName[])
{
	char tempName[PATH_LEN] = {0};
	strcpy(tempName, usrPath);
	strcat(tempName, fileName);
	int ret;
	ret = unlink(tempName);
	if(ret == -1)
	{
		perror("unlink");
		return -1;
	}
	return 0;
}
