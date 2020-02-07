//
//  server_2.cpp
//  img_proc_feb6
//
//  Created by ATW2 on 2/6/20.
//  Copyright © 2020 ATW2. All rights reserved.
//

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
#include <iostream>
#include <fstream>
#define PORT_ORIGINAL_VIDEO 8080
using namespace cv;
using namespace std;

#define PACKAGE_NUM 2

#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define width 640
#define height 480

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
    // params :    PORT    传输端口
    // return : -1        连接失败
    //            1        连接成功
    int socketConnect(int PORT);
    
    
    // 传输图像
    // params : image    待接收图像
    //        image    待接收图像
    // return : -1        接收失败
    //            1        接收成功
    int receive(cv::Mat& image);
    
    
    // 断开socket连接
    void socketDisconnect(void);
};

#endif

/*M///////////////////////////////////////////////////////////////////////////////////////
 //
 //  基于OpenCV和Socket的图像传输（接收）
 //
 //    By 彭曾 , at CUST, 2016.08.07
 //
 //    website: www.pengz0807.com  email: pengz0807@163.com
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


void shift_comp(Mat orig_img, Mat corr_img, int frame_num, ofstream &myfile){
    int p = 0;
    int cnt2 = 0;
    int cnt3 = 0;
    float percent_r = 0;
    float percent_l = 0;
    float percent_u = 0;
    float percent_d = 0;
    Mat black1(height, width, CV_8UC3, Scalar(0,0,0));
    Vec3b bgrPixel = corr_img.at<Vec3b>(0,0);
    Vec3b y = orig_img.at<Vec3b>(0,0);
    
    //-Y = up, +Y = down, -x = left, +x = right
    
    //check shift down ***********************************
    int cnt_u1 = 0;
    int cnt_u2 = 0;
    float pixel_u = 0;
    for(int i = 0; i < 120; i++){
        if(cnt_u1 >= 600){
            cnt_u2++    ;
        }
        cnt_u1 = 0;
        for(int j = 0; j < orig_img.cols; j++){ // 480
            bgrPixel = corr_img.at<Vec3b>(i,j);
            y = orig_img.at<Vec3b>(i,j);
            if(bgrPixel == black1.at<Vec3b>(0,0)){
                if(y != black1.at<Vec3b>(0,0)){
                    cnt_u1++;
                }
            }
            else{
                //cnt3 = 1;
            }
        } // inner for loop
    } // outer for loop
    //cout << "CNT 2 " << cnt_u2 << endl;
    pixel_u = cnt_u2;
    percent_u = (pixel_u/480.0)*100;
    percent_u = round(percent_u);
    if(percent_u >= 1){
        cout << "COMP: SHIFTED DOWN " << pixel_u << " pixels ";
        cout << "and " << percent_u << "%" << endl;
        myfile << frame_num << ": COMP SHIFTED DOWN " << percent_u << endl;
        return;
    }
    
    //check shift up ***************************
    cnt2 = 0;
    cnt3 = 0;
    for(int i = 360; i < orig_img.rows; i++){ // 640
        if(cnt3 >= 600){
            cnt2++;
        }
        cnt3 = 0;
        for(int j = 0; j < orig_img.cols; j++){ // 480
            bgrPixel = corr_img.at<Vec3b>(i,j);
            y = orig_img.at<Vec3b>(i,j);
            if(bgrPixel == black1.at<Vec3b>(0,0)){
                if(y != black1.at<Vec3b>(0,0)){
                    cnt3++;
                }
            }
            else{
            }
        } // inner for loop
    } // outer for loop
    cnt2++;
    float pixel_d = cnt2;
    percent_d = (cnt2/480.0)*100;
    percent_d = round(percent_d);
    if(percent_d >= 1){
        cout << "COMP: SHIFTED UP " << cnt2;
        cout << " and " << percent_d << "%" << endl;
        myfile << frame_num << ": COMP SHIFTED UP " << percent_d << endl;

        return;
    }
    
    // shift left check**************************************
    int cnt4 = 0;
    int cnt5 = 0;
    int cnt_l = 0;
    int check = 0;
    int cntl2 = 0;
    //for(int i =0; i < orig_img.rows; i++){
    for(int i = 480; i < orig_img.cols; i++){  
        //cout << "cnt_l " << cnt_l << endl;
        if(cnt_l > 100){
         cntl2++;   
        }
        /*
        if(cnt5 == cnt_l){
            cnt4 = 0;
        }
        
        else{
            cnt4 = cnt_l;
        }
        cnt5 = cnt_l;
        
        if(cnt_l ==0){
            check++;
        }
        * */
        cnt_l = 0;
        //cout << "COUNT L " << cnt_l << endl;
        //for(int j = 480; j < orig_img.cols; j++){
        for(int j = 0; j < orig_img.rows; j++){
            bgrPixel = corr_img.at<Vec3b>(j,i);
            y = orig_img.at<Vec3b>(j,i);
            if(bgrPixel == black1.at<Vec3b>(0,0)){
                if(y != black1.at<Vec3b>(0,0)){
                    cnt_l++;
                }
            }
        }// inner for loop
    } // outer for loop
    if(check > 10){
        cnt4 = 0;
    }
    //cnt4 = cntl2/9.39;
    //cout << "CNT4 " << cnt4 << endl;
    float pixel_l = (cntl2)/640.0;
    //pixel_l = round(pixel_l);
    percent_l = (pixel_l)*100;
    percent_l = round(percent_l);
    if(percent_l >= 2){
        cout << "COMP: SHIFTED LEFT " << cntl2  ;
        cout << " and " << percent_l << "%" << endl;
        myfile << frame_num << ": COMP SHIFTED left " << percent_l << endl;

        return;
    }
    
    //shift right check****************************
    int cnt6 = 0;
    int cnt7 = 0;
    int cnt_r = 0;
    for(int i =0; i < 161; i++){
        //cout << "CNT 6" << cnt6 << endl;
        if(cnt6 >= 100){
            //cnt_r = cnt6 + cnt_r;
            cnt_r++;
        }
        cnt6 = 0;
        for(int j = 0; j < orig_img.rows; j++){
            bgrPixel = corr_img.at<Vec3b>(j,i);
            y = orig_img.at<Vec3b>(j,i);
            if(bgrPixel == black1.at<Vec3b>(0,0)){
                if(y != black1.at<Vec3b>(0,0)){
                    cnt6++;
                }
            }
        } // inner for loop
    } // outer for loop
    float pixel_r = cnt_r/640.0;
    //pixel_r = pixel_r/(.3348925);
    //pixel_r = round(pixel_r);
    percent_r = (pixel_r)*100.0;
    percent_r = round(percent_r);
    if(percent_r >= 1){
        cout << "COMP: SHIFTED RIGHT " << cnt_r;
        cout << " and " << percent_r << "%" << endl;
        myfile << frame_num << ": COMP SHIFTED RIGHT " << percent_r << endl;
        return;
    }
    cout << "no shift detected" << endl;
    myfile << frame_num << ": no shift " << endl;
} // shift comp


