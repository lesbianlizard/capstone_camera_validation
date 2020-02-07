//
//  client2.cpp
//  img_proc_feb6
//
//  Created by ATW2 on 2/6/20.
//  Copyright © 2020 ATW2. All rights reserved.
//


/*M///////////////////////////////////////////////////////////////////////////////////////
 //
 //  基于OpenCV和Socket的图像传输（发送）
 //
 //    By 彭曾 , at CUST, 2016.08.07
 //
 //    website: www.pengz0807.com  email: pengz0807@163.com
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
#include <fstream>
#include <vector> 

using namespace cv;
using namespace std;

//待传输图像默认大小为 640*480，可修改
#define IMG_WIDTH 640    // 需传输图像的宽
#define IMG_HEIGHT 480    // 需传输图像的高
#define PACKAGE_NUM 2
#define PORT2 8080
#define MB 2591 * 1944

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
    // params :    IP        服务器的ip地址
    //            PORT    传输端口
    // return : -1        连接失败
    //            1        连接成功
    int socketConnect(const char* IP, int PORT);
    
    
    // 传输图像
    // params : image 待传输图像
    // return : -1        传输失败
    //            1        传输成功
    int transmit(cv::Mat image);
    
    
    // 断开socket连接
    void socketDisconnect(void);
};

#endif


/*M///////////////////////////////////////////////////////////////////////////////////////
 //
 //  基于OpenCV和Socket的图像传输（发送）
 //
 //    By 彭曾 , at CUST, 2016.08.07
 //
 //    website: www.pengz0807.com  email: pengz0807@163.com
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
    
    if(image.cols != 640 || image.rows != 480 || image.type() != CV_8UC3)
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


Mat translateImg(Mat& img, int offsetx, int offsety) {
    Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img, img, trans_mat, img.size());
    return img;
}


Mat shift_up(Mat img_corr, int percentage){
    cout << "Indicator 1: shift " << endl;
    cout << "Shift indicator: 12 - UP" << endl;
    cout << "Percentage shift indicator: " << percentage << endl;
    float up_percent = (percentage / 100.0)*480;
    up_percent = round(up_percent);
    cout << "Shifted UP by: " << up_percent << " = " << percentage << "%" << endl;
    //cvtColor(orig, img_corr, 0);
    translateImg(img_corr, 0, -up_percent);
    return img_corr;
} // shift up

Mat shift_left(Mat img_corr, int percentage){
    cout << "Indicator 1: shift " << endl;
    cout << "Shift indicator: 11 - LEFT" << endl;
    cout << "Percentage shift indicator: " << percentage << endl;
    float left_percent = (percentage / 100.0)*640.0;
    left_percent = round(left_percent);
    cout << "Shifted LEFT by: " << left_percent << " = " << percentage << "%" << endl;
    //cvtColor(orig, img_corr, 0);
    translateImg(img_corr, -left_percent, 0);
    return img_corr;
} // shift left

Mat shift_right(Mat img_corr, int percentage){
    cout << "Indicator 1: shift " << endl;
    cout << "Shift indicator: 10 - RIGHT" << endl;
    cout << "Percentage shift indicator: " << percentage << endl;
    float right_percent = (percentage / 100.0)*640.0;
    right_percent = round(right_percent);
    cout << "Shifted RIGHT by: " << right_percent << " = " << percentage << "%" << endl;
    //cvtColor(orig, img_corr, CV_8UC3);
    translateImg(img_corr, right_percent, 0);
    //img_corr.convertTo(img_corr, CvType.CV_8uC3);
    return img_corr;
} // shift right

Mat shift_down(Mat img_corr, int percentage){
    cout << "Indicator 1: shift " << endl;
    cout << "Shift indicator: 13 - DOWN" << endl;
    cout << "Percentage shift indicator: " << percentage << endl;
    float down_percent = (percentage / 100.0)*480.0;
    down_percent = round(down_percent);
    cout << "Shifted DOWN by: " << down_percent << " = " << percentage << "%" << endl;
    //cvtColor(orig, img_corr, 0);
    translateImg(img_corr, 0, down_percent);
    return img_corr;
} //  shift down

Mat discolor_red(){
    cout << "Indicator 3: Discolor " << endl;
    Mat img_corr = Mat(480, 640, CV_8UC3, Scalar(0,0,255));
    return img_corr;
} // discolor red

Mat discolor_green(){
    cout << "Indicator 3: Discolor " << endl;
    Mat img_corr = Mat(480, 640, CV_8UC3, Scalar(0,255,0));
    return img_corr;
} // discolor green

Mat discolor_blue(){
    cout << "Indicator 3: Discolor " << endl;
    Mat img_corr = Mat(480, 640, CV_8UC3, Scalar(255,0,0));
    return img_corr;
} // discolor blue

Mat discolor_white(){
    cout << "Indicator 3: Discolor " << endl;
    Mat img_corr = Mat(480, 640, CV_8UC3, Scalar(255,255,255));
    return img_corr;
} // discolor white



void split(Mat camera_frame, Mat corruption_frame, int frame_id, int socket){
	int buffer_for_size[1] = {0};
	vector <uchar> buffer;
	buffer.resize(MB);
	imencode(".jpg", camera_frame, buffer);
	int buffer_size = buffer.size();
	buffer_for_size[0] = buffer_size;
	send(socket, &buffer_for_size, sizeof(buffer_for_size), 0);

	size_t l = buffer_size;
	send(socket, buffer.data(), l, 0);
	send(socket, &frame_id, sizeof(frame_id), 0);
}


void freeze(Mat prev_frame, int freeze_num, int socket, SocketMatTransmissionClient &socketMat, VideoCapture &vid){
    cout << "Indicator 2: Freeze" << endl;
    //cout << "Freeze Entered: press a # 2-9" << endl;
    freeze_num = freeze_num*8;
    int k = 0;
    Mat frame;
    int frame_id = 0; 
    for(int i = 0; i < freeze_num; i++){
        vid >> frame;
        frame_id++;
        imshow("corruption2", prev_frame);
        socketMat.transmit(prev_frame);
        imshow("corruption", frame);
        split(frame, prev_frame, frame_id, socket);
        //socketMat.transmit(frame);
        
        if((i%8) == 0){
            k++;
            cout << k << " seconds" << endl;
        }
    }//for loop
} // freeze

void fps_calc(VideoCapture &vid){
    int j = 0;
    Mat frame;
    auto start = chrono::steady_clock::now();
    while(j < 30){
        vid>>frame;
        j++;
    }
    auto end = chrono::steady_clock::now();
    auto time1 = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << "Time "  << time1 << endl;
    float time2 = time1/1000.0;
    cout << "Frame rate: " << 30/time2 << endl;
} // fps


int socket_setup(){
	int opt = 1;

	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		printf("error in socket()\n");
	}

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		printf("error in setsockopt()\n");
	}

	struct sockaddr_in address;
	int addrlen = sizeof(address);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT2);
	int bind1 = bind(sockfd, (struct sockaddr*)&address, addrlen);
	if(bind1 < 0){
		printf("error in bind\n");
	}

	int listen1 = listen(sockfd, 3);

	int new_socket = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	return new_socket;
	
}



//#include "SocketMatTransmissionClient.h"

int main()
{
    SocketMatTransmissionClient socketMat;
    if (socketMat.socketConnect("192.168.1.123", 6666) < 0)
    {
        return 0;
    }
    int socket = socket_setup();
    int height = 480;
    int width = 640;
    int frame_num = 0;
    int frame_id = 0;
    ofstream myfile;
    myfile.open("corr_log.txt");
    
    cv::VideoCapture capture(0);
    capture.set(CAP_PROP_FRAME_WIDTH, width);
    capture.set(CAP_PROP_FRAME_HEIGHT, height);
    capture.set(CAP_PROP_FPS,70);
    
    cv::Mat image;
    cv::Mat prev_frame;
    cv::Mat img_corr;
    
    int percentage = 0;
            int k = 0;
            while(k == 0){
                cout << "Enter Shift percentage 2-25% " << endl;
                cin >> percentage;
                if(percentage >=2 || percentage <= 25){
                    k = 1;
                }
            }
            k=0;
            int frz_time = 0;
            while(k == 0){
                cout << "Enter freeze time " << endl;
                cin >> frz_time;
                if(frz_time >=2 || frz_time <=9){
                    k = 1;
                }
            }
    
    while (1)
    {
        frame_num++;
        frame_id++;
        myfile << frame_num << ": ";
        if (!capture.isOpened())
            return 0;
        
        capture >> image;
        prev_frame = image;
        img_corr = image;
        char c=(char)waitKey(25);
        if(c==27){ // press 'esc' to quit
            myfile << "EXIT" << endl;
            break;
        }
        
        else if(c=='f'){ // press 'f' to freeze
            myfile << "freeze" << endl;
            /*
            int num = 0;
            int k = 0;
            while(k == 0){
                cout << "HOW LONG FREEZE: 2-9 seconds: " << endl;
                cin >> num;
                if(num >=2 || num <= 9){
                    k = 1;
                }
            }
            */
            freeze(prev_frame, frz_time, socket,socketMat,  capture);
        }// freeze
        
        else if(c == 'w'){ // shift up
            myfile << "shift up: " << percentage << endl;
            imshow("corruption", image);
            split(image, img_corr, frame_id, socket);
            img_corr = shift_up(img_corr, percentage);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
            
            //socketMat.transmit(image);
            
        } // shift up
        
        else if(c == 's'){ // shift down
            myfile << "shift down: " << percentage << endl;
            imshow("corruption", image);
            split(image, img_corr, frame_id, socket);
            img_corr = shift_down(img_corr, percentage);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
            //socketMat.transmit(image);
          
        } // shift down
        
        else if(c == 'd'){ // shift right
            myfile << "shift right: " << percentage << endl;
            imshow("corruption", image);
            split(image, img_corr, frame_id, socket);
            img_corr = shift_right(img_corr, percentage);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
           
            
           
            
            //socketMat.transmit(image);
            
        } // shift right
        
        else if(c == 'a'){ // shift left
            myfile << "shift left: " << percentage << endl;
            imshow("corruption", image);
            split(image, image, frame_id, socket);
            img_corr = shift_left(img_corr, percentage);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
            
             
            //socketMat.transmit(image);
            
        } // shift left
        
        else if(c =='r'){
            myfile << "red" << endl;
            img_corr = discolor_red();
            imshow("corruption", image);
            split(image, img_corr, frame_id, socket);
            //socketMat.transmit(image);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
        }
        else if(c=='b'){
            myfile << "blue" << endl;
            img_corr = discolor_blue();
            imshow("corruption", image);
            split(image, img_corr, frame_id, socket);
            //socketMat.transmit(image);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
        }
        else if(c=='g'){
            myfile << "green" << endl;
            img_corr = discolor_green();
            imshow("corruption", image);
            split(image, img_corr, frame_id, socket);
            //socketMat.transmit(image);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
        }
        else if(c=='h'){
            myfile << "white" << endl;
            img_corr = discolor_white();
            imshow("corruption", image);
            split(image, img_corr, frame_id, socket);
            //socketMat.transmit(image);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
        }
        else{
            if (image.empty())
                return 0;
            myfile << "original" << endl;
            imshow("corruption", image);
            split(image, img_corr, frame_id, socket);
            //socketMat.transmit(image);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
        }
       
    } // while
    myfile.close();
    socketMat.socketDisconnect();
    return 0;
} // main
