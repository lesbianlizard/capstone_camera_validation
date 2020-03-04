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
#include <chrono>

#define PORT2 8080 // port used for original frames
#define PACKAGE_NUM 2
#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define width 640
#define height 480
#define BLOCKSIZE IMG_WIDTH*IMG_HEIGHT*3/PACKAGE_NUM

using namespace cv;
using namespace std;

struct recvBuf
{
    char buf[BLOCKSIZE];
    int flag;
};

//server creation 
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

// comparison module for shifts 
int shift_comp(Mat orig_img, Mat corr_img, int frame_num, ofstream &myfile){
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
    int cnt_out = 0;
    for(int i = 0; i < 120; i++){
        if(cnt_u1 >= 600){
            cnt_u2++    ;
        }
	   if(cnt_out > 50){
		break;
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
		    cnt_out++;
		    if(cnt_out > 50){
			return 0; 
			break;
		    }
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
        return 1;
    }
    
    //check shift up ***************************
    cnt2 = 0;
    cnt3 = 0;
    cnt_out = 0;
    for(int i = 360; i < orig_img.rows; i++){ // 640
        if(cnt3 >= 600){
            cnt2++;
        }
       if(cnt_out > 50){
	   break;
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
		    cnt_out++;
		    if(cnt_out > 50){
			return 0;
			break;
		    }
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

        return 1;
    }
    
    // shift left check**************************************
    int cnt4 = 0;
    int cnt5 = 0;
    int cnt_l = 0;
    int check = 0;
    int cntl2 = 0;
    cnt_out = 0; 
    //for(int i =0; i < orig_img.rows; i++){
    for(int i = 480; i < orig_img.cols; i++){  
        //cout << "cnt_l " << cnt_l << endl;
        if(cnt_l > 100){
         cntl2++;   
        }	
        if(cnt_out > 50){
		break;
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
	    else{
		    cnt_out++;
		    if(cnt_out > 50){
			return 0;
			break;
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
 
        return 1;
    }
    
    //shift right check****************************
    int cnt6 = 0;
    int cnt7 = 0;
    int cnt_r = 0;
    cnt_out = 0; 
    for(int i =0; i < 161; i++){
        //cout << "CNT 6" << cnt6 << endl;
        if(cnt6 >= 100){
            //cnt_r = cnt6 + cnt_r;
            cnt_r++;
        }
        if(cnt_out > 50){
		break;
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
	    else{
		    cnt_out++;
		    if(cnt_out > 50){
			return 0;
			break;
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
        return 1;
    }
    return 0;
    //cout << "no shift detected" << endl;
    myfile << frame_num << ": no shift " << endl;
} // shift comp


// color comparison module
int color_comp2(Mat corr_img, int frame_num, ofstream &myfile){
    //int height = 480;
    //int width = 640;
    Mat red(height, width, CV_8UC3, Scalar(0,0,255)); // red Mat object
    Mat blue(height, width, CV_8UC3, Scalar(255,0,0)); // blue Mat object
    Mat green(height, width, CV_8UC3, Scalar(0,255,0)); // green Mat object
    Mat white(height, width, CV_8UC3, Scalar(255,255,255)); // white Mat Object
    Mat black(height, width, CV_8UC3, Scalar(0,0,0)); // Black Mat object
    int ret = 0;
    int cntw = 0;
    int cntb = 0;
    int cntg = 0;
    int cntr = 0;
    int cntblk = 0;
    int n = 0;
    Vec3b white_o = white.at<Vec3b>(0,0); // pixel values of each color at (0,0) point of pixel array
    Vec3b red_o = red.at<Vec3b>(0,0);
    Vec3b blue_o = blue.at<Vec3b>(0,0);
    Vec3b green_o = green.at<Vec3b>(0,0);
    Vec3b corr_o = corr_img.at<Vec3b>(0,0);
        return 1;
    //}
    return 0;
    //cout << "no shift detected" << endl;
    myfile << frame_num << ": no shift " << endl;
} // shift comp


// shift comparison module
int color_comp(Mat corr_img, int frame_num, ofstream &myfile){
    //int height = 480;
    //int width = 640;
    Mat red(height, width, CV_8UC3, Scalar(0,0,255)); // red Mat object
    Mat blue(height, width, CV_8UC3, Scalar(255,0,0)); // blue Mat object
    Mat green(height, width, CV_8UC3, Scalar(0,255,0)); // green Mat object
    Mat white(height, width, CV_8UC3, Scalar(255,255,255)); // white Mat Object
    Mat black(height, width, CV_8UC3, Scalar(0,0,0)); // Black Mat object
    int ret = 0;
    int cntw = 0;
    int cntb = 0;
    int cntg = 0;
    int cntr = 0;
    int cntblk = 0;
    int n = 0;
    Vec3b white_o = white.at<Vec3b>(0,0); // pixel values of each color at (0,0) point of pixel array
    Vec3b red_o = red.at<Vec3b>(0,0);
    Vec3b blue_o = blue.at<Vec3b>(0,0);
    Vec3b green_o = green.at<Vec3b>(0,0);
    Vec3b corr_o = corr_img.at<Vec3b>(0,0);
    Vec3b black_o = black.at<Vec3b>(0,0);
    for(int i =0; i < 640; i++){
	    if( n > 50){
		    return 0;
	    }
	    for(int j = 0; j < 480; j++){
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
		    else if(black_o == corr_o){
			    cntblk++;
		    }
		    else{
			    n++;
		    }
		    if( n > 50){
			return 0;
			    break;
		    }
	    } // inner for loop
    } // outer for loop
    if(cntw >= 296000){
	    cout << "WHITE SCREEN COMPARED" << endl;
	    myfile << frame_num << ": WHITE SCREEN COMPARED" << endl;
	    ret = 1;
	    //break;
    }
    else if(cntblk >= 296000){
	    cout << "BLACK SCREEN COMPARED" << endl;
	    myfile << frame_num << ": WHITE SCREEN COMPARED" << endl;
	    ret = 1;
    }
    else if(cntb >= 296000){
	    cout << "BLUE SCREEN COMPARED" << endl;
	    myfile << frame_num << ": BLUE SCREEN COMPARED" << endl;
	    ret = 1; 
	    //break;
    }
    else if(cntg >= 296000){
	    cout << "GREEN SCREEN COMPARED" << endl;
	    myfile << frame_num << ": GREEN SCREEN COMPARED" << endl;
	    ret = 1;
	    //break;
    }
    else if(cntr >= 296000){
	    cout << "RED SCREEN COMPARED" << endl;
	    myfile << frame_num << ": RED SCREEN COMPARED" << endl;
	    ret = 1; 
	    //break;
    }
    else{
	    //cout << "no color detection " << endl;
	    myfile << frame_num << ": no color detection " << endl;
	    ret = 0;
    }
    return ret;
} // color comp


//Module for freeze comparison 
int freeze_comp(Mat prev, Mat corr, int freeze_cnt, int frame_num, ofstream &myfile, auto &start){
	int frz_cnt = 0;
	int frz_not = 0;
	int frz_check = 0;
	double count = 0;
	double count1 = 0; 
	//auto start = chrono::high_resolution_clock::now();
	auto end = chrono::high_resolution_clock::now();

	//cout << "here";
	Vec3b x = corr.at<Vec3b>(0,0);
	Vec3b y = prev.at<Vec3b>(0,0);
	//while(1){
		for(int i =0; i < 480; i++){
			for(int j = 0; j < 640; j++){ // 480
				//cout << i << " " << j << endl;
				x = corr.at<Vec3b>(i,j);
				y = prev.at<Vec3b>(i,j);
				if(x == y){
					frz_cnt++;
					//cout << "identical" << endl;
					//cout << "freeze cnt " << frz_cnt << endl;

				}
				else{
					frz_not++;
					if(freeze_cnt > 0){
						end = chrono::high_resolution_clock::now(); // end clock if no longer same frame
						count1 = chrono::duration_cast<chrono::nanoseconds>(end-start).count()+count;
						count = count1 *= 1e-9;
						count = round(count);
						//float counter = freeze_cnt/8.0;
						//int counter2 = round(counter);
						if(count >=2){
							cout << "COMP: frozen for: " << count << " secs" << endl;
							myfile << frame_num << ": COMP: frozen for: " << count << " secs" << endl;
						}

						freeze_cnt = 0;
						return 0;
						//break;
					}
					if(frz_not > 50){
						//cout << frame_num << ": " << "no freeeze " << endl;
						return 0;
						//break;
					}
				}
			} // inner for loop
		} // outer for loop
		//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!Freeze_cnt " << frz_cnt << endl;
		//cout << "Frz Count : " << frz_cnt << endl;
		if(frz_cnt >= 268000){
			//cout << "Froze" << endl;
			//myfile << frame_num << ": COMP: frozen "  << endl;
			if(freeze_cnt == 0){
				//myfile << frame_num << ": COMP: frozen caught "  << endl;
				start = chrono::high_resolution_clock::now(); // start clock upon freeze
			}
			freeze_cnt++;
		}
		//break;
	//}// while

	if(freeze_cnt == 0){
		myfile << frame_num << ": no freeze detected"  << endl;
	}
	return freeze_cnt;
} // end of freeze compare


// setup socket for original frame transmission
int socket_setup2(){
	int opt = 1;

	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == 0){
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
	if(listen1 < 0){
		printf("errot in listen\n");
	}

	int new_socket = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	if(new_socket < 0){
		printf("Error in accept\n");
	}
	return new_socket;

}

int socket_setup(){
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		cout << "error in socket()" << endl;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = PF_INET;
	serv_addr.sin_port = htons(PORT2);
	int addrlen = sizeof(serv_addr);

	if(inet_pton(AF_INET, "192.168.1.120", &serv_addr.sin_addr) <= 0){
		cout << "error in inet_pton" << endl;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, addrlen) < 0){
		cout << "error in connect: " << strerror(errno) << endl;
	}
	return sock;
}

// function for receiving frames
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


double fps_calc2(int &frame_num, int &socket, SocketMatTransmissionServer &socketMat, ofstream &myfile){
	int j = 0;
	Mat image;
	Mat image2;
	double time2 = 0;
	cout << "FPS calculating: " << endl;
	int img_buffer_size = 10;
	int* id_buffer = new int [img_buffer_size];
	Mat* img_buffer = new Mat [img_buffer_size];
	auto start = chrono::high_resolution_clock::now(); // start high res clock
	while(j < 28){ // run 28 frames 
		//myfile << frame_num << "original" << endl;
		receive_frame(socket, img_buffer, img_buffer_size, id_buffer); // receive original image
		image = img_buffer[img_buffer_size - 1]; // set Mat type to that image
		if(socketMat.receive(image2) > 0) // receive corrupted image
		{
		}
		//frame_id++;
		frame_num++;
		cout << ".";
		j++;
	}
	auto end = chrono::high_resolution_clock::now(); // end high res clock
	double time1 = chrono::duration_cast<chrono::nanoseconds>(end-start).count(); // calculates difference 
	//cout << "Time "  << time1 << endl;
	time1 *= 1e-9;
	time2 = 28.0/time1;
	cout << "Frame rate: " << time2 << endl;
	return time2;
}


struct resolution {
	int X;
	int Y;
};

void output_ui(Mat& img, bool shift_detect, bool noise_detect, bool freeze_detect, double fps) {

	//if (img.empty() || &img == NULL) {
	if (img.empty()) {
		cout << "Empty Image" << endl;
		return;
	}

	Point warning = Point(50, 50);
	//Point pt = Point(100, 200);
	Scalar color = Scalar(0, 0, 255);
	int thickness = 5;
	int font_size = 2;
	int font = FONT_HERSHEY_COMPLEX_SMALL;
	int text_x = 50;
	int text_y = 100;
	int width1 = img.cols;
	int height1 = img.rows;
	//FPS1080:
	//putText(img, "FPS:", Point(100, 980), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	//putText(img, to_string(fps), Point(210, 980), font, font_size, Scalar(0, 0, 0), thickness, 8, false);

	//FPS720:
	//putText(img, "FPS:", Point(100, 620), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	//putText(img, to_string(fps), Point(210, 620), font, font_size, Scalar(0, 0, 0), thickness, 8, false);

	//FPS480:
	putText(img, "FPS:", Point(50, 380), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	putText(img, to_string(fps), Point(160, 380), font, font_size, Scalar(0, 0, 0), thickness, 8, false);

	//Resolution1080:
	//putText(img, to_string(res.Y), Point(100, 1030), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	//putText(img, "x", Point(210, 1030), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	//putText(img, to_string(res.X), Point(240, 1030), font, font_size, Scalar(0, 0, 0), thickness, 8, false);

	putText(img, to_string(width1), Point(50, 430), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	putText(img, "x", Point(130, 430), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	putText(img, to_string(height1), Point(160, 430), font, font_size, Scalar(0, 0, 0), thickness, 8, false);

	if (shift_detect || noise_detect || freeze_detect) {
		putText(img, "Warning:", warning, font, font_size, color, thickness, 8, false);
		//void rectangle(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
		//rectangle(img, Point(0, 0), Point(1920, 1080), color, 20);
		rectangle(img, Point(0, 0), Point(640, 480), color, 20);
	}
	else {
		return;
	}
	if (shift_detect) {
		putText(img, "Video Shift Detected", Point(text_x, text_y), font, font_size, color, thickness, 8, false);
		//void arrowedLine(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int line_type=8, int shift=0, double tipLength=0.1)
		//arrowedLine(img, Point(200, 500), Point(10, 500), color, thickness);
		text_y = text_y + 50;
	}
	if (noise_detect) {
		putText(img, "Noise Detected", Point(text_x, text_y), font, font_size, color, thickness, 8, false);
		text_y = text_y + 50;
	}
	if (freeze_detect) {
		putText(img, "Video Frozen", Point(text_x, text_y), font, font_size, color, thickness, 8, false);
		text_y = text_y + 50;
	}

} // end output UI

int main()
{
	// create socket for corrupted frame transmission
	// uses poer 6670
	SocketMatTransmissionServer socketMat;
	if (socketMat.socketConnect(6670) < 0)
	{
		return 0;
	}
	int original_receive_socket = socket_setup();

	// create socket for original frame transmission

	ofstream myfile;
	myfile.open("comp_log.txt"); // output log for compared frames
	int it = 0;
	int loop1 = 0;
	int loop2 = 0;
	int loop3 = 0;
	int freeze_num = 0;
	int frame_num = 2;
	int frame_id =2;
	double fps_cal = 0.0;
	cv::Mat image; // original image
	cv::Mat image2; // corrupted image
	cv::Mat prev_img; // previous image for freeze comparison
	int img_buffer_size = 10;
	int* id_buffer = new int [img_buffer_size];
	Mat* img_buffer = new Mat [img_buffer_size];
	auto start = chrono::high_resolution_clock::now();
	float times[60];
	int index = 0;
	float sum = 0;
	double fps = 0;
	double avg_fps = 30;

	while (1)
	{
		auto start_fps_avg = std::chrono::high_resolution_clock::now();

		frame_num++;
		//if((it%2)==0){
		if(it==1){ // make sure one iteration has ran before setting previous image
			prev_img = image2;
		}
		//if(frame_num == 5){
		//	    fps_cal = fps_calc2(frame_num, original_receive_socket, socketMat, myfile);
		//
		//	}
		receive_frame(original_receive_socket, img_buffer, img_buffer_size, id_buffer); // receive original image
		image = img_buffer[img_buffer_size - 1]; // set Mat type to that image
		if(socketMat.receive(image2) > 0) // receive corrupted image
		{
			//cv::imshow("Corrupted",image2);
			//cv::imwrite("test.jpg",image2);
			//cv::waitKey(1);
			it++;
		}



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
			while(1){
				loop1 = color_comp(image2, frame_num, myfile); // check for discoloration 
				if(loop1 ==1){ // if black do not run shift comparator because cannot shift a black screen
					//cout << "cannot shift black screen" << endl;
					break; 
				}
				loop2 = shift_comp(image, image2, frame_num, myfile); // check for shift
				if(loop2 == 1){
					break;
				}
				loop3 = freeze_num = freeze_comp(prev_img, image2, freeze_num, frame_num, myfile, start); // check for frozen
				break; 
			}
			bool shift, noise, freeze; 
			if(loop1 == 1){
				noise = true; 
			}
			else if(loop2 == 1){
				shift = true;
			}
			else if(loop3 > 0){
				freeze = true; 
			}

			output_ui(image2, shift, noise, freeze, avg_fps);
			//imshow("COMP: Original", image); // show both imagess
			imshow("COMP: Corrupted",image2);
			shift = false;
			noise = false;
			freeze = false;
			loop1 = 0;
			loop2 = 0;
			loop3 = 0;
			//cout << "HERE" << endl;
			prev_img = image2; // set previous image
		}

		char c = (char)waitKey(1);
		if(c == 27){
			break;
		}
		//Calculated elapsed time:
		auto elapsed = std::chrono::high_resolution_clock::now() - start_fps_avg;
		//Convert to microseconds:
		long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
		//Convert to seconds:
		long double seconds = (long double)microseconds / 1e6;
		sum = 0;
		//cout << index << endl;
		times[index] = seconds;
		for (int j = 0; j <= index; j++) {
			sum = sum + times[j];
		}
		avg_fps = 1/(sum / index);
		if (index < 59) {
			index++;
		}
		else {
			for (int j = 0; j < 59; j++) {
				times[j] = times[j + 1];
			}
		}
	    }
	    delete[] id_buffer;
	    delete[] img_buffer;
	    myfile.close();
	    socketMat.socketDisconnect();
	    close(original_receive_socket);
	    return 0;
	}
