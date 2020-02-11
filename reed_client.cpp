/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  基于OpenCV和Socket的图像传输（发送）
//	
//	By 彭曾 , at CUST, 2016.08.07
//
//	website: www.pengz0807.com  email: pengz0807@163.com 
//	
//M*/
 
#ifndef __SOCKETMATTRANSMISSIONCLIENT_H__
#define __SOCKETMATTRANSMISSIONCLIENT_H__
 
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <iostream>
 
using namespace cv;
using namespace std;
 
//待传输图像默认大小为 640*480，可修改
#define IMG_WIDTH 640	// 需传输图像的宽
#define IMG_HEIGHT 480	// 需传输图像的高
#define PACKAGE_NUM 2
//默认格式为CV_8UC3
#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/PACKAGE_NUM
 
struct sentbuf
{
	char buf[BUFFER_SIZE];
	int flag;
};
 
class SocketMatTransmissionClient
{
public:
	SocketMatTransmissionClient(void);
	~SocketMatTransmissionClient(void);
 
private:
	int sockClient;
	struct sentbuf data;
 
public:
 
	// 打开socket连接
	// params :	IP		服务器的ip地址
	//			PORT	传输端口
	// return : -1		连接失败
	//			1		连接成功
	int socketConnect(const char* IP, int PORT);
 
 
	// 传输图像
	// params : image 待传输图像
	// return : -1		传输失败
	//			1		传输成功
	int transmit(cv::Mat image);
 
 
	// 断开socket连接
	void socketDisconnect(void);
};
 
#endif


/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  基于OpenCV和Socket的图像传输（发送）
//	
//	By 彭曾 , at CUST, 2016.08.07 
//
//	website: www.pengz0807.com  email: pengz0807@163.com 
//	
//M*/
 
 
//#include "SocketMatTransmissionClient.h"
 
SocketMatTransmissionClient::SocketMatTransmissionClient(void)
{
}
 
 
SocketMatTransmissionClient::~SocketMatTransmissionClient(void)
{
}
 
 
int SocketMatTransmissionClient::socketConnect(const char* IP, int PORT)
{
	struct sockaddr_in    servaddr;
 
	if ((sockClient = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		return -1;
	}
 
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0) 
	{
		printf("inet_pton error for %s\n", IP);
		return -1;
	}
 
	if (connect(sockClient, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
	{
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
		return -1;
	}
	else 
	{
		printf("connect successful!\n");
        return 0;
	}
}
 
 
void SocketMatTransmissionClient::socketDisconnect(void)
{
	close(sockClient);
}
 
int SocketMatTransmissionClient::transmit(cv::Mat image)
{
	if (image.empty())
	{
		printf("empty image\n\n");
		return -1;
	}
 
	if(image.cols != IMG_WIDTH || image.rows != IMG_HEIGHT || image.type() != CV_8UC3)
	{
		printf("the image must satisfy : cols == IMG_WIDTH（%d）  rows == IMG_HEIGHT（%d） type == CV_8UC3\n\n", IMG_WIDTH, IMG_HEIGHT);
		return -1;
	}
 
	for(int k = 0; k < PACKAGE_NUM; k++) 
	{
		int num1 = IMG_HEIGHT / PACKAGE_NUM * k;
		for (int i = 0; i < IMG_HEIGHT / PACKAGE_NUM; i++)
		{
			int num2 = i * IMG_WIDTH * 3;
			uchar* ucdata = image.ptr<uchar>(i + num1);
			for (int j = 0; j < IMG_WIDTH * 3; j++)
			{
				data.buf[num2 + j] = ucdata[j];
			}
		}
 
		if(k == PACKAGE_NUM - 1)
			data.flag = 2;
		else
			data.flag = 1;
 
		if (send(sockClient, (char *)(&data), sizeof(data), 0) < 0)
		{
			printf("send image error: %s(errno: %d)\n", strerror(errno), errno);
			return -1;
		}
	}
    return 0;
}



//#include "SocketMatTransmissionClient.h"
 
int main()
{
	SocketMatTransmissionClient socketMat;
	if (socketMat.socketConnect("192.168.1.120", 6666) < 0)
	{
		return 0;
	}
	
	cv::VideoCapture capture(0);
	capture.set(CAP_PROP_FPS, 40);
	cv::Mat image;
	cv::Mat prev;
	Mat white = Mat(480, 640, CV_8UC3, Scalar(255,255,255));
	int it = 0;
	if (!capture.isOpened())
			return 0;
	capture >> image;
    socketMat.transmit(image);
	auto start = chrono::steady_clock::now();
	 while(it < 30){
           capture >> image;
           socketMat.transmit(image);
           it++;

           
           //imshow("image", image);   
                    
       }
     auto end = chrono::steady_clock::now();
     auto time1 = chrono::duration_cast<chrono::milliseconds>(end-start).count();
     float time2 = time1/1000.0;
	 cout << "FPS: " << 30/time2 << endl;
     cout << "Size " << image.cols << " x " << image.rows << endl;
   

	while (1)
	{
		
 
		capture >> image;
		prev = image; 
		 char c=(char)waitKey(25);
	    if(c==27){ // press 'esc' to quit
		break;
	    }
	    else if(c=='w'){ // press 'f' to freeze 
		  //for(int i = 0; i< 30; i++){
			socketMat.transmit(white);
			usleep(1000000);

		// }
	    }
	    if (image.empty())
			return 0;
		imshow("image", image);
		socketMat.transmit(image);
		
	}
 
	socketMat.socketDisconnect();
	return 0;
}
