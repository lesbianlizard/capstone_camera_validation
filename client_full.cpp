






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
#include <pthread.h>  
#include <zmq.hpp> 

using namespace cv;
using namespace std;

//待传输图像默认大小为 640*480，可修改
#define IMG_WIDTH 640    // 需传输图像的宽
#define IMG_HEIGHT 480    // 需传输图像的高
#define PACKAGE_NUM 2
#define PORT2 8080
#define MB 2591 * 1944


#define PORT3 8081 // not in use 
#define PORT_node 6667 // not in use 
//char *global_str;
std::vector<std::string> cmd; // vector of strings from the GUI



// server 
/*
int socket_setup3(){
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		cout << "error in socket()" << endl;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT_node);
	int addrlen = sizeof(serv_addr);
	
	if(inet_pton(AF_INET, "192.168.1.120", &serv_addr.sin_addr) <= 0){
		cout << "error in inet_pton" << endl;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, addrlen) < 0){
		cout << "error in connect" << endl;
	}
	return sock;
}
*/

// end server

//CLient start
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


Mat translateImg(Mat& img, int offsetx, int offsety) { // function used to translate image
    Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img, img, trans_mat, img.size());
    return img;
}


Mat shift_up(Mat img_corr, int percentage){ // call this function to shift up (user percentage not pixels)
    cout << endl <<  "Indicator 1: shift " << endl;
    cout << "Shift indicator: 12 - UP" << endl;
    cout << "Percentage shift indicator: " << percentage << endl;
    float up_percent = (percentage / 100.0)*480;
    up_percent = round(up_percent);
    cout << "Shifted UP by: " << up_percent << " = " << percentage << "%" << endl;
    //cvtColor(orig, img_corr, 0);
    translateImg(img_corr, 0, -up_percent); // calls translate function
    return img_corr;
} // shift up

Mat shift_left(Mat img_corr, int percentage){
    cout << endl << "Indicator 1: shift " << endl;
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
    cout << endl << "Indicator 1: shift " << endl;
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
    cout << endl << "Indicator 1: shift " << endl;
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


// splits camera signal (original away from corruption)
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

// freeze function module 
void freeze(Mat prev_frame, int freeze_num, int &socket, SocketMatTransmissionClient &socketMat, VideoCapture &vid, int &frame_num, int & frame_id, ofstream &myfile){
    cout << "Indicator 2: Freeze" << endl;
    Mat frame;
	double count = 0;
	double count1 = 0;
	cout << "Frozen " << endl;
	//clock_t start, end; 
	auto start = chrono::high_resolution_clock::now();
	//start= clock();
	while(count < freeze_num){
		//end = clock();
		//count = double(end-start)/double(CLOCKS_PER_SEC);
		
		auto end = chrono::high_resolution_clock::now();
		count1 = chrono::duration_cast<chrono::nanoseconds>(end-start).count()+count;
		count1 *= 1e-9;
        count = count1;
        vid >> frame;
        imshow("Corr: Original", frame);
        split(frame, prev_frame, frame_id, socket);
        imshow("corruption2", prev_frame);
        socketMat.transmit(prev_frame);
        //cout << "Unfrozen" << endl;
        myfile << frame_num << ": frozen" << endl;
        frame_num++;
        frame_id++;
	}
    cout << "Unfrozen" << endl;
	
} // freeze

/*
void fps_calc(VideoCapture &vid){
    int j = 0;
    Mat frame;
    auto start = chrono::steady_clock::now();
    while(j < 28){
        vid>>frame;
        imshow("Corr: Original", frame);
        j++;
    }
    auto end = chrono::steady_clock::now();
    auto time1 = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    //cout << "Time "  << time1 << endl;
    float time2 = time1/1000.0;
    cout << "Frame rate: " << 28/time2 << endl;
} // fps
*/

//calculates fps 
void fps_calc2(VideoCapture &vid, int &frame_id, int &frame_num, int &socket, SocketMatTransmissionClient &socketMat, ofstream &myfile){
    int j = 0;
    Mat frame;
    double time2 = 0;
	auto start = chrono::high_resolution_clock::now(); // start high res clock
    cout << "FPS calculating: " << endl;
    while(j < 28){ // run 28 frames 
        myfile << frame_num << "original" << endl;
        vid>>frame;
        imshow("Corr: Original", frame);
        split(frame, frame, frame_id, socket);
        imshow("corruption2", frame);
        socketMat.transmit(frame);
        frame_id++;
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
}


// setup socket for second transmission of original frames 
int socket_setup2(){
	int sock = 0;
    sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		cout << "error in socket()" << endl;
        return -1; 
    }

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT2);
	//socklen_t addrlen = sizeof(serv_addr);
	
	if(inet_pton(AF_INET, "192.168.1.123", &serv_addr.sin_addr) <= 0){
		cout << "error in inet_pton" << endl;
        return -1; 
    }

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		cout << "error in connect: " << strerror(errno) << endl;
        return -1;
    }
	return sock;
}

