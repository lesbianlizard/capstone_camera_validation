
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

 
#define IMG_WIDTH 640	// 需传输图像的宽
#define IMG_HEIGHT 480	// 需传输图像的高
#define PACKAGE_NUM 2
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
	int socketConnect(const char* IP, int PORT);
	int transmit(cv::Mat image);
	void socketDisconnect(void);
};
 
#endif //__SOCKETMATTRANSMISSIONCLIENT_H__

 