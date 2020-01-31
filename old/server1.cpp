#ifndef __SOCKETMATTRANSMISSIONSEVER_H__
#define __SOCKETMATTRANSMISSIONSEVER_H__
 
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
 
using namespace cv;
 
#define PACKAGE_NUM 2
 
#define IMG_WIDTH 640
#define IMG_HEIGHT 480
 
#define BLOCKSIZE IMG_WIDTH*IMG_HEIGHT*3/PACKAGE_NUM
 
struct recvBuf
{
	char buf[BLOCKSIZE];
	int flag;
};
 
 
class SocketMatTransmissionServer
{
public:
	SocketMatTransmissionServer(void);
	~SocketMatTransmissionServer(void);
	int sockConn;
private:
	struct recvBuf data;
 
	int needRecv;
	int count;
 
public:
 
	// 打开socket连接
	// params :	PORT	传输端口
	// return : -1		连接失败
	//			1		连接成功
	int socketConnect(int PORT);
 
 
	// 传输图像
	// params : image	待接收图像
	//		image	待接收图像
	// return : -1		接收失败
	//			1		接收成功
	int receive(cv::Mat& image);
 
 
	// 断开socket连接
	void socketDisconnect(void);
};
 
#endif

/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  基于OpenCV和Socket的图像传输（接收）
//	
//	By 彭曾 , at CUST, 2016.08.07 
//
//	website: www.pengz0807.com  email: pengz0807@163.com 
//	
//M*/
 
 

 
SocketMatTransmissionServer::SocketMatTransmissionServer(void)
{
}
 
 
SocketMatTransmissionServer::~SocketMatTransmissionServer(void)
{
}
 
 
int SocketMatTransmissionServer::socketConnect(int PORT)
{
	int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
 
	struct sockaddr_in server_sockaddr;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(PORT);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
	if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
	{
		perror("bind");
		return -1;
	}
 
	if(listen(server_sockfd,5) == -1)
	{
		perror("listen");
		return -1;
	}
 
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
 
	sockConn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
	if(sockConn<0)
	{
		perror("connect");
		return -1;
	}
	else
	{
		printf("connect successful!\n");
		return 1;
	}
	
	close(server_sockfd);
}
 
 
void SocketMatTransmissionServer::socketDisconnect(void)
{
	close(sockConn);
}
 
int SocketMatTransmissionServer::receive(cv::Mat& image)
{
	int returnflag = 0;
	cv::Mat img(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, cv::Scalar(0));
	needRecv = sizeof(recvBuf);
	count = 0;
	memset(&data,0,sizeof(data));
 
	for (int i = 0; i < PACKAGE_NUM; i++)
	{
		int pos = 0;
		int len0 = 0;
 
		while (pos < needRecv)
		{
			len0 = recv(sockConn, (char*)(&data) + pos, needRecv - pos, 0);
			if (len0 < 0)
			{
				printf("Server Recieve Data Failed!\n");
				break;
			}
			pos += len0;
		}
 
		count = count + data.flag;
 
		int num1 = IMG_HEIGHT / PACKAGE_NUM * i;
		for (int j = 0; j < IMG_HEIGHT / PACKAGE_NUM; j++)
		{
			int num2 = j * IMG_WIDTH * 3;
			uchar* ucdata = img.ptr<uchar>(j + num1);
			for (int k = 0; k < IMG_WIDTH * 3; k++)
			{
				ucdata[k] = data.buf[num2 + k];
			}
		}
 
		if (data.flag == 2)
		{
			if (count == PACKAGE_NUM + 1)
			{
				image = img;
				returnflag = 1;
				count = 0;
			}
			else
			{
				count = 0;
				i = 0;
			}
		}
	}
	if(returnflag == 1)
		return 1;
	else
		return -1;
}

//#include "SocketMatTransmissionServer.h"
 
int main()
{
	SocketMatTransmissionServer socketMat;
	if (socketMat.socketConnect(6666) < 0)
	{
		return 0;
	}
	
	cv::VideoCapture capture(0);
	cv::Mat image2; 

	cv::Mat image;
	uint32_t counter = 0;
	while (1)
	{
		if(socketMat.receive(image) > 0)
		{
			cv::imshow("",image);
            cv::imwrite("test.jpg",image);
		}
	}
 
	socketMat.socketDisconnect();
	return 0;
}