int socket_setup(){
	int opt = 1;

	int sockfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
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

/*
int socket_setup2(){
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
	address.sin_port = htons(PORT3);
	int bind1 = bind(sockfd, (struct sockaddr*)&address, addrlen);
	if(bind1 < 0){
		printf("error in bind\n");
	}

	int listen1 = listen(sockfd, 3);

	int new_socket = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	return new_socket;
	
}
*/

//IPC handler
/**
 * Kamron Ebrahimi - 2020
 * **/
void *handleIPC(void *threadid)
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555");
    while (true) {
        zmq::message_t request;
        //  Wait for next request from client
        socket.recv (&request);

        // process the data input
        std::string cmd_str = std::string(static_cast<char*>(request.data()), request.size());
        //std::cout<< cmd_str <<std::endl;
        std::stringstream ss(cmd_str);
        //std::vector<std::string> cmd;
        while(ss.good())
        {   
            std::string substr;
            getline( ss, substr, ',' );
            cmd.push_back( substr );
        }
    
        for(int i=0; i < cmd.size(); i++)
            std::cout << "i: " << i << " " << cmd.at(i) << ' ';

        // lock the mutex and update the filter
     
        //  Send reply back to client
        zmq::message_t reply (3);
        memcpy (reply.data (), "ACK", 3);
        socket.send (reply);
    }
}


//#include "SocketMatTransmissionClient.h"

