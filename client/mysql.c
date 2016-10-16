#include"mysql.h"


/*int main(int argc, char* argv[])
{
	MYSQL * conn;
	char *server = "localhost";
	char *dbUser = "root";
	char *dbpwd = "abcde";
	char *dbName = "test2";
	int ret;
	conn = mysqlConnect(server, dbUser, dbpwd, dbName);
	loginInterface(conn);
	mysql_close(conn);
	return ret;
}
*/
int loginInterface(MYSQL *conn)
{
	char userName[NAME_LENGTH] = {0};
	//char password[PASSWD_LEN] = {0};
	char *password;
	char dbPwd[PASSWD_LEN] = {0};
	int loginRet = 1;
	while(loginRet != 0)
	{
		system("clear");
		printf("login as:");
		fflush(stdout);
		scanf("%s", userName);
		printf("password:");
		fflush(stdout);
		//scanf("%s", password);
		password = getpass("");
		loginRet = logIn(conn, userName, password);
		getchar();
		getchar();
	}

}
int logIn(MYSQL * conn, char userName[], char password[])
{
	char dbPwd[PASSWD_LEN] = {0};
	int ret;
	ret = getPassword(conn, userName, dbPwd);
	if(ret == -1)
	{
		printf("no such user\n");
		return -1;
	}
	if(strcmp(password, dbPwd) != 0)
	{
		printf("error password\n");
		return -1;
	}
	else
	{
		printf("login success\n");
		return 0;
	}
}

MYSQL *mysqlConnect(char server[], char userName[], char password[], char dbName[])
{
	MYSQL * conn;
	conn = mysql_init(NULL);
	if(mysql_real_connect(conn, server, userName, password, dbName, 0, NULL, 0) == 0)
	{
		printf("Error connecting to database : %s\n", mysql_error(conn));
	}
	else
	{
		printf("mysql connected....\n");
	}
	return conn;
}

int getPassword(MYSQL* conn, const char userName[], char passwd[])
{
	char query[QUERY_LEN] = {0};
	MYSQL_RES *res;
	MYSQL_ROW row;
	int ret = 0;
	sprintf(query, "select password from user where name =\'%s\'", userName);
//	sprintf(query, "select * from user");
	if(mysql_query(conn, query) != 0)
	{
		printf("Error making query:%s\n", mysql_error(conn));
		ret = -1;
	}
	else
	{
		res = mysql_use_result(conn);
		if(res != NULL)
		{
			row = mysql_fetch_row(res);
			if(row != NULL)
			{
				strcpy(passwd, (char*)row[0]);
				ret = 0;
			}
			else
			{
				ret = -1;
			}
		}
		else
		{
			ret = -1;
		}
		mysql_free_result(res);
	}
	return ret;
}
