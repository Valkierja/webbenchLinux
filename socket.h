/* $Id: socket.c 1.1 1995/01/01 07:11:14 cthuang Exp $
 *
 * This module has been modified by Radim Kolar for OS/2 emx
 */

/***********************************************************************
  module:       socket.c
  program:      popclient
  SCCS ID:      @(#)socket.c    1.5  4/1/94
  programmer:   Virginia Tech Computing Center
  compiler:     DEC RISC C compiler (Ultrix 4.1)
  environment:  DEC Ultrix 4.3 
  description:  UNIX sockets code.
 ***********************************************************************/
#define	h_addr	h_addr_list[0] /* Address, for backward compatibility.*/

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int fSocket(const char *host, int clientPort)
{
    int sock;
    unsigned long inaddr;//二进制形式地址
    struct sockaddr_in ad;//存放地址
    struct hostent *hp;
    
    memset(&ad, 0, sizeof(ad)); //分配内存
    ad.sin_family = AF_INET;  //IPv4

    inaddr = inet_addr(host);//将地址字符串转二进制形式的地址
    if (inaddr != INADDR_NONE)//INADDR_NONE报错,如果输入非法IP字符串255 IP,都会出现该报错
        memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));
    else
    {
        hp = gethostbyname(host);//接收到exception为INADDR_NONE的IP,尝试解析该IP
        if (hp == NULL)//无法解析IP,证明IP错误
            return -1;
        memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);//可以向DNS解析IP,IP正确
    }
    ad.sin_port = htons(clientPort);//把int型转换为网络传输字节型
    
    sock = socket(AF_INET, SOCK_STREAM, 0);//建立IPv4,流格式,TCP数据连接,获取连接FD
    if (sock < 0)
        return sock;//返回错误链接号
    if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0)//double check error exist
        return -1;
    return sock;
}