void color_comp(Mat corr_img, int frame_num, ofstream &myfile){
    //int height = 480;
    //int width = 640;
    Mat red(height, width, CV_8UC3, Scalar(0,0,255));
    Mat blue(height, width, CV_8UC3, Scalar(255,0,0));
    Mat green(height, width, CV_8UC3, Scalar(0,255,0));
    Mat white(height, width, CV_8UC3, Scalar(255,255,255));
    
    int cntw = 0;
    int cntb = 0;
    int cntg = 0;
    int cntr = 0;
    int n = 0;
    Vec3b white_o = white.at<Vec3b>(0,0);
    Vec3b red_o = red.at<Vec3b>(0,0);
    Vec3b blue_o = blue.at<Vec3b>(0,0);
    Vec3b green_o = green.at<Vec3b>(0,0);
    Vec3b corr_o = corr_img.at<Vec3b>(0,0);
    
    for(int i =0; i < 630; i++){
        for(int j = 0; j < 470; j++){
            corr_o = corr_img.at<Vec3b>(j,i);
            if(white_o == corr_o){
                cntw++;
            }
            else if(red_o == corr_o){
                cntr++;
            }
            else if(blue_o == corr_o){
                cntb++;
            }
            else if(green_o == corr_o){
                cntg++;
            }
            else{
                n = 0;
            }
            
        } // inner for loop
    } // outer for loop
    if(cntw >= 296000){
        cout << "WHITE SCREEN COMPARED" << endl;
        myfile << frame_num << ": WHITE SCREEN COMPARED" << endl;
        //break;
    }
    else if(cntb >= 296000){
        cout << "BLUE SCREEN COMPARED" << endl;
         myfile << frame_num << ": BLUE SCREEN COMPARED" << endl;
        //break;
    }
    else if(cntg >= 296000){
        cout << "GREEN SCREEN COMPARED" << endl;
         myfile << frame_num << ": GREEN SCREEN COMPARED" << endl;
        //break;
    }
    else if(cntr >= 296000){
        cout << "RED SCREEN COMPARED" << endl;
         myfile << frame_num << ": RED SCREEN COMPARED" << endl;
        //break;
    }
    else{
        cout << "no color detection " << endl;
        myfile << frame_num << ": no color detection " << endl;
    }
} // color comp

