#ifndef _MYSQL_
#define _MYSQL_

#include<mysql/mysql.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define QUERY_LEN 50
#define PASSWD_LEN 15
#define NAME_LENGTH 15

MYSQL *mysqlConnect(char server[], char userName[], char password[], char dbName[]);

int getPassword(MYSQL* conn, const char userName[], char passwd[]);

int logIn(MYSQL * conn, char userName[], char password[]);

int loginInterface(MYSQL *conn);

#endif
