#include"func.h"

#define TIME_LEN 50
#define TIME_STR_END 19

int showFileType(short int type, char lsBuf[])
{

	if(S_ISDIR(type))
	{
		strcat(lsBuf, "d");
	}
	else
	{
		strcat(lsBuf, "-");
	}
	int temp, i;
	temp = type;
	temp = temp << 7;
	for(i = 0; i < 3; i++)
	{
		if(temp & 0x8000) strcat(lsBuf,"r");
		else strcat(lsBuf,"-");

		if(temp & 0x4000) strcat(lsBuf,"w");
		else strcat(lsBuf,"-");

		if(temp & 0x2000) strcat(lsBuf,"x");
		else strcat(lsBuf,"-");
		temp = temp << 3;
	}
	return 0;
}

int findSecArgs(char lsInfo[])
{
	int len = strlen(lsInfo);
	int i;
	for(i = 1; i < len; i++)
	{
		if(lsInfo[i] != ' ' &&lsInfo[i-1] == ' ')
			break;
	}
	if(i == len)
		return 0;
	else
		return i;
}

void getdir(char usrPath[], char Info[], char dir[])
{

	strcpy(dir,usrPath);
	if(strncmp(Info, "-l", 2) == 0)
	{
		if(Info[3] != ' ' && Info[3] != '\0')
		{
			strcat(dir, Info + 3);
		}
		else
		{
			dir[strlen(dir) -1] = 0;
		}
	}
	else if(Info[0] != '\0')
	{
		strcat(dir, Info);
	}
	else
	{
		dir[strlen(dir) -1] = 0;
	}

}
int lsCmd(char usrPath[], char cmdInfo[], char lsBuf[])
{
	char tempBuf[BUF_LEN] = {0};
	int lflag;
	char dirName[PATH_LEN] = {0};
	if(strncmp(cmdInfo, "-l", 2) == 0)
	{
		lflag = 1;
	}
	else 
	{
		lflag = 0; 
	}

	getdir(usrPath, cmdInfo, dirName);
	DIR * dir;
	dir = opendir(dirName);
	if(NULL == dir)
	{
		perror("opendir");
		printf("%s\n", dirName);
		return -1;
	}
	struct dirent * p;
	struct stat buf;
	int ret;
	char timeStr[TIME_LEN] ={0};
	char pathName[PATH_LEN] = {0};

	while((p = readdir(dir)) != NULL)
	{
		if(p->d_name[0] == '.')	
			continue;
		memset(tempBuf, 0, sizeof(tempBuf));
		if(lflag == 1)
		{
			memset(pathName, 0, sizeof(pathName));
			strcpy(pathName, dirName);
			strcat(pathName, "/");
			strcat(pathName, p->d_name);
			ret = stat(pathName, &buf);
			if(ret != 0)
			{
				perror("stat");
				printf("stat:erro:%s\n",pathName);
				return -1;
			}
			strcpy(timeStr,ctime(&buf.st_mtime));
			timeStr[TIME_STR_END] = '\0';
			showFileType(buf.st_mode, lsBuf);
			snprintf(tempBuf, BUF_LEN,"%2ld %7s %7s %12ld %15s  %s\n",buf.st_nlink, getpwuid(buf.st_uid)->pw_name,getgrgid(buf.st_gid)->gr_name, buf.st_size, timeStr+4, p->d_name);
			strcat(lsBuf, tempBuf);
		}
		else
		{
			snprintf(tempBuf, BUF_LEN, "%s   ", p->d_name);
			strcat(lsBuf, tempBuf);
		}
	}
	closedir(dir);
	if(lsBuf[0] == '\0')
		strcat(lsBuf, "\n");

	return 0;

}


/*int main(int argc, char* argv[])
  {
  char lsBuf[2048] = {0};

  memset(lsBuf, 0, sizeof(lsBuf));
  lscmd("./", "-l .", lsBuf);
  printf("%s\n", lsBuf);
  return 0;
  }
 */


