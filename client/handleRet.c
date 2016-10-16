#include"func.h"

int handleRet(char cmdRet[], int sfd)
{
	char * p = cmdRet;
	char * recvReady = "recvReady";
	char * cdOK = "cd success";
	char * cdFail = "fail to cd";
	char * rmOK = "remove success";
	char * rmFail = "fail to remove";
//	char * inputNote = "^v^: ";
	char buf[BUF_LEN] = {0};
	char lsBuf[LS_LEN] = {0};
	char fileName[NAME_LEN] = {0};
	int nameLen = 0;
	int sendRet = 0;
	int recvRet = 0;
	char * password;
	switch(*p)
	{
		case 1:					//cd ret
			if(p[1] == 0)
			{
				printf("%s\n", cdOK);
			}
			else
			{
				printf("%s\n", cdFail);
			}
			break;
		case 2:                 //ls ret
			if(p[1] == 0)
			{
				memset(lsBuf, 0, sizeof(lsBuf));
				recv(sfd, lsBuf, sizeof(lsBuf), 0);
				printf("\n%s\n", lsBuf);
			}
			else
			{
				printf("fail to ls\n");
			}
			break;
		case 3:                 //puts ret
			if(p[1] == 0)
			{
				memset(buf, 0, sizeof(buf));
				recv(sfd, buf, sizeof(buf), 0);
				printf("sending %s\n", buf);
				sendRet = sendFile(sfd, buf);
				if(sendRet == -1)
				{
					printf("fail to send the file!\n");
				}
				else
				{
					printf("sended %s\n", buf);
				}
			}
			break;
		case 4:                 //gets ret
			if(p[1] == 0)
			{
				send(sfd, recvReady, strlen(recvReady), 0);
				recv(sfd, &nameLen, sizeof(nameLen), 0);
				memset(fileName, 0, sizeof(fileName));
				recv(sfd, fileName, nameLen, 0);
				recvRet = recvFile(sfd, fileName);
				if(recvRet == -1)
				{
					printf("fail to get file!\n");
				}
				else
				{
					printf("get it\n");
				}
			}
			else
			{
				printf("no such file\n");
			}
			break;
		case 5:                 //remove ret
			if(p[1] == 0)
			{
				printf("%s\n", rmOK);
			}
			else
			{
				printf("%s\n", rmFail);
			}
			break;
		case 6:                 //pwd ret
			if(p[1] == 0)
			{
				memset(buf, 0, sizeof(buf));
				recv(sfd, buf, sizeof(buf), 0);
				printf("%s\n", buf);
			}
			break;
		case 7:        //clear
			if(p[1] == 0)
			{
				system("clear");
			}
			break;
		case 8:				//need userName
			system("clear");
			printf("ftpserver login as:");
			fflush(stdout);
			break;
		case 9:				//need password
			printf("password:");
			fflush(stdout);
			password = getpass("");
			send(sfd, password, strlen(password), 0);
			break;
		case 10:			//login return
			if(p[1] == 0)
			{
				printf("login success!\n");
				return 1;
			}
			if(p[1] == -1)
			{
				printf("no such user!\n");

			}
			else if(p[1] == -2)
			{
				printf("error password!\n");
			}

			//	memset(buf, 0, sizeof(buf));
			read(STDIN_FILENO, buf, sizeof(buf));
			break;
		default:
			printf("cmd = %d, cmdInfo:%d\n", p[0],p[1]);
			printf("error cmd\n");
			break;
	}
	return 0;
	//	printf("%s", inputNote);
	//	fflush(stdin);
}

