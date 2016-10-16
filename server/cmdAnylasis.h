#include"func.h"

int devidCmdStr(char cmdBuf[], char cmd[], char info[]);

int cmdAnylasis(int sfd,char cmdInfo[]);

void packetRet(int cmd, int cmdRet, char retBuf[]);

int fileRight(char usrPath[], char cmdInfo[]);

int cmdResp(int sfd, int cmd, int cmdRet);
