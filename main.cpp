
/*************************************************
* Corruption Unit OPENCV Program
* Contributors: Kamron Ebrahimi, Aaron Walder, Pu Huang
************************************************/

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <pthread.h>  
#include <mutex>     
#include <zmq.hpp>     
#include "Distortion.hpp"
#include "Freeze.hpp"
#include "White.hpp"
#include "Translate.hpp"


enum distortion{FREEZE, WHITE, SHIFT};
std::mutex mtx;

void setup(cv::VideoCapture &vid, int &width, int &height);
void getType(distortion &type);
void *handleIPC(void *threadid);

int main()
{
    int width, height;
    cv::Mat frame; 
    std::vector<Distortion*> dis(3);
    enum distortion type = WHITE;
    std::string window = "window";
    cv::VideoCapture vid(0); 
    
    setup(vid, width, height);
    
    Freeze freeze(3000, &vid, &frame, window);
    White white(3000, &vid, &frame, window, 255, height, width); 
    Translate translate(3000, &vid, &frame, window, height, width); 

    cv::namedWindow(window, cv::WINDOW_AUTOSIZE);
    
    dis[0] = &freeze;
    dis[1] = &white;
    dis[2] = &translate;

    // spawn the IPC socket thread
    pthread_t ipcHandler;
    int rc = pthread_create(&ipcHandler, NULL, handleIPC, (void *)1); 
    if (rc) {
        std::cout << "Error:unable to start server" << rc << std::endl;
        std::exit(-1);
    }


    while(1)
        dis[type]->run();

    return 0; 
}

void setup(cv::VideoCapture &vid, int &width, int &height)
{
    int size_in;
    if(!vid.isOpened())
        return;

    while(1){
        std::cout << "Press 1: 640x480, 2: 1280x720, 3: 1920x1080" << std::endl;
        std::cin >> size_in;
        if(size_in == 1){
            height = 480;
            width = 640; 
            std::cout << "640x480" << std::endl;
            break;
        }
        else if(size_in == 2){
            height = 720;
            width = 1280;    
            std::cout << "1280x720" << std::endl;
            break;
        }
        else if(size_in == 3){
            height = 1080;
            width = 1920; 
            std::cout << "1920x1080" << std::endl;
            break; 
        }
    }

    vid.set(cv::CAP_PROP_FRAME_WIDTH, width);
    vid.set(cv::CAP_PROP_FRAME_HEIGHT, height); 
}

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
        std::cout << "Received Command" << std::endl;

        //  Do some 'work'
    	sleep(1);

        //  Send reply back to client
        zmq::message_t reply (5);
        memcpy (reply.data (), "ACK", 5);
        socket.send (reply);
    }

}