int main()
{
    //create first socket for corrupted image transmission
    // uses PORT 6670

    
    SocketMatTransmissionClient socketMat;
    if (socketMat.socketConnect("192.168.1.123", 6670) < 0)
    {
        return 0;
    }

    int socket = socket_setup(); //6668
    //Create second socket for original image transmission
    //uses PORT 6668

    pthread_t ipcHandler;
    // pthread_t ethernetHandler;
    pthread_create(&ipcHandler, NULL, handleIPC, (void *)1); 

    int height = 480; // defines height of image
    int width = 640; // defines width of image
    int frame_num = 0;
    int frame_id = 0;
    int frame_id2 = 0;
    ofstream myfile;
    myfile.open("corr_log.txt"); // log of corruption done
    
    cv::VideoCapture capture(0); // video capture object
    capture.set(CAP_PROP_FRAME_WIDTH, width); // set width
    capture.set(CAP_PROP_FRAME_HEIGHT, height); // set height
    capture.set(CAP_PROP_FPS,40); // set FPS of corruption system
    
    cv::Mat image; // original frame
    cv::Mat prev_frame; // previous frame for freeze
    cv::Mat img_corr; // corrupted frame
    
    /*
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
  */
  capture >> image;
  int percentage = 5;
  int frz_time = 5; 
    //cout << "frame num: " << frame_num << endl;
    frame_num = 0;
    //global_str = "5";
    cmd.push_back("5"); // set an original value for the function
    cmd.push_back("4");
    int val = 0;
    int val2 = 0;
    int prev_val2 = 0;
    while (1)
    {
        //if(frame_num == 2){ // test fps on start up
         //   cout << "Frame num: " << frame_num << endl;
         //   cout << "Size: " << image.cols << " x " << image.rows << endl;
         //   fps_calc2(capture, frame_id, frame_num, socket, socketMat, myfile);
          //  cout << "FPS done" << endl;

        //}
        frame_num++; // incrememnet frame num each loop
        frame_id++; // increment frame_id each loop
        myfile << frame_num << ": ";
        if (!capture.isOpened()) // if video capture object fails, leave program
            return 0;
        
        capture >> image; // capture frame 
        prev_frame = image;// set previous frame to current frame
        img_corr = image; // set corrupted image as original if no corruption
        
        // receiving data from web GUI
        val = stoi(cmd[0], nullptr); 
        val2 = stoi(cmd[1], nullptr);
        //if(va){
         //   val = 5; 
        //}
        cv::waitKey(5);

        //cout << "CMD[0] " << val << endl;
        // if freeze indicator
        if(val == 1){
            val = stoi(cmd[1], nullptr);
            if(val == 1){ // if enabled 
                val = stoi(cmd[2], nullptr); // freeze time
                myfile << frame_num << ": freeze" << endl;
                freeze(prev_frame, val, socket,socketMat,  capture, frame_num, frame_id, myfile);
            }
            else{
                imshow("Corr: Original", image);
                split(image, img_corr, frame_id, socket);
                    //socketMat.transmit(image);
                //imshow("corruption2", image);
                socketMat.transmit(img_corr);
            }
        }
        
        // if discoloration indicator 
        else if(val == 2){
            val = stoi(cmd[1], nullptr);
            if(val == 1){
                myfile << frame_num << ": white" << endl;
                img_corr = discolor_white();
                imshow("Corr: Original", image);
                split(image, img_corr, frame_id, socket);
                //imshow("corruption2", img_corr);
                socketMat.transmit(img_corr);
            }
            else{
                imshow("Corr: Original", image);
                split(image, img_corr, frame_id, socket);
                    //socketMat.transmit(image);
                //imshow("corruption2", image);
                socketMat.transmit(img_corr);
            }
        }
        
        // if shift indicator 
        else if(val == 3){
            val = stoi(cmd[1], nullptr);
            if(val == 1){
                int offx = stoi(cmd[3], nullptr);
                int offy = stoi(cmd[4], nullptr);
                split(image, image, frame_id, socket);
                imshow("Corr: Original", image);
                if(offx > 0){ // shift right
                    float percentagex = (offx/640.0)*100.0;
                    myfile << frame_num << ": shift right: " << percentagex << "%"<< endl;
                    Mat image2 = image; 
                    cv::waitKey(5);
                    img_corr = shift_right(image, percentagex);
              
                }
                else if(offx < 0){ // shift left
                    float percentagex = (-offx/640.0)*100.0;
                    myfile << frame_num << ": shift left: " << percentagex << "%"<< endl;
                    Mat image2 = image;
                    cv::waitKey(5);
                    img_corr = shift_left(image, percentagex);
                  
                }
                if(offy > 0){ // shift down
                    float percentagey = (offy/480.0)*100.0;
                    myfile << frame_num << ": shift down: " << percentagey << "%"<< endl;
                    Mat image2 = image;
                    cv::waitKey(5);
                    img_corr = shift_down(image, percentagey);
                   
                }
                else if(offy < 0){ // shift up
                    float percentagey = (-offy/480.0)*100.0;
                    myfile << frame_num << ": shift up: " << percentagey << "%"<< endl;
                    Mat image2 = image; 
                    //imshow("Corr: Original", image2);
                    //split(image2, image2, frame_id, socket);
                    cv::waitKey(5);
                    img_corr = shift_up(image, percentagey);

                }
                //imshow("corruption2", img_corr);
                socketMat.transmit(img_corr);
               
            }
            else{
                imshow("Corr: Original", image);
                split(image, img_corr, frame_id, socket);
                    //socketMat.transmit(image);
                //imshow("corruption2", image);
                socketMat.transmit(img_corr);
            }
        }
        else {
            myfile << frame_num << ": original" << endl;
            imshow("Corr: Original", image);
            split(image, img_corr, frame_id, socket);
            //socketMat.transmit(image);
            //imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
        }
        cmd.clear(); // clear string vecor from GUI
        prev_val2 = val2; 

        /*
        char c=(char)waitKey(5);
        if(c==27){ // press 'esc' to quit
            myfile << "EXIT" << endl;
            break;
        }
        
        else if(c=='f'){ // press 'f' to freeze
            myfile << "freeze" << endl;
            
            freeze(prev_frame, frz_time, socket,socketMat,  capture, frame_num, frame_id, myfile);
        }// freeze
        
        else if(c == 'w'){ // shift up
            myfile << frame_num << ": shift up: " << percentage << endl;
            imshow("Corr: Original", image);
            split(image, img_corr, frame_id, socket);
            img_corr = shift_up(img_corr, percentage);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
            
            //socketMat.transmit(image);
            
        } // shift up
        
        else if(c == 's'){ // shift down
            myfile << frame_num << ": shift down: " << percentage << endl;
            imshow("Corr: Original", image);
            split(image, img_corr, frame_id, socket);
            img_corr = shift_down(img_corr, percentage);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
            //socketMat.transmit(image);
          
        } // shift down
        
        else if(c == 'd'){ // shift right
            myfile << frame_num << ": shift right: " << percentage << endl;
            imshow("Corr: Original", image);
            split(image, img_corr, frame_id, socket);
            img_corr = shift_right(img_corr, percentage);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
           
            
           
            
            //socketMat.transmit(image);
            
        } // shift right
        
        else if(c == 'a'){ // shift left
            myfile << frame_num << ": shift left: " << percentage << endl;
            imshow("Corr: Original", image);
            split(image, image, frame_id, socket);
            img_corr = shift_left(img_corr, percentage);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
            
        } // shift left
        
        else if(c == 'z'){ // shift left
            myfile << frame_num << ": shift left: " << percentage << endl;
            imshow("Corr: Original", image);
            split(image, image, frame_id, socket);
            img_corr = shift_right(img_corr, percentage);
            img_corr = shift_up(img_corr, percentage);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
            
             
            //socketMat.transmit(image);
            
        } // shift left
        
        else if(c =='r'){
            myfile << frame_num << ": red" << endl;
            img_corr = discolor_red();
            imshow("Corr: Original", image);
            split(image, img_corr, frame_id, socket);
            //socketMat.transmit(image);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
        }
        else if(c=='b'){
            myfile << frame_num << ": blue" << endl;
            img_corr = discolor_blue();
            imshow("Corr: Original", image);
            split(image, img_corr, frame_id, socket);
            //socketMat.transmit(image);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
        }
        else if(c=='g'){
            myfile << frame_num << ": green" << endl;
            img_corr = discolor_green();
            imshow("Corr: Original", image);
            split(image, img_corr, frame_id, socket);
            //socketMat.transmit(image);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
        }
        else if(c=='h'){
            myfile << frame_num << ": white" << endl;
            img_corr = discolor_white();
            imshow("Corr: Original", image);
            split(image, img_corr, frame_id, socket);
            //socketMat.transmit(image);
            imshow("corruption2", img_corr);
            socketMat.transmit(img_corr);
        }
        else if(c=='p'){
         fps_calc2(capture, frame_id, frame_num, socket, socketMat, myfile);
         cout << "SIZE: " << image.cols << " x " << image.rows << endl;   
        }
        * */
        
        //global_str[0] = '4'; 
       
    } // while
    myfile.close(); // close file
    socketMat.socketDisconnect(); // disconnect socket
    close(socket);
    return 0;
} // main
