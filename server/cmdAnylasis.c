#include"cmdAnylasis.h"


int devidCmdStr(char cmdBuf[], char cmd[], char info[])
{
	char *p = NULL;
	char tempCmd[CMD_LEN] = {0};
//	int len = strlen(cmdBuf);
	//int i = 0;
	strcpy(tempCmd, cmdBuf);
	p = tempCmd;
	while(*p != ' ' && *p != '\0') p++;
   	*p = '\0';
	strcpy(cmd, tempCmd);
	p++;

	//	while(*p == ' ' && i++ < KONG_LEN) p++;
	if(*p != '\0' && *p != ' ')		
		strcpy(info, p);
	return 0;
}
int cmdAnylasis(int sfd,char cmdInfo[])
{
	char cmdBuf[CMD_LEN] = {0};
	char cmd[CMD_LEN] = {0};

	int ret;
	ret = recv(sfd,cmdBuf, sizeof(cmdBuf), 0);
	if(ret <= 0)
	{
		//perror("recv");
		return -1;
	}
#ifdef SER_DEBUG
	printf("get from sfd:%s$\n", cmdBuf);
#endif

	devidCmdStr(cmdBuf, cmd, cmdInfo);
	switch (cmd[0])
	{
		case 'c':
			if(strcmp(cmd, "cd") == 0)
				return 1;
			else if(strcmp(cmd, "clear") == 0)
				return 7; //system("clear")
			else
				return 0;
			break;
		case 'l':
			if(strcmp(cmd, "ls") == 0)
				return 2;
			else
				return 0;
			break;
		case 'p':
			if(strcmp(cmd, "puts") == 0)
				return 3;
			else if(strcmp(cmd, "pwd") == 0)
				return 6;
			else
				return 0;
			break;
		case 'g':
			if(strcmp(cmd, "gets") == 0)
				return 4;
			else
				return 0;
					break;
		case 'r':
			if(strcmp(cmd, "remove") == 0)
				return 5;
			else
				return 0;
			break;
		default:
			break;
	}
	return 0;

}

void packetRet(int cmd, int cmdRet, char retBuf[])
{
	retBuf[0] = (char)cmd;
	retBuf[1] = (char)cmdRet;
}

int fileRight(char usrPath[], char cmdInfo[])
{
	struct stat stBuf;
	char * chp;
	char fileName[NAME_LEN_SER] = {0};
	memset(&stBuf, 0, sizeof(stBuf));

	strcpy(fileName,usrPath);
	strcat(fileName,cmdInfo);
	chp = strstr(fileName, " ");
	if(chp != NULL)
	{
		*chp = '\0';
	}
	if(stat(fileName, &stBuf) == 0)
		return 0;
	else 
		return 1;
}

int cmdResp(int sfd, int cmd, int cmdRet)
{
	char retBuf[2] = {0};
	retBuf[0] = (char)cmd;
	retBuf[1] = (char)cmdRet;
	send(sfd,retBuf, 2, 0);
	return 0;
}
