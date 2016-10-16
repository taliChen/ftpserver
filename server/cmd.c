#include"func.h"

int cdPath(char usrPath[], char cmdInfo[])
{
	char tempPath[PATH_LEN] = ".";
	if(cmdInfo[0] == '/')
	{
		strcat(tempPath, cmdInfo);
	}
	else
	{
		strcpy(tempPath, usrPath);
		strcat(tempPath, cmdInfo);
	}
	if(strcmp(cmdInfo, "..") == 0)
	{
#ifdef SER_DEBUG
		printf("it is a ..hehe\n");
#endif
		int k = strlen(usrPath) - 2;
		while(usrPath[k] != '/' && usrPath[k] != '.') k--;
		if(usrPath[k] == '/')
			usrPath[k+1] = '\0';
		return 0;
	}
	else if(strcmp(cmdInfo, ".") == 0)
	{
		return 0;
	}

	DIR * dir;
	dir = opendir(tempPath);
	if(dir == NULL)
	{
		return -1;
	}
	else
	{
		strcpy(usrPath, tempPath);
		if(usrPath[strlen(usrPath)-1] != '/')
			strcat(usrPath,"/");
		closedir(dir);
		return 0;
	}
}

/*int lsCmd(char usrPath[], char cmdInfo[], char lsBuf[])
{


	return 0;
}

*/
