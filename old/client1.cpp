


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
 
using namespace cv;
 
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
	if (socketMat.socketConnect("192.168.1.121", 6666) < 0)
	{
		return 0;
	}
	
	cv::VideoCapture capture(0);
	cv::Mat image;
	cv::Mat prev;
 
	while (1)
	{
		if (!capture.isOpened())
			return 0;
 
		capture >> image;
		prev = image; 
		 char c=(char)waitKey(25);
	    if(c==27){ // press 'esc' to quit
		break;
	    }
	    else if(c==102){ // press 'f' to freeze 
		  for(int i = 0; i< 30; i++){
			socketMat.transmit(image);

		 }
	    }
	    if (image.empty())
			return 0;
		imshow("er", image);
		socketMat.transmit(image);
	}
 
	socketMat.socketDisconnect();
	return 0;
}




//MY CODE BELOW *******************************

/*******************************
#include <stdio.h>
#include <sys/socket.h> 
#include <opencv2/opencv.hpp>
#include <vector> 
#include <iostream> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <unistd.h> 

#ifndef __SOCKETMATTRANSMISSIONCLIENT_H__
#define __SOCKETMATTRANSMISSIONCLIENT_H__
*/

//#define PORT 5521


//using namespace cv;
//using namespace std;



/*
int main(){

	while(1){
	    Mat image;
		image = imread("white.jpg");
		imshow("e", image);
	}

   	int socket1 = socket(AF_INET, SOCK_STREAM, 0); 
	struct sockaddr_in client1; 
	char buff1[1024];
	if(socket1 < 0){
	    cout << "socket fail" << endl;
			// socket fail
	}
	client1.sin_family = AF_INET;
	client1.sin_port = htons(PORT); 
	
	int inet1 = inet_pton(AF_INET, "192.168.1.121", &client1.sin_addr); 
	if(inet1 <= 0){ 
	    cout << "inet fail" << endl;
			// inet fail 
	}
	int conn1 = connect(socket1, (struct sockaddr *)&client1, sizeof(client1)); 
	if(conn1 < 0){
	    cout << "connection fail" << endl;
			// connection fail
	}
	
	
	Mat image;
	VideoCapture capture(-1);
	vector<uchar> data_encode;
	
	while(1){
	    if(!capture.is_opened()){
		    return 0; 
	    }
	    capture >> image; 
	    if(image.empty()){
		    return 0; 
	    }
	    imshow("o", image);
	    transmit(image); 
		
	}
	

	while (1)
	{
		//if (!capture.read(image)) 
		//	break;
		//capture >> image; 
		//imshow("wind", image); 
		image = imread("image1.jpg");
		//imshow("e", image); 
		
		imencode(".jpg", image, data_encode);
		int len_encode = data_encode.size();
		string len = to_string(len_encode);
		int length = len.length();
		for (int i = 0; i < 16 - length; i++)
		{
			len = len + " ";
		}
		send(socket1, len.c_str(), strlen(len.c_str()), 0);
		char send_char[1];
		for (int i = 0; i < len_encode; i++)
		{
			send_char[0] = data_encode[i];
			send(socket1, send_char, 1, 0);
		}
		char recvBuf[32];
		if(recv(socket1, recvBuf, 32, 0))
			printf("%s\n", recvBuf);
			//cout << "socket rcv" << endl;
			
	}
	
	return 0;
	* */
//}



/*************************************************
 * Freeze image program
 * AARON WALDER
 * press 'f' to freeze and follow by pressing a number 
 * 1-9 in order to freeze for that amount of time
 * press 'p' for FPS
************************************************/
/*
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h> 
#include <stdio.h> 

using namespace cv;
using namespace std; 



int main(){
    Mat frame;
    Mat prev_frame; 
    Mat white;
    Mat mask; 
    Mat white_image;//= imread("white.jpg", 1);
    
    
    VideoCapture vid(-1); // start video capture
    if(!vid.isOpened()){
        return 0; 
    }
    int width, height, size_in;
    while(1){
        cout << "Press 1: 640x480, 2: 1280x720, 3: 1920x1080" << endl;
        cin >> size_in;
        if(size_in == 1){
            height = 480;
            width = 640; 
            cout << "640x480" << endl;
            break;
        }
        else if(size_in == 2){
            height = 720;
            width = 1280;    
            cout << "1280x720" << endl;
            break;
        }
        else if(size_in == 3){
            height = 1080;
            width = 1920; 
            cout << "1920x1080" << endl;
            break; 
        }
        
    }
    vid.set(CAP_PROP_FRAME_WIDTH, width);
    vid.set(CAP_PROP_FRAME_HEIGHT, height);
   
    vid>>frame; 
    //white = frame; 
    //imshow("froze", white_image); 
    //usleep(2000000);

    while(1){
        vid >> frame; // capture each frame
        prev_frame = frame; 
        if(frame.empty()){ // if there is no frame, break out
                break; 
        }
        char c=(char)waitKey(25);
        if(c==27){ // press 'esc' to quit
            break;
        }
        else if(c==102){ // press 'f' to freeze 
            // while loop to freeze indefinitely
            // press esc to end freeze
            cout << "Indicator 2 " << endl; 
            cout << "Freeze Entered: press a # 2-9" << endl;

            while(1){
                char c1 = (char)waitKey(25);
                int j = 0; 
                switch(c1){
                        case 50:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 2; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }
                            j=1;
                            break; 
                        case 51:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 3; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }
                            j=1;
                            break; 
                        case 52: 
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 4; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }                            
                            j=1; 
                            break; 
                        case 53:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 5; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }                            j=1; 
                            break; 
                        case 54:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 6; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }                            
                            j=1; 
                            break; 
                        case 55:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 7; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }
                            j=1; 
                            break; 
                        case 56:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 8; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }
                            j=1; 
                            break; 
                        case 57: 
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 9; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }
                            j=1; 
                            break; 
                        case 27: 
                            j=1; 
                            break;
                        default:
                            //vid>>frame; 
                            imshow("frozen",frame);
                            //usleep(1000000);
                }// switch 
                if(j==1){
                    cout << "Unfrozen" << endl;
                    break;
                }
            } // while 
        } // else if 
        else if(c==119){
            cvtColor(frame, white_image, 1);
            threshold(white_image, white_image, 0, 255, 0); 
            Size size2(640, 480);
            resize(white_image, white_image, size2);
            imwrite("white.jpg", white_image); 
            Mat whit = imread("white.jpg", 1); 
            imshow("frozen", whit);
            usleep(2000000);
        }
        
        else if(c==112){ // press 'p' to show fps
            double fps = vid.get(CAP_PROP_FPS); 
            cout << "FPS " << fps << " frames/sec" << endl; 
        }
        imshow("frozen", frame); 
    }
    
    return 0; 
}
*/
