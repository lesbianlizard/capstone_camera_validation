
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
#include <sched.h>
#include <zmq.hpp>     
#include "Distortion.hpp"
#include "Freeze.hpp"
#include "White.hpp"
#include "Translate.hpp"
#include "Socket.h"

#define COMP_IP "192.168.1.120"
enum distortion{FREEZE, WHITE, SHIFT};
std::mutex mtx;
std::vector<Distortion*> dis(3);
enum distortion type;

void setup(cv::VideoCapture &vid, int &width, int &height);
void *handleIPC(void *threadid);
void dispatch(std::vector<std::string> cmd);
void checkFrame(cv::Mat &frame);

int main()
{
    SocketMatTransmissionClient socketMat;
	if (socketMat.socketConnect(COMP_IP, 6666) < 0)
	{
		return 0;
	}
	

    int width, height;
    cv::Mat frame; 
    cv::Mat* dframe;
    type = SHIFT;
    std::string window = "window";
    cv::VideoCapture vid(0); 
    
    setup(vid, width, height);
    
    Freeze freeze(100);
    White white(50, 255, height, width); 
    Translate translate(50, height, width, 15, 15); 

    cv::namedWindow(window, cv::WINDOW_AUTOSIZE);
    
    dis[0] = &freeze;
    dis[1] = &white;
    dis[2] = &translate;

    // spawn the IPC socket thread
    pthread_t ipcHandler;
    pthread_create(&ipcHandler, NULL, handleIPC, (void *)1); 

    while(1)
    {
        vid >> frame;  
        checkFrame(frame);
        dframe = &frame; 
        mtx.lock();
        dis[type]->run(dframe);
        mtx.unlock();
        socketMat.transmit(*dframe);
        // cv::imshow(window, *dframe);
        // cv::waitKey(1);
    }

    return 0; 
}




void setup(cv::VideoCapture &vid, int &width, int &height)
{
    std::cout<< "--------- CORRUPTION UNIT DISTORTION PROGRAM ----------"<<std::endl;
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

        // process the data input
        std::string cmd_str = std::string(static_cast<char*>(request.data()), request.size());
        std::cout<< cmd_str <<std::endl;
        std::stringstream ss(cmd_str);
        std::vector<std::string> cmd;
        while(ss.good())
        {   
            std::string substr;
            getline( ss, substr, ',' );
            cmd.push_back( substr );
        }
        for(int i=0; i < cmd.size(); i++)
            std::cout << cmd.at(i) << ' ';


        // lock the mutex and update the filter
        mtx.lock();
        dispatch(cmd);
        mtx.unlock();

        //  Send reply back to client
        zmq::message_t reply (3);
        memcpy (reply.data (), "ACK", 3);
        socket.send (reply);
    }
}

// Format (use .join(',') method on a dynamically constructed list object) to produce these commands
// Freeze:   ['1', '1/0', 'dur(ms)']
// White:    ['2', '1/0', 'dur(ms)', 'shade(0-255)']
// Translate ['3', '1/0', 'dur(ms)', 'x_offset', 'y_offset']
// Introduce better error handling here!
void dispatch(std::vector<std::string> cmd)
{
    switch(stoi(cmd[0],nullptr))
    {
        case 1:
            type = FREEZE; 
            dis[type]->update(cmd);
            break;
        case 2:
            type = WHITE; 
            dis[type]->update(cmd);
            break;
        case 3:
            type = SHIFT; 
            dis[type]->update(cmd);
            break;
    }
}

void checkFrame(cv::Mat &frame)
{
    if(frame.empty())
    {
        std::cout<<"EXIT";
        exit(EXIT_FAILURE);
    }
}