int freeze_comp(Mat prev, Mat corr, int freeze_cnt, int frame_num, ofstream &myfile){
    int frz_cnt = 0;
    int frz_not = 0;
    int frz_check = 0;
    //cout << "here";
    Vec3b x = corr.at<Vec3b>(0,0);
    Vec3b y = prev.at<Vec3b>(0,0);
    while(1){
        for(int i =0; i < 480; i++){
            for(int j = 0; j < 640; j++){ // 480
                //cout << i << " " << j << endl;
                x = corr.at<Vec3b>(i,j);
                y = prev.at<Vec3b>(i,j);
                if(x == y){
                    frz_cnt++;
                    //cout << "identical" << endl;
                }
                else{
                    frz_not++;
                    if(freeze_cnt > 0){
                        float counter = freeze_cnt/8.0;
                        int counter2 = round(counter);
                        cout << "COMP: frozen for: " << counter << " secs" << endl;
                        myfile << frame_num << ": COMP: frozen for: " << counter << " secs" << endl;
                        freeze_cnt = 0;
                        break;
                    }
                    else if(frz_not > 50){
                        //cout << frame_num << ": " << "no freeeze " << endl;
                        //myfile << frame_num << ": no freeze detected"  << endl;
                        break;
                    }
                }
            } // inner for loop
        } // outer for loop
        //cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!Freeze_cnt " << frz_cnt << endl;
        if(frz_cnt >= 307000){
            freeze_cnt++;
            myfile << frame_num << ": COMP: frozen "  << endl;
        }
        break;
    }// while
    return freeze_cnt;
}

int socket_setup(){
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		cout << "error in socket()" << endl;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT_ORIGINAL_VIDEO);
	int addrlen = sizeof(serv_addr);
	
	if(inet_pton(AF_INET, "192.168.1.120", &serv_addr.sin_addr) <= 0){
		cout << "error in inet_pton" << endl;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, addrlen) < 0){
		cout << "error in connect" << endl;
	}
	return sock;
}

void receive_frame(int socket,  Mat *img_buffer, int img_buffer_size, int *id_buffer){

	int buffer1[4] = {0};
	int buffer_size = 0;
	int id = 9999;
	vector	<uchar> buffer;
	for(int i = 0; i < img_buffer_size-1; i++){
		img_buffer[i] = img_buffer[i+1];
		id_buffer[i] = id_buffer[i+1];
	}
	recv(socket, &buffer1, 4, MSG_WAITALL);
	buffer_size = buffer1[0];
	buffer.resize(buffer_size);	
	recv(socket, buffer.data(), buffer_size, MSG_WAITALL);

	img_buffer[img_buffer_size - 1] = imdecode(buffer, 1);
	recv(socket, &id, sizeof(id), MSG_WAITALL);
	id_buffer[img_buffer_size - 1] = id;


}

int main()
{
    SocketMatTransmissionServer socketMat;
    if (socketMat.socketConnect(6666) < 0)
    {
        return 0;
    }
    ofstream myfile;
    myfile.open("comp_log.txt");
    int it = 0;
    int freeze_num = 0;
    int frame_num = 2;
    cv::Mat image;
    cv::Mat image2;
    cv::Mat prev_img;
    int original_receive_socket = socket_setup();
    int img_buffer_size = 10;
    int* id_buffer = new int [img_buffer_size];
	Mat* img_buffer = new Mat [img_buffer_size];
    namedWindow("Original", CV_WINDOW_AUTOSIZE);
    while (1)
    {
        frame_num++;
        //if((it%2)==0){
       if(it==1){
           prev_img = image2;
        }
            if(socketMat.receive(image2) > 0)
            {
                //cv::imshow("Corrupted",image2);
                cv::imwrite("test.jpg",image2);
                cv::waitKey(1);
                it++;
            }
            receive_frame(original_receive_socket, img_buffer, img_buffer_size, id_buffer);
            image = img_buffer[img_buffer_size - 1];
            imshow("Original", image);
            cv::imshow("Corrupted",image2);

        //} // if it % 2
        
        //else{
            /*if(socketMat.receive(image2) > 0)
            {
                cv::imshow("comp 2",image2);
                cv::imwrite("test.jpg",image2);
                cv::waitKey(1);
                it++;
            }*/
            
           // it++;
       //} // else
        //cout << "here";
        if(it > 1){
            //cout <<"here2";
            freeze_num = freeze_comp(prev_img, image2, freeze_num, frame_num, myfile);
            color_comp(image2, frame_num, myfile);
            shift_comp(image, image2, frame_num, myfile);
            //cout << "HERE" << endl;
            prev_img = image2;
        }
        
        char c = (char)waitKey(1);
		if(c == 27){
			break;
		}
    } // while
    destroyWindow("Original");
    delete[] id_buffer;
    delete[] img_buffer;
    myfile.close();
    socketMat.socketDisconnect();
    return 0;
} // main
